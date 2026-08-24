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
    queue_len = None
    with open(path) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            parts = line.split()

            if line.startswith('QUEUE_LENGTH'):
                queue_len = parts[1]
                continue
            if len(parts) >= 2:
                typ, name = parts[0], parts[1]
                fields.append((TYPE_MAP.get(typ, typ), name))
    return fields, queue_len

def generate_header(msg_name, fields):
    topic_hash = hashlib.md5(msg_name.encode()).hexdigest()[:8]
    lines = []
    lines.append(f"// Auto-generated from {msg_name}.topic - DO NOT EDIT")
    lines.append(f"//                   Infinity-Autopilot")
    lines.append("#pragma once")
    lines.append("#include <cstdint>")
    lines.append("#include <cstddef>")
    lines.append("")
    lines.append(f"struct {msg_name}Data {{")
    lines.append("")
    
    for typ, name in fields:
        lines.append(f"    {typ} {name};")
    lines.append("};")
    return "\n".join(lines)

def generate_metadata(topic_name, queue_len):
    topic_hash = hashlib.md5(topic_name.encode()).hexdigest()[:8]
    lines = []
    
    lines.append("")
    lines.append(f"struct {topic_name}Metadata {{")
    lines.append(f"    static constexpr const char* TOPIC_NAME = \"{topic_name}\";")
    lines.append(f"    static constexpr size_t TOPIC_SIZE = sizeof({topic_name});")
    lines.append(f"    static constexpr uint32_t TOPIC_HASH = 0x{topic_hash};")
    if queue_len:
        lines.append(f"    static constexpr uint8_t QUEUE_LENGTH = {queue_len};")
    lines.append("};")
    return "\n".join(lines)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--input-dir', required=True)
    parser.add_argument('--output-dir', required=True)
    args = parser.parse_args()

    metadata = []

    metadata.append(f"// Auto-generated from topic folder - DO NOT EDIT")
    metadata.append(f"//                   Infinity-Autopilot")
    metadata.append("#pragma once")
    metadata.append("#include <cstdint>")
    metadata.append("#include <cstddef>")
    metadata.append("namespace srimb{")
    metadata.append("")

    os.makedirs(args.output_dir, exist_ok=True)

    for fname in os.listdir(args.input_dir):
        if not fname.endswith('.topic'):
            continue
        topic_name = fname[:-6]
        fields, queue_len = parse_msg_file(os.path.join(args.input_dir, fname))

        metadata.append(generate_metadata(topic_name, queue_len))

        header = generate_header(topic_name, fields)
        out_path = os.path.join(args.output_dir, f"{topic_name}.h")
        with open(out_path, 'w') as f:
            f.write(header)

    # Generate metadata
    metadata.append("")
    metadata.append("}")
    out_path = os.path.join(args.output_dir, f"TopicMetadata.h")
    with open(out_path, 'w') as f:
        f.write("\n".join(metadata))



if __name__ == '__main__':
    main()