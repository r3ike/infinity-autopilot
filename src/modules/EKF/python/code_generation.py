#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
File: derivation_utils.py
Descrizione:
    Utility per generare codice C++ a partire da funzioni simboliche SymForce,
    adattate per un EKF generico (non dipendente da PX4).
    Compatibile con Eigen e standard C++17.
"""

import symforce.symbolic as sf
import re
import os
import fileinput

def sign_no_zero(x) -> sf.Scalar:
    """
    Returns -1 if x is negative, 1 if x is positive, and 1 if x is zero
    """
    return 2 * sf.Min(sf.sign(x), 0) + 1

def add_epsilon_sign(expr, var, eps):
    # Avoids a singularity at 0 while keeping the derivative correct
    return expr.subs(var, var + eps * sign_no_zero(var))

def generate_cpp_function(function_name, output_names, namespace="ekf"):
    """
    Genera una funzione C++ (con Eigen) a partire da una funzione simbolica SymForce.
    Sostituisce:
      - std::max/min -> std::max/min (nessuna modifica)
      - Eigen rimane Eigen
      - Aggiunge il namespace specificato.
    """
    from symforce.codegen import Codegen, CppConfig

    codegen = Codegen.function(
        function_name,
        output_names=output_names,
        config=CppConfig(zero_initialization_sparsity_threshold=1)
    )
    metadata = codegen.generate_function(
        output_dir="generated",
        skip_directory_nesting=True
    )

    for f in metadata.generated_files:
        print("  |- {}".format(os.path.relpath(f, metadata.output_dir)))

    # Post‑processing minimo: nessun replace perché usiamo Eigen e std
    # Se vuoi aggiungere un namespace, puoi farlo qui.
    with fileinput.FileInput(os.path.abspath(metadata.generated_files[0]), inplace=True) as file:
        for line in file:
            # Aggiungi il namespace se non presente (esempio)
            # if "namespace" not in line:
            #     line = f"namespace {namespace} {{\n" + line + "\n} // namespace {namespace}\n"
            print(line, end='')

def generate_python_function(function_name, output_names):
    """Genera una funzione Python per test rapidi."""
    from symforce.codegen import Codegen, PythonConfig
    codegen = Codegen.function(
        function_name,
        output_names=output_names,
        config=PythonConfig()
    )
    metadata = codegen.generate_function(
        output_dir="generated",
        skip_directory_nesting=True
    )
    for f in metadata.generated_files:
        print("  |- {}".format(os.path.relpath(f, metadata.output_dir)))

def build_state_struct(state, T="float"):
    """
    Costruisce una struct C++ che rispecchia lo stato, usando tipi Eigen.
    Restituisce il codice C++ come stringa.
    """
    out = "struct StateSample {\n"

    def get_cpp_type(obj):
        """Mappa un tipo simbolico SymForce al tipo Eigen corrispondente."""
        if isinstance(obj, sf.M11):
            return f"{T}"
        elif isinstance(obj, sf.M21):
            return f"Eigen::Matrix<{T}, 2, 1>"
        elif isinstance(obj, sf.M31):
            return f"Eigen::Matrix<{T}, 3, 1>"
        elif isinstance(obj, sf.Rot3):
            # Potresti usare Eigen::Quaternion<T>, che ha ordine (w,x,y,z)
            # SymForce usa (x,y,z,w) internamente, ma Rot3 è comunque una rotazione.
            # Per compatibilità, definiamo un quaternione Eigen.
            return f"Eigen::Quaternion<{T}>"
        else:
            raise NotImplementedError(f"Tipo sconosciuto: {type(obj)}")

    # Dichiara i membri
    for key, val in state.items():
        out += f"\t{get_cpp_type(val)} {key}{{}};\n"

    state_size = state.storage_dim()
    out += f"\n\tEigen::Matrix<{T}, {state_size}, 1> Data() const {{\n" \
           + f"\t\tEigen::Matrix<{T}, {state_size}, 1> state_vec;\n"

    # Riempie il vettore piatto nell'ordine definito da state.index()
    index = state.index()
    for key in index:
        storage_dim = index[key].storage_dim
        offset = index[key].offset
        # Se il membro è un quaternione, dobbiamo estrarlo come vettore di 4 elementi
        if isinstance(state[key], sf.Rot3):
            # Assumiamo che il quaternione sia rappresentato come (x,y,z,w) in storage,
            # ma Eigen::Quaternion ha (w,x,y,z). Questo richiede attenzione.
            # Per semplicità, qui scriviamo il codice che mappa manualmente.
            out += f"\t\tstate_vec.template segment<4>({offset}) << {key}.x(), {key}.y(), {key}.z(), {key}.w();\n"
        elif isinstance(state[key], sf.M31):
            out += f"\t\tstate_vec.template segment<3>({offset}) = {key};\n"
        elif isinstance(state[key], sf.M21):
            out += f"\t\tstate_vec.template segment<2>({offset}) = {key};\n"
        elif isinstance(state[key], sf.M11):
            out += f"\t\tstate_vec({offset}) = {key};\n"
        else:
            raise NotImplementedError(f"Tipo non gestito in Data(): {type(state[key])}")

    out += "\t\treturn state_vec;\n"
    out += "\t}\n\n"

    # Accesso come vettore costante (puntatore al primo membro)
    first_field = next(iter(state))
    out += f"\tconst Eigen::Matrix<{T}, {state_size}, 1>& vector() const {{\n" \
           + f"\t\treturn *reinterpret_cast<const Eigen::Matrix<{T}, {state_size}, 1>*>(this);\n" \
           + f"\t}}\n\n"

    out += "};\n\n"
    out += f"static_assert(sizeof(Eigen::Matrix<{T}, {state_size}, 1>) == sizeof(StateSample), \"state vector doesn't match StateSample size\");\n"

    return out

def build_tangent_state_struct(state, tangent_state_index):
    """Genera la struttura per gli indici dello spazio tangente (error state)."""
    out = "struct IdxDof { unsigned idx; unsigned dof; };\n"
    out += "namespace State {\n"

    for key in tangent_state_index.keys():
        out += f"\tstatic constexpr IdxDof {key}{{{tangent_state_index[key].idx}, {tangent_state_index[key].dof}}};\n"

    out += f"\tstatic constexpr uint8_t size{{{state.tangent_dim()}}};\n"
    out += "};\n"
    return out

def generate_state_header(state, tangent_state_index, T="float"):
    """
    Genera l'header C++ (state.h) che definisce la struct dello stato
    e gli indici dello spazio tangente.
    """
    print("Generate state definition...")
    filename = "state.h"
    with open(f"./generated/{filename}", "w") as f:
        header = [
            "// --------------------------------------------------\n",
            "// This file was autogenerated, do NOT modify by hand\n",
            "// --------------------------------------------------\n",
            "\n#ifndef EKF_STATE_H",
            "\n#define EKF_STATE_H\n\n",
            "#include <Eigen/Dense>\n",
            "#include <Eigen/Geometry>\n\n",
            "namespace ekf {\n\n"
        ]
        f.writelines(header)
        f.write(build_state_struct(state, T))
        f.write("\n")
        f.write(build_tangent_state_struct(state, tangent_state_index))
        f.write("\n} // namespace ekf\n")
        f.write("#endif // EKF_STATE_H\n")
    print(f"  |- {filename}")