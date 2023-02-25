import os
from slpp import slpp as lua

with open("../modtest/scripts/enums_ex.lua", "r") as enums:
    data = lua.decode("{" + enums.read() + "}")

mkdocs_augment = 'Replace the mkdocs.yml enums with the following:\n            - "ModCallbacks": enums/ModCallbacks.md\n'

for value in data:
    if not ".MC_" in value and not ".ALL" in value:
        with open(f"../documentation/docs/enums/{value}.md", "w+") as md:
            md.write(f'---\ntags:\n  - Enum\n---\n# Enum "{value}"\n|Value|Enumerator|Comment|\n|:--|:--|:--|\n');
            if type(data[value]) is dict:
                for i, enum in enumerate(data[value]):
                    md.write(f"|{data[value][enum]} |{enum} \u007b: .copyable \u007d |  |\n")
            else:
                md.write(f"|{data[value]} |{value} \u007b: .copyable \u007d |  |\n")
        mkdocs_augment = mkdocs_augment + f'            - "{value}": enums/{value}.md\n'

print("" + mkdocs_augment);