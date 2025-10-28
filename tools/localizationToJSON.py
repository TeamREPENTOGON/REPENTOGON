# Converts .INL localization files to .JSON files for use in external translation tools
import re
import json
import os
from glob import glob

SCRIPT_PATH = os.path.realpath(__file__)
SOURCE_DIRECTORY = os.path.dirname(os.path.dirname(SCRIPT_PATH))
LOCALIZATION_DIRECTORY = SOURCE_DIRECTORY +"/repentogon/ImGuiFeatures/Localization"

def parse_file(filename):
    data = {}
    current_category = None

    with open(filename, encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            # Unterkategorie erkennen
            cat_match = re.match(r"//\s*=*\s*(.+)", line)
            if cat_match:
                current_category = cat_match.group(1).replace("=","").strip()
                if current_category not in data:
                    data[current_category] = {}
                continue

            # I() Funktion erkennen
            i_match_withIcon = re.match(r'I\(\s*(.+?)\s*,\s(ICON_.*)*(?:u8)*["\'](.+?)["\']\s*\)', line)
            i_match_noIcon = re.match(r'I\(\s*(.+?)\s*,[\s(u8)]*["\'](.+?)["\']\s*\)', line)
            if current_category:
                # Handle cases where icon is not present
                if i_match_noIcon:
                    key, value = i_match_noIcon.groups()
                    translatedWord = re.sub(r'\"\s{2,}u8\"', '', value)  # remove concatenation from string
                    data[current_category][key] = translatedWord.strip()
                elif i_match_withIcon:
                    key, icon, value = i_match_withIcon.groups()
                    icon = icon.replace(" u8","").strip()
                    value = icon + value
                    translatedWord = re.sub(r'\"\s{2,}u8\"', '', value)  # remove concatenation from string
                    data[current_category][key] = translatedWord.strip()

    # Remove empty categories
    data = {k: v for k, v in data.items() if v}
    return data

if __name__ == "__main__":
    inl_files = glob(os.path.join(LOCALIZATION_DIRECTORY, "*.inl"))

    for inl_file in inl_files:
        result = parse_file(inl_file)
        output_file = os.path.splitext(inl_file)[0] + ".json"
        with open(output_file, "w", encoding="utf-8") as f:
            json.dump(result, f, ensure_ascii=False, indent=2)
        print(f"'{output_file}' created.")