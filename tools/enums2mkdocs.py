import os
import sys
import yaml
from yaml import Loader, Dumper
import subprocess

BRANCH_NAME = "main"

SCRIPT_PATH = os.path.realpath(__file__)
SOURCE_DIRECTORY = os.path.dirname(SCRIPT_PATH)

ENUM_FILE_PATH = os.path.join(SOURCE_DIRECTORY, "../repentogon/resources/scripts/enums_ex.lua")
DOCS_FOLDER_PATH = os.path.join(SOURCE_DIRECTORY, "../docs/")

if "GITHUB_WORKSPACE" in os.environ:
    SOURCE_DIRECTORY = os.environ["GITHUB_WORKSPACE"]
    ENUM_FILE_PATH = os.path.join(SOURCE_DIRECTORY, "repentogon/resources/scripts/enums_ex.lua")
    DOCS_FOLDER_PATH = os.path.join(SOURCE_DIRECTORY, "docs/")


data = []

with open(ENUM_FILE_PATH, "r") as enums:

    for enum in enums.readlines():
        enum = enum.strip().split("--")
        content = enum[0].strip()
        content = content.split(", ")
        if len(enum) > 1:
            comment = enum[1].lstrip()
            content[len(content) - 1] = content[len(content) - 1] + " -- " + comment
        for entry in content:
            data.append(entry)    

with open(os.path.join(DOCS_FOLDER_PATH, "mkdocs.yml")) as cfg:
    mkdocs = yaml.load(cfg, Loader)

new_mkdocs_enums = [
    {"ModCallbacks": "enums/ModCallbacks.md"},
    {"AnimRenderFlags": "enums/AnimRenderFlags.md"},
    {"GLSLType": "enums/GLSLType.md"},
    {"ShaderType": "enums/ShaderType.md"}
]

blocked_values = [".MC_", "colorPresets", "Color(", "function", "rawset", "pairs", "0xffffffff", ".ALL"]
banned_classes = ["AnimRenderFlags"]

filtered_data = {}
bitwise_classes = []
current_class = ""

for value in data:
    beaned = False
    for subvalue in blocked_values:
        if isinstance(value, int) or subvalue.lower() in value.lower() or value.startswith(" --") or value == "" or value == "end" or value == "}":
            beaned = True
    if not beaned:
        if value.replace(" ", "").replace("--bitwise","").endswith("={"):
            current_class = value.split()[0]
            if current_class not in banned_classes:
                filtered_data[current_class] = []
                if value.replace(" ", "").endswith("--bitwise"):
                    bitwise_classes.append(current_class)
        elif current_class not in banned_classes:
            entry = {}
            value = value.split("=", 1)
            entry["name"] = value[0].strip()

            if len(value[1].split("--")) > 1:
                entry["value"] = value[1].split(",")[0].split("--")[0].replace(",", "").strip()
                entry["comment"] = value[1].split("--")[1].strip()
            else:
                entry["value"] = value[1].replace(",", "").strip()

            if "x" in entry["value"]:
                entry["value"] = int(entry["value"], 16)
            filtered_data[current_class].append(entry)


for class_name in filtered_data:
    with open(os.path.join(DOCS_FOLDER_PATH, f"docs/enums/{class_name}.md"), "w+") as md:
        md.write(f"---\ntags:\n  - Enum\n---\n# Enum \"{class_name}\"\n")
        if class_name in bitwise_classes:
            md.write("???+ tip \"Bitset Calculator\"\n    [](#){: .bitsetCalculator }\n")
        md.write("|Value|Enumerator|Comment|\n|:--|:--|:--|\n")

        for entry in filtered_data[class_name]:
            if "comment" in entry:
                md.write(f"|{entry['value']} |{entry['name']} \u007b: .copyable \u007d | {entry['comment'].replace('|', '&#124;')} |\n")
            else:
                md.write(f"|{entry['value']} |{entry['name']} \u007b: .copyable \u007d |  |\n")
    new_mkdocs_enums.append({class_name: f"enums/{class_name}.md"})

mkdocs['nav'][2]['Docs'][3]['Enums'] = sorted(new_mkdocs_enums, key=lambda x: list(x.keys())[0])

with open(os.path.join(DOCS_FOLDER_PATH, "mkdocs.yml"), 'w+') as cfg:
    cfg.write(yaml.dump(mkdocs))


# Try push changed doc files to git repo
def is_git_clean():
    stdout_bytes = subprocess.check_output(["git", "status", "--short"])
    stdout = stdout_bytes.decode("utf-8")
    trimmed_stdout = stdout.strip()
    return len(trimmed_stdout) == 0

if "GITHUB_WORKSPACE" in os.environ and not is_git_clean():
    subprocess.run(["git", "add", "--all"])
    subprocess.run(["git", "commit", "--message", f"chore: updated enum doc files"])
    subprocess.run(["git", "push", "--set-upstream", "origin", BRANCH_NAME])
