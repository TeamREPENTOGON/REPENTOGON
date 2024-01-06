#include "lang.h"
#include "IconsFontAwesome6.h"
#include "IconsFontAwesome6_unicode.h"
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ConsoleMega.h"
#include "GameOptions.h"
#include "HelpMenu.h"
#include "LogViewer.h"
#include "PerformanceWindow.h"

Lang LANG;

Lang::Lang(){
    reportWindowEnabled = false;
    currentDisplayLanguageName = "";
    currentDisplayLangInfo = {};

#define I(ITEM,TRANSLATE) ITEM = TRANSLATE;
#include "Localization/en_us.inl"
#undef I

}

void Lang::Load(unsigned int language){
    switch(language){
    case 2:
#define I(ITEM,TRANSLATE) ITEM = TRANSLATE;
#include "Localization/jp.inl"
#undef I
        break;
    case 11:
#define I(ITEM,TRANSLATE) ITEM = TRANSLATE;
#include "Localization/kr.inl"
#undef I
        break;
    case 13:
#define I(ITEM,TRANSLATE) ITEM = TRANSLATE;
#include "Localization/zh_cn.inl"
#undef I
        break;
    case 10:
#define I(ITEM,TRANSLATE) ITEM = TRANSLATE;
#include "Localization/ru.inl"
#undef I
        break;
    case 5:
#define I(ITEM,TRANSLATE) ITEM = TRANSLATE;
#include "Localization/de.inl"
#undef I
        break;
    case 4:
#define I(ITEM,TRANSLATE) ITEM = TRANSLATE;
#include "Localization/es.inl"
#undef I
        break;
    case 3:
#define I(ITEM,TRANSLATE) ITEM = TRANSLATE;
#include "Localization/fr.inl"
#undef I
        break;
    default:
    //use english texts as default, do nothing
    break;
    }
}

void Lang::ResetCurrentDisplayLangInfo() {
#define I(ITEM, TRANSLATE) currentDisplayLangInfo.ITEM = 1;
#include "Localization/en_us.inl"
#undef I
}

void Lang::DrawMenu() {
    if (ImGui::MenuItem("jp")) {
        reportWindowEnabled = true;
        currentDisplayLanguageName = LANG.TRANS_LANG_JAPANESE;
        ResetCurrentDisplayLangInfo();
#define I(ITEM, TRANSLATE) currentDisplayLangInfo.ITEM = 0;
#include "Localization/jp.inl"
#undef I
    }
    if (ImGui::MenuItem("kr")) {
        reportWindowEnabled = true;
        currentDisplayLanguageName = LANG.TRANS_LANG_KOREAN;
        ResetCurrentDisplayLangInfo();
#define I(ITEM, TRANSLATE) currentDisplayLangInfo.ITEM = 0;
#include "Localization/kr.inl"
#undef I
    }
    if (ImGui::MenuItem("zh_cn")) {
        reportWindowEnabled = true;
        currentDisplayLanguageName = LANG.TRANS_LANG_CHINESE_SIMPLE;
        ResetCurrentDisplayLangInfo();
#define I(ITEM, TRANSLATE) currentDisplayLangInfo.ITEM = 0;
#include "Localization/zh_cn.inl"
#undef I
    }
    if (ImGui::MenuItem("ru")) {
        reportWindowEnabled = true;
        currentDisplayLanguageName = LANG.TRANS_LANG_RUSSIAN;
        ResetCurrentDisplayLangInfo();
#define I(ITEM, TRANSLATE) currentDisplayLangInfo.ITEM = 0;
#include "Localization/ru.inl"
#undef I
    }
    if (ImGui::MenuItem("de")) {
        reportWindowEnabled = true;
        currentDisplayLanguageName = LANG.TRANS_LANG_GERMAN;
        ResetCurrentDisplayLangInfo();
#define I(ITEM, TRANSLATE) currentDisplayLangInfo.ITEM = 0;
#include "Localization/de.inl"
#undef I
    }
    if (ImGui::MenuItem("es")) {
        reportWindowEnabled = true;
        currentDisplayLanguageName = LANG.TRANS_LANG_SPANISH;
        ResetCurrentDisplayLangInfo();
#define I(ITEM, TRANSLATE) currentDisplayLangInfo.ITEM = 0;
#include "Localization/es.inl"
#undef I
    }
    if (ImGui::MenuItem("fr")) {
        reportWindowEnabled = true;
        currentDisplayLanguageName = LANG.TRANS_LANG_FRENCH;
        ResetCurrentDisplayLangInfo();
#define I(ITEM, TRANSLATE) currentDisplayLangInfo.ITEM = 0;
#include "Localization/fr.inl"
#undef I
    }
}
void Lang::DrawReportWindow(bool isImGuiActive) {
    if (!isImGuiActive || !reportWindowEnabled)
        return;

    int total_count = 0;
    int translated_count = 0;

    if (WindowBeginEx(LANG.TRANS_REPORT_WIN_NAME, &reportWindowEnabled, 0)) {
        ImGui::Text(LANG.TRANS_LANGUAGE_HINT, currentDisplayLanguageName);
        ImGui::Text(LANG.TRANS_UNTRANS_ITEMS);
#define I(ITEM, TRANSLATE) if(currentDisplayLangInfo.ITEM){ ImGui::Text("\t%s", #ITEM); } else { translated_count++; } total_count++;
#include "Localization/en_us.inl"
#undef I
        ImGui::Text(LANG.TRANS_COUNT_HINT, total_count, translated_count);
    }
    ImGui::End();

}

HOOK_METHOD(Manager, LoadConfigs, ()->void)
{
    super();
    LANG.Load(this->GetStringTable()->language);

    //do some language related init
    console.InitAfterLanguageAvaliable();
    gameOptionsWindow.InitAfterLanguageAvaliable();
    helpMenu.InitAfterLanguageAvaliable();
    logViewer.InitAfterLanguageAvaliable();
    performanceWindow.InitAfterLanguageAvaliable();
}