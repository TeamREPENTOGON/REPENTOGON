import os
import glob
import re

BRANCH_NAME = "main-msvc"

SCRIPT_PATH = os.path.realpath(__file__)
SOURCE_DIRECTORY = os.path.dirname(SCRIPT_PATH)

DOCS_FOLDER_PATH = os.path.join(SOURCE_DIRECTORY, "../docs/docs/")
CPP_FOLDER_PATH = os.path.join(SOURCE_DIRECTORY, "../repentogon/")

if "GITHUB_WORKSPACE" in os.environ:
    SOURCE_DIRECTORY = os.environ["GITHUB_WORKSPACE"]
    DOCS_FOLDER_PATH = os.path.join(SOURCE_DIRECTORY, "docs/docs/")
    CPP_FOLDER_PATH = os.path.join(SOURCE_DIRECTORY, "repentogon/")

ignoreLuaFunctions = ["lua_rawgeti", "luaL_argerror", "lua_type", "lua_next","lua_isnil", "lua_isinteger", "lua_isboolean", "lua_isnoneornil"]
ignoreFunctionNames = ["__index", "__newindex", "__call", "__len", "__gc"]
ignoreFiles = ["LuaASM.cpp", "LuaInit.cpp", "UNUSED"]

cppMetatableToDocumentation = { # lookup table to convert metatable names into documentation file names
    "pilleffect": "ItemConfig_PillEffect",
    "constpilleffect": "ItemConfig_PillEffect",
    "render": "Renderer"
}

luaCallToDataType = {
    "luaL_checkstring": "string",
    "luaL_optstring": "string",
    "luaL_checkinteger": "int",
    "luaL_optinteger": "int",
    "lua_tointegerx": "int",
    "luaL_checknumber": "float",
    "luaL_optnumber": "float",
    "lua_toboolean": "boolean",
    "luaL_checkboolean": "boolean",
    "luaL_optboolean": "boolean",
    "lua_isfunction": "function",
    "CheckAndSetCallback": "function",
    "lua_istable": "table",
    "lua_gettable":"table",
    "lua_rawlen": "table",
}
luaReturnDataType = {
    "lua_pushinteger": "int",
    "lua_pushnumber": "float",
    "lua_pushstring": "string",
    "lua_pushboolean": "boolean"
}


def searchInDocFile(docFilePath, luaFunctions):
    if os.path.isfile(docFilePath):
        docFile = open(docFilePath, 'r')
        for line in docFile:
            if "####" in line: # function header
                filterLinks = re.sub("\]\([a-zA-Z0-9\.\/:_]*html\)", "", line)
                filterMdLinks = re.sub("\]\([a-zA-Z0-9\.\/:_]*md\)", "", filterLinks).replace("[const ","[").replace("[","")
                filteredLine = filterMdLinks.split("####")[1].split("{:")[0].replace(")","").strip()
                lineSplit = filteredLine.split(" (")

                returnType = lineSplit[0].split(" ")[0]
                functionName = lineSplit[0].split(" ")[1]
                
                arguments = []

                if len(lineSplit) >1:
                    args = filteredLine.split(" (")[1].strip()
                    if args != "":
                        for pair in args.split(","):
                            cleanPair = pair.strip()
                            argType = cleanPair.split(" ")[0]
                            arguments.append(argType)
                

                # compare with CPP content
                for func in luaFunctions:
                    if func[0] == functionName:
                        func[3] = True
    else:
        print("###### No doc file found for file: "+docFilePath)

for file in glob.glob(CPP_FOLDER_PATH+"\**\Lua*.cpp", recursive=True):
    skip = False
    for ignoreFile in ignoreFiles:
        if ignoreFile in file:
            skip = True
    if skip: 
        continue

    print(os.path.abspath(file))
    cppFile = open(os.path.abspath(file), 'r', encoding="utf-8")

    referencedClasses = []
    referencedClasses.append(os.path.basename(file).split("Lua")[1].split(".")[0].lower())
    luaFunctions = []
    functionName = ""
    functionArgs = []
    functionOptArgs = []
    returnValue = "void"

    lastPushString = ""

    isInsideFunction = False
    functionNestingCount = 0
    for line in cppFile:
        if "(lua_State* L)" in line and not "/*" in line: # possible lua-related function that doesnt use macro
            functionDef = line.split("(lua_State")[0]
            functionName = functionDef.split(" ")[len(functionDef.split(" "))-1]
            if not functionName.startswith("Register") and "int" in line and not "inline" in line:
                isInsideFunction = True
                functionNestingCount = functionNestingCount + line.count('{')
                print("\t WARNING: Lua function found that doesn't use LUA_FUNCTION() macro: ", functionName)
        elif "LUA_FUNCTION(" in line and not "/*" in line: # lua-related function starts
            isInsideFunction = True
            functionNestingCount = functionNestingCount + line.count('{')
            functionName = line.split("(")[1].split(")")[0].strip()
        elif isInsideFunction:
            functionNestingCount = functionNestingCount + line.count('{') - line.count('}') # handle nesting
            # find return value
            if "luaL_setmetatable" in line or "UserdataPtr::push" in line:
                returnValue = line.split(",")[-1].split(")")[0].replace("\"","").replace("MT","").split(":")[-1].strip()
            else:
                for returnCommand in luaReturnDataType: # look for typical return values
                    if returnCommand in line:
                        returnValue = luaReturnDataType[returnCommand]
            
            # Handle function arguments and stuff
            if re.search("\((\s)*L,(\s)*([2-9]|\d{2,})", line): # access to lua stack; pattern searches for (L, [NUMBER bigger 1] 
                isIgnore = False
                for ignoreWord in ignoreLuaFunctions: # ignore arg error handling defs
                    if ignoreWord in line:
                        isIgnore = True
                        break
                if isIgnore:
                    continue

                variableType = ""
                variableName = "var"
                for luaType in luaCallToDataType:
                    if luaType in line:
                        variableType = luaCallToDataType[luaType]
                        break
                if "GetUserdata<" in line: # extract variableType from userdata
                    variableType = line.split("GetUserdata<")[1].split(">")[0].replace("*","")
                if "=" in line:
                    varDefinition = line.split("=")[0].strip().split(" ")
                    variableType = varDefinition[0].replace("*", "")
                    variableName = varDefinition[-1]
                if "luaL_opt" in line: # optional Argument
                    lastArgument = line.strip()[:-1].split(",")[-1]
                    
                    valueOfLastArgument = re.match("(\(.*?\))?(.*?)\)", lastArgument)
                    defaultVal = valueOfLastArgument.groups()[-1]

                    functionOptArgs.append(variableType + " " + variableName + " = " + defaultVal)
                else: # required Argument
                    functionArgs.append(variableType + " " + variableName)
                if variableType == "":
                    print("\t ERROR: no variable type found in line: ", line.strip())

            elif functionNestingCount == 0: # function ends. save collected data
                luaFunctions.append([functionName, functionArgs, functionOptArgs, False, referencedClasses[0], returnValue])

                functionName = ""
                functionArgs = []
                functionOptArgs = []
                isInsideFunction = False
                returnValue = "void"
        else:          
            if "lua_pushstring" in line and '"' in line: # Meta-Table entry defined with a given name
                lastPushString = line.split("\"")[1]
            if "lua::RegisterNewClass" in line or "lua::RegisterFunctions" in line:
                registerToClass = line.split(",")[1].strip() # second arg is always the metatable
                registerToClass = registerToClass.replace("\"","").replace("_","").replace("MT","").split(":")[-1] # clean up inconsistant metatable defs
                referencedClasses.append(registerToClass.lower())
            for func in luaFunctions: # Try match function name with lua name  
                if func[0] in line: # function was mentioned in this line
                    if "lua::Register" in line: # function was registered to a specific class
                        registerToClass = line.split(",")[1].strip() # second arg is always the metatable
                        registerToClass = registerToClass.replace("\"","").replace("_","").replace("MT","").split(":")[-1] # clean up inconsistant metatable defs
                        func[4] = registerToClass.lower()
                        referencedClasses.append(registerToClass.lower())
                    textPart = line.split("\"")
                    if len(textPart) > 2:
                        func[0] = textPart[-2]
                    elif lastPushString != "":
                        func[0] = lastPushString
    
    # possibly replace class names with documentation file names
    for i, className in enumerate(referencedClasses):
        if className in cppMetatableToDocumentation:
            referencedClasses[i] = cppMetatableToDocumentation[className].lower()

    # search in own documentation and the parent documentation
    for file in glob.glob(DOCS_FOLDER_PATH+"\**\*.md", recursive=True):
        filename = file.split("\\")[-1].replace(".md","").lower()
        if filename in referencedClasses:
            searchInDocFile(file, luaFunctions)

    # Print missing matches
    for func in luaFunctions:
        if func[0] in ignoreFunctionNames or func[0].endswith("_gc") or func[0].endswith("_Constructor"):
            continue
        if not func[3]:
            print("\t Missing: "+func[4]+"::"+func[0]+ "\targs:"+str(func[1])+ "\topt-args:"+str(func[2]))

            if func[0].lower().startswith("lua_"):
                # dont generate Docs of unmaped functions for now
                continue

            # generate Documentation entry
            args = str(func[1]).replace("[","").replace("]","").replace("'","")
            if len(func[1]) > 0 and len(func[2])>0:
                args += ", " 
            args+= str(func[2]).replace("[","").replace("]","").replace("'","")

            returnVal = func[5]
            if returnVal != "void" and returnVal != "int" and returnVal != "float" and returnVal != "string" and returnVal != "boolean":
                returnVal = "["+func[5]+"]("+func[5]+".md)"
            if returnVal == "void" and "get" in func[0].lower():
                returnVal = "<UNDEFINED>"

            titleStr = "### "+func[0]+ " () {: aria-label='Functions' }"
            funcStr = ("#### "+returnVal+" "+func[0]+ " ( "+args+" ) {: .copyable aria-label='Functions' }").replace("  "," ")

            print(titleStr)
            print(funcStr)
            print("\n___")
        
