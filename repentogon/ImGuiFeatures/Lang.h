#pragma once

class Lang{
public:
    Lang();
    void Load(unsigned int language);
    void DrawMenu();
    void DrawReportWindow(bool isImGuiActive);

    bool isLoaded;

#define I(ITEM, TRANSLATE) const char * ITEM;
#define V(TYPE, VAR, VALUE) TYPE VAR;
#include "Localization/en_us.inl"
#undef I
#undef V

private:
    bool reportWindowEnabled;
    const char* currentDisplayLanguageName;
    struct {
#define I(ITEM, TRANSLATE) int ITEM : 1;
#define V(TYPE, VAR, VALUE)
#include "Localization/en_us.inl"
#undef I
#undef V
    }currentDisplayLangInfo;

    void ResetCurrentDisplayLangInfo();
};

extern Lang LANG;