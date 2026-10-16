#include "IsaacRepentance.h"

extern "C" {
    MOD_EXPORT WeightedOutcomePicker* L_WeightedOutcomePicker_New() {
        return new WeightedOutcomePicker();
    }

    MOD_EXPORT void L_WeightedOutcomePicker_Delete(WeightedOutcomePicker* self) {
        ::operator delete(self);
    }

    MOD_EXPORT void L_WeightedOutcomePicker_AddOutcomeWeight(WeightedOutcomePicker* self, WeightedOutcomePicker_Outcome* outcome) {
        self->AddOutcomeWeight(*outcome, false);
    }

    MOD_EXPORT uint32_t L_WeightedOutcomePicker_PickOutcome(WeightedOutcomePicker* self, RNG* rng) {
        return self->PickOutcome(*rng);
    }

    MOD_EXPORT void L_WeightedOutcomePicker_RemoveOutcome(WeightedOutcomePicker* self, uint32_t value) {
        auto& outcomes = *self->GetOutcomes();
	outcomes.erase(std::remove_if(outcomes.begin(), outcomes.end(), [&](const auto& outcome) { return outcome._value == value; }), outcomes.end());
    }
}

