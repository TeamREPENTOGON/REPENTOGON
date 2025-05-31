parser grammar ZHLParser;

options {
    tokenVocab = ZHLLexer;
}

zhl:
    (signature | class | genericCode | typedef | functionPtr | forwardDecl | typeInfo | externalFunc | asmDef)* EOF;

externalFunc:
    External LeftParen Name Comma Name RightParen function;

typeInfo:
    Type Name LeftBracket typeInfoDef+ RightBracket Semi;

typeInfoDef:
    Size Number Semi |
    Synonym type Semi |
    Align Number Semi;

asmDef:
    Asm Name ColonLessSignature Semi;

function:
    Qualifier* CallingConvention? type nestedName LeftParen funArgs? RightParen Semi;
    
reference:
    Reference type Name Semi;
    
funArgs:
    funArg (Comma funArg)*;
    
funArg:
    type fullName argParam?;
    
argParam:
    Lt Register Gt;
    
optNamedFunArgs: 
    optNamedFunArg (Comma optNamedFunArg)*;
    
optNamedFunArg:
    type fullName?;
    
templateSpec:
    Lt type (Comma type)* Gt;
    
type:
    typeSpecifier templateSpec? pointerAttribute*;
    
typeSpecifier:
    Const? (simpleType | nestedName) Const?;
    
simpleType:
    simpleTypeSignedness
    | simpleTypeSignedness? simpleTypeLength+
    | simpleTypeSignedness? Char
    | Bool
    | Void
    | simpleTypeSignedness? simpleTypeLength* Int
    | Float
    | simpleTypeLength? Double
    ;
    
simpleTypeSignedness:
    Signed | Unsigned;
    
simpleTypeLength:
    Long | Short;
    
nestedName:
    Name (Colon Colon functionName)*;
    
pointerAttribute:
    CppRef CppRef? |
    Star Const?;
    
genericCode: 
    GenericCode; // LeftBracket LeftBracket Any* RightBracket RightBracket;
    
class: 
    (Class | Struct) Name depends? (Colon inheritance)? LeftBracket classBody RightBracket (Colon Number)? Semi;
    
inheritance:
    inheritanceDecl (Comma inheritanceDecl)*;

inheritanceDecl:
    Visibility? nestedName;
    
depends:
    Depends LeftParen (Name Comma)* Name RightParen;

classBody: 
    (vtable | classSignature | genericCode | classField | functionPtr | forwardDecl)*;

vtable:
    Vtable LeftBracket vtableEntry+ RightBracket Semi;

vtableEntry:
    vtableSignature | Skip Semi | Pure classFunction;

vtableSignature:
    Override? classSignature;

classSignature:
    Signature classFunction;

classFunction:
    Qualifier* type functionName LeftParen funArgs? RightParen Semi;

functionName:
    Name | Operator OpSymbol;

classField:
    type innerField (Comma innerField)* Semi;
    
innerField:
    innerFieldPtr* fullName Colon Number;

innerFieldPtr:
    Star Const?;
    
fullName:
    Name (LeftRBracket Number RightRBracket)?;
    
signature:
    ((Signature function) | (ReferenceSignature reference));
    
forwardDecl:
    (Struct | Class) Name Semi;
    
typedef:
    Typedef type Name Semi;
    
functionPtr:
    fptr | memberPtr;
    
fptr:
    type LeftParen CallingConvention? Star+ Name RightParen LeftParen optNamedFunArgs? RightParen Colon Number Semi;
    
memberPtr:
    type LeftParen CallingConvention? Name Colon Colon (Name Colon Colon)* Star+ Name RightParen LeftParen optNamedFunArgs? RightParen Colon Number Semi;
