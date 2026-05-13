#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
File: simple_ekf_derivation.py
Descrizione:
    Derivazione di un filtro EKF error‑state con SymForce, ispirato a EKF2 di PX4.
    Stati inclusi: quaternione, velocità, posizione, bias giroscopio, bias accelerometro.
    Sensori di update: GPS (posizione e velocità), magnetometro (campo nel frame corpo),
                      barometro (altezza), lidar (altezza).
    Predizione: IMU (accelerometro e giroscopio).
    Genera automaticamente funzioni C++ per predizione della covarianza,
    Jacobiane delle misure e struttura dello stato.
"""

import symforce
symforce.set_epsilon_to_symbol()

import symforce.symbolic as sf
from symforce.values import Values

# In un ambiente PX4 reale importeremmo le funzioni di generazione
# from utils.derivation_utils import generate_px4_function, generate_px4_state
# Per questo esempio mostriamo solo la struttura, commentando le chiamate finali.

# ==============================================================================
# 1. Definizione dello stato
# ==============================================================================
State = Values(
    quat_nominal = sf.Rot3(),   # orientamento (SO(3))
    vel          = sf.V3(),     # velocità nel mondo
    pos          = sf.V3(),     # posizione nel mondo
    gyro_bias    = sf.V3(),     # bias del giroscopio
    accel_bias   = sf.V3()      # bias dell'accelerometro
)

# ==============================================================================
# 2. Mappatura dello spazio tangente (per costruire vettori e matrici dell'errore)
# ==============================================================================
class IdxDof:
    def __init__(self, idx, dof):
        self.idx = idx
        self.dof = dof

def BuildTangentStateIndex():
    tangent_state_index = {}
    idx = 0
    for key in State.keys_recursive():
        dof = State[key].tangent_dim()
        tangent_state_index[key] = IdxDof(idx, dof)
        idx += dof
    return tangent_state_index

tangent_idx = BuildTangentStateIndex()

# Dimensioni utili
STATE_DIM = State.storage_dim()
TANGENT_DIM = State.tangent_dim()

class VState(sf.Matrix):
    SHAPE = (STATE_DIM, 1)

class VTangent(sf.Matrix):
    SHAPE = (TANGENT_DIM, 1)

class MTangent(sf.Matrix):
    SHAPE = (TANGENT_DIM, TANGENT_DIM)

# ==============================================================================
# 3. Conversione da vettore piatto (ordine PX4) a oggetti State
#    PX4 memorizza il quaternione come (w, x, y, z), SymForce usa (x, y, z, w)
# ==============================================================================
def vstate_to_state(v: VState):
    state = State.from_storage(v)
    q_px4 = state["quat_nominal"].to_storage()  # SymForce dà (x,y,z,w)
    # Rimappa in (w, x, y, z)
    state["quat_nominal"] = sf.Rot3(sf.Quaternion(xyz=sf.V3(q_px4[1], q_px4[2], q_px4[3]), w=q_px4[0]))
    return state

# ==============================================================================
# 4. Predizione della covarianza
# ==============================================================================
def predict_covariance(
    state: VState,      # stato nominale (vettore piatto)
    P: MTangent,        # covarianza dell'errore corrente
    accel: sf.V3,       # misura accelerometro
    accel_var: sf.V3,   # varianza rumore accelerometro (per asse)
    gyro: sf.V3,        # misura giroscopio
    gyro_var: sf.Scalar,# varianza rumore giroscopio (uguale per i tre assi)
    dt: sf.Scalar       # intervallo di tempo
) -> MTangent:

    state = vstate_to_state(state)
    g = sf.Symbol("g")  # accelerazione di gravità (non appare nelle Jacobiane)

    # Errore di stato (piccole deviazioni)
    state_error = Values(
        theta      = sf.V3.symbolic("delta_theta"),
        vel        = sf.V3.symbolic("delta_v"),
        pos        = sf.V3.symbolic("delta_p"),
        gyro_bias  = sf.V3.symbolic("delta_w_b"),
        accel_bias = sf.V3.symbolic("delta_a_b")
    )

    # Rumore di processo
    noise = Values(
        accel = sf.V3.symbolic("a_n"),
        gyro  = sf.V3.symbolic("w_n")
    )

    # ----- Cinematica vera (con errore e rumore) -----
    state_t = Values()
    for key in state.keys():
        if key == "quat_nominal":
            # Piccola rotazione di errore (angoli piccoli)
            state_t[key] = sf.Rot3(sf.Quaternion(xyz=(state_error["theta"] / 2), w=1)) * state[key]
        else:
            state_t[key] = state[key] + state_error[key]

    # Input veri e nominali
    input_t = Values(
        accel = accel - state_t["accel_bias"] - noise["accel"],
        gyro  = gyro  - state_t["gyro_bias"]  - noise["gyro"]
    )
    input = Values(
        accel = accel - state["accel_bias"],
        gyro  = gyro  - state["gyro_bias"]
    )

    # Propagazione vera
    R_t = state_t["quat_nominal"]
    state_t_pred = state_t.copy()
    state_t_pred["quat_nominal"] = state_t["quat_nominal"] * sf.Rot3(sf.Quaternion(xyz=(input_t["gyro"] * dt / 2), w=1))
    state_t_pred["vel"]          = state_t["vel"] + (R_t * input_t["accel"] + sf.V3(0, 0, g)) * dt
    state_t_pred["pos"]          = state_t["pos"] + state_t["vel"] * dt

    # Propagazione nominale
    R = state["quat_nominal"]
    state_pred = state.copy()
    state_pred["quat_nominal"] = state["quat_nominal"] * sf.Rot3(sf.Quaternion(xyz=(input["gyro"] * dt / 2), w=1))
    state_pred["vel"]          = state["vel"] + (R * input["accel"] + sf.V3(0, 0, g)) * dt
    state_pred["pos"]          = state["pos"] + state["vel"] * dt

    # Errore predetto (differenza vero - nominale)
    state_error_pred = Values()
    for key in state_error.keys():
        if key == "theta":
            delta_q = (sf.Quaternion.from_storage(state_t_pred["quat_nominal"].to_storage()) *
                       sf.Quaternion.from_storage(state_pred["quat_nominal"].to_storage()).conj())
            state_error_pred["theta"] = 2 * sf.V3(delta_q.x, delta_q.y, delta_q.z)
        else:
            state_error_pred[key] = state_t_pred[key] - state_pred[key]

    # Semplificazioni: trascura termini dt^2 e usa vincolo quaternione unitario
    import sympy as sp
    for i in range(3):
        state_error_pred["theta"][i] = sp.expand(state_error_pred["theta"][i]).subs(dt**2, 0)
        q_est = sf.Quaternion.from_storage(state["quat_nominal"].to_storage())
        state_error_pred["theta"][i] = sp.factor(state_error_pred["theta"][i]).subs(
            q_est.w**2 + q_est.x**2 + q_est.y**2 + q_est.z**2, 1
        )

    # Linearizzazione intorno a errore zero e rumore zero
    zero_state_error = {state_error[k]: state_error[k].zero() for k in state_error.keys()}
    zero_noise = {noise[k]: noise[k].zero() for k in noise.keys()}

    # Jacobiane A e G
    A = VTangent(state_error_pred.to_storage()).jacobian(state_error).subs(zero_state_error).subs(zero_noise)
    G = VTangent(state_error_pred.to_storage()).jacobian(noise).subs(zero_state_error).subs(zero_noise)

    # Covarianza del rumore di processo
    var_u = sf.Matrix.diag([accel_var[0], accel_var[1], accel_var[2], gyro_var, gyro_var, gyro_var])
    P_new = A * P * A.T + G * var_u * G.T

    # Mantiene solo la parte triangolare superiore (simmetria)
    for i in range(TANGENT_DIM):
        for j in range(TANGENT_DIM):
            if i > j:
                P_new[i, j] = 0

    return P_new

# ==============================================================================
# 5. Regola della catena per Jacobiane delle misure
# ==============================================================================
def jacobian_chain_rule(expr: sf.Scalar, state: State) -> VTangent:
    # 1. Jacobiana nello spazio dei parametri
    dh_dx = sf.V1(expr).jacobian(state, tangent_space=False)

    # 2. Matrice di trasformazione da spazio errore a parametri
    class MStorageTangent(sf.Matrix):
        SHAPE = (STATE_DIM, TANGENT_DIM)

    dx_derror = MStorageTangent()
    # Blocco quaternione (4x3)
    q = sf.Quaternion.from_storage(state["quat_nominal"].to_storage())
    p = sf.Quaternion.symbolic('p')
    pq = p * q
    qR = sf.M41(pq.to_storage()).jacobian(sf.M41(p.to_storage()))  # prodotto destro
    # Mappa errore (3) -> variazione quaternione (4)
    dx_derror[0:4, 0:3] = qR / 2 * sf.M43([[1,0,0],[0,1,0],[0,0,1],[0,0,0]])

    # Il resto è identità (parte vel, pos, bias)
    for i in range(4, STATE_DIM):
        for j in range(3, TANGENT_DIM):
            if i == j+1:
                dx_derror[i, j] = 1

    # 3. Jacobiana finale nello spazio errore
    H = dh_dx * dx_derror
    return H

# ==============================================================================
# 6. Modelli di misura e loro Jacobiane
# ==============================================================================

# ---------- GPS (posizione) ----------
def compute_gps_pos_innov_var_and_h(
    state: VState, P: MTangent, meas: sf.V3, R: sf.Scalar
) -> (sf.V3, sf.V3, VTangent):
    state = vstate_to_state(state)
    innov = state["vel"] - meas   # NOTA: probabilmente è "pos", ma per esempio si assume pos
    # NOTA: Qui correggo: la misura è posizione, quindi innov = state["pos"] - meas
    # (nel codice sopra c'è un refuso voluto? lo correggo)
    innov = state["pos"] - meas
    H = jacobian_chain_rule(state["pos"][0], state)  # per semplicità, una riga alla volta
    # In realtà si può fare per tutto il vettore, ma PX4 fa spesso per asse.
    # Qui mostriamo un'unica funzione che restituisce H per x (o tutto).
    # Per completezza generiamo tre funzioni separate o una sola con H completa.
    # Scegliamo di generare l'innovazione come vettore e H per ogni asse (come in PX4).
    Hx = jacobian_chain_rule(state["pos"][0], state)
    Hy = jacobian_chain_rule(state["pos"][1], state)
    Hz = jacobian_chain_rule(state["pos"][2], state)
    innov_var = sf.V3(
        (Hx * P * Hx.T + R)[0,0],
        (Hy * P * Hy.T + R)[0,0],
        (Hz * P * Hz.T + R)[0,0]
    )
    return (innov, innov_var, Hx.T)

# ---------- GPS (velocità) ----------
def compute_gps_vel_innov_var_and_h(
    state: VState, P: MTangent, meas: sf.V3, R: sf.Scalar
) -> (sf.V3, sf.V3, VTangent):
    state = vstate_to_state(state)
    innov = state["vel"] - meas
    Hx = jacobian_chain_rule(state["vel"][0], state)
    Hy = jacobian_chain_rule(state["vel"][1], state)
    Hz = jacobian_chain_rule(state["vel"][2], state)
    innov_var = sf.V3(
        (Hx * P * Hx.T + R)[0,0],
        (Hy * P * Hy.T + R)[0,0],
        (Hz * P * Hz.T + R)[0,0]
    )
    return (innov, innov_var, Hx.T)

# ---------- Magnetometro (campo magnetico nel frame corpo) ----------
def compute_mag_innov_var_and_h(
    state: VState, P: MTangent, meas: sf.V3, mag_ref: sf.V3, R: sf.Scalar
) -> (sf.V3, sf.V3, VTangent):
    state = vstate_to_state(state)
    # Misura predetta: ruota il campo di riferimento nel frame corpo
    meas_pred = state["quat_nominal"].inverse() * mag_ref
    innov = meas_pred - meas
    Hx = jacobian_chain_rule(meas_pred[0], state)
    Hy = jacobian_chain_rule(meas_pred[1], state)
    Hz = jacobian_chain_rule(meas_pred[2], state)
    innov_var = sf.V3(
        (Hx * P * Hx.T + R)[0,0],
        (Hy * P * Hy.T + R)[0,0],
        (Hz * P * Hz.T + R)[0,0]
    )
    return (innov, innov_var, Hx.T)

# ---------- Barometro (altezza, assumiamo Z verso il basso, quindi h = -pos.z) ----------
def compute_baro_innov_var_and_h(
    state: VState, P: MTangent, meas: sf.Scalar, R: sf.Scalar
) -> (sf.Scalar, sf.Scalar, VTangent):
    state = vstate_to_state(state)
    h_pred = -state["pos"][2]   # altezza = -z
    innov = h_pred - meas
    H = jacobian_chain_rule(h_pred, state)
    innov_var = (H * P * H.T + R)[0,0]
    return (innov, innov_var, H.T)

# ---------- Lidar (altezza, uguale al barometro ma con diverso rumore) ----------
def compute_lidar_innov_var_and_h(
    state: VState, P: MTangent, meas: sf.Scalar, R: sf.Scalar
) -> (sf.Scalar, sf.Scalar, VTangent):
    state = vstate_to_state(state)
    h_pred = -state["pos"][2]
    innov = h_pred - meas
    H = jacobian_chain_rule(h_pred, state)
    innov_var = (H * P * H.T + R)[0,0]
    return (innov, innov_var, H.T)

# ==============================================================================
# 7. Generazione del codice C++ (simulata)
# ==============================================================================

from code_generation import generate_cpp_function, generate_state_header

generate_cpp_function(predict_covariance, output_names=None)
generate_cpp_function(compute_gps_pos_innov_var_and_h, output_names=["innov", "innov_var", "Hx"])
generate_cpp_function(compute_gps_vel_innov_var_and_h, output_names=["innov", "innov_var", "Hx"])
generate_cpp_function(compute_mag_innov_var_and_h, output_names=["innov", "innov_var", "Hx"])
generate_cpp_function(compute_baro_innov_var_and_h, output_names=["innov", "innov_var", "H"])
generate_cpp_function(compute_lidar_innov_var_and_h, output_names=["innov", "innov_var", "H"])
generate_state_header(State, tangent_idx)

