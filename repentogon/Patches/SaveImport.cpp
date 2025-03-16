#include "SaveImport.h"
#include "HookSystem.h"
bool SaveImportHelper::IsForcedImportHijack = false;
SaveImportHelper::FORCEIMPORT_ERRORCODE SaveImportHelper::ForceImport(unsigned int slot_id) {
    if (g_Manager->GetOptions()->_enableSteamCloud) {
        return FORCEIMPORT_ERRORCODE::STEAMCLOUD_UNSUPPORTED;
    };
    if (Isaac::IsInGame() || (g_MenuManager==nullptr) || (g_MenuManager->_selectedMenuID != 2)) {
        return FORCEIMPORT_ERRORCODE::BAD_MENU;
    };
    if (slot_id < 1 || slot_id>3) {
        return FORCEIMPORT_ERRORCODE::BAD_SLOT;
    };
    g_Manager->SetSaveSlot(0);
    IsForcedImportHijack = true;
    g_Manager->SetSaveSlot(slot_id);
    g_MenuManager->GetMenuCharacter()->Reset();
    return FORCEIMPORT_ERRORCODE::SUCCESS;
};

//HOOK_METHOD(PersistentGameData, Load, (char const* path)->void) {
//    super(path);
//};

HOOK_METHOD(KAGE_Filesys_SaveManager, OpenSaveFileForReading, (int param_1)->void*) {
    if (SaveImportHelper::IsForcedImportHijack) {
        SaveImportHelper::IsForcedImportHijack = false;
        return 0x0;
    };
    return super(param_1);
};