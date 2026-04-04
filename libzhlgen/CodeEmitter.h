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
	CodeEmitter(TypeMap* types, AsmDefMap* asmDefs, std::string const& outputHeader,
		std::string const& outputImpl, std::string const& outputHooks,
		std::optional<std::string> outputAsmImpl,
		std::string const& asmLibFile, std::string const& genLibCmd,
		std::string const& genLibTc);

	bool ProcessZHLFiles(fs::path const& base);
	bool ProcessFile(fs::path const& path);

	void Emit();
	void Dump();

private:
	friend class TypeEmitter;
	friend class TypeGenerator;

	typedef std::variant<Signature, Function> SigOrFunc;

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

	void EmitHeader();
	void EmitHeader(Struct const& s);
	void EmitHeader(VariableSignature const& sig);
	void EmitHeader(Variable const& var);
	void EmitHeader(Signature const& sig, bool isVirtual);
	void EmitHeader(VTableEntry const& sig);

	void EmitImplementation();
	void EmitImpl(Struct const& s);
	void EmitImpl(VariableSignature const& sig);
	void EmitImpl(Signature const& sig, bool isVirtual);
	void EmitImpl(VTableEntry const& sig);

	void EmitJsonFile();

	void BuildExternalNamespaces();

	void EmitForwardDecl();
	void EmitJsonPrologue();
	void EmitJsonEpilogue();
	void EmitAsmDefinitions_Decl();
	void EmitAsmDefinitions_Impl();

	void Emit(Struct const& s);
	void Emit(std::string const& s);
	void Emit(Variable const& var);
	void Emit(Signature const& sig, bool isVirtual);
	void Emit(VTableEntry const& sig);
	void Emit(PointerDecl const& ptr);
	void EmitFunction(Function const& fun, bool withPrefix = false);
	void Emit(VariableSignature const& sig);
	void EmitNamespaceHeader(std::string const& name);
	void EmitNamespaceImpl(std::string const& name);
	void Emit(ExternalFunction const& fn);
	void Emit(std::vector<Variable> const& vars);
	void EmitParamData(Function const& fn, FunctionParam const& param, uint32_t* fnStackSize, uint32_t* stackSize, bool comma);

	void EmitAssembly(SigOrFunc const& sig, bool isVirtual, bool isPure);
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
	std::string GenerateFunctionPrototype(Function const& fn, bool includeName,
	    bool includeCallingConvention, bool protectName, bool link);
	void EmitFunctionPrototype(Function const& fn, bool includeName,
	    bool includeCallingConvention, bool protectName, bool link);
	uint32_t GetFlags(Function const& fn) const;

	void EmitGlobalPrologue();
	void EmitImplPrologue();

	void EmitTab();
	void EmitNL();

	void EmitDecl();
	void EmitImpl();
	void EmitAsmImpl();
	void EmitJson();

	inline void IncrDepth() {
		++_emitDepth;
	}

	inline void DecrDepth() {
		--_emitDepth;
	}

	bool Emitted(Struct const& s) const;
	bool InProcessing(Struct const& s) const;

	void EmitDependencies(Struct const& s);

	inline uint32_t ResetDepth() {
		uint32_t back = _emitDepth;
		_emitDepth = 0;
		return _emitDepth;
	}

	inline void RestoreDepth(uint32_t depth) {
		_emitDepth = depth;
	}

	Namespace _global;
	TypeMap* _types;
	AsmDefMap* _asmDefs;
	std::map<std::string, std::vector<ExternalFunction const*>> _externals;

	std::ofstream _decls, _impl, _json, _asmImpl;
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
	bool _emitAssembly = false;
	std::string _outputHeaderFilename;

	struct FunctionJsonData {
	    bool canHook;
	    std::string internalName;
	    std::string fullName;
	};
	std::vector<FunctionJsonData> _functionJsonData;
	std::string _asmLibFile;
	std::string _genLibCmd;
	std::string _genLibTc;
	void EmitJson(FunctionJsonData const& data);
	Function const* FunctionFromSigOrFunc(SigOrFunc const& fn);
	void ComputeMangledNames();
	void EmitStub(FILE* f, SigOrFunc const& fn);
};

Logger* ParserLogger();
