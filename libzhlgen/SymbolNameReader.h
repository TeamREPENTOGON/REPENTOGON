#pragma once

#include <string>
#include <vector>

struct SymbolName {
    std::string _mangledName;
    std::string _raw;
    std::string _visibility;
    std::string _qualifier;
    std::string _returnType;
    std::string _callingConvention;
    std::string _name;
    std::vector<std::string> _args;
};

/* Read a demangled symbol produced by dumpbin /SYMBOLS and split it into its
 * different components, so that we may remap the mangled symbol to its ZHL IR
 * symbol.
 */
class SymbolNameReader {
public:
    static bool ReadSymbolName(const char* mangledName, const char* text,
			       SymbolName& name);
};
