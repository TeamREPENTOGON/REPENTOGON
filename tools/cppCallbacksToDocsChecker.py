import os
import glob
import re

BRANCH_NAME = "main-msvc"

SCRIPT_PATH = os.path.realpath(__file__)
SOURCE_DIRECTORY = os.path.dirname(SCRIPT_PATH)

DOCS_FOLDER_PATH = os.path.join(SOURCE_DIRECTORY, "../docs/docs/")
CPP_FOLDER_PATH = os.path.join(SOURCE_DIRECTORY, "../repentogon")

IGNORE_DUPLICATE_CB_ID = [1006]


filesToSearch = glob.glob(CPP_FOLDER_PATH+"\**\**.cpp", recursive=True)
filesToSearch.extend(glob.glob(CPP_FOLDER_PATH+"\**\**.h", recursive=True))

callbacks = []
for file in filesToSearch:
    cppFile = open(os.path.abspath(file), 'r', encoding="utf-8")

    lines = cppFile.readlines()

    lastInt = 0
    for i in range(len(lines)):
        line = lines[i]
        if "HOOK_"in line and "_CALLBACKS(" in line and not "#" in line:
            splits = line.split(")")[0].split(",")
            for split in splits:
                if split.strip().isnumeric():
                    callbacks.append(int(split.strip()))

        if "int callbacks[" in line or ("callbacks[" in line and "struct" in line):
            curLineIndex = i
            searchLine = lines[curLineIndex]
            while curLineIndex < len(lines)-1:
                if "{" in searchLine:
                    searchLine = searchLine.split("{")[1]
                splits = searchLine.split("}")[0].split(",")
                for split in splits:
                    if split.strip().isnumeric():
                        callbacks.append(int(split.strip()))

                if searchLine.strip().endswith("};"):
                    break
                curLineIndex = curLineIndex + 1
                searchLine = lines[curLineIndex]

        if "CallbackState.test(" in line:
            splits = line.split(".test(")
            checkIndex = 1
            while checkIndex < len(splits):
                callbackIDPlaceholder = splits[checkIndex].split("-")[0].strip()
                callbackID = int(callbackIDPlaceholder) if callbackIDPlaceholder.isnumeric() else -1
                curLineIndex = i
                while callbackID == -1 and curLineIndex >0:
                    searchLine = lines[curLineIndex]
                    if searchLine.strip().startswith("const int") and callbackIDPlaceholder in searchLine:
                        callbackID = int(searchLine.split("=")[1].split(";")[0].strip())
                    curLineIndex = curLineIndex-1
                if callbackID == -1:
                    print(os.path.abspath(file))
                    print("\tCould not find callbackID for line "+str(i), line)
                callbacks.append(callbackID)
                checkIndex += 2

callbacks = list(set(callbacks)) # make unique

# Check lua file and compare with C++ callback definitions
luaCallbacks = {}
luaFile = open(os.path.abspath(CPP_FOLDER_PATH+"\\resources\scripts\enums_ex.lua"), 'r', encoding="utf-8")
for line in luaFile:
    if "ModCallbacks." in line and "--" not in line:
        cbInt = int(line.split("=")[1].split("-")[0].strip())

        if cbInt in luaCallbacks.values() and not cbInt in IGNORE_DUPLICATE_CB_ID:
            print("Duplicated Callback ID found in lua:", cbInt)
        if cbInt not in callbacks:
            print("Callback found in lua, but not in cpp file:", cbInt)

        luaCallbacks[line.split("=")[0].split(".")[1].strip()] = cbInt

# Reverse: Check if C++ callbacks are missing in lua file
print("")
for cb in callbacks:
    foundCb = False
    for luaCb in luaCallbacks.values():
        if cb == luaCb:
            foundCb = True
            break
    if not foundCb and cb != -1:
        print("Callback found in cpp, but not in lua file:", cb)

# Compare Lua callbacks with documentation
print("")
docsCallback =[]
docFile = open(DOCS_FOLDER_PATH+"enums/ModCallbacks.md", 'r')
for line in docFile:
    if "###" in line:
        callbackName = line.replace("###","").split("{")[0].strip()
        docsCallback.append(callbackName)
        if callbackName not in luaCallbacks:
            print("Callback found in docs, but not in lua file:", callbackName)
            
# Reverse: Check if Lua callbacks are missing in docs file
print("")
for cb in luaCallbacks:
    if cb not in docsCallback:
        print("Callback found in lua, but not in docs file:", cb, luaCallbacks[cb])

