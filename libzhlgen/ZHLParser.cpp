
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
      "zhl", "externalFunc", "typeInfo", "typeInfoDef", "asmDef", "function", 
      "reference", "funArgs", "funArg", "argParam", "optNamedFunArgs", "optNamedFunArg", 
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
      "'skip'", "'pure'", "'override'", "'debug'", "'nohook'", "'asm'", 
      "", "'public'", "'private'", "'protected'", "", "", "'eax'", "'ebx'", 
      "'ecx'", "'edx'", "'esi'", "'edi'", "'esp'", "'ebp'", "", "'xmm0'", 
      "'xmm1'", "'xmm2'", "'xmm3'", "'xmm4'", "'xmm5'", "'xmm6'", "'xmm7'", 
      "", "'__stdcall'", "'__cdecl'", "'__fastcall'", "'__thiscall'", "'__x86_64'", 
      "'__x86_64_output'", "", "", "", "'operator'"
    },
    std::vector<std::string>{
      "", "DoubleQuote", "Colon", "Semi", "Qualifier", "Cleanup", "Static", 
      "Virtual", "Declspec", "External", "LeftParen", "RightParen", "Comma", 
      "Star", "Lt", "Gt", "LeftRBracket", "RightRBracket", "LeftBracket", 
      "RightBracket", "Class", "Struct", "Reference", "Depends", "Typedef", 
      "Const", "CppRef", "Unsigned", "Signed", "Long", "Int", "Short", "Char", 
      "Bool", "Float", "Double", "Void", "Type", "Size", "Synonym", "Align", 
      "Vtable", "Skip", "Pure", "Override", "Debug", "NoHook", "Asm", "Visibility", 
      "Public", "Private", "Protected", "Register", "GeneralPurposeRegister", 
      "Eax", "Ebx", "Ecx", "Edx", "Esi", "Edi", "Esp", "Ebp", "SSERegister", 
      "Xmm0", "Xmm1", "Xmm2", "Xmm3", "Xmm4", "Xmm5", "Xmm6", "Xmm7", "CallingConvention", 
      "Stdcall", "Cdecl", "Fastcall", "Thiscall", "X8664", "X8664_Output", 
      "ColonLessSignature", "Signature", "ReferenceSignature", "Operator", 
      "OpSymbol", "Name", "Number", "HexNumber", "DecNumber", "GenericCode", 
      "Whitespace", "Newline", "BlockComment", "LineComment", "Any"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,92,479,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,1,0,1,
  	0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,5,0,94,8,0,10,0,12,0,97,9,0,1,0,1,0,1,1,
  	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,2,1,2,4,2,113,8,2,11,2,12,2,114,
  	1,2,1,2,1,2,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,3,3,130,8,3,1,4,1,
  	4,1,4,1,4,1,4,1,5,5,5,138,8,5,10,5,12,5,141,9,5,1,5,3,5,144,8,5,1,5,1,
  	5,1,5,1,5,3,5,150,8,5,1,5,1,5,1,5,1,6,1,6,1,6,1,6,1,6,1,7,1,7,1,7,5,7,
  	163,8,7,10,7,12,7,166,9,7,1,8,1,8,1,8,3,8,171,8,8,1,9,1,9,1,9,1,9,1,10,
  	1,10,1,10,5,10,180,8,10,10,10,12,10,183,9,10,1,11,1,11,3,11,187,8,11,
  	1,12,1,12,1,12,1,12,5,12,193,8,12,10,12,12,12,196,9,12,1,12,1,12,1,13,
  	1,13,3,13,202,8,13,1,13,5,13,205,8,13,10,13,12,13,208,9,13,1,14,3,14,
  	211,8,14,1,14,1,14,3,14,215,8,14,1,14,3,14,218,8,14,1,15,1,15,3,15,222,
  	8,15,1,15,4,15,225,8,15,11,15,12,15,226,1,15,3,15,230,8,15,1,15,1,15,
  	1,15,1,15,3,15,236,8,15,1,15,5,15,239,8,15,10,15,12,15,242,9,15,1,15,
  	1,15,1,15,3,15,247,8,15,1,15,3,15,250,8,15,1,16,1,16,1,17,1,17,1,18,1,
  	18,1,18,1,18,5,18,260,8,18,10,18,12,18,263,9,18,1,19,1,19,3,19,267,8,
  	19,1,19,1,19,3,19,271,8,19,3,19,273,8,19,1,20,1,20,1,21,1,21,1,21,3,21,
  	280,8,21,1,21,1,21,3,21,284,8,21,1,21,1,21,1,21,1,21,1,21,3,21,291,8,
  	21,1,21,1,21,1,22,1,22,1,22,5,22,298,8,22,10,22,12,22,301,9,22,1,23,3,
  	23,304,8,23,1,23,1,23,1,24,1,24,1,24,1,24,5,24,312,8,24,10,24,12,24,315,
  	9,24,1,24,1,24,1,24,1,25,1,25,1,25,1,25,1,25,1,25,5,25,326,8,25,10,25,
  	12,25,329,9,25,1,26,1,26,1,26,4,26,334,8,26,11,26,12,26,335,1,26,1,26,
  	1,26,1,27,1,27,1,27,1,27,1,27,3,27,346,8,27,1,28,3,28,349,8,28,1,28,1,
  	28,1,29,1,29,1,29,1,30,5,30,357,8,30,10,30,12,30,360,9,30,1,30,1,30,1,
  	30,1,30,3,30,366,8,30,1,30,1,30,1,30,1,31,1,31,1,31,3,31,374,8,31,1,32,
  	1,32,1,32,1,32,5,32,380,8,32,10,32,12,32,383,9,32,1,32,1,32,1,33,5,33,
  	388,8,33,10,33,12,33,391,9,33,1,33,1,33,1,33,1,33,1,34,1,34,3,34,399,
  	8,34,1,35,1,35,1,35,1,35,3,35,405,8,35,1,36,1,36,1,36,1,36,3,36,411,8,
  	36,1,37,1,37,1,37,1,37,1,38,1,38,1,38,1,38,1,38,1,39,1,39,3,39,424,8,
  	39,1,40,1,40,1,40,3,40,429,8,40,1,40,4,40,432,8,40,11,40,12,40,433,1,
  	40,1,40,1,40,1,40,3,40,440,8,40,1,40,1,40,1,40,1,40,1,40,1,41,1,41,1,
  	41,3,41,450,8,41,1,41,1,41,1,41,1,41,1,41,1,41,5,41,458,8,41,10,41,12,
  	41,461,9,41,1,41,4,41,464,8,41,11,41,12,41,465,1,41,1,41,1,41,1,41,3,
  	41,472,8,41,1,41,1,41,1,41,1,41,1,41,1,41,0,0,42,0,2,4,6,8,10,12,14,16,
  	18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,
  	64,66,68,70,72,74,76,78,80,82,0,3,1,0,27,28,2,0,29,29,31,31,1,0,20,21,
  	510,0,95,1,0,0,0,2,100,1,0,0,0,4,108,1,0,0,0,6,129,1,0,0,0,8,131,1,0,
  	0,0,10,139,1,0,0,0,12,154,1,0,0,0,14,159,1,0,0,0,16,167,1,0,0,0,18,172,
  	1,0,0,0,20,176,1,0,0,0,22,184,1,0,0,0,24,188,1,0,0,0,26,199,1,0,0,0,28,
  	210,1,0,0,0,30,249,1,0,0,0,32,251,1,0,0,0,34,253,1,0,0,0,36,255,1,0,0,
  	0,38,272,1,0,0,0,40,274,1,0,0,0,42,276,1,0,0,0,44,294,1,0,0,0,46,303,
  	1,0,0,0,48,307,1,0,0,0,50,327,1,0,0,0,52,330,1,0,0,0,54,345,1,0,0,0,56,
  	348,1,0,0,0,58,352,1,0,0,0,60,358,1,0,0,0,62,373,1,0,0,0,64,375,1,0,0,
  	0,66,389,1,0,0,0,68,396,1,0,0,0,70,400,1,0,0,0,72,410,1,0,0,0,74,412,
  	1,0,0,0,76,416,1,0,0,0,78,423,1,0,0,0,80,425,1,0,0,0,82,446,1,0,0,0,84,
  	94,3,72,36,0,85,94,3,42,21,0,86,94,3,40,20,0,87,94,3,76,38,0,88,94,3,
  	78,39,0,89,94,3,74,37,0,90,94,3,4,2,0,91,94,3,2,1,0,92,94,3,8,4,0,93,
  	84,1,0,0,0,93,85,1,0,0,0,93,86,1,0,0,0,93,87,1,0,0,0,93,88,1,0,0,0,93,
  	89,1,0,0,0,93,90,1,0,0,0,93,91,1,0,0,0,93,92,1,0,0,0,94,97,1,0,0,0,95,
  	93,1,0,0,0,95,96,1,0,0,0,96,98,1,0,0,0,97,95,1,0,0,0,98,99,5,0,0,1,99,
  	1,1,0,0,0,100,101,5,9,0,0,101,102,5,10,0,0,102,103,5,83,0,0,103,104,5,
  	12,0,0,104,105,5,83,0,0,105,106,5,11,0,0,106,107,3,10,5,0,107,3,1,0,0,
  	0,108,109,5,37,0,0,109,110,5,83,0,0,110,112,5,18,0,0,111,113,3,6,3,0,
  	112,111,1,0,0,0,113,114,1,0,0,0,114,112,1,0,0,0,114,115,1,0,0,0,115,116,
  	1,0,0,0,116,117,5,19,0,0,117,118,5,3,0,0,118,5,1,0,0,0,119,120,5,38,0,
  	0,120,121,5,84,0,0,121,130,5,3,0,0,122,123,5,39,0,0,123,124,3,26,13,0,
  	124,125,5,3,0,0,125,130,1,0,0,0,126,127,5,40,0,0,127,128,5,84,0,0,128,
  	130,5,3,0,0,129,119,1,0,0,0,129,122,1,0,0,0,129,126,1,0,0,0,130,7,1,0,
  	0,0,131,132,5,47,0,0,132,133,5,83,0,0,133,134,5,78,0,0,134,135,5,3,0,
  	0,135,9,1,0,0,0,136,138,5,4,0,0,137,136,1,0,0,0,138,141,1,0,0,0,139,137,
  	1,0,0,0,139,140,1,0,0,0,140,143,1,0,0,0,141,139,1,0,0,0,142,144,5,71,
  	0,0,143,142,1,0,0,0,143,144,1,0,0,0,144,145,1,0,0,0,145,146,3,26,13,0,
  	146,147,3,36,18,0,147,149,5,10,0,0,148,150,3,14,7,0,149,148,1,0,0,0,149,
  	150,1,0,0,0,150,151,1,0,0,0,151,152,5,11,0,0,152,153,5,3,0,0,153,11,1,
  	0,0,0,154,155,5,22,0,0,155,156,3,26,13,0,156,157,5,83,0,0,157,158,5,3,
  	0,0,158,13,1,0,0,0,159,164,3,16,8,0,160,161,5,12,0,0,161,163,3,16,8,0,
  	162,160,1,0,0,0,163,166,1,0,0,0,164,162,1,0,0,0,164,165,1,0,0,0,165,15,
  	1,0,0,0,166,164,1,0,0,0,167,168,3,26,13,0,168,170,3,70,35,0,169,171,3,
  	18,9,0,170,169,1,0,0,0,170,171,1,0,0,0,171,17,1,0,0,0,172,173,5,14,0,
  	0,173,174,5,52,0,0,174,175,5,15,0,0,175,19,1,0,0,0,176,181,3,22,11,0,
  	177,178,5,12,0,0,178,180,3,22,11,0,179,177,1,0,0,0,180,183,1,0,0,0,181,
  	179,1,0,0,0,181,182,1,0,0,0,182,21,1,0,0,0,183,181,1,0,0,0,184,186,3,
  	26,13,0,185,187,3,70,35,0,186,185,1,0,0,0,186,187,1,0,0,0,187,23,1,0,
  	0,0,188,189,5,14,0,0,189,194,3,26,13,0,190,191,5,12,0,0,191,193,3,26,
  	13,0,192,190,1,0,0,0,193,196,1,0,0,0,194,192,1,0,0,0,194,195,1,0,0,0,
  	195,197,1,0,0,0,196,194,1,0,0,0,197,198,5,15,0,0,198,25,1,0,0,0,199,201,
  	3,28,14,0,200,202,3,24,12,0,201,200,1,0,0,0,201,202,1,0,0,0,202,206,1,
  	0,0,0,203,205,3,38,19,0,204,203,1,0,0,0,205,208,1,0,0,0,206,204,1,0,0,
  	0,206,207,1,0,0,0,207,27,1,0,0,0,208,206,1,0,0,0,209,211,5,25,0,0,210,
  	209,1,0,0,0,210,211,1,0,0,0,211,214,1,0,0,0,212,215,3,30,15,0,213,215,
  	3,36,18,0,214,212,1,0,0,0,214,213,1,0,0,0,215,217,1,0,0,0,216,218,5,25,
  	0,0,217,216,1,0,0,0,217,218,1,0,0,0,218,29,1,0,0,0,219,250,3,32,16,0,
  	220,222,3,32,16,0,221,220,1,0,0,0,221,222,1,0,0,0,222,224,1,0,0,0,223,
  	225,3,34,17,0,224,223,1,0,0,0,225,226,1,0,0,0,226,224,1,0,0,0,226,227,
  	1,0,0,0,227,250,1,0,0,0,228,230,3,32,16,0,229,228,1,0,0,0,229,230,1,0,
  	0,0,230,231,1,0,0,0,231,250,5,32,0,0,232,250,5,33,0,0,233,250,5,36,0,
  	0,234,236,3,32,16,0,235,234,1,0,0,0,235,236,1,0,0,0,236,240,1,0,0,0,237,
  	239,3,34,17,0,238,237,1,0,0,0,239,242,1,0,0,0,240,238,1,0,0,0,240,241,
  	1,0,0,0,241,243,1,0,0,0,242,240,1,0,0,0,243,250,5,30,0,0,244,250,5,34,
  	0,0,245,247,3,34,17,0,246,245,1,0,0,0,246,247,1,0,0,0,247,248,1,0,0,0,
  	248,250,5,35,0,0,249,219,1,0,0,0,249,221,1,0,0,0,249,229,1,0,0,0,249,
  	232,1,0,0,0,249,233,1,0,0,0,249,235,1,0,0,0,249,244,1,0,0,0,249,246,1,
  	0,0,0,250,31,1,0,0,0,251,252,7,0,0,0,252,33,1,0,0,0,253,254,7,1,0,0,254,
  	35,1,0,0,0,255,261,5,83,0,0,256,257,5,2,0,0,257,258,5,2,0,0,258,260,3,
  	62,31,0,259,256,1,0,0,0,260,263,1,0,0,0,261,259,1,0,0,0,261,262,1,0,0,
  	0,262,37,1,0,0,0,263,261,1,0,0,0,264,266,5,26,0,0,265,267,5,26,0,0,266,
  	265,1,0,0,0,266,267,1,0,0,0,267,273,1,0,0,0,268,270,5,13,0,0,269,271,
  	5,25,0,0,270,269,1,0,0,0,270,271,1,0,0,0,271,273,1,0,0,0,272,264,1,0,
  	0,0,272,268,1,0,0,0,273,39,1,0,0,0,274,275,5,87,0,0,275,41,1,0,0,0,276,
  	277,7,2,0,0,277,279,5,83,0,0,278,280,3,48,24,0,279,278,1,0,0,0,279,280,
  	1,0,0,0,280,283,1,0,0,0,281,282,5,2,0,0,282,284,3,44,22,0,283,281,1,0,
  	0,0,283,284,1,0,0,0,284,285,1,0,0,0,285,286,5,18,0,0,286,287,3,50,25,
  	0,287,290,5,19,0,0,288,289,5,2,0,0,289,291,5,84,0,0,290,288,1,0,0,0,290,
  	291,1,0,0,0,291,292,1,0,0,0,292,293,5,3,0,0,293,43,1,0,0,0,294,299,3,
  	46,23,0,295,296,5,12,0,0,296,298,3,46,23,0,297,295,1,0,0,0,298,301,1,
  	0,0,0,299,297,1,0,0,0,299,300,1,0,0,0,300,45,1,0,0,0,301,299,1,0,0,0,
  	302,304,5,48,0,0,303,302,1,0,0,0,303,304,1,0,0,0,304,305,1,0,0,0,305,
  	306,3,36,18,0,306,47,1,0,0,0,307,308,5,23,0,0,308,313,5,10,0,0,309,310,
  	5,83,0,0,310,312,5,12,0,0,311,309,1,0,0,0,312,315,1,0,0,0,313,311,1,0,
  	0,0,313,314,1,0,0,0,314,316,1,0,0,0,315,313,1,0,0,0,316,317,5,83,0,0,
  	317,318,5,11,0,0,318,49,1,0,0,0,319,326,3,52,26,0,320,326,3,58,29,0,321,
  	326,3,40,20,0,322,326,3,64,32,0,323,326,3,78,39,0,324,326,3,74,37,0,325,
  	319,1,0,0,0,325,320,1,0,0,0,325,321,1,0,0,0,325,322,1,0,0,0,325,323,1,
  	0,0,0,325,324,1,0,0,0,326,329,1,0,0,0,327,325,1,0,0,0,327,328,1,0,0,0,
  	328,51,1,0,0,0,329,327,1,0,0,0,330,331,5,41,0,0,331,333,5,18,0,0,332,
  	334,3,54,27,0,333,332,1,0,0,0,334,335,1,0,0,0,335,333,1,0,0,0,335,336,
  	1,0,0,0,336,337,1,0,0,0,337,338,5,19,0,0,338,339,5,3,0,0,339,53,1,0,0,
  	0,340,346,3,56,28,0,341,342,5,42,0,0,342,346,5,3,0,0,343,344,5,43,0,0,
  	344,346,3,60,30,0,345,340,1,0,0,0,345,341,1,0,0,0,345,343,1,0,0,0,346,
  	55,1,0,0,0,347,349,5,44,0,0,348,347,1,0,0,0,348,349,1,0,0,0,349,350,1,
  	0,0,0,350,351,3,58,29,0,351,57,1,0,0,0,352,353,5,79,0,0,353,354,3,60,
  	30,0,354,59,1,0,0,0,355,357,5,4,0,0,356,355,1,0,0,0,357,360,1,0,0,0,358,
  	356,1,0,0,0,358,359,1,0,0,0,359,361,1,0,0,0,360,358,1,0,0,0,361,362,3,
  	26,13,0,362,363,3,62,31,0,363,365,5,10,0,0,364,366,3,14,7,0,365,364,1,
  	0,0,0,365,366,1,0,0,0,366,367,1,0,0,0,367,368,5,11,0,0,368,369,5,3,0,
  	0,369,61,1,0,0,0,370,374,5,83,0,0,371,372,5,81,0,0,372,374,5,82,0,0,373,
  	370,1,0,0,0,373,371,1,0,0,0,374,63,1,0,0,0,375,376,3,26,13,0,376,381,
  	3,66,33,0,377,378,5,12,0,0,378,380,3,66,33,0,379,377,1,0,0,0,380,383,
  	1,0,0,0,381,379,1,0,0,0,381,382,1,0,0,0,382,384,1,0,0,0,383,381,1,0,0,
  	0,384,385,5,3,0,0,385,65,1,0,0,0,386,388,3,68,34,0,387,386,1,0,0,0,388,
  	391,1,0,0,0,389,387,1,0,0,0,389,390,1,0,0,0,390,392,1,0,0,0,391,389,1,
  	0,0,0,392,393,3,70,35,0,393,394,5,2,0,0,394,395,5,84,0,0,395,67,1,0,0,
  	0,396,398,5,13,0,0,397,399,5,25,0,0,398,397,1,0,0,0,398,399,1,0,0,0,399,
  	69,1,0,0,0,400,404,5,83,0,0,401,402,5,16,0,0,402,403,5,84,0,0,403,405,
  	5,17,0,0,404,401,1,0,0,0,404,405,1,0,0,0,405,71,1,0,0,0,406,407,5,79,
  	0,0,407,411,3,10,5,0,408,409,5,80,0,0,409,411,3,12,6,0,410,406,1,0,0,
  	0,410,408,1,0,0,0,411,73,1,0,0,0,412,413,7,2,0,0,413,414,5,83,0,0,414,
  	415,5,3,0,0,415,75,1,0,0,0,416,417,5,24,0,0,417,418,3,26,13,0,418,419,
  	5,83,0,0,419,420,5,3,0,0,420,77,1,0,0,0,421,424,3,80,40,0,422,424,3,82,
  	41,0,423,421,1,0,0,0,423,422,1,0,0,0,424,79,1,0,0,0,425,426,3,26,13,0,
  	426,428,5,10,0,0,427,429,5,71,0,0,428,427,1,0,0,0,428,429,1,0,0,0,429,
  	431,1,0,0,0,430,432,5,13,0,0,431,430,1,0,0,0,432,433,1,0,0,0,433,431,
  	1,0,0,0,433,434,1,0,0,0,434,435,1,0,0,0,435,436,5,83,0,0,436,437,5,11,
  	0,0,437,439,5,10,0,0,438,440,3,20,10,0,439,438,1,0,0,0,439,440,1,0,0,
  	0,440,441,1,0,0,0,441,442,5,11,0,0,442,443,5,2,0,0,443,444,5,84,0,0,444,
  	445,5,3,0,0,445,81,1,0,0,0,446,447,3,26,13,0,447,449,5,10,0,0,448,450,
  	5,71,0,0,449,448,1,0,0,0,449,450,1,0,0,0,450,451,1,0,0,0,451,452,5,83,
  	0,0,452,453,5,2,0,0,453,459,5,2,0,0,454,455,5,83,0,0,455,456,5,2,0,0,
  	456,458,5,2,0,0,457,454,1,0,0,0,458,461,1,0,0,0,459,457,1,0,0,0,459,460,
  	1,0,0,0,460,463,1,0,0,0,461,459,1,0,0,0,462,464,5,13,0,0,463,462,1,0,
  	0,0,464,465,1,0,0,0,465,463,1,0,0,0,465,466,1,0,0,0,466,467,1,0,0,0,467,
  	468,5,83,0,0,468,469,5,11,0,0,469,471,5,10,0,0,470,472,3,20,10,0,471,
  	470,1,0,0,0,471,472,1,0,0,0,472,473,1,0,0,0,473,474,5,11,0,0,474,475,
  	5,2,0,0,475,476,5,84,0,0,476,477,5,3,0,0,477,83,1,0,0,0,55,93,95,114,
  	129,139,143,149,164,170,181,186,194,201,206,210,214,217,221,226,229,235,
  	240,246,249,261,266,270,272,279,283,290,299,303,313,325,327,335,345,348,
  	358,365,373,381,389,398,404,410,423,428,433,439,449,459,465,471
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

std::vector<ZHLParser::AsmDefContext *> ZHLParser::ZhlContext::asmDef() {
  return getRuleContexts<ZHLParser::AsmDefContext>();
}

ZHLParser::AsmDefContext* ZHLParser::ZhlContext::asmDef(size_t i) {
  return getRuleContext<ZHLParser::AsmDefContext>(i);
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
    setState(95);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 141012285522432) != 0) || ((((_la - 79) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 79)) & 275) != 0)) {
      setState(93);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
      case 1: {
        setState(84);
        signature();
        break;
      }

      case 2: {
        setState(85);
        class_();
        break;
      }

      case 3: {
        setState(86);
        genericCode();
        break;
      }

      case 4: {
        setState(87);
        typedef_();
        break;
      }

      case 5: {
        setState(88);
        functionPtr();
        break;
      }

      case 6: {
        setState(89);
        forwardDecl();
        break;
      }

      case 7: {
        setState(90);
        typeInfo();
        break;
      }

      case 8: {
        setState(91);
        externalFunc();
        break;
      }

      case 9: {
        setState(92);
        asmDef();
        break;
      }

      default:
        break;
      }
      setState(97);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(98);
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
    setState(100);
    match(ZHLParser::External);
    setState(101);
    match(ZHLParser::LeftParen);
    setState(102);
    match(ZHLParser::Name);
    setState(103);
    match(ZHLParser::Comma);
    setState(104);
    match(ZHLParser::Name);
    setState(105);
    match(ZHLParser::RightParen);
    setState(106);
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
    setState(108);
    match(ZHLParser::Type);
    setState(109);
    match(ZHLParser::Name);
    setState(110);
    match(ZHLParser::LeftBracket);
    setState(112); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(111);
      typeInfoDef();
      setState(114); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 1924145348608) != 0));
    setState(116);
    match(ZHLParser::RightBracket);
    setState(117);
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
    setState(129);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ZHLParser::Size: {
        enterOuterAlt(_localctx, 1);
        setState(119);
        match(ZHLParser::Size);
        setState(120);
        match(ZHLParser::Number);
        setState(121);
        match(ZHLParser::Semi);
        break;
      }

      case ZHLParser::Synonym: {
        enterOuterAlt(_localctx, 2);
        setState(122);
        match(ZHLParser::Synonym);
        setState(123);
        type();
        setState(124);
        match(ZHLParser::Semi);
        break;
      }

      case ZHLParser::Align: {
        enterOuterAlt(_localctx, 3);
        setState(126);
        match(ZHLParser::Align);
        setState(127);
        match(ZHLParser::Number);
        setState(128);
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

//----------------- AsmDefContext ------------------------------------------------------------------

ZHLParser::AsmDefContext::AsmDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::AsmDefContext::Asm() {
  return getToken(ZHLParser::Asm, 0);
}

tree::TerminalNode* ZHLParser::AsmDefContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

tree::TerminalNode* ZHLParser::AsmDefContext::ColonLessSignature() {
  return getToken(ZHLParser::ColonLessSignature, 0);
}

tree::TerminalNode* ZHLParser::AsmDefContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}


size_t ZHLParser::AsmDefContext::getRuleIndex() const {
  return ZHLParser::RuleAsmDef;
}

void ZHLParser::AsmDefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAsmDef(this);
}

void ZHLParser::AsmDefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAsmDef(this);
}


std::any ZHLParser::AsmDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitAsmDef(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::AsmDefContext* ZHLParser::asmDef() {
  AsmDefContext *_localctx = _tracker.createInstance<AsmDefContext>(_ctx, getState());
  enterRule(_localctx, 8, ZHLParser::RuleAsmDef);

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
    match(ZHLParser::Asm);
    setState(132);
    match(ZHLParser::Name);
    setState(133);
    match(ZHLParser::ColonLessSignature);
    setState(134);
    match(ZHLParser::Semi);
   
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
  enterRule(_localctx, 10, ZHLParser::RuleFunction);
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
    setState(139);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Qualifier) {
      setState(136);
      match(ZHLParser::Qualifier);
      setState(141);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(143);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::CallingConvention) {
      setState(142);
      match(ZHLParser::CallingConvention);
    }
    setState(145);
    type();
    setState(146);
    nestedName();
    setState(147);
    match(ZHLParser::LeftParen);
    setState(149);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 25) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 25)) & 288230376151715837) != 0)) {
      setState(148);
      funArgs();
    }
    setState(151);
    match(ZHLParser::RightParen);
    setState(152);
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
  enterRule(_localctx, 12, ZHLParser::RuleReference);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(154);
    match(ZHLParser::Reference);
    setState(155);
    type();
    setState(156);
    match(ZHLParser::Name);
    setState(157);
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
  enterRule(_localctx, 14, ZHLParser::RuleFunArgs);
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
    funArg();
    setState(164);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(160);
      match(ZHLParser::Comma);
      setState(161);
      funArg();
      setState(166);
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
  enterRule(_localctx, 16, ZHLParser::RuleFunArg);
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
    setState(167);
    type();
    setState(168);
    fullName();
    setState(170);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Lt) {
      setState(169);
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
  enterRule(_localctx, 18, ZHLParser::RuleArgParam);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(172);
    match(ZHLParser::Lt);
    setState(173);
    match(ZHLParser::Register);
    setState(174);
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
  enterRule(_localctx, 20, ZHLParser::RuleOptNamedFunArgs);
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
    optNamedFunArg();
    setState(181);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(177);
      match(ZHLParser::Comma);
      setState(178);
      optNamedFunArg();
      setState(183);
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
  enterRule(_localctx, 22, ZHLParser::RuleOptNamedFunArg);
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
    setState(184);
    type();
    setState(186);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Name) {
      setState(185);
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
  enterRule(_localctx, 24, ZHLParser::RuleTemplateSpec);
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
    setState(188);
    match(ZHLParser::Lt);
    setState(189);
    type();
    setState(194);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(190);
      match(ZHLParser::Comma);
      setState(191);
      type();
      setState(196);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(197);
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
  enterRule(_localctx, 26, ZHLParser::RuleType);
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
    setState(199);
    typeSpecifier();
    setState(201);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Lt) {
      setState(200);
      templateSpec();
    }
    setState(206);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(203);
        pointerAttribute(); 
      }
      setState(208);
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
  enterRule(_localctx, 28, ZHLParser::RuleTypeSpecifier);
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
    setState(210);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Const) {
      setState(209);
      match(ZHLParser::Const);
    }
    setState(214);
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
        setState(212);
        simpleType();
        break;
      }

      case ZHLParser::Name: {
        setState(213);
        nestedName();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(217);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Const) {
      setState(216);
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
  enterRule(_localctx, 30, ZHLParser::RuleSimpleType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(249);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(219);
      simpleTypeSignedness();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(221);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ZHLParser::Unsigned

      || _la == ZHLParser::Signed) {
        setState(220);
        simpleTypeSignedness();
      }
      setState(224); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(223);
        simpleTypeLength();
        setState(226); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == ZHLParser::Long

      || _la == ZHLParser::Short);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(229);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ZHLParser::Unsigned

      || _la == ZHLParser::Signed) {
        setState(228);
        simpleTypeSignedness();
      }
      setState(231);
      match(ZHLParser::Char);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(232);
      match(ZHLParser::Bool);
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(233);
      match(ZHLParser::Void);
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(235);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ZHLParser::Unsigned

      || _la == ZHLParser::Signed) {
        setState(234);
        simpleTypeSignedness();
      }
      setState(240);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == ZHLParser::Long

      || _la == ZHLParser::Short) {
        setState(237);
        simpleTypeLength();
        setState(242);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(243);
      match(ZHLParser::Int);
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(244);
      match(ZHLParser::Float);
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(246);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ZHLParser::Long

      || _la == ZHLParser::Short) {
        setState(245);
        simpleTypeLength();
      }
      setState(248);
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
  enterRule(_localctx, 32, ZHLParser::RuleSimpleTypeSignedness);
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
    setState(251);
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
  enterRule(_localctx, 34, ZHLParser::RuleSimpleTypeLength);
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
    setState(253);
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
  enterRule(_localctx, 36, ZHLParser::RuleNestedName);
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
    setState(255);
    match(ZHLParser::Name);
    setState(261);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Colon) {
      setState(256);
      match(ZHLParser::Colon);
      setState(257);
      match(ZHLParser::Colon);
      setState(258);
      functionName();
      setState(263);
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
  enterRule(_localctx, 38, ZHLParser::RulePointerAttribute);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(272);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ZHLParser::CppRef: {
        enterOuterAlt(_localctx, 1);
        setState(264);
        match(ZHLParser::CppRef);
        setState(266);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx)) {
        case 1: {
          setState(265);
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
        setState(268);
        match(ZHLParser::Star);
        setState(270);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ZHLParser::Const) {
          setState(269);
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
  enterRule(_localctx, 40, ZHLParser::RuleGenericCode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(274);
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
  enterRule(_localctx, 42, ZHLParser::RuleClass);
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
    setState(276);
    _la = _input->LA(1);
    if (!(_la == ZHLParser::Class

    || _la == ZHLParser::Struct)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(277);
    match(ZHLParser::Name);
    setState(279);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Depends) {
      setState(278);
      depends();
    }
    setState(283);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Colon) {
      setState(281);
      match(ZHLParser::Colon);
      setState(282);
      inheritance();
    }
    setState(285);
    match(ZHLParser::LeftBracket);
    setState(286);
    classBody();
    setState(287);
    match(ZHLParser::RightBracket);
    setState(290);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Colon) {
      setState(288);
      match(ZHLParser::Colon);
      setState(289);
      match(ZHLParser::Number);
    }
    setState(292);
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
  enterRule(_localctx, 44, ZHLParser::RuleInheritance);
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
    setState(294);
    inheritanceDecl();
    setState(299);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(295);
      match(ZHLParser::Comma);
      setState(296);
      inheritanceDecl();
      setState(301);
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
  enterRule(_localctx, 46, ZHLParser::RuleInheritanceDecl);
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
    setState(303);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Visibility) {
      setState(302);
      match(ZHLParser::Visibility);
    }
    setState(305);
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
  enterRule(_localctx, 48, ZHLParser::RuleDepends);

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
    setState(307);
    match(ZHLParser::Depends);
    setState(308);
    match(ZHLParser::LeftParen);
    setState(313);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(309);
        match(ZHLParser::Name);
        setState(310);
        match(ZHLParser::Comma); 
      }
      setState(315);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx);
    }
    setState(316);
    match(ZHLParser::Name);
    setState(317);
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
  enterRule(_localctx, 50, ZHLParser::RuleClassBody);
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
    setState(327);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 2336364691456) != 0) || ((((_la - 79) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 79)) & 273) != 0)) {
      setState(325);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx)) {
      case 1: {
        setState(319);
        vtable();
        break;
      }

      case 2: {
        setState(320);
        classSignature();
        break;
      }

      case 3: {
        setState(321);
        genericCode();
        break;
      }

      case 4: {
        setState(322);
        classField();
        break;
      }

      case 5: {
        setState(323);
        functionPtr();
        break;
      }

      case 6: {
        setState(324);
        forwardDecl();
        break;
      }

      default:
        break;
      }
      setState(329);
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
  enterRule(_localctx, 52, ZHLParser::RuleVtable);
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
    setState(330);
    match(ZHLParser::Vtable);
    setState(331);
    match(ZHLParser::LeftBracket);
    setState(333); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(332);
      vtableEntry();
      setState(335); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (((((_la - 42) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 42)) & 137438953479) != 0));
    setState(337);
    match(ZHLParser::RightBracket);
    setState(338);
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
  enterRule(_localctx, 54, ZHLParser::RuleVtableEntry);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(345);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ZHLParser::Override:
      case ZHLParser::Signature: {
        enterOuterAlt(_localctx, 1);
        setState(340);
        vtableSignature();
        break;
      }

      case ZHLParser::Skip: {
        enterOuterAlt(_localctx, 2);
        setState(341);
        match(ZHLParser::Skip);
        setState(342);
        match(ZHLParser::Semi);
        break;
      }

      case ZHLParser::Pure: {
        enterOuterAlt(_localctx, 3);
        setState(343);
        match(ZHLParser::Pure);
        setState(344);
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
  enterRule(_localctx, 56, ZHLParser::RuleVtableSignature);
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
    setState(348);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Override) {
      setState(347);
      match(ZHLParser::Override);
    }
    setState(350);
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
  enterRule(_localctx, 58, ZHLParser::RuleClassSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(352);
    match(ZHLParser::Signature);
    setState(353);
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
  enterRule(_localctx, 60, ZHLParser::RuleClassFunction);
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
    setState(358);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Qualifier) {
      setState(355);
      match(ZHLParser::Qualifier);
      setState(360);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(361);
    type();
    setState(362);
    functionName();
    setState(363);
    match(ZHLParser::LeftParen);
    setState(365);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 25) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 25)) & 288230376151715837) != 0)) {
      setState(364);
      funArgs();
    }
    setState(367);
    match(ZHLParser::RightParen);
    setState(368);
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
  enterRule(_localctx, 62, ZHLParser::RuleFunctionName);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(373);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ZHLParser::Name: {
        enterOuterAlt(_localctx, 1);
        setState(370);
        match(ZHLParser::Name);
        break;
      }

      case ZHLParser::Operator: {
        enterOuterAlt(_localctx, 2);
        setState(371);
        match(ZHLParser::Operator);
        setState(372);
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
  enterRule(_localctx, 64, ZHLParser::RuleClassField);
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
    setState(375);
    type();
    setState(376);
    innerField();
    setState(381);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(377);
      match(ZHLParser::Comma);
      setState(378);
      innerField();
      setState(383);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(384);
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
  enterRule(_localctx, 66, ZHLParser::RuleInnerField);
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
    setState(389);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Star) {
      setState(386);
      innerFieldPtr();
      setState(391);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(392);
    fullName();
    setState(393);
    match(ZHLParser::Colon);
    setState(394);
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
  enterRule(_localctx, 68, ZHLParser::RuleInnerFieldPtr);
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
    setState(396);
    match(ZHLParser::Star);
    setState(398);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Const) {
      setState(397);
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
  enterRule(_localctx, 70, ZHLParser::RuleFullName);
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
    setState(400);
    match(ZHLParser::Name);
    setState(404);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::LeftRBracket) {
      setState(401);
      match(ZHLParser::LeftRBracket);
      setState(402);
      match(ZHLParser::Number);
      setState(403);
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
  enterRule(_localctx, 72, ZHLParser::RuleSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(410);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ZHLParser::Signature: {
        setState(406);
        match(ZHLParser::Signature);
        setState(407);
        function();
        break;
      }

      case ZHLParser::ReferenceSignature: {
        setState(408);
        match(ZHLParser::ReferenceSignature);
        setState(409);
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
  enterRule(_localctx, 74, ZHLParser::RuleForwardDecl);
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
    setState(412);
    _la = _input->LA(1);
    if (!(_la == ZHLParser::Class

    || _la == ZHLParser::Struct)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(413);
    match(ZHLParser::Name);
    setState(414);
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
  enterRule(_localctx, 76, ZHLParser::RuleTypedef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(416);
    match(ZHLParser::Typedef);
    setState(417);
    type();
    setState(418);
    match(ZHLParser::Name);
    setState(419);
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
  enterRule(_localctx, 78, ZHLParser::RuleFunctionPtr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(423);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 47, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(421);
      fptr();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(422);
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

tree::TerminalNode* ZHLParser::FptrContext::Colon() {
  return getToken(ZHLParser::Colon, 0);
}

tree::TerminalNode* ZHLParser::FptrContext::Number() {
  return getToken(ZHLParser::Number, 0);
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
  enterRule(_localctx, 80, ZHLParser::RuleFptr);
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
    setState(425);
    type();
    setState(426);
    match(ZHLParser::LeftParen);
    setState(428);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::CallingConvention) {
      setState(427);
      match(ZHLParser::CallingConvention);
    }
    setState(431); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(430);
      match(ZHLParser::Star);
      setState(433); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == ZHLParser::Star);
    setState(435);
    match(ZHLParser::Name);
    setState(436);
    match(ZHLParser::RightParen);
    setState(437);
    match(ZHLParser::LeftParen);
    setState(439);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 25) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 25)) & 288230376151715837) != 0)) {
      setState(438);
      optNamedFunArgs();
    }
    setState(441);
    match(ZHLParser::RightParen);
    setState(442);
    match(ZHLParser::Colon);
    setState(443);
    match(ZHLParser::Number);
    setState(444);
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

tree::TerminalNode* ZHLParser::MemberPtrContext::Number() {
  return getToken(ZHLParser::Number, 0);
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
  enterRule(_localctx, 82, ZHLParser::RuleMemberPtr);
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
    setState(446);
    type();
    setState(447);
    match(ZHLParser::LeftParen);
    setState(449);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::CallingConvention) {
      setState(448);
      match(ZHLParser::CallingConvention);
    }
    setState(451);
    match(ZHLParser::Name);
    setState(452);
    match(ZHLParser::Colon);
    setState(453);
    match(ZHLParser::Colon);
    setState(459);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Name) {
      setState(454);
      match(ZHLParser::Name);
      setState(455);
      match(ZHLParser::Colon);
      setState(456);
      match(ZHLParser::Colon);
      setState(461);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(463); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(462);
      match(ZHLParser::Star);
      setState(465); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == ZHLParser::Star);
    setState(467);
    match(ZHLParser::Name);
    setState(468);
    match(ZHLParser::RightParen);
    setState(469);
    match(ZHLParser::LeftParen);
    setState(471);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 25) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 25)) & 288230376151715837) != 0)) {
      setState(470);
      optNamedFunArgs();
    }
    setState(473);
    match(ZHLParser::RightParen);
    setState(474);
    match(ZHLParser::Colon);
    setState(475);
    match(ZHLParser::Number);
    setState(476);
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
