#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../Patches/CardsExtras.h"

MOD_EXPORT bool L_ItemConfigCard_IsAvailable(ItemConfig_Card* card) {
	return card->IsAvailable();
}

MOD_EXPORT bool L_ItemConfigCard_GetHidden(ItemConfig_Card* card) {
	return CardsEX::GetCardConfigEX(card)->hidden;
}

MOD_EXPORT float L_ItemConfigCard_GetInitialWeight(ItemConfig_Card* card) {
	return CardsEX::GetCardConfigEX(card)->initialWeight;
}

MOD_EXPORT float L_ItemConfigCard_GetWeight(ItemConfig_Card* card) {
	return CardsEX::GetCardConfigEX(card)->weight;
}

MOD_EXPORT void L_ItemConfigCard_SetWeight(ItemConfig_Card* card, float weight) {
	weight = std::max(weight, 0.0f);
	ItemConfig_Card_EX* configEX = CardsEX::GetCardConfigEX(card);
	configEX->weight = weight;
	configEX->invalidateVanillaMethod = weight != 1.0f;
}

MOD_EXPORT void L_ItemConfigCard_SetAvailabilityCondition(ItemConfig_Card* card, bool (__cdecl* condition)()) {
	CardsEX::GetCardConfigEX(card)->availabilityCondition = condition;
}

MOD_EXPORT void L_ItemConfigCard_ReportAvailabilityError(ItemConfig_Card* card, const char* error) {
	CardsEX::ReportAvailabilityError(card, error);
}
