#pragma once

#include <optional>
#include <set>
#include <string>
#include <variant>
#include <vector>

enum PointerKind {
    LREF,
    RREF,
    POINTER
};

std::string PointerKindToString(PointerKind kind);

struct PointerDecl {
    bool _const;
    PointerKind _kind;

    std::string ToString() const;
    std::string GetPrefix() const;
};

enum Signedness {
    SIGNED,
    UNSIGNED
};

std::string SignednessToString(Signedness sign);

enum Length {
    LONG,
    LONGLONG,
    SHORT
};

std::string LengthToString(Length length);

enum BasicTypes {
    CHAR,
    INT,
    BOOL,
    VOID,
    FLOAT,
    DOUBLE
};

std::string BasicTypeToString(BasicTypes type);

struct BasicType {
    std::optional<Signedness> _sign;
    std::optional<Length> _length;
    BasicTypes _type;

    std::string ToString() const;
    std::string GetAbsoluteName() const;
    size_t size() const;
};

struct Function;
struct Variable;
struct Signature;
struct VariableSignature;
struct ExternalFunction;

typedef std::string GenericCode;

struct Namespace {
    // std::vector<Namespace> _children;
    // std::vector<Function> _functions;
    std::vector<GenericCode> _generic;
    std::vector<Variable> _fields;
    std::vector<Signature> _signatures;
    std::vector<VariableSignature> _references;
    std::vector<ExternalFunction> _externs;
};

enum Visibility {
    PUBLIC,
    PRIVATE,
    PROTECTED
};

std::string VisibilityToString(Visibility v);

struct Type;
struct VirtualFunction;
struct Skip { };

struct Struct {
    Namespace _namespace;
    std::string _name;
    std::set<Type*> _deps;
    std::vector<std::tuple<Type*, Visibility>> _parents;
    std::vector<std::variant<Signature, Skip, Function /* Pure virtual */>> _virtualFunctions;
    std::vector<Signature> _overridenVirtualFunctions;

    std::string ToString(bool full) const;
    size_t size() const;
    std::string GetTemplateName() const;
    uint32_t GetVirtualFunctionSlot(Signature const& sig, bool checkParent) const;
    uint32_t GetVirtualFunctionSlot(Function const& fn) const;
    uint32_t GetVirtualFunctionSlotInternal(Signature const& sig) const;
    uint32_t GetNbVirtualFunctions() const;
    std::tuple<Struct*, Signature*> GetVirtualFunctionSource(Function const& fn) const;

    bool _template = false;
    std::vector<Type*> _templateParams;
};

struct EmptyType {};
struct FunctionPtr;

struct Type {
    bool _const = false;
    bool _array = false;
    size_t _arraySize = 0;
    // Basic type represents the char, int, bool etc. types and their composites.
    // std::string represents a type that is composed of a single word (minus pointers) that
    // can be a typedef or the name of a class.
    // unique_ptr<Struct> represent a type defined in a structure.
    // unique_ptr<FunctionPtr> represents a pointer to function. unique_ptr used because FunctionPtr
    // contains a Type, which would create an infinite structure if it was added here without an
    // indirection.
    // Nested names are not allowed because properly processing them would require interpreting the files.
    std::variant<EmptyType, BasicType, FunctionPtr*, Struct, std::string> _value;
    std::vector<PointerDecl> _pointerDecl;
    bool _pending = true;

    // Name used to register the type in the global map
    std::string _name;

    // Points to the true version of this type in case we are dealing with a synonym
    // e.g. uint32_t has "unsigned int" as synonym.
    Type* _synonym = nullptr;

    // Points to the base, unqualified  version, of this type.
    Type* _base = nullptr;

    // If set, this overrides computation of the size (but not pointer size)
    std::optional<size_t> _size;

    // If set to true, indicates that this is the base for a template. 
    // Such a type is never resolved to anything other than a string, and is
    // used as a base key to get an instanciation of a template type.
    bool _templateBase = false;

    ~Type();

    bool IsPointer() const;
    bool IsEmpty() const;
    bool IsArray() const;
    bool IsResolved() const;
    bool IsStruct() const;
    bool IsBasic() const;
    bool IsFunctionPointer() const;

    Struct const& GetStruct() const;

    std::string ToString(bool full) const;
    size_t size() const;

    Type* GetTrueType();
    std::string GetPrefix() const;
    std::string GetFullName() const;

    bool operator==(const Type& other) const;
    bool operator!=(const Type& other) const;
};

enum CallingConventions {
    CDECL,
    STDCALL,
    FASTCALL,
    THISCALL
};

struct FunctionPtr {
    Type* _ret;
    std::optional<CallingConventions> _convention;
    Type* _scope = nullptr;
    std::vector<Type*> _parameters;

    std::string GetName() const;
    std::string ToString() const;
};

enum NamespaceKind {
    GLOBAL,
    CLASS
};

enum FunctionKind {
    FUNCTION,
    METHOD
};

enum Registers {
    // Written in x86 order : eax = 0, edx = 3 etc.
    EAX,
    ECX,
    EDX,
    EBX,
    ESP,
    EBP,
    ESI,
    EDI,

    XMM0,
    XMM1,
    XMM2,
    XMM3,
    XMM4,
    XMM5,
    XMM6,
    XMM7,
    MAX
};

std::string RegisterToString(Registers reg);

enum FunctionQualifiers {
    STATIC = 1 << 0,
    VIRTUAL = 1 << 1,
    CLEANUP = 1 << 2,
    PURE = 1 << 3
};

std::string CallingConventionToString(CallingConventions convention);


struct FunctionParam {
    Type* _type;
    std::string _name;
    std::optional<Registers> _reg;

    std::string ToString() const;

    bool operator==(const FunctionParam& other) const;
    bool operator!=(const FunctionParam& other) const;
};

struct Function {
    uint32_t _qualifiers = 0;
    FunctionKind _kind;
    std::optional<CallingConventions> _convention;
    Type* _ret;
    std::string _name;
    std::vector<FunctionParam> _params;

    bool IsVirtual() const;
    bool IsCleanup() const;
    bool IsStatic() const;
    bool IsPure() const;
    std::string ToString() const;

    bool operator==(const Function& other) const;
    bool operator!=(const Function& other) const;
};

struct ExternalFunction {
    std::string _dll;
    std::string _name;
    std::string _namespace;

    Function _fn;
};

struct Variable {
    Type* _type;
    std::string _name;
};

struct Signature {
    std::string _sig;
    Function _function;

    bool operator==(Signature const& other) const;
};

struct VirtualFunction {
    Signature _signature;
    bool _override = false;
};

struct VariableSignature {
    std::string _sig;
    Variable _variable;
};

typedef std::variant<Signature, VariableSignature> SignatureV;

struct Array {
    std::string _name;
    size_t _size = 0;
};

typedef std::variant<Type*, size_t> TypeInfoV;
typedef std::variant<std::string, Array> FullNameV;