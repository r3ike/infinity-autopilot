#!/usr/bin/env python3
import os
import sys
import argparse
import hashlib

TYPE_MAP = {
    'uint8': 'uint8_t',
    'uint16': 'uint16_t',
    'uint32': 'uint32_t',
    'uint64': 'uint64_t',
    'int8': 'int8_t',
    'int16': 'int16_t',
    'int32': 'int32_t',
    'int64': 'int64_t',
    'float32': 'float',
    'float64': 'double',
}

def parse_msg_file(path):
    fields = []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            parts = line.split()
            if len(parts) >= 2:
                typ, name = parts[0], parts[1]
                fields.append((TYPE_MAP.get(typ, typ), name))
    return fields

def generate_header(msg_name, fields):
    topic_hash = hashlib.md5(msg_name.encode()).hexdigest()[:8]
    lines = []
    lines.append(f"// Auto-generated from {msg_name}.msg - DO NOT EDIT")
    lines.append("#pragma once")
    lines.append("#include <cstdint>")
    lines.append("#include <cstddef>")
    lines.append("")
    lines.append(f"struct {msg_name} {{")
    lines.append(f"    static constexpr const char* TOPIC_NAME = \"{msg_name}\";")
    lines.append(f"    static constexpr size_t TOPIC_SIZE = sizeof({msg_name});")
    lines.append(f"    static constexpr uint32_t TOPIC_HASH = 0x{topic_hash};")
    lines.append("")
    for typ, name in fields:
        lines.append(f"    {typ} {name};")
    lines.append("};")
    return "\n".join(lines)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--input-dir', required=True)
    parser.add_argument('--output-dir', required=True)
    args = parser.parse_args()

    os.makedirs(args.output_dir, exist_ok=True)

    for fname in os.listdir(args.input_dir):
        if not fname.endswith('.topic'):
            continue
        msg_name = fname[:-4]
        fields = parse_msg_file(os.path.join(args.input_dir, fname))
        header = generate_header(msg_name, fields)
        out_path = os.path.join(args.output_dir, f"{msg_name}.h")
        with open(out_path, 'w') as f:
            f.write(header)

if __name__ == '__main__':
    main()