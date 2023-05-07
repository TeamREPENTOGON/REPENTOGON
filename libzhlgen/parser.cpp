#include <filesystem>

#include "CodeEmitter.h"

int main(int argc, char** argv) {
    bool test = false;
    if (argc == 2) {
        test = strcmp(argv[1], "--test") == 0;
    }

    std::string zhlPath = "../libzhl/functions";
    std::string extraTypesPath = "../libzhl/functions/ExtraTypes";
    if (test) {
        zhlPath = "../../libzhl/functions";
        extraTypesPath = "../../libzhl/functions/ExtraTypes";
    }

    CodeEmitter emitter(test);
    emitter.ProcessZHLFiles(zhlPath);
    // emitter.ProcessFile("D:/Dev/Isaac/IsaacZHL/libzhlgen/IsaacRepentanceStripped.h");
    emitter.ProcessFile(extraTypesPath);
    emitter.Dump();
    emitter.Emit();

	return 0;
}
