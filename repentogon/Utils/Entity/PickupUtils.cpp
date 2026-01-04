#include "PickupUtils.h"
#include "../../SaveStateManagement/EntitySaveStateManagement.h"

namespace ESSM = EntitySaveStateManagement;

void PickupUtils::InitFlipState(Entity_Pickup& pickup, CollectibleType collectType, bool setupCollectibleGraphics)
{
    if (!(pickup._variant == PICKUP_COLLECTIBLE && pickup.CanReroll() && !pickup._dead))
    {
        return;
    }

    EntitySaveState* emptySaveState = new EntitySaveState();
    ESSM::RegisterSaveState(*emptySaveState);

    pickup._flipSaveState.SetP(emptySaveState);
    EntitySaveState* flipState = pickup._flipSaveState.saveState;

    flipState->type = pickup._type, flipState->variant = pickup._variant;

    RNG rng = RNG();
    rng.SetSeed(pickup._initSeed, 39);
    unsigned int seed = rng.Next();

    flipState->_initSeed = seed;

    int collectibleID = (collectType != COLLECTIBLE_NULL) ? collectType :  g_Game->_itemPool.GetSeededCollectible(flipState->_initSeed, true, g_Game->_room->_descriptor); //to-do: add valid itemconfig check

    flipState->subtype = collectibleID;

    pickup._altPedestalANM2.Reset();
    if (setupCollectibleGraphics)
    {
        ANM2 copySprite = ANM2();
        copySprite.construct_from_copy(&pickup._sprite);

        Isaac::SwapANM2(&pickup._altPedestalANM2, &copySprite);

        Entity_Pickup::SetupCollectibleGraphics(&pickup._altPedestalANM2, 1, (CollectibleType)flipState->subtype, flipState->_initSeed, false);

        pickup._altPedestalANM2.LoadGraphics(true);

        pickup._altPedestalANM2.Play(pickup._sprite.GetAnimationData(0)->GetName().c_str(), true);
        pickup._altPedestalANM2.Update();
    }
}