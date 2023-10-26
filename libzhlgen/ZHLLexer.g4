lexer grammar ZHLLexer;

DoubleQuote: '"';
Colon: ':';
Semi: ';';

Qualifier: 
    Cleanup | Static | Virtual | Declspec | Debug;

Cleanup: 'cleanup';
Static: 'static';
Virtual: 'virtual';
Declspec: '__declspec';
External: 'external';
LeftParen: '(';
RightParen: ')';
Comma: ',';
Star: '*';
Lt: '<';
Gt: '>';
LeftRBracket: '[';
RightRBracket: ']';
LeftBracket: '{';
RightBracket: '}';
Class: 'class';
Struct: 'struct';
Reference: 'reference';
Depends: 'depends';
Typedef: 'typedef';
Const: 'const';
CppRef: '&';
Unsigned: 'unsigned';
Signed: 'signed';
Long: 'long';
Int: 'int';
Short: 'short';
Char: 'char';
Bool: 'bool';
Float: 'float';
Double: 'double';
Void: 'void';
Type: 'TypeInfo';
Size: 'Size';
Synonym: 'Synonym';
Align: 'Align';
Vtable: '__vtable';
Skip: 'skip';
Pure: 'pure';
Override: 'override';
Debug: 'debug';

Visibility: Public | Private | Protected;
Public: 'public';
Private: 'private';
Protected: 'protected';

Register: GeneralPurposeRegister | SSERegister;

GeneralPurposeRegister: Eax | Ebx | Ecx | Edx | Esi | Edi | Esp | Ebp;
Eax: 'eax';
Ebx: 'ebx';
Ecx: 'ecx';
Edx: 'edx';
Esi: 'esi';
Edi: 'edi';
Esp: 'esp';
Ebp: 'ebp';

SSERegister: Xmm0 | Xmm1 | Xmm2 | Xmm3 | Xmm4 | Xmm5 | Xmm6 | Xmm7;
Xmm0: 'xmm0';
Xmm1: 'xmm1';
Xmm2: 'xmm2';
Xmm3: 'xmm3';
Xmm4: 'xmm4';
Xmm5: 'xmm5';
Xmm6: 'xmm6';
Xmm7: 'xmm7';

CallingConvention: Stdcall | Cdecl | Fastcall | Thiscall | X8664 | X8664_Output;
Stdcall: '__stdcall';
Cdecl: '__cdecl';
Fastcall: '__fastcall';
Thiscall: '__thiscall';
X8664: '__x86_64';
X8664_Output: '__x86_64_output';

Signature: 
    DoubleQuote '.'? [0-9A-Fa-f?]+ DoubleQuote Colon;
    
ReferenceSignature:
    DoubleQuote '.'? [0-9A-Fa-f()?]+ DoubleQuote Colon;

Operator: 'operator';

OpSymbol: ('+' | '-' | '*' | '/') '='? | 
    '==' | '=';

Name: [a-zA-Z_] [a-zA-Z0-9_]*;

Number:
    HexNumber | DecNumber;

HexNumber:
    '0x' [0-9a-fA-F]+;
    
DecNumber:
    [0-9]+;

GenericCode: '{{' .*? '}}';

Whitespace: [ \t]+ -> skip;
Newline: ('\r' '\n'? | '\n') -> skip;
BlockComment: '/*' .*? '*/' -> skip;
LineComment: '//' ~ [\r\n]* -> skip;

Any: .;
