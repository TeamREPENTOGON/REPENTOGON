import os
import glob
import re

BRANCH_NAME = "main-msvc"

GHIDRA_FILEPATH = "C:\\Users\\Jan-m\\Downloads\\isaac-ng.exe.unpacked_dump_179b.exe.h"

blockSizes = {
    "char": 1,
    "int": 4,
    "bool": 1,
    "short": 4,
    "long": 4,
    "float": 4,
    "longlong": 4,
    "double": 4,
    "undefined1":1,
    "structstd::string": 24,
    "pointer32": 32
}

classToExport = input("Enter a class you want to export: ")

file = open(os.path.abspath(GHIDRA_FILEPATH), 'r', encoding="utf-8")
isInStruct = False
hexValue = 0
for line in file:
    line = line.strip()
    # collect size of data types
    if "typedef" in line and not "struct" in line and not "union" in line and not "enum" in line:
        line = line.replace(";","").replace("typedef ","").replace("unsigned","")
        defs = line.split(" ")
        sourceType = ""
        for i in range(0, len(defs)-1):
            sourceType += defs[i]
        targetType = defs[len(defs)-1]
        if sourceType in blockSizes:
            blockSizes[targetType] = blockSizes[sourceType]
        elif "*" in sourceType: # pointers always size 4
            blockSizes[targetType] = 4
        else:
            blockSizes[targetType] = 4
            print("Blocksize not defined for '"+targetType+"'. Using size 4.")

    # Iterate over struct with given classname and export it
    if "struct" in line and classToExport in line and not "typedef" in line:
        isInStruct = True
        print("struct "+classToExport+" {")
        continue
    if isInStruct:
        if "}" in line:
            print("} : "+str(hex(hexValue))+";")
            break # struct ended. stop further processing 
        line = line.split(";")[0]
        defs = line.split(" ")
        sourceType = ""
        sourceTypeSplit = ""
        argName = defs[len(defs)-1].replace("?","")
        for i in range(0, len(defs)-1):
            sourceType += defs[i]
            sourceTypeSplit += defs[i] + " "
        
        print("    "+sourceTypeSplit+argName+" : "+str(hex(hexValue))+";")

        if "*" in sourceType: # pointers always size 4
            hexValue += 4
        elif sourceType in blockSizes:
            hexValue += blockSizes[sourceType]
        else:
            print("//     Datatype is not supported right now:", sourceTypeSplit)
        