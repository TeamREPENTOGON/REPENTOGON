#pragma once
#include "IsaacRepentance.h"
class SaveImportHelper {
public:
    enum FORCEIMPORT_ERRORCODE {
        SUCCESS,
        BAD_SLOT,
        BAD_MENU
    };

    FORCEIMPORT_ERRORCODE ForceImport(unsigned int slot_id);
    static bool IsForcedImportHijack;
};