#include <queue>
#include <sstream>
#include <stack>

#include "TypeValidator.h"

TypeValidator::TypeValidator(TypeMap* types) : _types(types) { 
    _hasCycle = !BuildDependencyGraph();
}

std::map<std::string, ValidateTypeResult> TypeValidator::ValidateTypes() {
	std::map<std::string, ValidateTypeResult> results;
	for (auto& [name, type] : *_types) {
		results[name] = ValidateType(*type);
	}
	return results;
}

ValidateTypeResult TypeValidator::ValidateType(Type& type) {
    ValidateTypeResult result = CheckDependencies(type);
    if (result != VALIDATE_TYPE_OK) {
        return result;
    }

    result = CheckVTable(type);
    if (result != VALIDATE_TYPE_OK) {
        return result;
    }

    result = CheckSizeAndAlignment(type);
    if (result != VALIDATE_TYPE_OK) {
        return result;
    }

    return result;
}

ValidateTypeResult TypeValidator::CheckDependencies(Type const& type) {
	if (!type.IsStruct())
		return VALIDATE_TYPE_OK;

	Struct const& s = type.GetStruct();
    // Assert dependencies have a definition
    for (Type* t : s._deps) {
        if (!IsTypeReady(*t)) {
            ParserLogger()->Error("Dependency %s of structure %s is an undefined type\n",
                t->GetFullName().c_str(), s._name.c_str());
            return VALIDATE_TYPE_DEPENDENCY_NOT_READY;
        }
    }

    // Assert parents have a definition
    for (auto const& [type, _] : s._parents) {
        if (!IsTypeReady(*type)) {
            ParserLogger()->Error("Parent class %s of structure %s is an undefined type\n",
                type->GetFullName().c_str(), s._name.c_str());
            return VALIDATE_TYPE_DEPENDENCY_NOT_READY;
        }
    }

    // Check fields
    for (Variable const& v : s._namespace._fields) {
        if (v._type->IsStruct() && !v._type->IsPointer()) {
            if (!IsTypeReady(*v._type)) {
                ParserLogger()->Error("Field %s of structure %s has undefined type %s\n",
                    v._name.c_str(), s._name.c_str(), v._type->GetFullName().c_str());
                return VALIDATE_TYPE_DEPENDENCY_NOT_READY;
            }

            Struct const& var = v._type->GetStruct();

            // Check for definition of type in direct dependencies
            bool found = false;
            for (Type* t : s._deps) {
                Struct const& dep = t->GetStruct();
                if (dep._name == var._name) {
                    found = true;
                    break;
                }
            }

            // Check for definition of type in parents
            if (!found) {
                std::stack<Type*> parents;
                for (auto const& [type, _] : s._parents) {
                    parents.push(type);
                }

                while (!parents.empty()) {
                    Type* t = parents.top();
                    parents.pop();

                    if (t->GetStruct()._name == var._name) {
                        found = true;
                        break;
                    }

                    for (auto const& [type, _] : t->GetStruct()._parents) {
                        parents.push(type);
                    }
                }
            }

            if (!found) {
                ParserLogger()->Error("Field %s of structure %s has undefined type %s\n",
                    v._name.c_str(), s._name.c_str(), v._type->GetFullName().c_str());
                return VALIDATE_TYPE_DEPENDENCY_NOT_READY;
            }
        }
    }

    return VALIDATE_TYPE_OK;
}

ValidateTypeResult TypeValidator::CheckVTable(Type const& type) {
    if (!type.IsStruct()) {
        return VALIDATE_TYPE_OK;
    }

    Struct const& s = type.GetStruct();
    ValidateTypeResult result = CheckVTableInternalConsistency(s);
    if (result != VALIDATE_TYPE_OK) {
        return result;
    }

    result = CheckVTableHierarchyConsistency(s);
    return result;
}

ValidateTypeResult TypeValidator::CheckVTableInternalConsistency(Struct const& s) {
    // Check that each function declared as override does not conflict with 
    // a function not declared as override (i.e. no self-override).
    for (Signature const& sig : s._overridenVirtualFunctions) {
        for (VTableEntry const& fn : s._virtualFunctions) {
            Function const* function = VTableEntryToFunction(fn);

            if (function) {
                if (function->_name == sig._function._name) {
                    if (*function == sig._function) {
                        ParserLogger()->Error("Structure %s has an inconsistent vtable: method %s overrides itself\n",
                            s._name.c_str(), function->_name);
                        return VALIDATE_TYPE_VTABLE_INTERNAL_INCONSISTENCY;
                    }
                }
            }
        }
    }

    return VALIDATE_TYPE_OK;
}

ValidateTypeResult TypeValidator::CheckVTableHierarchyConsistency(Struct const& s) {
    std::vector<Struct const*> parents;
    GetParents(s, parents);

    if (!CheckOverridenFunctions(s, parents)) {
        return VALIDATE_TYPE_VTABLE_HIERARCHY_INCONSISTENCY;
    }

    if (!CheckNonOverridenVirtualFunctions(s, parents)) {
        return VALIDATE_TYPE_VTABLE_HIERARCHY_INCONSISTENCY;
    }

    return VALIDATE_TYPE_OK;
}

void TypeValidator::GetParents(Struct const& source, std::vector<Struct const*>& parents) {
    std::set<std::string> seen;
    std::stack<Struct const*> parentsBuffer;

    for (auto const& [parent, _] : source._parents) {
        Struct const& parentStruct = parent->GetStruct();
        if (seen.find(parentStruct._name) != seen.end()) {
            continue;
        }

        parentsBuffer.push(&parentStruct);
        seen.insert(parentStruct._name);
    }

    while (!parentsBuffer.empty()) {
        Struct const* parent = parentsBuffer.top();
        parentsBuffer.pop();

        for (auto const& [parentParent, _] : parent->_parents) {
            Struct const& parentStruct = parentParent->GetStruct();
            if (seen.find(parentStruct._name) != seen.end()) {
                continue;
            }

            parentsBuffer.push(&parentStruct);
        }

        parents.push_back(parent);
    }
}

bool TypeValidator::CheckOverridenFunctions(Struct const& source, std::vector<Struct const*> const& parents) {
    for (Signature const& sig : source._overridenVirtualFunctions) {
        bool found = false;
        for (Struct const* parent : parents) {
            for (VTableEntry const& fn : parent->_virtualFunctions) {
                Function const* function = VTableEntryToFunction(fn);
                if (function) {
                    if (*function == sig._function) {
                        found = true;
                        break;
                    }
                }
            }

            if (found) {
                break;
            }
        }

        if (!found) {
            ParserLogger()->Error("Structure %s overrides method %s that does not appear in any parent vtable\n",
                source._name.c_str(), sig._function._name.c_str());
            return false;
        }
    }

    return true;
}

bool TypeValidator::CheckNonOverridenVirtualFunctions(Struct const& source, std::vector<Struct const*> const& parents) {
    for (VTableEntry const& fun : source._virtualFunctions) {
        if (std::holds_alternative<Skip>(fun)) {
            continue;
        }

        Function const* sourceFn = VTableEntryToFunction(fun);
        std::optional<std::string> badOverride;

        for (Struct const* parent : parents) {
            for (VTableEntry const& other : parent->_virtualFunctions) {
                Function const* function = VTableEntryToFunction(other);
                if (function) {
                    if (*sourceFn == *function) {
                        badOverride = parent->_name;
                        break;
                    }
                }
            }

            if (badOverride) {
                ParserLogger()->Error("Structure %s overrides method %s in parent class %s, but no override specifier found\n",
                    source._name.c_str(), sourceFn->_name.c_str(), badOverride->c_str());
                return false;
            }
        }
    }

    return true;
}

ValidateTypeResult TypeValidator::CheckSizeAndAlignment(Type& type) {
    if (!type.IsStruct()) {
        return VALIDATE_TYPE_OK;
    }

    return CheckSizeAndAlignment(type.GetStruct());
}

ValidateTypeResult TypeValidator::CheckSizeAndAlignment(Struct& s) {
    auto iter = _structToDeps.find(&s);
    ValidateTypeResult result;
    if (iter == _structToDeps.end() || iter->second->_deps.empty()) {
        result = CheckSizeAndAlignmentNoDeps(s, 0, 0);
    } else {
        result = CheckSizeAndAlignmentDeps(s, iter->second);
    }

    if (result == VALIDATE_TYPE_OK && iter != _structToDeps.end()) {
        iter->second->_visited = true;
    }

    return result;
}

ValidateTypeResult TypeValidator::CheckSizeAndAlignmentNoDeps(Struct& s, size_t startingOffset, size_t startingAlignment) {
    Fields fields(_types);
    if (!BuildUniqueFields(s, fields)) {
        return VALIDATE_TYPE_STRUCTURE_ERROR;
    }

    size_t finalOffset = startingOffset, alignment = startingAlignment;
    if (!AlignStructure(s, fields, finalOffset, alignment)) {
        return VALIDATE_TYPE_ALIGNMENT_ERROR;
    }

    size_t size = 0;
    if (!CheckStructureSize(s, fields, finalOffset, alignment, size)) {
        return VALIDATE_TYPE_SIZE_ERROR;
    }

    FinalizeStructure(s, fields, size, alignment);
    return VALIDATE_TYPE_OK;
}

ValidateTypeResult TypeValidator::CheckSizeAndAlignmentDeps(Struct& s, DependencyNode* deps) {
    for (DependencyNode* dep : deps->_deps) {
        if (dep->_visited) {
            continue;
        }

        ValidateTypeResult depResult = CheckSizeAndAlignment(*dep->_struct);
        if (depResult != VALIDATE_TYPE_OK) {
            return depResult;
        }
        
        dep->_visited = true;
    }
   
    size_t startingOffset = 0, startingAlignment = 0;
    for (auto const& [type, _] : s._parents) {
        startingOffset += type->size();
        startingAlignment = std::max(startingAlignment, type->alignment());
    }

    return CheckSizeAndAlignmentNoDeps(s, startingOffset, startingAlignment);
}

bool TypeValidator::BuildUniqueFields(Struct const& s, Fields& fields) {
    bool ok = true;
    for (Variable const& v : s._namespace._fields) {
        auto [position, inserted] = fields._fields.insert(v);
        if (!inserted) {
            ParserLogger()->Error("Field %s in structure %s has offset %lu, same as field %s\n",
                v._name.c_str(), s._name.c_str(), *v._offset, position->_name.c_str());
            ok = false;
        }
    }

    return ok;
}

bool TypeValidator::AlignStructure(Struct const& s, Fields& fields, 
    size_t& finalOffset, size_t& alignment) {
    size_t currentOffset = finalOffset;
    std::set<Variable>::iterator iter = fields._fields.begin(), end = fields._fields.end(), previous = iter;
    uint32_t paddingCount = 0;

    while (iter != end) {
        if (*iter->_offset != currentOffset) {
            /* Example:
             * struct { int a: 0x0; char b: 0x2; };
             * currentOffset = 4
             * iter = "char b: 0x2"
             * 
             * Layout is invalid.
             */
            if (*iter->_offset < currentOffset) {
                ParserLogger()->Error("Field %s at offset %lu in structure %s overlaps with previous field %s (previous offset %lu, expected %lu)\n",
                    iter->_name.c_str(), *iter->_offset, s._name.c_str(), previous->_name.c_str(), *previous->_offset, currentOffset);
                return false;
            } 
            /* Example:
             * struct { int a: 0x0; char b: 0x5; };
             * currentOffset = 4
             * iter = "char b: 0x5"
             * 
             * Padding needs to be inserted.
             */
            else {
                size_t padding = *iter->_offset - currentOffset;
                if (!fields.AddPadding(currentOffset, padding)) {
                    ParserLogger()->Error("Unable to add padding of %lu in structure %s at offset %lu\n",
                        padding, s._name.c_str(), currentOffset);
                    return false;
                } else {
                    ParserLogger()->Debug("Adding %lu bytes of padding at offset %lu of structure %s\n",
                        padding, currentOffset, s._name.c_str());
                }

                // Do not move iter (not formally processed yet), but offset to it
                currentOffset += padding;
            }
        } else {
            size_t fieldAlignment = iter->_type->alignment();
            if (fieldAlignment == 0) {
                ParserLogger()->Error("Field %s in structure %s of type %s has alignment of 0\n",
                    iter->_name.c_str(), s._name.c_str(), iter->_type->GetFullName().c_str());
                throw std::runtime_error("Type has alignment of 0");
            }

            if (currentOffset % fieldAlignment != 0) {
                ParserLogger()->Error("Field %s at offset %lu is not aligned (expected alignment: %lu)\n",
                    iter->_name.c_str(), currentOffset, fieldAlignment);
                return false;
            }

            alignment = std::max(alignment, fieldAlignment);

            currentOffset += iter->_type->size();
            previous = iter;
            ++iter;
        }
    }

    if (alignment == 0) {
        ParserLogger()->Warn("Structure %s has no alignment, defaulting to 1\n", s._name.c_str());
        alignment = 1;
    }

    finalOffset = currentOffset;
    return true;
}

bool TypeValidator::CheckStructureSize(Struct const& s, Fields& fields, size_t finalOffset, size_t alignment, size_t& size) {
    if (s._size) {
        // Validate the size given. If valid, add padding if necessary.
        size_t givenSize = *s._size;
        if (givenSize % alignment != 0) {
            ParserLogger()->Error("Structure %s of size %lu is not aligned (expected alignment: %lu)\n",
                s._name.c_str(), givenSize, alignment);
            return false;
        }

        if (finalOffset > givenSize) {
            ParserLogger()->Error("Structure %s memory footprint is at least %lu, and exceeds specified size %lu\n",
                s._name.c_str(), finalOffset, givenSize);
            return false;
        }

        if (finalOffset < givenSize) {
            size_t padding = givenSize - finalOffset;
            if (!fields.AddPadding(finalOffset, padding)) {
                ParserLogger()->Error("Unable to add %lu bytes of padding at end of structure %s to align it\n",
                    padding, s._name.c_str());
                return false;
            } else {
                ParserLogger()->Debug("Adding %lu bytes of padding at offset %lu of structure %s\n",
                    padding, finalOffset, s._name.c_str());
            }
        }

        size = *s._size;
    } else {
        // No size given, align on next natural boundary if needed
        if (finalOffset % alignment != 0) {
            size_t padding = alignment - (finalOffset % alignment);
            if (!fields.AddPadding(finalOffset, padding)) {
                ParserLogger()->Error("Unable to add %lu bytes of padding at end of structure %s to align it\n",
                    padding, s._name.c_str());
                return false;
            } else {
                ParserLogger()->Debug("Adding %lu bytes of padding at offset %lu of structure %s\n",
                    padding, finalOffset, s._name.c_str());
            }

            size = finalOffset + padding;
        } else {
            size = finalOffset;
        }
    }

    if (size == 0) {
        ParserLogger()->Warn("Structure %s has size of 0, defaulting to 1\n", s._name.c_str());
        size = 1;
    }

    return true;
}

void TypeValidator::FinalizeStructure(Struct& s, Fields const& fields, size_t size, size_t alignment) {
    s._size = size;
    s._namespace._fields.clear();
    std::copy(fields._fields.begin(), fields._fields.end(), std::back_inserter(s._namespace._fields));
}

bool TypeValidator::Fields::AddPadding(size_t offset, size_t padding) {
    BasicType type;
    type._type = CHAR;
    Array array;
    array._size = padding;

    char buffer[10 + 3 + 1];
    sprintf(buffer, "pad%d", _paddingCount);

    Variable var;
    var._name = buffer;
    var._offset = offset;
    var._type = _types->MakeArray(_types->Get(type), array);

    if (!_fields.insert(var).second) {
        return false;
    }

    ++_paddingCount;
    return true;
}

class TarjanSCC {
    using DependencyNode = TypeValidator::DependencyNode;
public:
    typedef std::vector<DependencyNode*> SCCSet;
    TarjanSCC(std::vector<DependencyNode*>* nodes);

    bool FindStronglyConnectedComponents();
    inline std::vector<SCCSet> const& GetStronglyConnectedComponents() const {
        return _components;
    }

private:
    void Connect(DependencyNode* node);

    std::vector<std::vector<DependencyNode*>> _components;
    std::vector<DependencyNode*>* _nodes;

    uint32_t _index = 0;
    std::stack<DependencyNode*> _stack;
};

TarjanSCC::TarjanSCC(std::vector<DependencyNode*>* nodes) : _nodes(nodes) {

}

bool TarjanSCC::FindStronglyConnectedComponents() {
    for (DependencyNode* node : *_nodes) {
        if (node->_index == node->TARJAN_SCC_UNDEFINED) {
            Connect(node);
        }
    }
    
    for (SCCSet const& set : _components) {
        if (set.size() != 1) {
            return true;
        }
    }

    return false;
}

void TarjanSCC::Connect(DependencyNode* vertex) {
    vertex->_index = _index;
    vertex->_lowlink = _index;
    _stack.push(vertex);
    vertex->_onStack = true;
    ++_index;

    for (DependencyNode* neighbor : vertex->_deps) {
        if (neighbor->_index == neighbor->TARJAN_SCC_UNDEFINED) {
            Connect(neighbor);
            vertex->_lowlink = std::min(vertex->_lowlink, neighbor->_lowlink);
        } else if (neighbor->_onStack) {
            vertex->_lowlink = std::min(vertex->_lowlink, neighbor->_index);
        }
    }

    if (vertex->_lowlink == vertex->_index) {
        DependencyNode* neighbor = nullptr;
        SCCSet component;
        do {
            neighbor = _stack.top();
            _stack.pop();
            neighbor->_onStack = false;
            component.push_back(neighbor);
        } while (neighbor != vertex);
        _components.push_back(std::move(component));
    }
}

class DependencyGraphBuilder {
    using DependencyNode = TypeValidator::DependencyNode;
    using StructToDeps = TypeValidator::StructToDeps;
public:
    DependencyGraphBuilder(TypeMap* types) : _types(types) { }
    bool BuildGraph(std::vector<DependencyNode*>& graph, StructToDeps& map);

private:
    void BuildAllNodes();
    bool CheckCycles();
    void ReduceGraph();
    void EmitGraph(std::vector<DependencyNode*>& graph);

    TypeMap* _types;
    StructToDeps _structToNode;
    std::vector<DependencyNode*> _allNodes;
};

bool DependencyGraphBuilder::BuildGraph(std::vector<DependencyNode*>& graph,
    StructToDeps& map) {
    BuildAllNodes();
    if (!CheckCycles()) {
        ParserLogger()->Fatal("Cannot generate code: dependency cycles were detected\n");
        return false;
    }
    // ReduceGraph();
    EmitGraph(graph);
    map = _structToNode;
    return true;
}

void DependencyGraphBuilder::BuildAllNodes() {
    _allNodes.reserve(_types->size());

    auto pushNode = [this](Struct* s) -> DependencyNode* {
        DependencyNode* node = new DependencyNode();
        node->_struct = s;
        this->_structToNode[s] = node;
        this->_allNodes.push_back(node);
        return node;
    };

    auto processDependency = [this, pushNode](DependencyNode* node, Struct& dep) -> void {
        auto iter = _structToNode.find(&dep);
        if (iter != _structToNode.end()) {
            node->_deps.insert(iter->second);
        } else {
            DependencyNode* depNode = pushNode(&dep);
            node->_deps.insert(depNode);
        }
    };

    for (auto& [name, type] : *_types) {
        if (!type->IsStruct()) {
            continue;
        }

        Struct& s = type->GetStruct();
        auto iter = _structToNode.find(&s);
        DependencyNode* node = nullptr;
        if (iter == _structToNode.end()) {
            node = pushNode(&s);
        } else {
            node = iter->second;
        }

        for (Type* type : s._deps) {
            processDependency(node, type->GetStruct());
        }

        for (auto& [type, _] : s._parents) {
            processDependency(node, type->GetStruct());
        }
    }
}

bool DependencyGraphBuilder::CheckCycles() {
    TarjanSCC scc(&_allNodes);
    if (scc.FindStronglyConnectedComponents()) {
        std::vector<TarjanSCC::SCCSet> const& components = scc.GetStronglyConnectedComponents();
        for (TarjanSCC::SCCSet const& component : components) {
            if (component.size() == 1) {
                continue;
            }
            std::ostringstream stream;
            stream << "Found cycle: ";
            for (size_t i = 0; i < component.size(); ++i) {
                stream << component[i]->_struct->_name << " => ";
            }
            stream << component[0]->_struct->_name << std::endl;
            ParserLogger()->Error("%s", stream.str().c_str());
        }
        return false;
    }

    return true;
}

void DependencyGraphBuilder::ReduceGraph() {
    for (DependencyNode* node : _allNodes) {
        if (node->_visited)
            continue;

        /* Do not color a node as soon as we touch it. Instead, only color nodes
         * that can be reached from other nodes. This way, the only non colored
         * nodes at the end are the ones that cannot be reached from another node,
         * thus isolating the leaves.
         * 
         * The algorithm is a breadth-first search: from a given node, we check
         * which ones can be reached and add them to the list, as long as they
         * have not already been visisted (colored). We then repeat until the
         * list is empty. Each time a node is added to the list, it becomes
         * colored.
         * 
         * The dependency graph is a (disconnected) DAG. The lack of cycles
         * ensures the amount of nodes we push is finite and the loop
         * terminates.
         * 
         * The starting node is the current node. This ensures we do not color
         * it, while still coloring the children as we go through them.
         */
        std::queue<DependencyNode*> allChildren;
        allChildren.push(node);

        while (!allChildren.empty()) {
            DependencyNode* child = allChildren.front();
            allChildren.pop();

            for (DependencyNode* grandChild : child->_deps) {
                if (grandChild->_visited)
                    continue;

                grandChild->_visited = true;
                allChildren.push(grandChild);
            }
        }
    }
}

void DependencyGraphBuilder::EmitGraph(std::vector<DependencyNode*>& graph) {
    for (DependencyNode* node : _allNodes) {
        if (node->_visited) {
            node->_visited = false;
        } else {
            graph.push_back(node);
        }
    }
}

bool TypeValidator::BuildDependencyGraph() {
    DependencyGraphBuilder builder(_types);
    return builder.BuildGraph(_dependencyGraph, _structToDeps);
}