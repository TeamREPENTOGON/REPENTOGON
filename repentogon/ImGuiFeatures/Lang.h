#pragma once

class Lang{
public:
    Lang();
    void Load(unsigned int language);

#define I(ITEM, TRANSLATE) const char * ITEM;
#include "Localization/en_us.inl"
#undef I
};

extern Lang LANG;