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
    topic_name = None
    with open(path) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            parts = line.split()

            if line.startswith('QUEUE_LENGTH'):
                queue_len = parts[1]
                continue
            if line.startswith('TOPIC_NAME'):
                topic_name = parts[1]
                continue

            if len(parts) >= 2:
                typ, name = parts[0], parts[1]
                fields.append((TYPE_MAP.get(typ, typ), name))
    return fields, queue_len, topic_name

def generate_header(data_struct_name, fields):
    topic_hash = hashlib.md5(data_struct_name.encode()).hexdigest()[:8]
    lines = []
    lines.append(f"// Auto-generated from {data_struct_name}.topic - DO NOT EDIT")
    lines.append(f"//                   Infinity-Autopilot")
    lines.append("#pragma once")
    lines.append("#include <cstdint>")
    lines.append("#include <cstddef>")
    lines.append("")
    lines.append(f"struct {data_struct_name}Data {{")
    lines.append("")
    
    for typ, name in fields:
        lines.append(f"    {typ} {name};")
    lines.append("};")
    return "\n".join(lines)

def generate_metadata(struct_name, queue_len, topic_name):
    
    srimb_name = topic_name if topic_name else struct_name
    
    topic_hash = hashlib.md5(srimb_name.encode()).hexdigest()[:8]
    lines = []
    
    lines.append("")
    lines.append(f"struct {struct_name}Metadata {{")
    lines.append(f"    static constexpr const char* TOPIC_NAME = \"{srimb_name}\";")
    lines.append(f"    static constexpr size_t TOPIC_SIZE = sizeof({srimb_name});")
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

    # header of TopicTypes.h file
    aliases = []
    aliases.append(f"// Auto-generated from topic folder - DO NOT EDIT")
    aliases.append(f"//                   Infinity-Autopilot")
    aliases.append("#pragma once")
    aliases.append("#include <cstdint>")
    aliases.append("#include <cstddef>")
    # Includi il file che definisce SRIMBQueueTopic (assumendo che sia in un header separato)
    aliases.append('#include "SRIMBQueueTopic.hpp"')
    aliases.append('#include "SRIMB.hpp"')
    aliases.append('#include "data_types/DataTypes.h"')
    
    # Includi il file TopicMetadata.h (serve per avere le costanti QUEUE_LENGTH)
    aliases.append('#include "TopicMetadata.h"')
    aliases.append("")
    aliases.append("namespace srimb {")
    aliases.append("")

    #header of DataTypes inclde file
    data_types_lines = []
    data_types_lines.append(f"// Auto-generated from topic folder - DO NOT EDIT")
    data_types_lines.append(f"//                   Infinity-Autopilot")
    data_types_lines.append("#pragma once")  

    
    srimb_topics_dir = os.path.join(args.output_dir,"srimb_topics")
    data_types_dir = os.path.join(args.output_dir,"data_types")
    os.makedirs(srimb_topics_dir, exist_ok=True)
    os.makedirs(data_types_dir, exist_ok=True)

    for fname in os.listdir(args.input_dir):
        if not fname.endswith('.topic'):
            continue
        struct_name = fname[:-6]
        fields, queue_len, topic_name = parse_msg_file(os.path.join(args.input_dir, fname))

        metadata.append(generate_metadata(struct_name, queue_len, topic_name))

        data_types_lines.append(f'#include "{struct_name}.h"')

        if queue_len:
            aliases.append(f'using {topic_name if topic_name else struct_name} = SRIMBQueueTopic<{struct_name}Data, {struct_name}Metadata::QUEUE_LENGTH>')
        else:
            aliases.append(f'using {topic_name if topic_name else struct_name} = SRIMBTopic<{struct_name}Data>')


        header = generate_header(struct_name, fields)
        out_path = os.path.join(data_types_dir, f"{struct_name}.h")
        with open(out_path, 'w') as f:
            f.write(header)

    # Generate metadata
    metadata.append("")
    metadata.append("}")

    out_path = os.path.join(srimb_topics_dir, f"TopicMetadata.h")
    with open(out_path, 'w') as f:
        f.write("\n".join(metadata))

    out_path = os.path.join(data_types_dir, f"DataTypes.h")
    with open(out_path, 'w') as f:
            f.write("\n".join(data_types_lines))

    aliases.append("")
    aliases.append("}")
    out_path = os.path.join(srimb_topics_dir, f"TopicTypes.h")
    with open(out_path, 'w') as f:
            f.write("\n".join(aliases))



if __name__ == '__main__':
    main()