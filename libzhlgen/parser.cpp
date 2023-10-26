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

    try {
        CodeEmitter emitter(test);
        emitter.ProcessZHLFiles(zhlPath);
        // emitter.ProcessFile("D:/Dev/Isaac/IsaacZHL/libzhlgen/IsaacRepentanceStripped.h");
        emitter.ProcessFile(extraTypesPath);
        emitter.Dump();
        emitter.Emit();

        if (ErrorsHolder::HasErrors()) {
            for (std::string const& err : ErrorsHolder::GetErrors()) {
                std::cerr << err << std::endl;
            }
        }
    }
    catch (std::exception& e) {
        std::ofstream s("parser.log", std::ios::out);
        s << e.what() << std::endl;
        s.close();
        throw;
    }

	return 0;
}
