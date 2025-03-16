#pragma once
#include "IsaacRepentance.h"
class SaveImportHelper {
public:
    enum FORCEIMPORT_ERRORCODE {
        SUCCESS,
        BAD_SLOT,
        BAD_MENU,
        STEAMCLOUD_UNSUPPORTED
    };

    FORCEIMPORT_ERRORCODE ForceImport(unsigned int slot_id);
    static bool IsForcedImportHijack;
};
extern SaveImportHelper saveimport;