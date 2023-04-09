import os
import yaml
from yaml import Loader, Dumper
from slpp import slpp as lua

with open("../modtest/scripts/enums_ex.lua", "r") as enums:
    data = lua.decode("{" + enums.read() + "}")

with open("../documentation/mkdocs.yml") as cfg:
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
        with open(f"../documentation/docs/enums/{value}.md", "w+") as md:
            md.write(f'---\ntags:\n  - Enum\n---\n# Enum "{value}"\n|Value|Enumerator|Comment|\n|:--|:--|:--|\n');
            if type(data[value]) is dict:
               for i, enum in enumerate(data[value]):
                    md.write(f"|{data[value][enum]} |{enum} \u007b: .copyable \u007d |  |\n")
            else:
                md.write(f"|{data[value]} |{value} \u007b: .copyable \u007d |  |\n")
        new_mkdocs_enums.append({value: f"enums/{value}.md"})

mkdocs['nav'][1]['Docs'][1]['Enums'] = sorted(new_mkdocs_enums, key=lambda x: list(x.keys())[0])

print(mkdocs['nav'][1]['Docs'][1]['Enums'])
with open("../documentation/mkdocs.yml", 'w+') as cfg:
    cfg.write(yaml.dump(mkdocs))