import os

SCRIPT_PATH = os.path.realpath(__file__)
SOURCE_DIRECTORY = os.path.dirname(SCRIPT_PATH)

DOCS_FOLDER_PATH = os.path.join(SOURCE_DIRECTORY, "../docs/docs/")

docsCallback ={}
lastCallbackName = ""
lines = []
docFile = open(DOCS_FOLDER_PATH+"enums/ModCallbacks.md", 'r')
isIgnoringFilepart = True
for line in docFile:
    if isIgnoringFilepart:
        lines.append(line)
        if "## New Callbacks" in line:
            isIgnoringFilepart = False
    else:
        if "###" in line:
            callbackName = line.replace("###","").split("{")[0].strip()
            if callbackName not in docsCallback.keys():
                docsCallback[callbackName] = []
                lastCallbackName = callbackName
        elif lastCallbackName != "":
            docsCallback[lastCallbackName].append(line)

# Make sure content has correct layout
for key in docsCallback.keys():
    hasTable = False
    for line in docsCallback[key]:
        if "|ID|Name|Function Args|Optional Args|Return Type|" in line:
            hasTable= True
            break
    if not hasTable:
        print("Entry has no or wrong Table:", key)

    lastContentLine = docsCallback[key][len(docsCallback[key])-2]
    if "|" not in lastContentLine: # should have table at end
        print("Entry does not end with a Table:",key)
    if "{: .copyable }" not in lastContentLine.split("|")[2]: # has copy operator
        print("Table Entry does not have a copy operator:",key)
    argsLine= lastContentLine.split("|")[3].strip()
    args = argsLine.replace("<br>","").split(",")
    if args[0].startswith("("):
        args[0] = args[0][1:]
    if args[len(args)-1].endswith(")"):
        args[len(args)-1] = args[len(args)-1][:-1]

    formattedArgsLine = "("
    for arg in args:
        formattedArgsLine = formattedArgsLine + arg.strip()
        if arg != args[len(args)-1]:
            formattedArgsLine = formattedArgsLine + ", <br>"
    formattedArgsLine = formattedArgsLine+")"
    if "(-)" in formattedArgsLine or formattedArgsLine == "()":
        formattedArgsLine = "-"
    docsCallback[key][len(docsCallback[key])-2] = docsCallback[key][len(docsCallback[key])-2].replace(argsLine, formattedArgsLine)



# add sorting helper
tempList = []
for key in docsCallback.keys():
    trimStr = key.replace("MC_","")
    trimStr = trimStr.replace("PRE_","")+"__0" if trimStr.startswith("PRE_") else trimStr
    trimStr = trimStr.replace("POST_","")+"__1" if trimStr.startswith("POST_") else trimStr
    if "POST_" in trimStr or "PRE_" in trimStr:
        print("Possible wrong naming convention:",key)
    tempList.append(trimStr)

# sort entries        
tempList = sorted(tempList)

sortedCallbackNames = []
# undo sorting helpers
for cbName in tempList:
    if cbName.endswith("__0"):
        cbName = "PRE_"+cbName.replace("__0","")
    if cbName.endswith("__1"):
        cbName = "POST_"+cbName.replace("__1","")
    cbName = "MC_"+cbName
    sortedCallbackNames.append(cbName)

# apply new sorting and formatting
writeFile = open(DOCS_FOLDER_PATH+"enums/ModCallbacks.md", "w")
for line in lines:
    writeFile.write(line)

for key in sortedCallbackNames:
    writeFile.write("### "+key+ " {: .copyable }\n")
    for line in docsCallback[key]:
        writeFile.write(line)
    if docsCallback[key][len(docsCallback[key])-1].strip() != "":
        writeFile.write("\n")
writeFile.close()

            

