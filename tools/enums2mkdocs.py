import os
import yaml
from yaml import Loader, Dumper
from slpp import slpp as lua
import subprocess

BRANCH_NAME = "main-msvc"

SCRIPT_PATH = os.path.realpath(__file__)
SOURCE_DIRECTORY = os.path.dirname(SCRIPT_PATH)

ENUM_FILE_PATH = os.path.join(SOURCE_DIRECTORY, "../repentogon/resources/scripts/enums_ex.lua")
DOCS_FOLDER_PATH = os.path.join(SOURCE_DIRECTORY, "../documentation/")

if "GITHUB_WORKSPACE" in os.environ:
    SOURCE_DIRECTORY = os.environ["GITHUB_WORKSPACE"]
    ENUM_FILE_PATH = os.path.join(SOURCE_DIRECTORY, "repentogon/resources/scripts/enums_ex.lua")
    DOCS_FOLDER_PATH = os.path.join(SOURCE_DIRECTORY, "documentation/")


with open(ENUM_FILE_PATH, "r") as enums:
    data = lua.decode("{" + enums.read() + "}")

with open(os.path.join(DOCS_FOLDER_PATH, "mkdocs.yml")) as cfg:
    mkdocs = yaml.load(cfg, Loader)

new_mkdocs_enums = [
    {"ModCallbacks": "enums/ModCallbacks.md"},
]

blocked_values = [".MC_", "colorPresets", "Color(", "function", "rawset", "pairs", "0xffffffff", ".Set", "Vector", "= {", "SetColorize", ".ALL"]

for value in data:
    beaned = False
    for subvalue in blocked_values:
        if isinstance(value, int) or subvalue.lower() in value.lower():
            beaned = True
    if not beaned: 
        with open(os.path.join(DOCS_FOLDER_PATH, f"docs/enums/{value}.md"), "w+") as md:
            md.write(f'---\ntags:\n  - Enum\n---\n# Enum "{value}"\n|Value|Enumerator|Comment|\n|:--|:--|:--|\n')
            enumLinesStart = 0
            enumLinesEnd = 0
            with open(ENUM_FILE_PATH, "r") as enums:
                for l_no, line in enumerate(enums):
                    if f"{value} = " in line:
                        enumLinesStart = l_no
                    if enumLinesStart > 0 and "}" in line:
                        enumLinesEnd = l_no
                        break

            if type(data[value]) is dict:
                for i, enum in enumerate(data[value]):
                    # get comment from line
                    comment = ""
                    with open(ENUM_FILE_PATH, "r") as enums:
                        for l_no, line in enumerate(enums):
                            if l_no > enumLinesStart and l_no < enumLinesEnd and "--" in line and f"\t{enum} = " in line:
                                comment = line.split("--")[1].strip().replace("|","&#124;") # get comment and escape pipe characters
                    # write table entry
                    md.write(f"|{data[value][enum]} |{enum} \u007b: .copyable \u007d | {comment} |\n")
            else:
                md.write(f"|{data[value]} |{value} \u007b: .copyable \u007d |  |\n")
        new_mkdocs_enums.append({value: f"enums/{value}.md"})

mkdocs['nav'][1]['Docs'][1]['Enums'] = sorted(new_mkdocs_enums, key=lambda x: list(x.keys())[0])

print(mkdocs['nav'][1]['Docs'][1]['Enums'])
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
