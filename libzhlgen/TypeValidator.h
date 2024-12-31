#pragma once

#include <memory>

#include "CodeEmitter.h"
#include "TypeMap.h"

enum ValidateTypeResult {
	VALIDATE_TYPE_OK,
	VALIDATE_TYPE_NOT_READY,
	VALIDATE_TYPE_DEPENDENCY_NOT_READY,
	VALIDATE_TYPE_VTABLE_INTERNAL_INCONSISTENCY,
	VALIDATE_TYPE_VTABLE_HIERARCHY_INCONSISTENCY,
	VALIDATE_TYPE_SIZE_ERROR,
	VALIDATE_TYPE_ALIGNMENT_ERROR,
	VALIDATE_TYPE_STRUCTURE_ERROR
};

class TypeValidator {
public:
	TypeValidator(TypeMap* types);
	std::map<std::string, ValidateTypeResult> ValidateTypes();
	ValidateTypeResult ValidateType(Type& type);
	inline bool HasCycle() const { return _hasCycle; }

	friend class DependencyGraphBuilder;
	friend class TarjanSCC;

private:
	struct DependencyNode {
		Struct* _struct = nullptr;
		/* There cannot be multiple DependencyNode associated with the same
		 * structure. Use a set here to avoid situation where a type depends
		 * multiple times on the same thing (for instance, class B inherits
		 * from class A *and* is composed of A -> keep only one dep).
		 */
		std::set<DependencyNode*> _deps;
		// DFS and BFS
		bool _visited = false;
		// Tarjan SCC
		static constexpr int TARJAN_SCC_UNDEFINED = -1;
		int _index = TARJAN_SCC_UNDEFINED;
		int _lowlink = -1;
		bool _onStack = false;
	};

	struct Fields {
		Fields(TypeMap* types) : _types(types) { }

		TypeMap* _types;
		std::set<Variable> _fields;
		uint32_t _paddingCount = 0;

		bool AddPadding(size_t offset, size_t size);
	};

	inline bool IsTypeReady(Type const& type) { return !type._pending || type._name == "std"; }
	ValidateTypeResult CheckDependencies(Type const& type);
	ValidateTypeResult CheckVTable(Type const& type);
	ValidateTypeResult CheckVTableInternalConsistency(Struct const& s);
	ValidateTypeResult CheckVTableHierarchyConsistency(Struct const& s);
	ValidateTypeResult CheckSizeAndAlignment(Type& s);
	ValidateTypeResult CheckSizeAndAlignment(Struct& s);
	
	/* Iterate the fields of the structure and put them in the set, sorted by
	 * offset.
	 * 
	 * Return true if there are no repeated offsets, false otherwise.
	 */
	bool BuildUniqueFields(Struct const& s, Fields& fields);
	/* Add padding to the set of fields, and check the structure is properly
	 * aligned.
	 * 
	 * Return true if the structure is aligned, false otherwise.
	 */
	bool AlignStructure(Struct const& s, Fields& fields, size_t& finalOffset, size_t& alignment);
	/* Check that the size of the structure, if given, is coherent with the offset
	 * of the last field, and with the alignment constraint of the structure.
	 * 
	 * If these checks pass, the function adds padding to the structure, if the
	 * final offset is such that the size of the structure would result in incorrect
	 * alignment. If a size was given to the structure, padding is added to 
	 * reach that size. If no size was given, the structure is aligned on the
	 * next natural boundary.
	 * 
	 * Return true if the size of the structure is coherent with the alignment
	 * and with the finalOffset, false otherwise. If no size was given to the
	 * structure, the function always returns true.
	 */
	bool CheckStructureSize(Struct const& s, Fields& fields, size_t finalOffset, size_t alignment, size_t& size);
	/* Finalize a structure, settings all its variables alongside padding,
	 * and freezing its size and alignment.
	 */
	void FinalizeStructure(Struct& s, Fields const& fields, size_t size, size_t alignment);
	bool CheckOverridenFunctions(Struct const& source, std::vector<Struct const*> const& parents);
	bool CheckNonOverridenVirtualFunctions(Struct const& source, std::vector<Struct const*> const& parents);
	void GetParents(Struct const& source, std::vector<Struct const*>& parents);
	bool BuildDependencyGraph();
	ValidateTypeResult CheckSizeAndAlignmentNoDeps(Struct& s, size_t startingOffset, size_t startingAlignment);
	ValidateTypeResult CheckSizeAndAlignmentDeps(Struct& s, DependencyNode* deps);

	typedef std::map<Struct*, DependencyNode*> StructToDeps;
	TypeMap* _types;
	std::vector<DependencyNode*> _dependencyGraph;
	StructToDeps _structToDeps;
	bool _hasCycle = false;
};