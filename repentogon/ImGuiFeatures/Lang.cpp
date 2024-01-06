#include "lang.h"
#include "IconsFontAwesome6.h"
#include "IconsFontAwesome6_unicode.h"
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ConsoleMega.h"
#include "GameOptions.h"

Lang LANG;

Lang::Lang(){

#define I(ITEM,TRANSLATE) ITEM = TRANSLATE;
#include "Localization/en_us.inl"
#undef I

}

void Lang::Load(unsigned int language){
    switch(language){
        case 13:
#define I(ITEM,TRANSLATE) ITEM = TRANSLATE;
#include "Localization/zh_cn.inl"
#undef I
        break;
        default:
        //use english texts as default, do nothing
        break;
    }
}

HOOK_METHOD(Manager, LoadConfigs, ()->void)
{
    super();
    LANG.Load(this->GetStringTable()->language);

    //do some language related init
    console.InitAfterLanguageAvaliable();
    gameOptionsWindow.InitAfterLanguageAvaliable();
}