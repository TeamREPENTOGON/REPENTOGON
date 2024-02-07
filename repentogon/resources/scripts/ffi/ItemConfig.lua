ffi.cdef[[
ItemConfig_Item* L_ItemConfig_GetCollectible(const int);
ItemConfig_Item* L_ItemConfig_GetNullItem(const int);
ItemConfig_Item* L_ItemConfig_GetTrinket(const int);
ItemConfig_Card* L_ItemConfig_GetCard(const int);
ItemConfig_Pill* L_ItemConfig_GetPillEffect(const int);

int L_ItemConfig_GetNumCollectibles();
int L_ItemConfig_GetNumNullItems();
int L_ItemConfig_GetNumTrinkets();
int L_ItemConfig_GetNumCards();
int L_ItemConfig_GetNumPillEffects();

bool L_ItemConfig_IsValidCollectible(const int);
]]

local repentogon = ffidll
local lffi = ffi

-- ItemConfig is now a global table.
ItemConfig = {}

-- Isaac.GetItemConfig() backwards compatability.
Isaac.GetItemConfig = function()
	return ItemConfig
end

-- Cheekily allow both . and : to call ItemConfig functions
-- since we turned it into a global table.
local function ItemConfigGetFunc(func)
	return function(selfOrID, id)
		if selfOrID ~= ItemConfig then
			-- Called with `.`
			ffichecks.checknumber(1, selfOrID)
			id = selfOrID
		else
			-- Called with `:`
			ffichecks.checknumber(2, id)
		end
		return ffichecks.fixreturn(func(id))
	end
end

ItemConfig.GetCollectible = ItemConfigGetFunc(repentogon.L_ItemConfig_GetCollectible)
ItemConfig.GetNullItem = ItemConfigGetFunc(repentogon.L_ItemConfig_GetNullItem)
ItemConfig.GetTrinket = ItemConfigGetFunc(repentogon.L_ItemConfig_GetTrinket)
ItemConfig.GetCard = ItemConfigGetFunc(repentogon.L_ItemConfig_GetCard)
ItemConfig.GetPillEffect = ItemConfigGetFunc(repentogon.L_ItemConfig_GetPillEffect)

ItemConfig.GetNumCollectibles = repentogon.L_ItemConfig_GetNumCollectibles
ItemConfig.GetNumNullItems = repentogon.L_ItemConfig_GetNumNullItems
ItemConfig.GetNumTrinkets = repentogon.L_ItemConfig_GetNumTrinkets
ItemConfig.GetNumCards = repentogon.L_ItemConfig_GetNumCards
ItemConfig.GetNumPillEffects = repentogon.L_ItemConfig_GetNumPillEffects

ItemConfig.ShouldAddCostumeOnPickup = function(item)
	ffichecks.checkcdata(1, item, "ItemConfig_Item")
	return item.AddCostumeOnPickup
end

ItemConfig.IsValidCollectible = function(id)
	ffichecks.checknumber(1, id)
	return repentogon.L_ItemConfig_IsValidCollectible(id)
end

-- Helper to set up a lua metatable to mimic the behaviour of the vanilla API's
-- ItemConfig cppcontainer thingies like GetCollectibles()
local function MakeItemConfigList(typeName, sizeFunc, getFunc)
	-- The "Get" function in the vanilla API's cppcontainer-like lists were called with `:`
	local getFuncWrapper = function(self, idx)
		return getFunc(idx)
	end
	return setmetatable({}, {
		__len = function(self)  -- This doesn't work natively in lua 5.1 but can hopefully be fixed somehow
			return self.Size
		end,
		__tostring = function(self)
			return typeName
		end,
		__index = function(self, key)
			if key == "Size" then
				return sizeFunc()
			elseif key == "Get" then
				return getFuncWrapper
			elseif type(key) == "number" then
				return getFunc(key)
			end
		end,
		__newindex = function(self, key, value)
			error(typeName .. " cannot be modified")
		end,
	})
end

-- Set up all the list thingies like ItemConfig.GetCollectibles()
ItemConfig.CollectibleList = MakeItemConfigList("ItemConfigList", ItemConfig.GetNumCollectibles, ItemConfig.GetCollectible)
ItemConfig.GetCollectibles = function() return ItemConfig.CollectibleList end

ItemConfig.NullItemList = MakeItemConfigList("ItemConfigList", ItemConfig.GetNumNullItems, ItemConfig.GetNullItem)
ItemConfig.GetNullItems = function() return ItemConfig.NullItemList end

ItemConfig.TrinketList = MakeItemConfigList("ItemConfigList", ItemConfig.GetNumTrinkets, ItemConfig.GetTrinket)
ItemConfig.GetTrinkets = function() return ItemConfig.TrinketList end

ItemConfig.CardList = MakeItemConfigList("CardConfigList", ItemConfig.GetNumCards, ItemConfig.GetCard)
ItemConfig.GetCards = function() return ItemConfig.CardList end

ItemConfig.PillEffectList = MakeItemConfigList("PillConfigList", ItemConfig.GetNumPillEffects, ItemConfig.GetPillEffect)
ItemConfig.GetPillEffects = function() return ItemConfig.PillEffectList end

local function AddItemConfigEnums(enums)
	for k,v in pairs(enums) do
		ItemConfig[k] = v
	end
end

-- Stuff below copy-pasted from enums.lua

AddItemConfigEnums({
	-- ItemConfig.CHARGE_*
	CHARGE_NORMAL = 0,
	CHARGE_TIMED = 1,
	CHARGE_SPECIAL = 2,
	
	--[[ (Bitwise stuff doesn't work yet!)
	-- ItemConfig.TAG_*
	TAG_DEAD = 1,					-- Dead things (for the Parasite unlock)
	TAG_SYRINGE = 1<<1,				-- Syringes (for Little Baggy and the Spun! transformation)
	TAG_MOM = 1<<2,					-- Mom's things (for Mom's Contact and the Yes Mother? transformation)
	TAG_TECH = 1<<3,				-- Technology items (for the Technology Zero unlock)
	TAG_BATTERY = 1<<4,				-- Battery items (for the Jumper Cables unlock)
	TAG_GUPPY = 1<<5,				-- Guppy items (Guppy transformation)
	TAG_FLY = 1<<6,					-- Fly items (Beelzebub transformation)
	TAG_BOB = 1<<7,					-- Bob items (Bob transformation)
	TAG_MUSHROOM = 1<<8,			-- Mushroom items (Fun Guy transformation)
	TAG_BABY = 1<<9,				-- Baby items (Conjoined transformation)
	TAG_ANGEL = 1<<10,				-- Angel items (Seraphim transformation)
	TAG_DEVIL = 1<<11,				-- Devil items (Leviathan transformation)
	TAG_POOP = 1<<12,				-- Poop items (Oh Shit transformation)
	TAG_BOOK = 1<<13,				-- Book items (Book Worm transformat)
	TAG_SPIDER = 1<<14,				-- Spider items (Spider Baby transformation)
	TAG_QUEST = 1<<15,				-- Quest item (cannot be rerolled or randomly obtained)
	TAG_MONSTER_MANUAL = 1<<16,		-- Can be spawned by Monster Manual
	TAG_NO_GREED = 1<<17,			-- Cannot appear in Greed Mode
	TAG_FOOD = 1<<18,				-- Food item (for Binge Eater)
	TAG_TEARS_UP = 1<<19,			-- Tears up item (for Lachryphagy unlock detection)
	TAG_OFFENSIVE = 1<<20,			-- Whitelisted item for Lost B
	TAG_NO_KEEPER = 1<<21,			-- Blacklisted item for Keeper/Keeper B
	TAG_NO_LOST_BR = 1<<22,			-- Blacklisted item for Lost's Birthright
	TAG_STARS = 1<<23,				-- Star themed items (for the Planetarium unlock)
	TAG_SUMMONABLE = 1<<24,			-- Summonable items (for Bethany B)
	TAG_NO_CANTRIP = 1<<25,			-- Can't be obtained in Cantripped challenge
	TAG_WISP = 1<<26,				-- Active items that have wisps attached to them (automatically set)
	TAG_UNIQUE_FAMILIAR = 1<<27,	-- Unique familiars that cannot be duplicated
	TAG_NO_CHALLENGE = 1<<28,		-- Items that shouldn't be obtainable in challenges
	TAG_NO_DAILY = 1<<29,			-- Items that shouldn't be obtainable in daily runs
	TAG_LAZ_SHARED = 1<<30,			-- Items that should be shared between Tainted Lazarus' forms
	TAG_LAZ_SHARED_GLOBAL = 1<<31,	-- Items that should be shared between Tainted Lazarus' forms but only through global checks (such as PlayerManager::HasCollectible)
	TAG_NO_EDEN = 1<<32,			-- Items that can't be randomly rolled
	]]
	
	-- ItemConfig.CARDTYPE_*
	CARDTYPE_TAROT = 0,				-- Tarot cards
	CARDTYPE_SUIT = 1,				-- Standard playing cards (twos, aces and Joker, does not include Suicide King, Rules Card or Queen of Hearts)
	CARDTYPE_RUNE = 2,				-- Runes
	CARDTYPE_SPECIAL	= 3,		-- Special cards (anything that doesn't fall in the above categories excludes non-cards such as Dice Shard, see below)
	CARDTYPE_SPECIAL_OBJECT = 4,	-- Special pocket items that do not qualify as "cards"
	CARDTYPE_TAROT_REVERSE = 5, 	-- Reversed tarot cards
})
