#pragma once

#include <filesystem>
#include <map>
#include "ParserCore.h"

namespace fs = std::filesystem;

class CodeEmitter {
public:
	CodeEmitter();

	void ProcessZHLFiles(fs::path const& base);
	void ProcessFile(fs::path const& path);

	void Emit();
	void Dump();

private:
	friend class TypeEmitter;

	class TypeEmitter {
	public:
		TypeEmitter(CodeEmitter*);

		template<typename T>
		void operator()(T const& t);

	private:
		CodeEmitter* _emitter;
	};

	void CheckVTables();
	void CheckVTableInternalConsistency(Struct const& s);
	void CheckVTableHierarchyConsistency(Struct const& s, std::vector<Struct const*> const& parents);

	void AssertReady(Type const* t);
	void CheckDependencies();
	void CheckDependencies(Type const& t);

	void ForwardDecl();

	void Emit(Type const& type);
	void Emit(Struct const& s);
	void Emit(std::string const& s);
	void Emit(Variable const& var);
	void Emit(Signature const& sig, bool isVirtual);
	void Emit(std::variant<Signature, Skip> const& sig);
	void Emit(PointerDecl const& ptr);
	void Emit(Function const& fun);

	void EmitAssembly(Signature const& sig);
	void EmitInstruction(std::string const& ins);
	
	void EmitType(BasicType const& t);
	void EmitType(Struct const& s);
	void EmitType(FunctionPtr* ptr);
	void EmitType(std::string const& s);
	void EmitType(EmptyType const&);

	void EmitTab();
	void EmitNL();

	void EmitDecl();
	void EmitImpl();

	void IncrDepth();
	void DecrDepth();

	bool Emitted(Struct const& s) const;
	bool InProcessing(Struct const& s) const;

	void EmitDependencies(Struct const& s);
	void AssertEmitted(Struct const& s);

	Namespace _global;
	std::map<std::string, Type> _types;

	std::ofstream _decls, _impl;
	std::ofstream* _emitContext = nullptr;
	Variable const* _variableContext = nullptr;
	Struct const* _currentStructure;
	uint32_t _virtualFnUnk = 0;
	uint32_t _nEmittedFuns = 0;

	// Allow for a white / grey / black coloration of the dependencies graph
	std::set<std::string> _emittedStructures;
	std::set<std::string> _processingStructures;

	std::set<std::string> _vtableProcessingStructures;
	ErrorLogger _logger;

	uint32_t _emitDepth = 0;
};