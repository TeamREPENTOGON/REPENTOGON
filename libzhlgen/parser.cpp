#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "CodeEmitter.h"
#include "TypeMap.h"
#include "TypeValidator.h"

typedef std::map<std::string, std::vector<std::string>> Args;
typedef std::vector<std::string> FreeArgs;

/* Split arg on the first '=' character found outside of quoted string.
 * The part to the left is stored in lhs, the part to the right in rhs.
 * If no such '=' is found, lhs is set to arg and rhs is not changed.
 * Return true if such an '=' was found, false otherwise.
 */
bool SplitArg(char* arg, std::string& lhs, std::string& rhs) {
    int len = strlen(arg);
    bool inQuote = false;
    char quoteChar = '\0';
    bool isEscaped = false;

    for (int i = 0; i < len; ++i) {
        char c = arg[i];
        if (c == '\\') {
            isEscaped = !isEscaped;
        } else {
            if (c == '=') {
                if (!inQuote) {
                    arg[i] = '\0';
                    lhs = arg;
                    rhs = arg + i + 1;
                    arg[i] = '=';
                    return true;
                }
            } else if (inQuote && !isEscaped && quoteChar == c) {
                inQuote = false;
            } else if (c == '\'' || c == '"') {
                inQuote = true;
                quoteChar = c;
            }

            isEscaped = false;
        }
    }

    lhs = arg;
    return false;
}

void ParseArguments(int argc, char** argv, Args& args, FreeArgs& freeArgs) {
    bool expectingArgValue = false;
    std::string expectedArg;
    for (int i = 1; i < argc; ++i) {
        char* arg = argv[i];
        if (strstr(arg, "--") != arg) {
            if (expectingArgValue) {
                args[expectedArg].push_back(arg);
            } else {
                freeArgs.push_back(arg);
            }
        } else {
            arg += 2; // Ignore leading --
            std::string left, right;
            bool hasValue = SplitArg(arg, left, right);

            if (hasValue) {
                args[left].push_back(std::move(right));
            } else {
                expectingArgValue = true;
                expectedArg = std::move(left);
            }
        }
    }
}

void ReadOneArgument(Args const& args, std::string const& arg, std::string& value) {
    if (std::vector<std::string> const& values = args.find(arg)->second; !values.empty()) {
        value = values.front();
        if (values.size() > 1) {
            fprintf(stderr, "[WARN] Argument --%s given multiple times, ignoring repeats\n", arg.c_str());
        }
    }
}

static const std::string OutputHeader = "output-header";
static const std::string OutputImpl = "output-impl";
static const std::string OutputHooks = "output-hooks";
static const std::string Folder = "folder";
static const std::string File = "file";

int main(int argc, char** argv) {
    Args args;
    FreeArgs freeArgs;

    ParseArguments(argc, argv, args, freeArgs);

    std::string outputHeader = "ZHLInterface.h";
    std::string outputImpl = "ZHLInterface.cpp";
    std::string outputHooks = "ZHLHooks.json";

    ReadOneArgument(args, OutputHeader, outputHeader);
    ReadOneArgument(args, OutputImpl, outputImpl);
    ReadOneArgument(args, OutputHooks, outputHooks);

    try {
        TypeMap types;
        AsmDefMap asmDefs;
        CodeEmitter emitter(&types, &asmDefs, outputHeader, outputImpl, outputHooks);
        bool ok = true;
        for (std::string const& path : args[Folder]) {
            if (!emitter.ProcessZHLFiles(path)) {
                ok = false;
            }
        }

        for (std::string const& path : args[File]) {
            if (!emitter.ProcessFile(path)) {
                ok = false;
            }
        }

        if (!ok) {
            ParserLogger()->Fatal("Errors encountered while processing ZHL files\n");
            return -1;
        }

        TypeValidator validator(&types);
        if (validator.HasCycle()) {
            ParserLogger()->Fatal("Circular dependencies detected between structures\n");
            return -1;
        }

        std::map<std::string, ValidateTypeResult> validationResults = validator.ValidateTypes();

        bool validationOk = true;
        for (auto const& [name, validation] : validationResults) {
            if (validation != VALIDATE_TYPE_OK) {
                validationOk = false;
                break;
            }
        }

        if (!validationOk) {
            ParserLogger()->Fatal("Invalid structures detected\n\n");
            return -1;
        }

        emitter.Dump();
        emitter.Emit();
    }
    catch (std::exception& e) {
        ParserLogger()->Fatal("Exception raised while parsing ZHL files / emitting C++ code: %s\n", e.what());
        return -1;
    }

	return 0;
}
