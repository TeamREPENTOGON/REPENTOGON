#pragma once

#include <fstream>
#include <filesystem>
#include <map>

#include "ParserDefinitions.h"
#include "TypeMap.h"

#include "logger/logger.h"

namespace fs = std::filesystem;

class CodeEmitter {
public:
	CodeEmitter(TypeMap* types, bool test);

	bool ProcessZHLFiles(fs::path const& base);
	bool ProcessFile(fs::path const& path);

	void Emit();
	void Dump();

private:
	friend class TypeEmitter;
	friend class TypeGenerator;

	class TypeEmitter {
	public:
		TypeEmitter(CodeEmitter*);

		template<typename T>
		void operator()(T const& t);

	private:
		CodeEmitter* _emitter;
	};

	class TypeGenerator {
	public:
		TypeGenerator(CodeEmitter*);

		template<typename T>
		std::string operator()(T const& t);

	private:
		CodeEmitter* _emitter;
	};

	void BuildExternalNamespaces();

	void EmitForwardDecl();
	void EmitJsonPrologue();
	void EmitJsonEpilogue();

	void Emit(Struct const& s);
	void Emit(std::string const& s);
	void Emit(Variable const& var);
	void Emit(Signature const& sig, bool isVirtual);
	void Emit(std::variant<Signature, Skip, Function> const& sig);
	void Emit(PointerDecl const& ptr);
	void EmitFunction(Function const& fun, bool withPrefix = false);
	void Emit(VariableSignature const& sig);
	void EmitNamespace(std::string const& name);
	void Emit(ExternalFunction const& fn);
	void Emit(std::vector<Variable> const& vars);
	void EmitParamData(Function const& fn, FunctionParam const& param, uint32_t* fnStackSize, uint32_t* stackSize, bool comma);
	void EmitJson(Function const& fn, std::string const& internalName,
		std::string const& fullName);

	void EmitAssembly(std::variant<Signature, Function> const& sig, bool isVirtual, bool isPure);
	void EmitAssembly(VariableSignature const& sig);
	void EmitInstruction(std::string const& ins);
	
	std::string GenerateType(Type const& type);
	std::string GenerateType(BasicType const& t);
	std::string GenerateType(Struct const& s);
	std::string GenerateType(FunctionPtr* ptr);
	std::string GenerateType(std::string const& s);
	std::string GenerateType(EmptyType const&);
	std::string GenerateType(const PointerDecl& decl);

	void Emit(Type const& type);
	void EmitType(BasicType const& t);
	void EmitType(Struct const& s);
	void EmitType(FunctionPtr* ptr);
	void EmitType(std::string const& s);
	void EmitType(EmptyType const&);

	std::tuple<bool, uint32_t, uint32_t> EmitArgData(Function const& fn);
	void EmitTypeID(Function const& fn);
	std::string GenerateFunctionPrototype(Function const& fn, bool includeName);
	void EmitFunctionPrototype(Function const& fn, bool includeName);
	uint32_t GetFlags(Function const& fn) const;

	void EmitGlobalPrologue();
	void EmitImplPrologue();

	void EmitTab();
	void EmitNL();

	void EmitDecl();
	void EmitImpl();
	void EmitJson();

	void IncrDepth();
	void DecrDepth();

	bool Emitted(Struct const& s) const;
	bool InProcessing(Struct const& s) const;

	void EmitDependencies(Struct const& s);
	void AssertEmitted(Struct const& s);

	Namespace _global;
	TypeMap* _types;
	std::map<std::string, std::vector<ExternalFunction const*>> _externals;

	std::ofstream _decls, _impl, _json;
	std::ofstream* _emitContext = nullptr;
	Variable const* _variableContext = nullptr;
	Struct const* _currentStructure = nullptr;
	uint32_t _virtualFnUnk = 0;
	uint32_t _nEmittedFuns = 0;
	uint32_t _nEmittedVars = 0;
	uint32_t _nEmittedExternal = 0;

	// Allows for a white / grey / black coloration of the dependencies graph
	std::set<std::string> _emittedStructures;
	std::set<std::string> _processingStructures;

	// Track the structures whose methods have been emitted in InitializeSymbols
	std::set<std::string> _initializedStructures;

	std::set<std::string> _vtableProcessingStructures;

	uint32_t _emitDepth = 0;

	/* Convert from a function's fully qualified name to its internal name
	 * used in the hook enabler system. FQFN is <ret> <full_namespace>::name([params]*);,
	 * internal name is _fun<ID>.
	 */
	std::map<std::string, std::string> _fullNameToInternalName;
};

Logger* ParserLogger();