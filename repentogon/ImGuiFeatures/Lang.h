#pragma once

class Lang{
public:
    Lang();
    void Load(unsigned int language);
    void DrawMenu();
    void DrawReportWindow(bool isImGuiActive);
#define I(ITEM, TRANSLATE) const char * ITEM;
#include "Localization/en_us.inl"
#undef I

private:
    bool reportWindowEnabled;
    const char* currentDisplayLanguageName;
    struct {
#define I(ITEM, TRANSLATE) int ITEM : 1;
#include "Localization/en_us.inl"
#undef I
    }currentDisplayLangInfo;

    void ResetCurrentDisplayLangInfo();
};

extern Lang LANG;