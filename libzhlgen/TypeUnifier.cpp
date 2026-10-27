#include <deque>
#include <sstream>
#include <stdexcept>

#include "TypeUnifier.h"

namespace TypeUnifier {
    std::string GetBasicTypeCanonicalName(BasicType const& type) {
	std::ostringstream stream;
	BasicTypes base = type._type;
	/* Rules: the sign appears first, but only if it is unsigned, or if the
	 * type is explicitly signed char.
	 */
	if (type._sign) {
	    Signedness sign = *type._sign;
	    if (sign == UNSIGNED)
		stream << "unsigned ";
	    else {
		if (base == BasicTypes::CHAR)
		    stream << "signed ";
	    }
	}

	/* Rules: the length is always present, but long long is interpreted
	 * as __int64.
	 */
	bool needSpace = false;
	if (type._length) {
	    Length length = *type._length;
	    switch (length) {
	    case Length::SHORT:
		stream << "short";
		needSpace = true;
		break;

	    case Length::LONG:
		stream << "long";
		needSpace = true;
		break;

	    case Length::LONGLONG:
		stream << "__int64";
		break;

	    default:
		throw std::runtime_error("Unknown length");
	    }
	}

	switch (base) {
	case BasicTypes::CHAR:
	    if (needSpace)
		stream << " ";
	    stream << "char";
	    break;

	case BasicTypes::INT:
	    if (type._length)
		break;

	    if (needSpace)
		stream << " ";
	    stream << "int";
	    break;

	case BasicTypes::BOOL:
	    stream << "bool";
	    break;

	case BasicTypes::TVOID:
	    throw std::runtime_error("Cannot unify void parameter type");

	case BasicTypes::FLOAT:
	    stream << "float";
	    break;

	case BasicTypes::DOUBLE:
	    if (needSpace)
		stream << " ";
	    stream << "double";
	    break;

	case BasicTypes::BT_INVALID:
	    throw std::runtime_error("Cannot unify BT_INVALID parameter");
	}

	return stream.str();
    }

    bool AreTypesUnified(Type const& type, std::string const& name) {
	return GetCanonicalName(type) == GetCanonicalName(name);
    }

    std::string GetCanonicalName(Type const& type) {
	auto const& variant = type._value;
	if (std::holds_alternative<EmptyType>(variant)) {
	    if (!type._base) {
		fprintf(stderr, "[FATAL] GetCanonicalName called on baseless EmptyType\n");
		exit(-1);
	    }

	    std::string base = GetCanonicalName(*type._base);
	    std::ostringstream stream;
	    stream << base;
	    if (type._const) {
		stream << " const";
	    } else if (type._array) {
		stream << " * const";
	    }

	    for (PointerDecl const& decl: type._pointerDecl) {
		switch (decl._kind) {
		case LREF:
		    stream << " &";
		    break;

		case RREF:
		    stream << " &&";
		    break;

		case POINTER:
		    stream << " *";
		    break;
		}

		if (decl._const) {
		    stream << " const";
		}
	    }

	    return stream.str();
	} else if (std::holds_alternative<BasicType>(variant)) {
	    return GetBasicTypeCanonicalName(std::get<BasicType>(variant));
	} else if (std::holds_alternative<FunctionPtr*>(variant)) {
	    fprintf(stderr, "[FATAL] GetCanonicalName called on FunctionPtr\n");
	    exit(-1);
	} else if (std::holds_alternative<Struct>(variant)) {
	    return std::get<Struct>(variant)._name;
	} else {
	    fprintf(stderr, "[WARN] GetCanonicalName called on string\n");
	    if (!type._synonym) {
		fprintf(stderr, "[FATAL] GetCanonicalName called on non synonym string\n");
		exit(-1);
	    }
	    return GetCanonicalName(*type._synonym);
	}
    }

    std::string GetCanonicalName(std::string const& name) {
	/* Split the type along spaces. We need to do it manually because of
	 * templates, as we need to identify the leading part of the type
	 * from time to time.
	 */
	std::deque<std::string> tokens;
	const char* s = name.c_str();
	const char* start = s;
	const char* ptr = s;
	int brackets = 0;
	while (*ptr) {
	    switch (*ptr) {
	    case '>':
		--brackets;
		break;

	    case '<':
		++brackets;
		break;

	    case ' ':
		if (brackets == 0) {
		    tokens.push_back(std::string(start, ptr));
		    start = ptr + 1;
		}
		break;

	    default:
		break;
	    }

	    ++ptr;
	}

	tokens.push_back(std::string(start, ptr));
	if (tokens[0] == "struct" || tokens[0] == "class" ||
		tokens[0] == "enum" || tokens[0] == "union") {
	    tokens.pop_front();
	}

	if (tokens.size() == 1) {
	    return tokens.front();
	} else {
	    std::ostringstream stream;
	    stream << tokens[0];
	    for (int i = 1; i < tokens.size(); ++i) {
		stream << " " << tokens[i];
	    }

	    return stream.str();
	}
    }
}
