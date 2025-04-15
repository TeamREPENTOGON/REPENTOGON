#include <filesystem>
#include <fstream>
#include <iostream>

#include "CodeEmitter.h"
#include "TypeMap.h"
#include "TypeValidator.h"

int main(int argc, char** argv) {
    bool test = false;
    if (argc == 2) {
        test = strcmp(argv[1], "--test") == 0;
    }
    
    printf("%ls\n", std::filesystem::current_path().c_str());
    std::string zhlPath = "../libzhl/functions";
    std::string extraTypesPath = "../libzhl/functions/ExtraTypes";
    if (test) {
        zhlPath = "../REPENTOGON/libzhl/functions";
        extraTypesPath = "../REPENTOGON/libzhl/functions/ExtraTypes";
    }

    try {
        TypeMap types;
        CodeEmitter emitter(&types, test);
        bool ok = emitter.ProcessZHLFiles(zhlPath);
        // emitter.ProcessFile("D:/Dev/Isaac/IsaacZHL/libzhlgen/IsaacRepentanceStripped.h");
        ok = ok && emitter.ProcessFile(extraTypesPath);

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
