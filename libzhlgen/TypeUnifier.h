#pragma once

#include "ParserDefinitions.h"

namespace TypeUnifier {
    std::string GetCanonicalName(Type const& type);
    std::string GetCanonicalName(std::string const& name);

    bool AreTypesUnified(Type const& type, std::string const& name);
}
