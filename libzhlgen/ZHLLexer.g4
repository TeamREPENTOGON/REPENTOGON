lexer grammar ZHLLexer;

DoubleQuote: '"';
Colon: ':';
Semi: ';';

Cleanup: 'cleanup';
Static: 'static';
Virtual: 'virtual';
Declspec: '__declspec';
LeftParen: '(';
RightParen: ')';
Comma: ',';
Ptr: '*';
Lt: '<';
Rt: '>';
LeftRBracket: '[';
RightRBracket: ']';
LeftBracket: '{';
RightBracket: '}';
Class: 'class';
Struct: 'struct';
Reference: 'reference';
Depends: 'depends';

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

Qualifier: 
    Cleanup | Static | Virtual | Declspec;

Signature: 
    DoubleQuote '.'? [0-9a-f]+ DoubleQuote Colon;
    
ReferenceSignature:
    DoubleQuote '.'? [0-9a-f()]+ DoubleQuote Colon;

Name: [a-zA-Z0-9_]+;

GenericCode: '<<<' .* '>>>';

fragment HexNumber:
    '0x' [0-9a-fA-F]+;
    
fragment DecNumber:
    [0-9]+;

Number:
    HexNumber | DecNumber;

Whitespace: [ \t]+ -> skip;
Newline: ('\r' '\n'? | '\n') -> skip;
BlockComment: '/*' .*? '*/' -> skip;
LineComment: '//' ~ [\r\n]* -> skip;

Any: .;