
// Generated from ZHLParser.g4 by ANTLR 4.12.0


#include "ZHLParserListener.h"
#include "ZHLParserVisitor.h"

#include "ZHLParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct ZHLParserStaticData final {
  ZHLParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  ZHLParserStaticData(const ZHLParserStaticData&) = delete;
  ZHLParserStaticData(ZHLParserStaticData&&) = delete;
  ZHLParserStaticData& operator=(const ZHLParserStaticData&) = delete;
  ZHLParserStaticData& operator=(ZHLParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag zhlparserParserOnceFlag;
ZHLParserStaticData *zhlparserParserStaticData = nullptr;

void zhlparserParserInitialize() {
  assert(zhlparserParserStaticData == nullptr);
  auto staticData = std::make_unique<ZHLParserStaticData>(
    std::vector<std::string>{
      "zhl", "externalFunc", "typeInfo", "typeInfoDef", "function", "reference", 
      "funArgs", "funArg", "argParam", "optNamedFunArgs", "optNamedFunArg", 
      "templateSpec", "type", "typeSpecifier", "simpleType", "simpleTypeSignedness", 
      "simpleTypeLength", "nestedName", "pointerAttribute", "genericCode", 
      "class", "inheritance", "inheritanceDecl", "depends", "classBody", 
      "vtable", "vtableEntry", "vtableSignature", "classSignature", "classFunction", 
      "functionName", "classField", "innerField", "innerFieldPtr", "fullName", 
      "signature", "forwardDecl", "typedef", "functionPtr", "fptr", "memberPtr"
    },
    std::vector<std::string>{
      "", "'\"'", "':'", "';'", "", "'cleanup'", "'static'", "'virtual'", 
      "'__declspec'", "'external'", "'('", "')'", "','", "'*'", "'<'", "'>'", 
      "'['", "']'", "'{'", "'}'", "'class'", "'struct'", "'reference'", 
      "'depends'", "'typedef'", "'const'", "'&'", "'unsigned'", "'signed'", 
      "'long'", "'int'", "'short'", "'char'", "'bool'", "'float'", "'double'", 
      "'void'", "'TypeInfo'", "'Size'", "'Synonym'", "'Align'", "'__vtable'", 
      "'skip'", "'pure'", "'override'", "", "'public'", "'private'", "'protected'", 
      "", "", "'eax'", "'ebx'", "'ecx'", "'edx'", "'esi'", "'edi'", "'esp'", 
      "'ebp'", "", "'xmm0'", "'xmm1'", "'xmm2'", "'xmm3'", "'xmm4'", "'xmm5'", 
      "'xmm6'", "'xmm7'", "", "'__stdcall'", "'__cdecl'", "'__fastcall'", 
      "'__thiscall'", "", "", "'operator'"
    },
    std::vector<std::string>{
      "", "DoubleQuote", "Colon", "Semi", "Qualifier", "Cleanup", "Static", 
      "Virtual", "Declspec", "External", "LeftParen", "RightParen", "Comma", 
      "Star", "Lt", "Gt", "LeftRBracket", "RightRBracket", "LeftBracket", 
      "RightBracket", "Class", "Struct", "Reference", "Depends", "Typedef", 
      "Const", "CppRef", "Unsigned", "Signed", "Long", "Int", "Short", "Char", 
      "Bool", "Float", "Double", "Void", "Type", "Size", "Synonym", "Align", 
      "Vtable", "Skip", "Pure", "Override", "Visibility", "Public", "Private", 
      "Protected", "Register", "GeneralPurposeRegister", "Eax", "Ebx", "Ecx", 
      "Edx", "Esi", "Edi", "Esp", "Ebp", "SSERegister", "Xmm0", "Xmm1", 
      "Xmm2", "Xmm3", "Xmm4", "Xmm5", "Xmm6", "Xmm7", "CallingConvention", 
      "Stdcall", "Cdecl", "Fastcall", "Thiscall", "Signature", "ReferenceSignature", 
      "Operator", "OpSymbol", "Name", "Number", "HexNumber", "DecNumber", 
      "GenericCode", "Whitespace", "Newline", "BlockComment", "LineComment", 
      "Any"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,86,467,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,1,0,1,0,1,0,1,0,
  	1,0,1,0,1,0,1,0,5,0,91,8,0,10,0,12,0,94,9,0,1,0,1,0,1,1,1,1,1,1,1,1,1,
  	1,1,1,1,1,1,1,1,2,1,2,1,2,1,2,4,2,110,8,2,11,2,12,2,111,1,2,1,2,1,2,1,
  	3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,3,3,127,8,3,1,4,5,4,130,8,4,10,
  	4,12,4,133,9,4,1,4,3,4,136,8,4,1,4,1,4,1,4,1,4,3,4,142,8,4,1,4,1,4,1,
  	4,1,5,1,5,1,5,1,5,1,5,1,6,1,6,1,6,5,6,155,8,6,10,6,12,6,158,9,6,1,7,1,
  	7,1,7,3,7,163,8,7,1,8,1,8,1,8,1,8,1,9,1,9,1,9,5,9,172,8,9,10,9,12,9,175,
  	9,9,1,10,1,10,3,10,179,8,10,1,11,1,11,1,11,1,11,5,11,185,8,11,10,11,12,
  	11,188,9,11,1,11,1,11,1,12,1,12,3,12,194,8,12,1,12,5,12,197,8,12,10,12,
  	12,12,200,9,12,1,13,3,13,203,8,13,1,13,1,13,3,13,207,8,13,1,13,3,13,210,
  	8,13,1,14,1,14,3,14,214,8,14,1,14,4,14,217,8,14,11,14,12,14,218,1,14,
  	3,14,222,8,14,1,14,1,14,1,14,1,14,3,14,228,8,14,1,14,5,14,231,8,14,10,
  	14,12,14,234,9,14,1,14,1,14,1,14,3,14,239,8,14,1,14,3,14,242,8,14,1,15,
  	1,15,1,16,1,16,1,17,1,17,1,17,1,17,5,17,252,8,17,10,17,12,17,255,9,17,
  	1,18,1,18,3,18,259,8,18,1,18,1,18,3,18,263,8,18,3,18,265,8,18,1,19,1,
  	19,1,20,1,20,1,20,3,20,272,8,20,1,20,1,20,3,20,276,8,20,1,20,1,20,1,20,
  	1,20,1,20,3,20,283,8,20,1,20,1,20,1,21,1,21,1,21,5,21,290,8,21,10,21,
  	12,21,293,9,21,1,22,3,22,296,8,22,1,22,1,22,1,23,1,23,1,23,1,23,5,23,
  	304,8,23,10,23,12,23,307,9,23,1,23,1,23,1,23,1,24,1,24,1,24,1,24,1,24,
  	1,24,5,24,318,8,24,10,24,12,24,321,9,24,1,25,1,25,1,25,4,25,326,8,25,
  	11,25,12,25,327,1,25,1,25,1,25,1,26,1,26,1,26,1,26,1,26,3,26,338,8,26,
  	1,27,3,27,341,8,27,1,27,1,27,1,28,1,28,1,28,1,29,5,29,349,8,29,10,29,
  	12,29,352,9,29,1,29,1,29,1,29,1,29,3,29,358,8,29,1,29,1,29,1,29,1,30,
  	1,30,1,30,3,30,366,8,30,1,31,1,31,1,31,1,31,5,31,372,8,31,10,31,12,31,
  	375,9,31,1,31,1,31,1,32,5,32,380,8,32,10,32,12,32,383,9,32,1,32,1,32,
  	1,32,1,32,1,33,1,33,3,33,391,8,33,1,34,1,34,1,34,1,34,3,34,397,8,34,1,
  	35,1,35,1,35,1,35,3,35,403,8,35,1,36,1,36,1,36,1,36,1,37,1,37,1,37,1,
  	37,1,37,1,38,1,38,3,38,416,8,38,1,39,1,39,1,39,3,39,421,8,39,1,39,4,39,
  	424,8,39,11,39,12,39,425,1,39,1,39,1,39,1,39,3,39,432,8,39,1,39,1,39,
  	1,39,1,40,1,40,1,40,3,40,440,8,40,1,40,1,40,1,40,1,40,1,40,1,40,5,40,
  	448,8,40,10,40,12,40,451,9,40,1,40,4,40,454,8,40,11,40,12,40,455,1,40,
  	1,40,1,40,1,40,3,40,462,8,40,1,40,1,40,1,40,1,40,0,0,41,0,2,4,6,8,10,
  	12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,
  	58,60,62,64,66,68,70,72,74,76,78,80,0,3,1,0,27,28,2,0,29,29,31,31,1,0,
  	20,21,498,0,92,1,0,0,0,2,97,1,0,0,0,4,105,1,0,0,0,6,126,1,0,0,0,8,131,
  	1,0,0,0,10,146,1,0,0,0,12,151,1,0,0,0,14,159,1,0,0,0,16,164,1,0,0,0,18,
  	168,1,0,0,0,20,176,1,0,0,0,22,180,1,0,0,0,24,191,1,0,0,0,26,202,1,0,0,
  	0,28,241,1,0,0,0,30,243,1,0,0,0,32,245,1,0,0,0,34,247,1,0,0,0,36,264,
  	1,0,0,0,38,266,1,0,0,0,40,268,1,0,0,0,42,286,1,0,0,0,44,295,1,0,0,0,46,
  	299,1,0,0,0,48,319,1,0,0,0,50,322,1,0,0,0,52,337,1,0,0,0,54,340,1,0,0,
  	0,56,344,1,0,0,0,58,350,1,0,0,0,60,365,1,0,0,0,62,367,1,0,0,0,64,381,
  	1,0,0,0,66,388,1,0,0,0,68,392,1,0,0,0,70,402,1,0,0,0,72,404,1,0,0,0,74,
  	408,1,0,0,0,76,415,1,0,0,0,78,417,1,0,0,0,80,436,1,0,0,0,82,91,3,70,35,
  	0,83,91,3,40,20,0,84,91,3,38,19,0,85,91,3,74,37,0,86,91,3,76,38,0,87,
  	91,3,72,36,0,88,91,3,4,2,0,89,91,3,2,1,0,90,82,1,0,0,0,90,83,1,0,0,0,
  	90,84,1,0,0,0,90,85,1,0,0,0,90,86,1,0,0,0,90,87,1,0,0,0,90,88,1,0,0,0,
  	90,89,1,0,0,0,91,94,1,0,0,0,92,90,1,0,0,0,92,93,1,0,0,0,93,95,1,0,0,0,
  	94,92,1,0,0,0,95,96,5,0,0,1,96,1,1,0,0,0,97,98,5,9,0,0,98,99,5,10,0,0,
  	99,100,5,77,0,0,100,101,5,12,0,0,101,102,5,77,0,0,102,103,5,11,0,0,103,
  	104,3,8,4,0,104,3,1,0,0,0,105,106,5,37,0,0,106,107,5,77,0,0,107,109,5,
  	18,0,0,108,110,3,6,3,0,109,108,1,0,0,0,110,111,1,0,0,0,111,109,1,0,0,
  	0,111,112,1,0,0,0,112,113,1,0,0,0,113,114,5,19,0,0,114,115,5,3,0,0,115,
  	5,1,0,0,0,116,117,5,38,0,0,117,118,5,78,0,0,118,127,5,3,0,0,119,120,5,
  	39,0,0,120,121,3,24,12,0,121,122,5,3,0,0,122,127,1,0,0,0,123,124,5,40,
  	0,0,124,125,5,78,0,0,125,127,5,3,0,0,126,116,1,0,0,0,126,119,1,0,0,0,
  	126,123,1,0,0,0,127,7,1,0,0,0,128,130,5,4,0,0,129,128,1,0,0,0,130,133,
  	1,0,0,0,131,129,1,0,0,0,131,132,1,0,0,0,132,135,1,0,0,0,133,131,1,0,0,
  	0,134,136,5,68,0,0,135,134,1,0,0,0,135,136,1,0,0,0,136,137,1,0,0,0,137,
  	138,3,24,12,0,138,139,3,34,17,0,139,141,5,10,0,0,140,142,3,12,6,0,141,
  	140,1,0,0,0,141,142,1,0,0,0,142,143,1,0,0,0,143,144,5,11,0,0,144,145,
  	5,3,0,0,145,9,1,0,0,0,146,147,5,22,0,0,147,148,3,24,12,0,148,149,5,77,
  	0,0,149,150,5,3,0,0,150,11,1,0,0,0,151,156,3,14,7,0,152,153,5,12,0,0,
  	153,155,3,14,7,0,154,152,1,0,0,0,155,158,1,0,0,0,156,154,1,0,0,0,156,
  	157,1,0,0,0,157,13,1,0,0,0,158,156,1,0,0,0,159,160,3,24,12,0,160,162,
  	3,68,34,0,161,163,3,16,8,0,162,161,1,0,0,0,162,163,1,0,0,0,163,15,1,0,
  	0,0,164,165,5,14,0,0,165,166,5,49,0,0,166,167,5,15,0,0,167,17,1,0,0,0,
  	168,173,3,20,10,0,169,170,5,12,0,0,170,172,3,20,10,0,171,169,1,0,0,0,
  	172,175,1,0,0,0,173,171,1,0,0,0,173,174,1,0,0,0,174,19,1,0,0,0,175,173,
  	1,0,0,0,176,178,3,24,12,0,177,179,3,68,34,0,178,177,1,0,0,0,178,179,1,
  	0,0,0,179,21,1,0,0,0,180,181,5,14,0,0,181,186,3,24,12,0,182,183,5,12,
  	0,0,183,185,3,24,12,0,184,182,1,0,0,0,185,188,1,0,0,0,186,184,1,0,0,0,
  	186,187,1,0,0,0,187,189,1,0,0,0,188,186,1,0,0,0,189,190,5,15,0,0,190,
  	23,1,0,0,0,191,193,3,26,13,0,192,194,3,22,11,0,193,192,1,0,0,0,193,194,
  	1,0,0,0,194,198,1,0,0,0,195,197,3,36,18,0,196,195,1,0,0,0,197,200,1,0,
  	0,0,198,196,1,0,0,0,198,199,1,0,0,0,199,25,1,0,0,0,200,198,1,0,0,0,201,
  	203,5,25,0,0,202,201,1,0,0,0,202,203,1,0,0,0,203,206,1,0,0,0,204,207,
  	3,28,14,0,205,207,3,34,17,0,206,204,1,0,0,0,206,205,1,0,0,0,207,209,1,
  	0,0,0,208,210,5,25,0,0,209,208,1,0,0,0,209,210,1,0,0,0,210,27,1,0,0,0,
  	211,242,3,30,15,0,212,214,3,30,15,0,213,212,1,0,0,0,213,214,1,0,0,0,214,
  	216,1,0,0,0,215,217,3,32,16,0,216,215,1,0,0,0,217,218,1,0,0,0,218,216,
  	1,0,0,0,218,219,1,0,0,0,219,242,1,0,0,0,220,222,3,30,15,0,221,220,1,0,
  	0,0,221,222,1,0,0,0,222,223,1,0,0,0,223,242,5,32,0,0,224,242,5,33,0,0,
  	225,242,5,36,0,0,226,228,3,30,15,0,227,226,1,0,0,0,227,228,1,0,0,0,228,
  	232,1,0,0,0,229,231,3,32,16,0,230,229,1,0,0,0,231,234,1,0,0,0,232,230,
  	1,0,0,0,232,233,1,0,0,0,233,235,1,0,0,0,234,232,1,0,0,0,235,242,5,30,
  	0,0,236,242,5,34,0,0,237,239,3,32,16,0,238,237,1,0,0,0,238,239,1,0,0,
  	0,239,240,1,0,0,0,240,242,5,35,0,0,241,211,1,0,0,0,241,213,1,0,0,0,241,
  	221,1,0,0,0,241,224,1,0,0,0,241,225,1,0,0,0,241,227,1,0,0,0,241,236,1,
  	0,0,0,241,238,1,0,0,0,242,29,1,0,0,0,243,244,7,0,0,0,244,31,1,0,0,0,245,
  	246,7,1,0,0,246,33,1,0,0,0,247,253,5,77,0,0,248,249,5,2,0,0,249,250,5,
  	2,0,0,250,252,3,60,30,0,251,248,1,0,0,0,252,255,1,0,0,0,253,251,1,0,0,
  	0,253,254,1,0,0,0,254,35,1,0,0,0,255,253,1,0,0,0,256,258,5,26,0,0,257,
  	259,5,26,0,0,258,257,1,0,0,0,258,259,1,0,0,0,259,265,1,0,0,0,260,262,
  	5,13,0,0,261,263,5,25,0,0,262,261,1,0,0,0,262,263,1,0,0,0,263,265,1,0,
  	0,0,264,256,1,0,0,0,264,260,1,0,0,0,265,37,1,0,0,0,266,267,5,81,0,0,267,
  	39,1,0,0,0,268,269,7,2,0,0,269,271,5,77,0,0,270,272,3,46,23,0,271,270,
  	1,0,0,0,271,272,1,0,0,0,272,275,1,0,0,0,273,274,5,2,0,0,274,276,3,42,
  	21,0,275,273,1,0,0,0,275,276,1,0,0,0,276,277,1,0,0,0,277,278,5,18,0,0,
  	278,279,3,48,24,0,279,282,5,19,0,0,280,281,5,2,0,0,281,283,5,78,0,0,282,
  	280,1,0,0,0,282,283,1,0,0,0,283,284,1,0,0,0,284,285,5,3,0,0,285,41,1,
  	0,0,0,286,291,3,44,22,0,287,288,5,12,0,0,288,290,3,44,22,0,289,287,1,
  	0,0,0,290,293,1,0,0,0,291,289,1,0,0,0,291,292,1,0,0,0,292,43,1,0,0,0,
  	293,291,1,0,0,0,294,296,5,45,0,0,295,294,1,0,0,0,295,296,1,0,0,0,296,
  	297,1,0,0,0,297,298,3,34,17,0,298,45,1,0,0,0,299,300,5,23,0,0,300,305,
  	5,10,0,0,301,302,5,77,0,0,302,304,5,12,0,0,303,301,1,0,0,0,304,307,1,
  	0,0,0,305,303,1,0,0,0,305,306,1,0,0,0,306,308,1,0,0,0,307,305,1,0,0,0,
  	308,309,5,77,0,0,309,310,5,11,0,0,310,47,1,0,0,0,311,318,3,50,25,0,312,
  	318,3,56,28,0,313,318,3,38,19,0,314,318,3,62,31,0,315,318,3,76,38,0,316,
  	318,3,72,36,0,317,311,1,0,0,0,317,312,1,0,0,0,317,313,1,0,0,0,317,314,
  	1,0,0,0,317,315,1,0,0,0,317,316,1,0,0,0,318,321,1,0,0,0,319,317,1,0,0,
  	0,319,320,1,0,0,0,320,49,1,0,0,0,321,319,1,0,0,0,322,323,5,41,0,0,323,
  	325,5,18,0,0,324,326,3,52,26,0,325,324,1,0,0,0,326,327,1,0,0,0,327,325,
  	1,0,0,0,327,328,1,0,0,0,328,329,1,0,0,0,329,330,5,19,0,0,330,331,5,3,
  	0,0,331,51,1,0,0,0,332,338,3,54,27,0,333,334,5,42,0,0,334,338,5,3,0,0,
  	335,336,5,43,0,0,336,338,3,58,29,0,337,332,1,0,0,0,337,333,1,0,0,0,337,
  	335,1,0,0,0,338,53,1,0,0,0,339,341,5,44,0,0,340,339,1,0,0,0,340,341,1,
  	0,0,0,341,342,1,0,0,0,342,343,3,56,28,0,343,55,1,0,0,0,344,345,5,73,0,
  	0,345,346,3,58,29,0,346,57,1,0,0,0,347,349,5,4,0,0,348,347,1,0,0,0,349,
  	352,1,0,0,0,350,348,1,0,0,0,350,351,1,0,0,0,351,353,1,0,0,0,352,350,1,
  	0,0,0,353,354,3,24,12,0,354,355,3,60,30,0,355,357,5,10,0,0,356,358,3,
  	12,6,0,357,356,1,0,0,0,357,358,1,0,0,0,358,359,1,0,0,0,359,360,5,11,0,
  	0,360,361,5,3,0,0,361,59,1,0,0,0,362,366,5,77,0,0,363,364,5,75,0,0,364,
  	366,5,76,0,0,365,362,1,0,0,0,365,363,1,0,0,0,366,61,1,0,0,0,367,368,3,
  	24,12,0,368,373,3,64,32,0,369,370,5,12,0,0,370,372,3,64,32,0,371,369,
  	1,0,0,0,372,375,1,0,0,0,373,371,1,0,0,0,373,374,1,0,0,0,374,376,1,0,0,
  	0,375,373,1,0,0,0,376,377,5,3,0,0,377,63,1,0,0,0,378,380,3,66,33,0,379,
  	378,1,0,0,0,380,383,1,0,0,0,381,379,1,0,0,0,381,382,1,0,0,0,382,384,1,
  	0,0,0,383,381,1,0,0,0,384,385,3,68,34,0,385,386,5,2,0,0,386,387,5,78,
  	0,0,387,65,1,0,0,0,388,390,5,13,0,0,389,391,5,25,0,0,390,389,1,0,0,0,
  	390,391,1,0,0,0,391,67,1,0,0,0,392,396,5,77,0,0,393,394,5,16,0,0,394,
  	395,5,78,0,0,395,397,5,17,0,0,396,393,1,0,0,0,396,397,1,0,0,0,397,69,
  	1,0,0,0,398,399,5,73,0,0,399,403,3,8,4,0,400,401,5,74,0,0,401,403,3,10,
  	5,0,402,398,1,0,0,0,402,400,1,0,0,0,403,71,1,0,0,0,404,405,7,2,0,0,405,
  	406,5,77,0,0,406,407,5,3,0,0,407,73,1,0,0,0,408,409,5,24,0,0,409,410,
  	3,24,12,0,410,411,5,77,0,0,411,412,5,3,0,0,412,75,1,0,0,0,413,416,3,78,
  	39,0,414,416,3,80,40,0,415,413,1,0,0,0,415,414,1,0,0,0,416,77,1,0,0,0,
  	417,418,3,24,12,0,418,420,5,10,0,0,419,421,5,68,0,0,420,419,1,0,0,0,420,
  	421,1,0,0,0,421,423,1,0,0,0,422,424,5,13,0,0,423,422,1,0,0,0,424,425,
  	1,0,0,0,425,423,1,0,0,0,425,426,1,0,0,0,426,427,1,0,0,0,427,428,5,77,
  	0,0,428,429,5,11,0,0,429,431,5,10,0,0,430,432,3,18,9,0,431,430,1,0,0,
  	0,431,432,1,0,0,0,432,433,1,0,0,0,433,434,5,11,0,0,434,435,5,3,0,0,435,
  	79,1,0,0,0,436,437,3,24,12,0,437,439,5,10,0,0,438,440,5,68,0,0,439,438,
  	1,0,0,0,439,440,1,0,0,0,440,441,1,0,0,0,441,442,5,77,0,0,442,443,5,2,
  	0,0,443,449,5,2,0,0,444,445,5,77,0,0,445,446,5,2,0,0,446,448,5,2,0,0,
  	447,444,1,0,0,0,448,451,1,0,0,0,449,447,1,0,0,0,449,450,1,0,0,0,450,453,
  	1,0,0,0,451,449,1,0,0,0,452,454,5,13,0,0,453,452,1,0,0,0,454,455,1,0,
  	0,0,455,453,1,0,0,0,455,456,1,0,0,0,456,457,1,0,0,0,457,458,5,77,0,0,
  	458,459,5,11,0,0,459,461,5,10,0,0,460,462,3,18,9,0,461,460,1,0,0,0,461,
  	462,1,0,0,0,462,463,1,0,0,0,463,464,5,11,0,0,464,465,5,3,0,0,465,81,1,
  	0,0,0,55,90,92,111,126,131,135,141,156,162,173,178,186,193,198,202,206,
  	209,213,218,221,227,232,238,241,253,258,262,264,271,275,282,291,295,305,
  	317,319,327,337,340,350,357,365,373,381,390,396,402,415,420,425,431,439,
  	449,455,461
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  zhlparserParserStaticData = staticData.release();
}

}

ZHLParser::ZHLParser(TokenStream *input) : ZHLParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

ZHLParser::ZHLParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  ZHLParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *zhlparserParserStaticData->atn, zhlparserParserStaticData->decisionToDFA, zhlparserParserStaticData->sharedContextCache, options);
}

ZHLParser::~ZHLParser() {
  delete _interpreter;
}

const atn::ATN& ZHLParser::getATN() const {
  return *zhlparserParserStaticData->atn;
}

std::string ZHLParser::getGrammarFileName() const {
  return "ZHLParser.g4";
}

const std::vector<std::string>& ZHLParser::getRuleNames() const {
  return zhlparserParserStaticData->ruleNames;
}

const dfa::Vocabulary& ZHLParser::getVocabulary() const {
  return zhlparserParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView ZHLParser::getSerializedATN() const {
  return zhlparserParserStaticData->serializedATN;
}


//----------------- ZhlContext ------------------------------------------------------------------

ZHLParser::ZhlContext::ZhlContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::ZhlContext::EOF() {
  return getToken(ZHLParser::EOF, 0);
}

std::vector<ZHLParser::SignatureContext *> ZHLParser::ZhlContext::signature() {
  return getRuleContexts<ZHLParser::SignatureContext>();
}

ZHLParser::SignatureContext* ZHLParser::ZhlContext::signature(size_t i) {
  return getRuleContext<ZHLParser::SignatureContext>(i);
}

std::vector<ZHLParser::ClassContext *> ZHLParser::ZhlContext::class_() {
  return getRuleContexts<ZHLParser::ClassContext>();
}

ZHLParser::ClassContext* ZHLParser::ZhlContext::class_(size_t i) {
  return getRuleContext<ZHLParser::ClassContext>(i);
}

std::vector<ZHLParser::GenericCodeContext *> ZHLParser::ZhlContext::genericCode() {
  return getRuleContexts<ZHLParser::GenericCodeContext>();
}

ZHLParser::GenericCodeContext* ZHLParser::ZhlContext::genericCode(size_t i) {
  return getRuleContext<ZHLParser::GenericCodeContext>(i);
}

std::vector<ZHLParser::TypedefContext *> ZHLParser::ZhlContext::typedef_() {
  return getRuleContexts<ZHLParser::TypedefContext>();
}

ZHLParser::TypedefContext* ZHLParser::ZhlContext::typedef_(size_t i) {
  return getRuleContext<ZHLParser::TypedefContext>(i);
}

std::vector<ZHLParser::FunctionPtrContext *> ZHLParser::ZhlContext::functionPtr() {
  return getRuleContexts<ZHLParser::FunctionPtrContext>();
}

ZHLParser::FunctionPtrContext* ZHLParser::ZhlContext::functionPtr(size_t i) {
  return getRuleContext<ZHLParser::FunctionPtrContext>(i);
}

std::vector<ZHLParser::ForwardDeclContext *> ZHLParser::ZhlContext::forwardDecl() {
  return getRuleContexts<ZHLParser::ForwardDeclContext>();
}

ZHLParser::ForwardDeclContext* ZHLParser::ZhlContext::forwardDecl(size_t i) {
  return getRuleContext<ZHLParser::ForwardDeclContext>(i);
}

std::vector<ZHLParser::TypeInfoContext *> ZHLParser::ZhlContext::typeInfo() {
  return getRuleContexts<ZHLParser::TypeInfoContext>();
}

ZHLParser::TypeInfoContext* ZHLParser::ZhlContext::typeInfo(size_t i) {
  return getRuleContext<ZHLParser::TypeInfoContext>(i);
}

std::vector<ZHLParser::ExternalFuncContext *> ZHLParser::ZhlContext::externalFunc() {
  return getRuleContexts<ZHLParser::ExternalFuncContext>();
}

ZHLParser::ExternalFuncContext* ZHLParser::ZhlContext::externalFunc(size_t i) {
  return getRuleContext<ZHLParser::ExternalFuncContext>(i);
}


size_t ZHLParser::ZhlContext::getRuleIndex() const {
  return ZHLParser::RuleZhl;
}

void ZHLParser::ZhlContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterZhl(this);
}

void ZHLParser::ZhlContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitZhl(this);
}


std::any ZHLParser::ZhlContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitZhl(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ZhlContext* ZHLParser::zhl() {
  ZhlContext *_localctx = _tracker.createInstance<ZhlContext>(_ctx, getState());
  enterRule(_localctx, 0, ZHLParser::RuleZhl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(92);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 274797167104) != 0) || ((((_la - 73) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 73)) & 275) != 0)) {
      setState(90);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
      case 1: {
        setState(82);
        signature();
        break;
      }

      case 2: {
        setState(83);
        class_();
        break;
      }

      case 3: {
        setState(84);
        genericCode();
        break;
      }

      case 4: {
        setState(85);
        typedef_();
        break;
      }

      case 5: {
        setState(86);
        functionPtr();
        break;
      }

      case 6: {
        setState(87);
        forwardDecl();
        break;
      }

      case 7: {
        setState(88);
        typeInfo();
        break;
      }

      case 8: {
        setState(89);
        externalFunc();
        break;
      }

      default:
        break;
      }
      setState(94);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(95);
    match(ZHLParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExternalFuncContext ------------------------------------------------------------------

ZHLParser::ExternalFuncContext::ExternalFuncContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::ExternalFuncContext::External() {
  return getToken(ZHLParser::External, 0);
}

tree::TerminalNode* ZHLParser::ExternalFuncContext::LeftParen() {
  return getToken(ZHLParser::LeftParen, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::ExternalFuncContext::Name() {
  return getTokens(ZHLParser::Name);
}

tree::TerminalNode* ZHLParser::ExternalFuncContext::Name(size_t i) {
  return getToken(ZHLParser::Name, i);
}

tree::TerminalNode* ZHLParser::ExternalFuncContext::Comma() {
  return getToken(ZHLParser::Comma, 0);
}

tree::TerminalNode* ZHLParser::ExternalFuncContext::RightParen() {
  return getToken(ZHLParser::RightParen, 0);
}

ZHLParser::FunctionContext* ZHLParser::ExternalFuncContext::function() {
  return getRuleContext<ZHLParser::FunctionContext>(0);
}


size_t ZHLParser::ExternalFuncContext::getRuleIndex() const {
  return ZHLParser::RuleExternalFunc;
}

void ZHLParser::ExternalFuncContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExternalFunc(this);
}

void ZHLParser::ExternalFuncContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExternalFunc(this);
}


std::any ZHLParser::ExternalFuncContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitExternalFunc(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ExternalFuncContext* ZHLParser::externalFunc() {
  ExternalFuncContext *_localctx = _tracker.createInstance<ExternalFuncContext>(_ctx, getState());
  enterRule(_localctx, 2, ZHLParser::RuleExternalFunc);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(97);
    match(ZHLParser::External);
    setState(98);
    match(ZHLParser::LeftParen);
    setState(99);
    match(ZHLParser::Name);
    setState(100);
    match(ZHLParser::Comma);
    setState(101);
    match(ZHLParser::Name);
    setState(102);
    match(ZHLParser::RightParen);
    setState(103);
    function();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeInfoContext ------------------------------------------------------------------

ZHLParser::TypeInfoContext::TypeInfoContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::TypeInfoContext::Type() {
  return getToken(ZHLParser::Type, 0);
}

tree::TerminalNode* ZHLParser::TypeInfoContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

tree::TerminalNode* ZHLParser::TypeInfoContext::LeftBracket() {
  return getToken(ZHLParser::LeftBracket, 0);
}

tree::TerminalNode* ZHLParser::TypeInfoContext::RightBracket() {
  return getToken(ZHLParser::RightBracket, 0);
}

tree::TerminalNode* ZHLParser::TypeInfoContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}

std::vector<ZHLParser::TypeInfoDefContext *> ZHLParser::TypeInfoContext::typeInfoDef() {
  return getRuleContexts<ZHLParser::TypeInfoDefContext>();
}

ZHLParser::TypeInfoDefContext* ZHLParser::TypeInfoContext::typeInfoDef(size_t i) {
  return getRuleContext<ZHLParser::TypeInfoDefContext>(i);
}


size_t ZHLParser::TypeInfoContext::getRuleIndex() const {
  return ZHLParser::RuleTypeInfo;
}

void ZHLParser::TypeInfoContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeInfo(this);
}

void ZHLParser::TypeInfoContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeInfo(this);
}


std::any ZHLParser::TypeInfoContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitTypeInfo(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::TypeInfoContext* ZHLParser::typeInfo() {
  TypeInfoContext *_localctx = _tracker.createInstance<TypeInfoContext>(_ctx, getState());
  enterRule(_localctx, 4, ZHLParser::RuleTypeInfo);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(105);
    match(ZHLParser::Type);
    setState(106);
    match(ZHLParser::Name);
    setState(107);
    match(ZHLParser::LeftBracket);
    setState(109); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(108);
      typeInfoDef();
      setState(111); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 1924145348608) != 0));
    setState(113);
    match(ZHLParser::RightBracket);
    setState(114);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeInfoDefContext ------------------------------------------------------------------

ZHLParser::TypeInfoDefContext::TypeInfoDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::TypeInfoDefContext::Size() {
  return getToken(ZHLParser::Size, 0);
}

tree::TerminalNode* ZHLParser::TypeInfoDefContext::Number() {
  return getToken(ZHLParser::Number, 0);
}

tree::TerminalNode* ZHLParser::TypeInfoDefContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}

tree::TerminalNode* ZHLParser::TypeInfoDefContext::Synonym() {
  return getToken(ZHLParser::Synonym, 0);
}

ZHLParser::TypeContext* ZHLParser::TypeInfoDefContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

tree::TerminalNode* ZHLParser::TypeInfoDefContext::Align() {
  return getToken(ZHLParser::Align, 0);
}


size_t ZHLParser::TypeInfoDefContext::getRuleIndex() const {
  return ZHLParser::RuleTypeInfoDef;
}

void ZHLParser::TypeInfoDefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeInfoDef(this);
}

void ZHLParser::TypeInfoDefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeInfoDef(this);
}


std::any ZHLParser::TypeInfoDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitTypeInfoDef(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::TypeInfoDefContext* ZHLParser::typeInfoDef() {
  TypeInfoDefContext *_localctx = _tracker.createInstance<TypeInfoDefContext>(_ctx, getState());
  enterRule(_localctx, 6, ZHLParser::RuleTypeInfoDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(126);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ZHLParser::Size: {
        enterOuterAlt(_localctx, 1);
        setState(116);
        match(ZHLParser::Size);
        setState(117);
        match(ZHLParser::Number);
        setState(118);
        match(ZHLParser::Semi);
        break;
      }

      case ZHLParser::Synonym: {
        enterOuterAlt(_localctx, 2);
        setState(119);
        match(ZHLParser::Synonym);
        setState(120);
        type();
        setState(121);
        match(ZHLParser::Semi);
        break;
      }

      case ZHLParser::Align: {
        enterOuterAlt(_localctx, 3);
        setState(123);
        match(ZHLParser::Align);
        setState(124);
        match(ZHLParser::Number);
        setState(125);
        match(ZHLParser::Semi);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionContext ------------------------------------------------------------------

ZHLParser::FunctionContext::FunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::TypeContext* ZHLParser::FunctionContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

ZHLParser::NestedNameContext* ZHLParser::FunctionContext::nestedName() {
  return getRuleContext<ZHLParser::NestedNameContext>(0);
}

tree::TerminalNode* ZHLParser::FunctionContext::LeftParen() {
  return getToken(ZHLParser::LeftParen, 0);
}

tree::TerminalNode* ZHLParser::FunctionContext::RightParen() {
  return getToken(ZHLParser::RightParen, 0);
}

tree::TerminalNode* ZHLParser::FunctionContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::FunctionContext::Qualifier() {
  return getTokens(ZHLParser::Qualifier);
}

tree::TerminalNode* ZHLParser::FunctionContext::Qualifier(size_t i) {
  return getToken(ZHLParser::Qualifier, i);
}

tree::TerminalNode* ZHLParser::FunctionContext::CallingConvention() {
  return getToken(ZHLParser::CallingConvention, 0);
}

ZHLParser::FunArgsContext* ZHLParser::FunctionContext::funArgs() {
  return getRuleContext<ZHLParser::FunArgsContext>(0);
}


size_t ZHLParser::FunctionContext::getRuleIndex() const {
  return ZHLParser::RuleFunction;
}

void ZHLParser::FunctionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunction(this);
}

void ZHLParser::FunctionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunction(this);
}


std::any ZHLParser::FunctionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitFunction(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::FunctionContext* ZHLParser::function() {
  FunctionContext *_localctx = _tracker.createInstance<FunctionContext>(_ctx, getState());
  enterRule(_localctx, 8, ZHLParser::RuleFunction);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(131);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Qualifier) {
      setState(128);
      match(ZHLParser::Qualifier);
      setState(133);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(135);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::CallingConvention) {
      setState(134);
      match(ZHLParser::CallingConvention);
    }
    setState(137);
    type();
    setState(138);
    nestedName();
    setState(139);
    match(ZHLParser::LeftParen);
    setState(141);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 25) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 25)) & 4503599627374589) != 0)) {
      setState(140);
      funArgs();
    }
    setState(143);
    match(ZHLParser::RightParen);
    setState(144);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReferenceContext ------------------------------------------------------------------

ZHLParser::ReferenceContext::ReferenceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::ReferenceContext::Reference() {
  return getToken(ZHLParser::Reference, 0);
}

ZHLParser::TypeContext* ZHLParser::ReferenceContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

tree::TerminalNode* ZHLParser::ReferenceContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

tree::TerminalNode* ZHLParser::ReferenceContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}


size_t ZHLParser::ReferenceContext::getRuleIndex() const {
  return ZHLParser::RuleReference;
}

void ZHLParser::ReferenceContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterReference(this);
}

void ZHLParser::ReferenceContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitReference(this);
}


std::any ZHLParser::ReferenceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitReference(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ReferenceContext* ZHLParser::reference() {
  ReferenceContext *_localctx = _tracker.createInstance<ReferenceContext>(_ctx, getState());
  enterRule(_localctx, 10, ZHLParser::RuleReference);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(146);
    match(ZHLParser::Reference);
    setState(147);
    type();
    setState(148);
    match(ZHLParser::Name);
    setState(149);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunArgsContext ------------------------------------------------------------------

ZHLParser::FunArgsContext::FunArgsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ZHLParser::FunArgContext *> ZHLParser::FunArgsContext::funArg() {
  return getRuleContexts<ZHLParser::FunArgContext>();
}

ZHLParser::FunArgContext* ZHLParser::FunArgsContext::funArg(size_t i) {
  return getRuleContext<ZHLParser::FunArgContext>(i);
}

std::vector<tree::TerminalNode *> ZHLParser::FunArgsContext::Comma() {
  return getTokens(ZHLParser::Comma);
}

tree::TerminalNode* ZHLParser::FunArgsContext::Comma(size_t i) {
  return getToken(ZHLParser::Comma, i);
}


size_t ZHLParser::FunArgsContext::getRuleIndex() const {
  return ZHLParser::RuleFunArgs;
}

void ZHLParser::FunArgsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunArgs(this);
}

void ZHLParser::FunArgsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunArgs(this);
}


std::any ZHLParser::FunArgsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitFunArgs(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::FunArgsContext* ZHLParser::funArgs() {
  FunArgsContext *_localctx = _tracker.createInstance<FunArgsContext>(_ctx, getState());
  enterRule(_localctx, 12, ZHLParser::RuleFunArgs);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(151);
    funArg();
    setState(156);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(152);
      match(ZHLParser::Comma);
      setState(153);
      funArg();
      setState(158);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunArgContext ------------------------------------------------------------------

ZHLParser::FunArgContext::FunArgContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::TypeContext* ZHLParser::FunArgContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

ZHLParser::FullNameContext* ZHLParser::FunArgContext::fullName() {
  return getRuleContext<ZHLParser::FullNameContext>(0);
}

ZHLParser::ArgParamContext* ZHLParser::FunArgContext::argParam() {
  return getRuleContext<ZHLParser::ArgParamContext>(0);
}


size_t ZHLParser::FunArgContext::getRuleIndex() const {
  return ZHLParser::RuleFunArg;
}

void ZHLParser::FunArgContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunArg(this);
}

void ZHLParser::FunArgContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunArg(this);
}


std::any ZHLParser::FunArgContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitFunArg(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::FunArgContext* ZHLParser::funArg() {
  FunArgContext *_localctx = _tracker.createInstance<FunArgContext>(_ctx, getState());
  enterRule(_localctx, 14, ZHLParser::RuleFunArg);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(159);
    type();
    setState(160);
    fullName();
    setState(162);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Lt) {
      setState(161);
      argParam();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgParamContext ------------------------------------------------------------------

ZHLParser::ArgParamContext::ArgParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::ArgParamContext::Lt() {
  return getToken(ZHLParser::Lt, 0);
}

tree::TerminalNode* ZHLParser::ArgParamContext::Register() {
  return getToken(ZHLParser::Register, 0);
}

tree::TerminalNode* ZHLParser::ArgParamContext::Gt() {
  return getToken(ZHLParser::Gt, 0);
}


size_t ZHLParser::ArgParamContext::getRuleIndex() const {
  return ZHLParser::RuleArgParam;
}

void ZHLParser::ArgParamContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArgParam(this);
}

void ZHLParser::ArgParamContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArgParam(this);
}


std::any ZHLParser::ArgParamContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitArgParam(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ArgParamContext* ZHLParser::argParam() {
  ArgParamContext *_localctx = _tracker.createInstance<ArgParamContext>(_ctx, getState());
  enterRule(_localctx, 16, ZHLParser::RuleArgParam);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(164);
    match(ZHLParser::Lt);
    setState(165);
    match(ZHLParser::Register);
    setState(166);
    match(ZHLParser::Gt);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OptNamedFunArgsContext ------------------------------------------------------------------

ZHLParser::OptNamedFunArgsContext::OptNamedFunArgsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ZHLParser::OptNamedFunArgContext *> ZHLParser::OptNamedFunArgsContext::optNamedFunArg() {
  return getRuleContexts<ZHLParser::OptNamedFunArgContext>();
}

ZHLParser::OptNamedFunArgContext* ZHLParser::OptNamedFunArgsContext::optNamedFunArg(size_t i) {
  return getRuleContext<ZHLParser::OptNamedFunArgContext>(i);
}

std::vector<tree::TerminalNode *> ZHLParser::OptNamedFunArgsContext::Comma() {
  return getTokens(ZHLParser::Comma);
}

tree::TerminalNode* ZHLParser::OptNamedFunArgsContext::Comma(size_t i) {
  return getToken(ZHLParser::Comma, i);
}


size_t ZHLParser::OptNamedFunArgsContext::getRuleIndex() const {
  return ZHLParser::RuleOptNamedFunArgs;
}

void ZHLParser::OptNamedFunArgsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOptNamedFunArgs(this);
}

void ZHLParser::OptNamedFunArgsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOptNamedFunArgs(this);
}


std::any ZHLParser::OptNamedFunArgsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitOptNamedFunArgs(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::OptNamedFunArgsContext* ZHLParser::optNamedFunArgs() {
  OptNamedFunArgsContext *_localctx = _tracker.createInstance<OptNamedFunArgsContext>(_ctx, getState());
  enterRule(_localctx, 18, ZHLParser::RuleOptNamedFunArgs);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(168);
    optNamedFunArg();
    setState(173);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(169);
      match(ZHLParser::Comma);
      setState(170);
      optNamedFunArg();
      setState(175);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OptNamedFunArgContext ------------------------------------------------------------------

ZHLParser::OptNamedFunArgContext::OptNamedFunArgContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::TypeContext* ZHLParser::OptNamedFunArgContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

ZHLParser::FullNameContext* ZHLParser::OptNamedFunArgContext::fullName() {
  return getRuleContext<ZHLParser::FullNameContext>(0);
}


size_t ZHLParser::OptNamedFunArgContext::getRuleIndex() const {
  return ZHLParser::RuleOptNamedFunArg;
}

void ZHLParser::OptNamedFunArgContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOptNamedFunArg(this);
}

void ZHLParser::OptNamedFunArgContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOptNamedFunArg(this);
}


std::any ZHLParser::OptNamedFunArgContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitOptNamedFunArg(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::OptNamedFunArgContext* ZHLParser::optNamedFunArg() {
  OptNamedFunArgContext *_localctx = _tracker.createInstance<OptNamedFunArgContext>(_ctx, getState());
  enterRule(_localctx, 20, ZHLParser::RuleOptNamedFunArg);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(176);
    type();
    setState(178);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Name) {
      setState(177);
      fullName();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TemplateSpecContext ------------------------------------------------------------------

ZHLParser::TemplateSpecContext::TemplateSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::TemplateSpecContext::Lt() {
  return getToken(ZHLParser::Lt, 0);
}

std::vector<ZHLParser::TypeContext *> ZHLParser::TemplateSpecContext::type() {
  return getRuleContexts<ZHLParser::TypeContext>();
}

ZHLParser::TypeContext* ZHLParser::TemplateSpecContext::type(size_t i) {
  return getRuleContext<ZHLParser::TypeContext>(i);
}

tree::TerminalNode* ZHLParser::TemplateSpecContext::Gt() {
  return getToken(ZHLParser::Gt, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::TemplateSpecContext::Comma() {
  return getTokens(ZHLParser::Comma);
}

tree::TerminalNode* ZHLParser::TemplateSpecContext::Comma(size_t i) {
  return getToken(ZHLParser::Comma, i);
}


size_t ZHLParser::TemplateSpecContext::getRuleIndex() const {
  return ZHLParser::RuleTemplateSpec;
}

void ZHLParser::TemplateSpecContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTemplateSpec(this);
}

void ZHLParser::TemplateSpecContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTemplateSpec(this);
}


std::any ZHLParser::TemplateSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitTemplateSpec(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::TemplateSpecContext* ZHLParser::templateSpec() {
  TemplateSpecContext *_localctx = _tracker.createInstance<TemplateSpecContext>(_ctx, getState());
  enterRule(_localctx, 22, ZHLParser::RuleTemplateSpec);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(180);
    match(ZHLParser::Lt);
    setState(181);
    type();
    setState(186);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(182);
      match(ZHLParser::Comma);
      setState(183);
      type();
      setState(188);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(189);
    match(ZHLParser::Gt);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeContext ------------------------------------------------------------------

ZHLParser::TypeContext::TypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::TypeSpecifierContext* ZHLParser::TypeContext::typeSpecifier() {
  return getRuleContext<ZHLParser::TypeSpecifierContext>(0);
}

ZHLParser::TemplateSpecContext* ZHLParser::TypeContext::templateSpec() {
  return getRuleContext<ZHLParser::TemplateSpecContext>(0);
}

std::vector<ZHLParser::PointerAttributeContext *> ZHLParser::TypeContext::pointerAttribute() {
  return getRuleContexts<ZHLParser::PointerAttributeContext>();
}

ZHLParser::PointerAttributeContext* ZHLParser::TypeContext::pointerAttribute(size_t i) {
  return getRuleContext<ZHLParser::PointerAttributeContext>(i);
}


size_t ZHLParser::TypeContext::getRuleIndex() const {
  return ZHLParser::RuleType;
}

void ZHLParser::TypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterType(this);
}

void ZHLParser::TypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitType(this);
}


std::any ZHLParser::TypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitType(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::TypeContext* ZHLParser::type() {
  TypeContext *_localctx = _tracker.createInstance<TypeContext>(_ctx, getState());
  enterRule(_localctx, 24, ZHLParser::RuleType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(191);
    typeSpecifier();
    setState(193);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Lt) {
      setState(192);
      templateSpec();
    }
    setState(198);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(195);
        pointerAttribute(); 
      }
      setState(200);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeSpecifierContext ------------------------------------------------------------------

ZHLParser::TypeSpecifierContext::TypeSpecifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::SimpleTypeContext* ZHLParser::TypeSpecifierContext::simpleType() {
  return getRuleContext<ZHLParser::SimpleTypeContext>(0);
}

ZHLParser::NestedNameContext* ZHLParser::TypeSpecifierContext::nestedName() {
  return getRuleContext<ZHLParser::NestedNameContext>(0);
}

std::vector<tree::TerminalNode *> ZHLParser::TypeSpecifierContext::Const() {
  return getTokens(ZHLParser::Const);
}

tree::TerminalNode* ZHLParser::TypeSpecifierContext::Const(size_t i) {
  return getToken(ZHLParser::Const, i);
}


size_t ZHLParser::TypeSpecifierContext::getRuleIndex() const {
  return ZHLParser::RuleTypeSpecifier;
}

void ZHLParser::TypeSpecifierContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeSpecifier(this);
}

void ZHLParser::TypeSpecifierContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeSpecifier(this);
}


std::any ZHLParser::TypeSpecifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitTypeSpecifier(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::TypeSpecifierContext* ZHLParser::typeSpecifier() {
  TypeSpecifierContext *_localctx = _tracker.createInstance<TypeSpecifierContext>(_ctx, getState());
  enterRule(_localctx, 26, ZHLParser::RuleTypeSpecifier);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(202);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Const) {
      setState(201);
      match(ZHLParser::Const);
    }
    setState(206);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ZHLParser::Unsigned:
      case ZHLParser::Signed:
      case ZHLParser::Long:
      case ZHLParser::Int:
      case ZHLParser::Short:
      case ZHLParser::Char:
      case ZHLParser::Bool:
      case ZHLParser::Float:
      case ZHLParser::Double:
      case ZHLParser::Void: {
        setState(204);
        simpleType();
        break;
      }

      case ZHLParser::Name: {
        setState(205);
        nestedName();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(209);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Const) {
      setState(208);
      match(ZHLParser::Const);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SimpleTypeContext ------------------------------------------------------------------

ZHLParser::SimpleTypeContext::SimpleTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::SimpleTypeSignednessContext* ZHLParser::SimpleTypeContext::simpleTypeSignedness() {
  return getRuleContext<ZHLParser::SimpleTypeSignednessContext>(0);
}

std::vector<ZHLParser::SimpleTypeLengthContext *> ZHLParser::SimpleTypeContext::simpleTypeLength() {
  return getRuleContexts<ZHLParser::SimpleTypeLengthContext>();
}

ZHLParser::SimpleTypeLengthContext* ZHLParser::SimpleTypeContext::simpleTypeLength(size_t i) {
  return getRuleContext<ZHLParser::SimpleTypeLengthContext>(i);
}

tree::TerminalNode* ZHLParser::SimpleTypeContext::Char() {
  return getToken(ZHLParser::Char, 0);
}

tree::TerminalNode* ZHLParser::SimpleTypeContext::Bool() {
  return getToken(ZHLParser::Bool, 0);
}

tree::TerminalNode* ZHLParser::SimpleTypeContext::Void() {
  return getToken(ZHLParser::Void, 0);
}

tree::TerminalNode* ZHLParser::SimpleTypeContext::Int() {
  return getToken(ZHLParser::Int, 0);
}

tree::TerminalNode* ZHLParser::SimpleTypeContext::Float() {
  return getToken(ZHLParser::Float, 0);
}

tree::TerminalNode* ZHLParser::SimpleTypeContext::Double() {
  return getToken(ZHLParser::Double, 0);
}


size_t ZHLParser::SimpleTypeContext::getRuleIndex() const {
  return ZHLParser::RuleSimpleType;
}

void ZHLParser::SimpleTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSimpleType(this);
}

void ZHLParser::SimpleTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSimpleType(this);
}


std::any ZHLParser::SimpleTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitSimpleType(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::SimpleTypeContext* ZHLParser::simpleType() {
  SimpleTypeContext *_localctx = _tracker.createInstance<SimpleTypeContext>(_ctx, getState());
  enterRule(_localctx, 28, ZHLParser::RuleSimpleType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(241);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(211);
      simpleTypeSignedness();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(213);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ZHLParser::Unsigned

      || _la == ZHLParser::Signed) {
        setState(212);
        simpleTypeSignedness();
      }
      setState(216); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(215);
        simpleTypeLength();
        setState(218); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == ZHLParser::Long

      || _la == ZHLParser::Short);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(221);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ZHLParser::Unsigned

      || _la == ZHLParser::Signed) {
        setState(220);
        simpleTypeSignedness();
      }
      setState(223);
      match(ZHLParser::Char);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(224);
      match(ZHLParser::Bool);
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(225);
      match(ZHLParser::Void);
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(227);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ZHLParser::Unsigned

      || _la == ZHLParser::Signed) {
        setState(226);
        simpleTypeSignedness();
      }
      setState(232);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == ZHLParser::Long

      || _la == ZHLParser::Short) {
        setState(229);
        simpleTypeLength();
        setState(234);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(235);
      match(ZHLParser::Int);
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(236);
      match(ZHLParser::Float);
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(238);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ZHLParser::Long

      || _la == ZHLParser::Short) {
        setState(237);
        simpleTypeLength();
      }
      setState(240);
      match(ZHLParser::Double);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SimpleTypeSignednessContext ------------------------------------------------------------------

ZHLParser::SimpleTypeSignednessContext::SimpleTypeSignednessContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::SimpleTypeSignednessContext::Signed() {
  return getToken(ZHLParser::Signed, 0);
}

tree::TerminalNode* ZHLParser::SimpleTypeSignednessContext::Unsigned() {
  return getToken(ZHLParser::Unsigned, 0);
}


size_t ZHLParser::SimpleTypeSignednessContext::getRuleIndex() const {
  return ZHLParser::RuleSimpleTypeSignedness;
}

void ZHLParser::SimpleTypeSignednessContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSimpleTypeSignedness(this);
}

void ZHLParser::SimpleTypeSignednessContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSimpleTypeSignedness(this);
}


std::any ZHLParser::SimpleTypeSignednessContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitSimpleTypeSignedness(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::SimpleTypeSignednessContext* ZHLParser::simpleTypeSignedness() {
  SimpleTypeSignednessContext *_localctx = _tracker.createInstance<SimpleTypeSignednessContext>(_ctx, getState());
  enterRule(_localctx, 30, ZHLParser::RuleSimpleTypeSignedness);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(243);
    _la = _input->LA(1);
    if (!(_la == ZHLParser::Unsigned

    || _la == ZHLParser::Signed)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SimpleTypeLengthContext ------------------------------------------------------------------

ZHLParser::SimpleTypeLengthContext::SimpleTypeLengthContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::SimpleTypeLengthContext::Long() {
  return getToken(ZHLParser::Long, 0);
}

tree::TerminalNode* ZHLParser::SimpleTypeLengthContext::Short() {
  return getToken(ZHLParser::Short, 0);
}


size_t ZHLParser::SimpleTypeLengthContext::getRuleIndex() const {
  return ZHLParser::RuleSimpleTypeLength;
}

void ZHLParser::SimpleTypeLengthContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSimpleTypeLength(this);
}

void ZHLParser::SimpleTypeLengthContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSimpleTypeLength(this);
}


std::any ZHLParser::SimpleTypeLengthContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitSimpleTypeLength(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::SimpleTypeLengthContext* ZHLParser::simpleTypeLength() {
  SimpleTypeLengthContext *_localctx = _tracker.createInstance<SimpleTypeLengthContext>(_ctx, getState());
  enterRule(_localctx, 32, ZHLParser::RuleSimpleTypeLength);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(245);
    _la = _input->LA(1);
    if (!(_la == ZHLParser::Long

    || _la == ZHLParser::Short)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NestedNameContext ------------------------------------------------------------------

ZHLParser::NestedNameContext::NestedNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::NestedNameContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::NestedNameContext::Colon() {
  return getTokens(ZHLParser::Colon);
}

tree::TerminalNode* ZHLParser::NestedNameContext::Colon(size_t i) {
  return getToken(ZHLParser::Colon, i);
}

std::vector<ZHLParser::FunctionNameContext *> ZHLParser::NestedNameContext::functionName() {
  return getRuleContexts<ZHLParser::FunctionNameContext>();
}

ZHLParser::FunctionNameContext* ZHLParser::NestedNameContext::functionName(size_t i) {
  return getRuleContext<ZHLParser::FunctionNameContext>(i);
}


size_t ZHLParser::NestedNameContext::getRuleIndex() const {
  return ZHLParser::RuleNestedName;
}

void ZHLParser::NestedNameContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNestedName(this);
}

void ZHLParser::NestedNameContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNestedName(this);
}


std::any ZHLParser::NestedNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitNestedName(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::NestedNameContext* ZHLParser::nestedName() {
  NestedNameContext *_localctx = _tracker.createInstance<NestedNameContext>(_ctx, getState());
  enterRule(_localctx, 34, ZHLParser::RuleNestedName);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(247);
    match(ZHLParser::Name);
    setState(253);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Colon) {
      setState(248);
      match(ZHLParser::Colon);
      setState(249);
      match(ZHLParser::Colon);
      setState(250);
      functionName();
      setState(255);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PointerAttributeContext ------------------------------------------------------------------

ZHLParser::PointerAttributeContext::PointerAttributeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> ZHLParser::PointerAttributeContext::CppRef() {
  return getTokens(ZHLParser::CppRef);
}

tree::TerminalNode* ZHLParser::PointerAttributeContext::CppRef(size_t i) {
  return getToken(ZHLParser::CppRef, i);
}

tree::TerminalNode* ZHLParser::PointerAttributeContext::Star() {
  return getToken(ZHLParser::Star, 0);
}

tree::TerminalNode* ZHLParser::PointerAttributeContext::Const() {
  return getToken(ZHLParser::Const, 0);
}


size_t ZHLParser::PointerAttributeContext::getRuleIndex() const {
  return ZHLParser::RulePointerAttribute;
}

void ZHLParser::PointerAttributeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPointerAttribute(this);
}

void ZHLParser::PointerAttributeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPointerAttribute(this);
}


std::any ZHLParser::PointerAttributeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitPointerAttribute(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::PointerAttributeContext* ZHLParser::pointerAttribute() {
  PointerAttributeContext *_localctx = _tracker.createInstance<PointerAttributeContext>(_ctx, getState());
  enterRule(_localctx, 36, ZHLParser::RulePointerAttribute);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(264);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ZHLParser::CppRef: {
        enterOuterAlt(_localctx, 1);
        setState(256);
        match(ZHLParser::CppRef);
        setState(258);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx)) {
        case 1: {
          setState(257);
          match(ZHLParser::CppRef);
          break;
        }

        default:
          break;
        }
        break;
      }

      case ZHLParser::Star: {
        enterOuterAlt(_localctx, 2);
        setState(260);
        match(ZHLParser::Star);
        setState(262);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ZHLParser::Const) {
          setState(261);
          match(ZHLParser::Const);
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GenericCodeContext ------------------------------------------------------------------

ZHLParser::GenericCodeContext::GenericCodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::GenericCodeContext::GenericCode() {
  return getToken(ZHLParser::GenericCode, 0);
}


size_t ZHLParser::GenericCodeContext::getRuleIndex() const {
  return ZHLParser::RuleGenericCode;
}

void ZHLParser::GenericCodeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGenericCode(this);
}

void ZHLParser::GenericCodeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGenericCode(this);
}


std::any ZHLParser::GenericCodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitGenericCode(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::GenericCodeContext* ZHLParser::genericCode() {
  GenericCodeContext *_localctx = _tracker.createInstance<GenericCodeContext>(_ctx, getState());
  enterRule(_localctx, 38, ZHLParser::RuleGenericCode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(266);
    match(ZHLParser::GenericCode);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassContext ------------------------------------------------------------------

ZHLParser::ClassContext::ClassContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::ClassContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

tree::TerminalNode* ZHLParser::ClassContext::LeftBracket() {
  return getToken(ZHLParser::LeftBracket, 0);
}

ZHLParser::ClassBodyContext* ZHLParser::ClassContext::classBody() {
  return getRuleContext<ZHLParser::ClassBodyContext>(0);
}

tree::TerminalNode* ZHLParser::ClassContext::RightBracket() {
  return getToken(ZHLParser::RightBracket, 0);
}

tree::TerminalNode* ZHLParser::ClassContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}

tree::TerminalNode* ZHLParser::ClassContext::Class() {
  return getToken(ZHLParser::Class, 0);
}

tree::TerminalNode* ZHLParser::ClassContext::Struct() {
  return getToken(ZHLParser::Struct, 0);
}

ZHLParser::DependsContext* ZHLParser::ClassContext::depends() {
  return getRuleContext<ZHLParser::DependsContext>(0);
}

std::vector<tree::TerminalNode *> ZHLParser::ClassContext::Colon() {
  return getTokens(ZHLParser::Colon);
}

tree::TerminalNode* ZHLParser::ClassContext::Colon(size_t i) {
  return getToken(ZHLParser::Colon, i);
}

ZHLParser::InheritanceContext* ZHLParser::ClassContext::inheritance() {
  return getRuleContext<ZHLParser::InheritanceContext>(0);
}

tree::TerminalNode* ZHLParser::ClassContext::Number() {
  return getToken(ZHLParser::Number, 0);
}


size_t ZHLParser::ClassContext::getRuleIndex() const {
  return ZHLParser::RuleClass;
}

void ZHLParser::ClassContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterClass(this);
}

void ZHLParser::ClassContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitClass(this);
}


std::any ZHLParser::ClassContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitClass(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ClassContext* ZHLParser::class_() {
  ClassContext *_localctx = _tracker.createInstance<ClassContext>(_ctx, getState());
  enterRule(_localctx, 40, ZHLParser::RuleClass);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(268);
    _la = _input->LA(1);
    if (!(_la == ZHLParser::Class

    || _la == ZHLParser::Struct)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(269);
    match(ZHLParser::Name);
    setState(271);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Depends) {
      setState(270);
      depends();
    }
    setState(275);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Colon) {
      setState(273);
      match(ZHLParser::Colon);
      setState(274);
      inheritance();
    }
    setState(277);
    match(ZHLParser::LeftBracket);
    setState(278);
    classBody();
    setState(279);
    match(ZHLParser::RightBracket);
    setState(282);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Colon) {
      setState(280);
      match(ZHLParser::Colon);
      setState(281);
      match(ZHLParser::Number);
    }
    setState(284);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InheritanceContext ------------------------------------------------------------------

ZHLParser::InheritanceContext::InheritanceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ZHLParser::InheritanceDeclContext *> ZHLParser::InheritanceContext::inheritanceDecl() {
  return getRuleContexts<ZHLParser::InheritanceDeclContext>();
}

ZHLParser::InheritanceDeclContext* ZHLParser::InheritanceContext::inheritanceDecl(size_t i) {
  return getRuleContext<ZHLParser::InheritanceDeclContext>(i);
}

std::vector<tree::TerminalNode *> ZHLParser::InheritanceContext::Comma() {
  return getTokens(ZHLParser::Comma);
}

tree::TerminalNode* ZHLParser::InheritanceContext::Comma(size_t i) {
  return getToken(ZHLParser::Comma, i);
}


size_t ZHLParser::InheritanceContext::getRuleIndex() const {
  return ZHLParser::RuleInheritance;
}

void ZHLParser::InheritanceContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInheritance(this);
}

void ZHLParser::InheritanceContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInheritance(this);
}


std::any ZHLParser::InheritanceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitInheritance(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::InheritanceContext* ZHLParser::inheritance() {
  InheritanceContext *_localctx = _tracker.createInstance<InheritanceContext>(_ctx, getState());
  enterRule(_localctx, 42, ZHLParser::RuleInheritance);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(286);
    inheritanceDecl();
    setState(291);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(287);
      match(ZHLParser::Comma);
      setState(288);
      inheritanceDecl();
      setState(293);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InheritanceDeclContext ------------------------------------------------------------------

ZHLParser::InheritanceDeclContext::InheritanceDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::NestedNameContext* ZHLParser::InheritanceDeclContext::nestedName() {
  return getRuleContext<ZHLParser::NestedNameContext>(0);
}

tree::TerminalNode* ZHLParser::InheritanceDeclContext::Visibility() {
  return getToken(ZHLParser::Visibility, 0);
}


size_t ZHLParser::InheritanceDeclContext::getRuleIndex() const {
  return ZHLParser::RuleInheritanceDecl;
}

void ZHLParser::InheritanceDeclContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInheritanceDecl(this);
}

void ZHLParser::InheritanceDeclContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInheritanceDecl(this);
}


std::any ZHLParser::InheritanceDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitInheritanceDecl(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::InheritanceDeclContext* ZHLParser::inheritanceDecl() {
  InheritanceDeclContext *_localctx = _tracker.createInstance<InheritanceDeclContext>(_ctx, getState());
  enterRule(_localctx, 44, ZHLParser::RuleInheritanceDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(295);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Visibility) {
      setState(294);
      match(ZHLParser::Visibility);
    }
    setState(297);
    nestedName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DependsContext ------------------------------------------------------------------

ZHLParser::DependsContext::DependsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::DependsContext::Depends() {
  return getToken(ZHLParser::Depends, 0);
}

tree::TerminalNode* ZHLParser::DependsContext::LeftParen() {
  return getToken(ZHLParser::LeftParen, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::DependsContext::Name() {
  return getTokens(ZHLParser::Name);
}

tree::TerminalNode* ZHLParser::DependsContext::Name(size_t i) {
  return getToken(ZHLParser::Name, i);
}

tree::TerminalNode* ZHLParser::DependsContext::RightParen() {
  return getToken(ZHLParser::RightParen, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::DependsContext::Comma() {
  return getTokens(ZHLParser::Comma);
}

tree::TerminalNode* ZHLParser::DependsContext::Comma(size_t i) {
  return getToken(ZHLParser::Comma, i);
}


size_t ZHLParser::DependsContext::getRuleIndex() const {
  return ZHLParser::RuleDepends;
}

void ZHLParser::DependsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDepends(this);
}

void ZHLParser::DependsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDepends(this);
}


std::any ZHLParser::DependsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitDepends(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::DependsContext* ZHLParser::depends() {
  DependsContext *_localctx = _tracker.createInstance<DependsContext>(_ctx, getState());
  enterRule(_localctx, 46, ZHLParser::RuleDepends);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(299);
    match(ZHLParser::Depends);
    setState(300);
    match(ZHLParser::LeftParen);
    setState(305);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(301);
        match(ZHLParser::Name);
        setState(302);
        match(ZHLParser::Comma); 
      }
      setState(307);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx);
    }
    setState(308);
    match(ZHLParser::Name);
    setState(309);
    match(ZHLParser::RightParen);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassBodyContext ------------------------------------------------------------------

ZHLParser::ClassBodyContext::ClassBodyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ZHLParser::VtableContext *> ZHLParser::ClassBodyContext::vtable() {
  return getRuleContexts<ZHLParser::VtableContext>();
}

ZHLParser::VtableContext* ZHLParser::ClassBodyContext::vtable(size_t i) {
  return getRuleContext<ZHLParser::VtableContext>(i);
}

std::vector<ZHLParser::ClassSignatureContext *> ZHLParser::ClassBodyContext::classSignature() {
  return getRuleContexts<ZHLParser::ClassSignatureContext>();
}

ZHLParser::ClassSignatureContext* ZHLParser::ClassBodyContext::classSignature(size_t i) {
  return getRuleContext<ZHLParser::ClassSignatureContext>(i);
}

std::vector<ZHLParser::GenericCodeContext *> ZHLParser::ClassBodyContext::genericCode() {
  return getRuleContexts<ZHLParser::GenericCodeContext>();
}

ZHLParser::GenericCodeContext* ZHLParser::ClassBodyContext::genericCode(size_t i) {
  return getRuleContext<ZHLParser::GenericCodeContext>(i);
}

std::vector<ZHLParser::ClassFieldContext *> ZHLParser::ClassBodyContext::classField() {
  return getRuleContexts<ZHLParser::ClassFieldContext>();
}

ZHLParser::ClassFieldContext* ZHLParser::ClassBodyContext::classField(size_t i) {
  return getRuleContext<ZHLParser::ClassFieldContext>(i);
}

std::vector<ZHLParser::FunctionPtrContext *> ZHLParser::ClassBodyContext::functionPtr() {
  return getRuleContexts<ZHLParser::FunctionPtrContext>();
}

ZHLParser::FunctionPtrContext* ZHLParser::ClassBodyContext::functionPtr(size_t i) {
  return getRuleContext<ZHLParser::FunctionPtrContext>(i);
}

std::vector<ZHLParser::ForwardDeclContext *> ZHLParser::ClassBodyContext::forwardDecl() {
  return getRuleContexts<ZHLParser::ForwardDeclContext>();
}

ZHLParser::ForwardDeclContext* ZHLParser::ClassBodyContext::forwardDecl(size_t i) {
  return getRuleContext<ZHLParser::ForwardDeclContext>(i);
}


size_t ZHLParser::ClassBodyContext::getRuleIndex() const {
  return ZHLParser::RuleClassBody;
}

void ZHLParser::ClassBodyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterClassBody(this);
}

void ZHLParser::ClassBodyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitClassBody(this);
}


std::any ZHLParser::ClassBodyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitClassBody(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ClassBodyContext* ZHLParser::classBody() {
  ClassBodyContext *_localctx = _tracker.createInstance<ClassBodyContext>(_ctx, getState());
  enterRule(_localctx, 48, ZHLParser::RuleClassBody);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(319);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 20) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 20)) & 2458965396546518947) != 0)) {
      setState(317);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx)) {
      case 1: {
        setState(311);
        vtable();
        break;
      }

      case 2: {
        setState(312);
        classSignature();
        break;
      }

      case 3: {
        setState(313);
        genericCode();
        break;
      }

      case 4: {
        setState(314);
        classField();
        break;
      }

      case 5: {
        setState(315);
        functionPtr();
        break;
      }

      case 6: {
        setState(316);
        forwardDecl();
        break;
      }

      default:
        break;
      }
      setState(321);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VtableContext ------------------------------------------------------------------

ZHLParser::VtableContext::VtableContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::VtableContext::Vtable() {
  return getToken(ZHLParser::Vtable, 0);
}

tree::TerminalNode* ZHLParser::VtableContext::LeftBracket() {
  return getToken(ZHLParser::LeftBracket, 0);
}

tree::TerminalNode* ZHLParser::VtableContext::RightBracket() {
  return getToken(ZHLParser::RightBracket, 0);
}

tree::TerminalNode* ZHLParser::VtableContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}

std::vector<ZHLParser::VtableEntryContext *> ZHLParser::VtableContext::vtableEntry() {
  return getRuleContexts<ZHLParser::VtableEntryContext>();
}

ZHLParser::VtableEntryContext* ZHLParser::VtableContext::vtableEntry(size_t i) {
  return getRuleContext<ZHLParser::VtableEntryContext>(i);
}


size_t ZHLParser::VtableContext::getRuleIndex() const {
  return ZHLParser::RuleVtable;
}

void ZHLParser::VtableContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVtable(this);
}

void ZHLParser::VtableContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVtable(this);
}


std::any ZHLParser::VtableContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitVtable(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::VtableContext* ZHLParser::vtable() {
  VtableContext *_localctx = _tracker.createInstance<VtableContext>(_ctx, getState());
  enterRule(_localctx, 50, ZHLParser::RuleVtable);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(322);
    match(ZHLParser::Vtable);
    setState(323);
    match(ZHLParser::LeftBracket);
    setState(325); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(324);
      vtableEntry();
      setState(327); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (((((_la - 42) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 42)) & 2147483655) != 0));
    setState(329);
    match(ZHLParser::RightBracket);
    setState(330);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VtableEntryContext ------------------------------------------------------------------

ZHLParser::VtableEntryContext::VtableEntryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::VtableSignatureContext* ZHLParser::VtableEntryContext::vtableSignature() {
  return getRuleContext<ZHLParser::VtableSignatureContext>(0);
}

tree::TerminalNode* ZHLParser::VtableEntryContext::Skip() {
  return getToken(ZHLParser::Skip, 0);
}

tree::TerminalNode* ZHLParser::VtableEntryContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}

tree::TerminalNode* ZHLParser::VtableEntryContext::Pure() {
  return getToken(ZHLParser::Pure, 0);
}

ZHLParser::ClassFunctionContext* ZHLParser::VtableEntryContext::classFunction() {
  return getRuleContext<ZHLParser::ClassFunctionContext>(0);
}


size_t ZHLParser::VtableEntryContext::getRuleIndex() const {
  return ZHLParser::RuleVtableEntry;
}

void ZHLParser::VtableEntryContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVtableEntry(this);
}

void ZHLParser::VtableEntryContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVtableEntry(this);
}


std::any ZHLParser::VtableEntryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitVtableEntry(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::VtableEntryContext* ZHLParser::vtableEntry() {
  VtableEntryContext *_localctx = _tracker.createInstance<VtableEntryContext>(_ctx, getState());
  enterRule(_localctx, 52, ZHLParser::RuleVtableEntry);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(337);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ZHLParser::Override:
      case ZHLParser::Signature: {
        enterOuterAlt(_localctx, 1);
        setState(332);
        vtableSignature();
        break;
      }

      case ZHLParser::Skip: {
        enterOuterAlt(_localctx, 2);
        setState(333);
        match(ZHLParser::Skip);
        setState(334);
        match(ZHLParser::Semi);
        break;
      }

      case ZHLParser::Pure: {
        enterOuterAlt(_localctx, 3);
        setState(335);
        match(ZHLParser::Pure);
        setState(336);
        classFunction();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VtableSignatureContext ------------------------------------------------------------------

ZHLParser::VtableSignatureContext::VtableSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::ClassSignatureContext* ZHLParser::VtableSignatureContext::classSignature() {
  return getRuleContext<ZHLParser::ClassSignatureContext>(0);
}

tree::TerminalNode* ZHLParser::VtableSignatureContext::Override() {
  return getToken(ZHLParser::Override, 0);
}


size_t ZHLParser::VtableSignatureContext::getRuleIndex() const {
  return ZHLParser::RuleVtableSignature;
}

void ZHLParser::VtableSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVtableSignature(this);
}

void ZHLParser::VtableSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVtableSignature(this);
}


std::any ZHLParser::VtableSignatureContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitVtableSignature(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::VtableSignatureContext* ZHLParser::vtableSignature() {
  VtableSignatureContext *_localctx = _tracker.createInstance<VtableSignatureContext>(_ctx, getState());
  enterRule(_localctx, 54, ZHLParser::RuleVtableSignature);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(340);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Override) {
      setState(339);
      match(ZHLParser::Override);
    }
    setState(342);
    classSignature();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassSignatureContext ------------------------------------------------------------------

ZHLParser::ClassSignatureContext::ClassSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::ClassSignatureContext::Signature() {
  return getToken(ZHLParser::Signature, 0);
}

ZHLParser::ClassFunctionContext* ZHLParser::ClassSignatureContext::classFunction() {
  return getRuleContext<ZHLParser::ClassFunctionContext>(0);
}


size_t ZHLParser::ClassSignatureContext::getRuleIndex() const {
  return ZHLParser::RuleClassSignature;
}

void ZHLParser::ClassSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterClassSignature(this);
}

void ZHLParser::ClassSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitClassSignature(this);
}


std::any ZHLParser::ClassSignatureContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitClassSignature(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ClassSignatureContext* ZHLParser::classSignature() {
  ClassSignatureContext *_localctx = _tracker.createInstance<ClassSignatureContext>(_ctx, getState());
  enterRule(_localctx, 56, ZHLParser::RuleClassSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(344);
    match(ZHLParser::Signature);
    setState(345);
    classFunction();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassFunctionContext ------------------------------------------------------------------

ZHLParser::ClassFunctionContext::ClassFunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::TypeContext* ZHLParser::ClassFunctionContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

ZHLParser::FunctionNameContext* ZHLParser::ClassFunctionContext::functionName() {
  return getRuleContext<ZHLParser::FunctionNameContext>(0);
}

tree::TerminalNode* ZHLParser::ClassFunctionContext::LeftParen() {
  return getToken(ZHLParser::LeftParen, 0);
}

tree::TerminalNode* ZHLParser::ClassFunctionContext::RightParen() {
  return getToken(ZHLParser::RightParen, 0);
}

tree::TerminalNode* ZHLParser::ClassFunctionContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::ClassFunctionContext::Qualifier() {
  return getTokens(ZHLParser::Qualifier);
}

tree::TerminalNode* ZHLParser::ClassFunctionContext::Qualifier(size_t i) {
  return getToken(ZHLParser::Qualifier, i);
}

ZHLParser::FunArgsContext* ZHLParser::ClassFunctionContext::funArgs() {
  return getRuleContext<ZHLParser::FunArgsContext>(0);
}


size_t ZHLParser::ClassFunctionContext::getRuleIndex() const {
  return ZHLParser::RuleClassFunction;
}

void ZHLParser::ClassFunctionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterClassFunction(this);
}

void ZHLParser::ClassFunctionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitClassFunction(this);
}


std::any ZHLParser::ClassFunctionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitClassFunction(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ClassFunctionContext* ZHLParser::classFunction() {
  ClassFunctionContext *_localctx = _tracker.createInstance<ClassFunctionContext>(_ctx, getState());
  enterRule(_localctx, 58, ZHLParser::RuleClassFunction);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(350);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Qualifier) {
      setState(347);
      match(ZHLParser::Qualifier);
      setState(352);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(353);
    type();
    setState(354);
    functionName();
    setState(355);
    match(ZHLParser::LeftParen);
    setState(357);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 25) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 25)) & 4503599627374589) != 0)) {
      setState(356);
      funArgs();
    }
    setState(359);
    match(ZHLParser::RightParen);
    setState(360);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionNameContext ------------------------------------------------------------------

ZHLParser::FunctionNameContext::FunctionNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::FunctionNameContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

tree::TerminalNode* ZHLParser::FunctionNameContext::Operator() {
  return getToken(ZHLParser::Operator, 0);
}

tree::TerminalNode* ZHLParser::FunctionNameContext::OpSymbol() {
  return getToken(ZHLParser::OpSymbol, 0);
}


size_t ZHLParser::FunctionNameContext::getRuleIndex() const {
  return ZHLParser::RuleFunctionName;
}

void ZHLParser::FunctionNameContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionName(this);
}

void ZHLParser::FunctionNameContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionName(this);
}


std::any ZHLParser::FunctionNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitFunctionName(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::FunctionNameContext* ZHLParser::functionName() {
  FunctionNameContext *_localctx = _tracker.createInstance<FunctionNameContext>(_ctx, getState());
  enterRule(_localctx, 60, ZHLParser::RuleFunctionName);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(365);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ZHLParser::Name: {
        enterOuterAlt(_localctx, 1);
        setState(362);
        match(ZHLParser::Name);
        break;
      }

      case ZHLParser::Operator: {
        enterOuterAlt(_localctx, 2);
        setState(363);
        match(ZHLParser::Operator);
        setState(364);
        match(ZHLParser::OpSymbol);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassFieldContext ------------------------------------------------------------------

ZHLParser::ClassFieldContext::ClassFieldContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::TypeContext* ZHLParser::ClassFieldContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

std::vector<ZHLParser::InnerFieldContext *> ZHLParser::ClassFieldContext::innerField() {
  return getRuleContexts<ZHLParser::InnerFieldContext>();
}

ZHLParser::InnerFieldContext* ZHLParser::ClassFieldContext::innerField(size_t i) {
  return getRuleContext<ZHLParser::InnerFieldContext>(i);
}

tree::TerminalNode* ZHLParser::ClassFieldContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::ClassFieldContext::Comma() {
  return getTokens(ZHLParser::Comma);
}

tree::TerminalNode* ZHLParser::ClassFieldContext::Comma(size_t i) {
  return getToken(ZHLParser::Comma, i);
}


size_t ZHLParser::ClassFieldContext::getRuleIndex() const {
  return ZHLParser::RuleClassField;
}

void ZHLParser::ClassFieldContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterClassField(this);
}

void ZHLParser::ClassFieldContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitClassField(this);
}


std::any ZHLParser::ClassFieldContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitClassField(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ClassFieldContext* ZHLParser::classField() {
  ClassFieldContext *_localctx = _tracker.createInstance<ClassFieldContext>(_ctx, getState());
  enterRule(_localctx, 62, ZHLParser::RuleClassField);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(367);
    type();
    setState(368);
    innerField();
    setState(373);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(369);
      match(ZHLParser::Comma);
      setState(370);
      innerField();
      setState(375);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(376);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InnerFieldContext ------------------------------------------------------------------

ZHLParser::InnerFieldContext::InnerFieldContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::FullNameContext* ZHLParser::InnerFieldContext::fullName() {
  return getRuleContext<ZHLParser::FullNameContext>(0);
}

tree::TerminalNode* ZHLParser::InnerFieldContext::Colon() {
  return getToken(ZHLParser::Colon, 0);
}

tree::TerminalNode* ZHLParser::InnerFieldContext::Number() {
  return getToken(ZHLParser::Number, 0);
}

std::vector<ZHLParser::InnerFieldPtrContext *> ZHLParser::InnerFieldContext::innerFieldPtr() {
  return getRuleContexts<ZHLParser::InnerFieldPtrContext>();
}

ZHLParser::InnerFieldPtrContext* ZHLParser::InnerFieldContext::innerFieldPtr(size_t i) {
  return getRuleContext<ZHLParser::InnerFieldPtrContext>(i);
}


size_t ZHLParser::InnerFieldContext::getRuleIndex() const {
  return ZHLParser::RuleInnerField;
}

void ZHLParser::InnerFieldContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInnerField(this);
}

void ZHLParser::InnerFieldContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInnerField(this);
}


std::any ZHLParser::InnerFieldContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitInnerField(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::InnerFieldContext* ZHLParser::innerField() {
  InnerFieldContext *_localctx = _tracker.createInstance<InnerFieldContext>(_ctx, getState());
  enterRule(_localctx, 64, ZHLParser::RuleInnerField);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(381);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Star) {
      setState(378);
      innerFieldPtr();
      setState(383);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(384);
    fullName();
    setState(385);
    match(ZHLParser::Colon);
    setState(386);
    match(ZHLParser::Number);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InnerFieldPtrContext ------------------------------------------------------------------

ZHLParser::InnerFieldPtrContext::InnerFieldPtrContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::InnerFieldPtrContext::Star() {
  return getToken(ZHLParser::Star, 0);
}

tree::TerminalNode* ZHLParser::InnerFieldPtrContext::Const() {
  return getToken(ZHLParser::Const, 0);
}


size_t ZHLParser::InnerFieldPtrContext::getRuleIndex() const {
  return ZHLParser::RuleInnerFieldPtr;
}

void ZHLParser::InnerFieldPtrContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInnerFieldPtr(this);
}

void ZHLParser::InnerFieldPtrContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInnerFieldPtr(this);
}


std::any ZHLParser::InnerFieldPtrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitInnerFieldPtr(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::InnerFieldPtrContext* ZHLParser::innerFieldPtr() {
  InnerFieldPtrContext *_localctx = _tracker.createInstance<InnerFieldPtrContext>(_ctx, getState());
  enterRule(_localctx, 66, ZHLParser::RuleInnerFieldPtr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(388);
    match(ZHLParser::Star);
    setState(390);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Const) {
      setState(389);
      match(ZHLParser::Const);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FullNameContext ------------------------------------------------------------------

ZHLParser::FullNameContext::FullNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::FullNameContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

tree::TerminalNode* ZHLParser::FullNameContext::LeftRBracket() {
  return getToken(ZHLParser::LeftRBracket, 0);
}

tree::TerminalNode* ZHLParser::FullNameContext::Number() {
  return getToken(ZHLParser::Number, 0);
}

tree::TerminalNode* ZHLParser::FullNameContext::RightRBracket() {
  return getToken(ZHLParser::RightRBracket, 0);
}


size_t ZHLParser::FullNameContext::getRuleIndex() const {
  return ZHLParser::RuleFullName;
}

void ZHLParser::FullNameContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFullName(this);
}

void ZHLParser::FullNameContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFullName(this);
}


std::any ZHLParser::FullNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitFullName(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::FullNameContext* ZHLParser::fullName() {
  FullNameContext *_localctx = _tracker.createInstance<FullNameContext>(_ctx, getState());
  enterRule(_localctx, 68, ZHLParser::RuleFullName);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(392);
    match(ZHLParser::Name);
    setState(396);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::LeftRBracket) {
      setState(393);
      match(ZHLParser::LeftRBracket);
      setState(394);
      match(ZHLParser::Number);
      setState(395);
      match(ZHLParser::RightRBracket);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SignatureContext ------------------------------------------------------------------

ZHLParser::SignatureContext::SignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::SignatureContext::Signature() {
  return getToken(ZHLParser::Signature, 0);
}

ZHLParser::FunctionContext* ZHLParser::SignatureContext::function() {
  return getRuleContext<ZHLParser::FunctionContext>(0);
}

tree::TerminalNode* ZHLParser::SignatureContext::ReferenceSignature() {
  return getToken(ZHLParser::ReferenceSignature, 0);
}

ZHLParser::ReferenceContext* ZHLParser::SignatureContext::reference() {
  return getRuleContext<ZHLParser::ReferenceContext>(0);
}


size_t ZHLParser::SignatureContext::getRuleIndex() const {
  return ZHLParser::RuleSignature;
}

void ZHLParser::SignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSignature(this);
}

void ZHLParser::SignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSignature(this);
}


std::any ZHLParser::SignatureContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitSignature(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::SignatureContext* ZHLParser::signature() {
  SignatureContext *_localctx = _tracker.createInstance<SignatureContext>(_ctx, getState());
  enterRule(_localctx, 70, ZHLParser::RuleSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(402);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ZHLParser::Signature: {
        setState(398);
        match(ZHLParser::Signature);
        setState(399);
        function();
        break;
      }

      case ZHLParser::ReferenceSignature: {
        setState(400);
        match(ZHLParser::ReferenceSignature);
        setState(401);
        reference();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ForwardDeclContext ------------------------------------------------------------------

ZHLParser::ForwardDeclContext::ForwardDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::ForwardDeclContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

tree::TerminalNode* ZHLParser::ForwardDeclContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}

tree::TerminalNode* ZHLParser::ForwardDeclContext::Struct() {
  return getToken(ZHLParser::Struct, 0);
}

tree::TerminalNode* ZHLParser::ForwardDeclContext::Class() {
  return getToken(ZHLParser::Class, 0);
}


size_t ZHLParser::ForwardDeclContext::getRuleIndex() const {
  return ZHLParser::RuleForwardDecl;
}

void ZHLParser::ForwardDeclContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForwardDecl(this);
}

void ZHLParser::ForwardDeclContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForwardDecl(this);
}


std::any ZHLParser::ForwardDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitForwardDecl(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ForwardDeclContext* ZHLParser::forwardDecl() {
  ForwardDeclContext *_localctx = _tracker.createInstance<ForwardDeclContext>(_ctx, getState());
  enterRule(_localctx, 72, ZHLParser::RuleForwardDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(404);
    _la = _input->LA(1);
    if (!(_la == ZHLParser::Class

    || _la == ZHLParser::Struct)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(405);
    match(ZHLParser::Name);
    setState(406);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypedefContext ------------------------------------------------------------------

ZHLParser::TypedefContext::TypedefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::TypedefContext::Typedef() {
  return getToken(ZHLParser::Typedef, 0);
}

ZHLParser::TypeContext* ZHLParser::TypedefContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

tree::TerminalNode* ZHLParser::TypedefContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

tree::TerminalNode* ZHLParser::TypedefContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}


size_t ZHLParser::TypedefContext::getRuleIndex() const {
  return ZHLParser::RuleTypedef;
}

void ZHLParser::TypedefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypedef(this);
}

void ZHLParser::TypedefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypedef(this);
}


std::any ZHLParser::TypedefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitTypedef(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::TypedefContext* ZHLParser::typedef_() {
  TypedefContext *_localctx = _tracker.createInstance<TypedefContext>(_ctx, getState());
  enterRule(_localctx, 74, ZHLParser::RuleTypedef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(408);
    match(ZHLParser::Typedef);
    setState(409);
    type();
    setState(410);
    match(ZHLParser::Name);
    setState(411);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionPtrContext ------------------------------------------------------------------

ZHLParser::FunctionPtrContext::FunctionPtrContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::FptrContext* ZHLParser::FunctionPtrContext::fptr() {
  return getRuleContext<ZHLParser::FptrContext>(0);
}

ZHLParser::MemberPtrContext* ZHLParser::FunctionPtrContext::memberPtr() {
  return getRuleContext<ZHLParser::MemberPtrContext>(0);
}


size_t ZHLParser::FunctionPtrContext::getRuleIndex() const {
  return ZHLParser::RuleFunctionPtr;
}

void ZHLParser::FunctionPtrContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionPtr(this);
}

void ZHLParser::FunctionPtrContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionPtr(this);
}


std::any ZHLParser::FunctionPtrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitFunctionPtr(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::FunctionPtrContext* ZHLParser::functionPtr() {
  FunctionPtrContext *_localctx = _tracker.createInstance<FunctionPtrContext>(_ctx, getState());
  enterRule(_localctx, 76, ZHLParser::RuleFunctionPtr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(415);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 47, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(413);
      fptr();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(414);
      memberPtr();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FptrContext ------------------------------------------------------------------

ZHLParser::FptrContext::FptrContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::TypeContext* ZHLParser::FptrContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

std::vector<tree::TerminalNode *> ZHLParser::FptrContext::LeftParen() {
  return getTokens(ZHLParser::LeftParen);
}

tree::TerminalNode* ZHLParser::FptrContext::LeftParen(size_t i) {
  return getToken(ZHLParser::LeftParen, i);
}

tree::TerminalNode* ZHLParser::FptrContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::FptrContext::RightParen() {
  return getTokens(ZHLParser::RightParen);
}

tree::TerminalNode* ZHLParser::FptrContext::RightParen(size_t i) {
  return getToken(ZHLParser::RightParen, i);
}

tree::TerminalNode* ZHLParser::FptrContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}

tree::TerminalNode* ZHLParser::FptrContext::CallingConvention() {
  return getToken(ZHLParser::CallingConvention, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::FptrContext::Star() {
  return getTokens(ZHLParser::Star);
}

tree::TerminalNode* ZHLParser::FptrContext::Star(size_t i) {
  return getToken(ZHLParser::Star, i);
}

ZHLParser::OptNamedFunArgsContext* ZHLParser::FptrContext::optNamedFunArgs() {
  return getRuleContext<ZHLParser::OptNamedFunArgsContext>(0);
}


size_t ZHLParser::FptrContext::getRuleIndex() const {
  return ZHLParser::RuleFptr;
}

void ZHLParser::FptrContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFptr(this);
}

void ZHLParser::FptrContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFptr(this);
}


std::any ZHLParser::FptrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitFptr(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::FptrContext* ZHLParser::fptr() {
  FptrContext *_localctx = _tracker.createInstance<FptrContext>(_ctx, getState());
  enterRule(_localctx, 78, ZHLParser::RuleFptr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(417);
    type();
    setState(418);
    match(ZHLParser::LeftParen);
    setState(420);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::CallingConvention) {
      setState(419);
      match(ZHLParser::CallingConvention);
    }
    setState(423); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(422);
      match(ZHLParser::Star);
      setState(425); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == ZHLParser::Star);
    setState(427);
    match(ZHLParser::Name);
    setState(428);
    match(ZHLParser::RightParen);
    setState(429);
    match(ZHLParser::LeftParen);
    setState(431);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 25) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 25)) & 4503599627374589) != 0)) {
      setState(430);
      optNamedFunArgs();
    }
    setState(433);
    match(ZHLParser::RightParen);
    setState(434);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MemberPtrContext ------------------------------------------------------------------

ZHLParser::MemberPtrContext::MemberPtrContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::TypeContext* ZHLParser::MemberPtrContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

std::vector<tree::TerminalNode *> ZHLParser::MemberPtrContext::LeftParen() {
  return getTokens(ZHLParser::LeftParen);
}

tree::TerminalNode* ZHLParser::MemberPtrContext::LeftParen(size_t i) {
  return getToken(ZHLParser::LeftParen, i);
}

std::vector<tree::TerminalNode *> ZHLParser::MemberPtrContext::Name() {
  return getTokens(ZHLParser::Name);
}

tree::TerminalNode* ZHLParser::MemberPtrContext::Name(size_t i) {
  return getToken(ZHLParser::Name, i);
}

std::vector<tree::TerminalNode *> ZHLParser::MemberPtrContext::Colon() {
  return getTokens(ZHLParser::Colon);
}

tree::TerminalNode* ZHLParser::MemberPtrContext::Colon(size_t i) {
  return getToken(ZHLParser::Colon, i);
}

std::vector<tree::TerminalNode *> ZHLParser::MemberPtrContext::RightParen() {
  return getTokens(ZHLParser::RightParen);
}

tree::TerminalNode* ZHLParser::MemberPtrContext::RightParen(size_t i) {
  return getToken(ZHLParser::RightParen, i);
}

tree::TerminalNode* ZHLParser::MemberPtrContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}

tree::TerminalNode* ZHLParser::MemberPtrContext::CallingConvention() {
  return getToken(ZHLParser::CallingConvention, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::MemberPtrContext::Star() {
  return getTokens(ZHLParser::Star);
}

tree::TerminalNode* ZHLParser::MemberPtrContext::Star(size_t i) {
  return getToken(ZHLParser::Star, i);
}

ZHLParser::OptNamedFunArgsContext* ZHLParser::MemberPtrContext::optNamedFunArgs() {
  return getRuleContext<ZHLParser::OptNamedFunArgsContext>(0);
}


size_t ZHLParser::MemberPtrContext::getRuleIndex() const {
  return ZHLParser::RuleMemberPtr;
}

void ZHLParser::MemberPtrContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMemberPtr(this);
}

void ZHLParser::MemberPtrContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMemberPtr(this);
}


std::any ZHLParser::MemberPtrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitMemberPtr(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::MemberPtrContext* ZHLParser::memberPtr() {
  MemberPtrContext *_localctx = _tracker.createInstance<MemberPtrContext>(_ctx, getState());
  enterRule(_localctx, 80, ZHLParser::RuleMemberPtr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(436);
    type();
    setState(437);
    match(ZHLParser::LeftParen);
    setState(439);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::CallingConvention) {
      setState(438);
      match(ZHLParser::CallingConvention);
    }
    setState(441);
    match(ZHLParser::Name);
    setState(442);
    match(ZHLParser::Colon);
    setState(443);
    match(ZHLParser::Colon);
    setState(449);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Name) {
      setState(444);
      match(ZHLParser::Name);
      setState(445);
      match(ZHLParser::Colon);
      setState(446);
      match(ZHLParser::Colon);
      setState(451);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(453); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(452);
      match(ZHLParser::Star);
      setState(455); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == ZHLParser::Star);
    setState(457);
    match(ZHLParser::Name);
    setState(458);
    match(ZHLParser::RightParen);
    setState(459);
    match(ZHLParser::LeftParen);
    setState(461);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 25) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 25)) & 4503599627374589) != 0)) {
      setState(460);
      optNamedFunArgs();
    }
    setState(463);
    match(ZHLParser::RightParen);
    setState(464);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void ZHLParser::initialize() {
  ::antlr4::internal::call_once(zhlparserParserOnceFlag, zhlparserParserInitialize);
}
