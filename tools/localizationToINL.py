# Converts JSON localization files to .INL files for use in the project.

import json
import os
import re
from glob import glob
SCRIPT_PATH = os.path.realpath(__file__)
SOURCE_DIRECTORY = os.path.dirname(os.path.dirname(SCRIPT_PATH))
LOCALIZATION_DIRECTORY = SOURCE_DIRECTORY +"/repentogon/ImGuiFeatures/Localization"

def json_to_inl(json_file, inl_file):
    with open(json_file, encoding="utf-8") as f:
        data = json.load(f)

    lines = []
    lines.append("/* This is saved as UTF-8 with BOM(code page 65001) */")
    lines.append("#ifndef I")
    lines.append("#define I(ITEM, TRANSLATE)")
    lines.append("#endif\n")
    lines.append("#ifndef V")
    lines.append("#define V(TYPE, VAR, VALUE)")
    lines.append("#endif\n")
    lines.append("/*")
    lines.append(" * note: Remove items inside \"en_us.inl\" may cause compile error, and you")
    lines.append(" * need also remove all of them in other language files.")
    lines.append(" */\n")

    for category, items in data.items():
        lines.append(f"// =========== {category} ===========")
        for key, value in items.items():
            # Move icon definition outside of the string definition if it exists
            value = value.replace("\"", "\\\"")
            value = value.replace("\n", "\\\n")
            if "ICON_" in value:
                icon_match = re.search(r'(ICON_[A-Z0-9_]+)', value)
                if icon_match:
                    icon = icon_match.group(1)
                    value = value.replace(icon, "")
                    lines.append(f'I({key}, {icon} u8"{value}")')
                else:
                    lines.append(f'I({key}, u8"{value}")')
        # Handle cases without icon
            else:
                lines.append(f'I({key}, u8"{value}")')
        lines.append("")  # Blank line after each category

    with open(inl_file, "w", encoding="utf-8-sig") as f:
        f.write("\n".join(lines))
    print(f"'{inl_file}' created.")

if __name__ == "__main__":
    json_files = glob(os.path.join(LOCALIZATION_DIRECTORY, "*.json"))
    for json_file in json_files:
        inl_file = os.path.splitext(json_file)[0] + ".inl"
        json_to_inl(json_file, inl_file)