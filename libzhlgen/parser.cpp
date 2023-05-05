#include <filesystem>

#include "CodeEmitter.h"

int main(int argc, char** argv) {
    CodeEmitter emitter;
    emitter.ProcessZHLFiles("D:/Dev/Isaac/IsaacZHL/libzhl/functions");
    emitter.ProcessFile("D:/Dev/Isaac/IsaacZHL/libzhlgen/IsaacRepentanceStripped.h");
    emitter.ProcessFile("D:/Dev/Isaac/IsaacZHL/libzhl/functions/ExtraTypes");
    emitter.Dump();
    emitter.Emit();

	return 0;
}
