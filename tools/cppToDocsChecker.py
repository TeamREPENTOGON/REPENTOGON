import os
import glob
import re

BRANCH_NAME = "main-msvc"

SCRIPT_PATH = os.path.realpath(__file__)
SOURCE_DIRECTORY = os.path.dirname(SCRIPT_PATH)

DOCS_FOLDER_PATH = os.path.join(SOURCE_DIRECTORY, "../documentation/docs/")
CPP_FOLDER_PATH = os.path.join(SOURCE_DIRECTORY, "../repentogon/")

if "GITHUB_WORKSPACE" in os.environ:
    SOURCE_DIRECTORY = os.environ["GITHUB_WORKSPACE"]
    DOCS_FOLDER_PATH = os.path.join(SOURCE_DIRECTORY, "documentation/docs/")
    CPP_FOLDER_PATH = os.path.join(SOURCE_DIRECTORY, "repentogon/")

ignoreFunctionWords = ["Register","_Test","__gc", "Patch", "FixMusicManager", "lua_LGR_gc", "lua_Image_gc", "lua_Transformer_gc"]
ignoreFiles = ["LuaASM.cpp", "LuaInit.cpp"]
subFolders = [["Menu","menus"]]

parentClass = {
               "Ambush":"Game",
               "AnimationData":"Sprite",
               "Backdrop":"Room",
               "Camera": "Room",
               "Capsule": "Entity",
               "ChallengeParam": "Game",
               "ColorCorrction": "FXParams",
               "Console":"Game",
               "DailyChallenge":"Isaac",
               "DebugRenderer":"Game,Entity,Shape",
               "EntityPlayer":"PocketItem,MultiShotParams",
               "EntitySlot":"Entity",
               "FXParams": "Room",
               "GridEntity":"Isaac,GridEntityRock",
               "History":"EntityPlayer,HistoryItem",
               "ItemConfig":"ItemConfig_PillEffect",
               "ItemOverlay":"Game",
               "ImGui":"Isaac",
               "LocalizationStuff":"Isaac",
               "LevelGenerator": "LevelGeneratorRoom",
               "MenuManager":"Isaac",
               "Minimap":"Game",
               "NightmareScene":"Isaac",
               "NullFrame":"Sprite",
               "PersistentGameData":"Isaac",
               "PlayerHUD":"HUD",
               "PlayerManager":"Game",
               "ProceduralItemManager":"Game",
               "RoomTransition":"Game",
               "RoomConfigHolder":"Game",
               "ScoreSheet":"Game",
               "StageTransition":"Game",
               "Sprite":"LayerState",
               "Weapon":"Isaac,EntityPlayer",
               }


def searchInDocFile(docFilePath, luaFunctions):
    if os.path.isfile(docFilePath):
        docFile = open(docFilePath, 'r')
        for line in docFile:
            if "####" in line: # function header
                filterLinks = re.sub("\]\([a-zA-z0-9\.\/:]*html\)", "", line)
                filterMdLinks = re.sub("\]\(.*md\)", "", filterLinks).replace("[","")
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

    className = ""
    luaFunctions = []
    functionName = ""
    functionArgs = []
    functionOptArgs = []

    lastPushString = ""

    isInsideFunction = False
    for line in cppFile:
        if "(lua_State* L)" in line and not "/*" in line:
            # function body found
            isInsideFunction = True

            functionDef = line.split("(lua_State")[0]
            functionName = functionDef.split(" ")[len(functionDef.split(" "))-1]
        elif "LUA_FUNCTION(" in line:
            # function body found
            isInsideFunction = True
            functionName = line.split("(")[1].split(")")[0].strip()
        elif "lua::RegisterFunction" in line:
            # check if an existing function name was mentioned in a later line. this means the function got registered under a new name
            for func in luaFunctions:
                if func[0] in line:
                    func[0] = line.split("\"")[1]
        elif isInsideFunction:
            if "lua_pushstring" in line and len(line.split("\"")) > 1:
                lastPushString = line.split("\"")[1]
            # check if an existing function name was mentioned in a later line. this means the function got registered under a new name
            for func in luaFunctions:
                if func[0] in line:
                    if len(line.split("\"")) > 1:
                        func[0] = line.split("\"")[1]
                    else:
                        func[0] = lastPushString
                    lastPushString = ""

            # Handle function specific stuff
            if "luaL_check" in line:
                # args
                functionArgs.append(line.split("luaL_check")[1].replace("(L,","").split(")")[0].strip())
            elif "luaL_opt" in line:
                # optional args
                functionOptArgs.append(line.split("luaL_opt")[1].replace("(L,","").split(")")[0].strip())
            elif "lua::GetUserdata" in line and "(L, 1" not in line:
                # special args like vectors
                functionArgs.append(line.strip().split(" ")[0])
            elif "}" == line.strip() or "};" == line.strip():
                luaFunctions.append([functionName, functionArgs, functionOptArgs,False])

                functionName = ""
                functionArgs = []
                functionOptArgs = []
                isInsideFunction = False
    
    if className == "":
        className = os.path.basename(file).split("Lua")[1].split(".")[0]
    
    subFolder = ""
    for entry in subFolders:
        if entry[0] in className:
            subFolder = entry[1] + "/"
            break
    
    # search in own documentation and the parent documentation
    searchInDocFile(DOCS_FOLDER_PATH + subFolder+ className + ".md", luaFunctions)
    if className in parentClass:
        for parent in parentClass[className].split(","):
            searchInDocFile(DOCS_FOLDER_PATH + parent + ".md", luaFunctions)

    # Print missing matches
    for func in luaFunctions:
        for ignoreWord in ignoreFunctionWords:
            if ignoreWord in func[0]:
                func[3] = True
        
        if not func[3]:
            print("\t Missing: "+func[0]+ "\targs:"+str(func[1])+ "\topt-args:"+str(func[2]))
        
