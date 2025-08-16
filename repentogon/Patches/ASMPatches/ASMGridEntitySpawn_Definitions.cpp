#include "ASMGridEntitySpawn_Definitions.h"
#include "IsaacRepentance.h"
#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

// default empty register, tells patcher to use offset as the variant
std::optional<ASMPatch::Registers> NO_VARIANT_REG;
std::optional<int> NO_COND_JMP;

static inline void* get_sig_address(const char* signature)
{
	SigScan scanner(signature);
	if (!scanner.Scan())
	{
		ZHL::Log("[ASSERT] [ASMPatch] Did not find \"%s\"\n", signature);
		assert(false);
	}
	return scanner.GetAddress();
}

// Make the class "static"
namespace {
	class GridEntitySpawnPatchBuilder 
	{
	private:
		std::string m_Signature;
		std::string m_Comment;
		int m_SigOffset = 0;
		GridEntityType m_Type = GridEntityType::GRID_DECORATION;
		std::optional<ASMPatch::Registers> m_VariantReg = NO_VARIANT_REG;
		int m_VariantOffset    = 0;
		ASMPatch::Registers m_GridIdxReg = ASMPatch::Registers::ESI;
		int m_GridIdxRegOffset = 0;
		ASMPatch::Registers m_SeedReg = ASMPatch::Registers::EAX;
		int m_SeedRegOffset = 0;
		std::optional<std::string> m_JumpSignature;
		int m_JumpOffset = 0;
		std::optional<int> m_JumpCondOffset = NO_COND_JMP;

	private:
		int get_jump_offset_from_signature(std::string patchSignature, std::string jumpSignature, int offset)
		{
			void* patchAddress = get_sig_address(m_Signature.c_str());
			void* jumpAddress = get_sig_address(jumpSignature.c_str());
			return (uintptr_t)jumpAddress - (uintptr_t)patchAddress + offset;
		}

	public:
		GridEntitySpawnPatchBuilder() = default;

		GridEntitySpawnPatchBuilder& SetSignature(const char* signature, int offset = 0)
		{
			assert(signature);
			m_Signature = signature ? signature : "";
			m_SigOffset = offset;
			return *this;
		}

		GridEntitySpawnPatchBuilder& SetSignature(const std::string& signature, int offset = 0)
		{
			assert(!signature.empty());
			m_Signature = signature;
			m_SigOffset = offset;
			return *this;
		}

		GridEntitySpawnPatchBuilder& SetComment(const char* comment)
		{
			assert(comment);
			m_Comment = comment ? comment : "";
			return *this;
		}

		GridEntitySpawnPatchBuilder& SetComment(const std::string& comment)
		{
			assert(!comment.empty());
			m_Comment = comment;
			return *this;
		}

		GridEntitySpawnPatchBuilder& SetType(GridEntityType type)
		{
			m_Type = type;
			return *this;
		}

		GridEntitySpawnPatchBuilder& SetVariantByReg(ASMPatch::Registers reg, int offset = 0)
		{
			m_VariantReg = reg;
			m_VariantOffset = offset;
			return *this;
		}

		GridEntitySpawnPatchBuilder& SetVariant(int variant)
		{
			m_VariantReg = NO_VARIANT_REG;
			m_VariantOffset = variant;
			return *this;
		}

		GridEntitySpawnPatchBuilder& SetGridIdxReg(ASMPatch::Registers reg, int offset = 0)
		{
			m_GridIdxReg = reg;
			m_GridIdxRegOffset = offset;
			return *this;
		}

		GridEntitySpawnPatchBuilder& SetSeedReg(ASMPatch::Registers reg, int offset = 0)
		{
			m_SeedReg = reg;
			m_SeedRegOffset = offset;
			return *this;
		}

		GridEntitySpawnPatchBuilder& SetRawJumpOffset(int offset)
		{
			m_JumpSignature = std::nullopt;
			m_JumpOffset = offset;
			return *this;
		}

		GridEntitySpawnPatchBuilder& SetJumpOffsetSignature(const char* signature, int offset = 0)
		{
			assert(signature);
			m_JumpSignature = signature ? signature : "";
			m_JumpOffset = offset;
			return *this;
		}

		GridEntitySpawnPatchBuilder& SetJumpOffsetSignature(const std::string& signature, int offset = 0)
		{
			assert(!signature.empty());
			m_JumpSignature = signature;
			m_JumpOffset = offset;
			return *this;
		}

		GridEntitySpawnPatchBuilder& SetJumpCondOffset(int condOff)
		{
			m_JumpCondOffset = condOff;
			return *this;
		}

		GridEntitySpawnPatchInfo Build()
		{
			assert(!m_Signature.empty() && "GridEntitySpawnPatchInfoBuilder: signature is required!");
			assert(!m_Comment.empty() && "GridEntitySpawnPatchInfoBuilder: comment is required!");

			GridEntitySpawnPatchInfo out;
			out.signature = m_Signature;
			out.comment = m_Comment;
			out.sigOffset = m_SigOffset;
			out.type = m_Type;
			out.variantReg = m_VariantReg;
			out.variantOffset = m_VariantOffset;
			out.idxReg = m_GridIdxReg;
			out.idxOffset = m_GridIdxRegOffset;
			out.seedReg = m_SeedReg;
			out.seedOffset = m_SeedRegOffset;
			if (m_JumpSignature)
			{
				out.jumpOffset = get_jump_offset_from_signature(m_Signature, m_JumpSignature.value(), m_JumpOffset);
			}
			else
			{
				out.jumpOffset = m_JumpOffset;
			}
			out.jumpCondOffset = m_JumpCondOffset;
			return out;
		}
	};
}

// reference for future maintainers:
// these patches mostly all start after the spawn seed is calculated, slightly before the grid idx is checked if between 0-447,
// and then end immediately after GridEntity::Init, after _spawnedFrame is filled (or where the ai function would jump to immediately after this call)
static inline std::array<GridEntitySpawnPatchInfo, 23> build_patches()
{
	GridEntitySpawnPatchBuilder PressurePlateGoldPoopReward;
	PressurePlateGoldPoopReward.SetSignature("8bf885ff78??81ffc00100007c??68????????6a03e8????????83c40881ffbf0100000f87????????81bc??????????840300000f8f????????689c010000e8????????8bf083c4048975??8bcec745??06000000")
		.SetComment("GridEntity_PressurePlate::Reward")
		.SetType(GRID_POOP)
		.SetVariant(3)
		.SetGridIdxReg(ASMPatch::Registers::EAX, 0)
		.SetSeedReg(ASMPatch::Registers::EBP, -0x18)
		.SetJumpOffsetSignature("e9????????83fe0b0f8d");

	GridEntitySpawnPatchBuilder PressurePlatePoopReward;
	PressurePlatePoopReward.SetSignature("8bf885ff78??81ffc00100007c??68????????6a03e8????????83c40881ffbf0100000f87????????81bc??????????840300000f8f????????689c010000e8????????8bf083c4048975??8bcec745??1c000000")
		.SetComment("GridEntity_PressurePlate::Reward")
		.SetType(GRID_POOP)
		.SetVariant(0)
		.SetGridIdxReg(ASMPatch::Registers::EAX, 0)
		.SetSeedReg(ASMPatch::Registers::EBP, -0x18)
		.SetJumpOffsetSignature("e9????????83fe0b0f8d");

	std::array<GridEntitySpawnPatchInfo, 23> patches = {{
		{"8b0d????????8985????????8b81????????8985????????85f678??81fec00100007c??68????????6a03e8????????8b85????????83c40881febf0100000f87????????81bc??????????840300000f8f????????689c010000e8????????8bf083c40489b5????????8bcec745??06000000e8????????c706????????c786????????00000000c786????????00000000c786????????0f000000c686????????00c786????????00000000c786????????00000000c786????????00000000c786????????00000000a1????????8b0d????????8986????????898e????????a1????????8b0d????????8986????????898e????????8b8d????????c745??ffffffff8b85????????c786????????00000000c786????????00000000c746??0e0000008946??8b54????85d274??8b028bca6a01ff108b85????????8b8d????????ffb5????????8974????8bcec746??01000000",
		"ai_mega_fatty (red champion)", 0, GRID_POOP, NO_VARIANT_REG, 1, ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EAX, 0, 0x166, NO_COND_JMP},

		{"8b0d????????8985????????8b81????????8985????????85f678??81fec00100007c??68????????6a03e8????????83c408",
		"ai_larryjr", 0, GRID_POOP, NO_VARIANT_REG, 0, ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EAX, 0, 0x186, NO_COND_JMP},

		{"8b0d????????8945??8b81????????8985",
		"ai_chub (carrion queen)", 0, GRID_POOP, NO_VARIANT_REG, 1, ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EAX, 0, 0x172, NO_COND_JMP},

		{"8b0d????????8985????????8b81????????8985????????85f678??81fec00100007c??68????????6a03e8????????8b85????????83c40881febf0100000f87????????81bc??????????840300000f8f????????689c010000e8????????8bf083c40489b5????????8bcec745??06000000e8????????c706????????c786????????00000000c786????????00000000c786????????0f000000c686????????00c786????????00000000c786????????00000000c786????????00000000c786????????00000000a1????????8b0d????????8986????????898e????????a1????????8b0d????????8986????????898e????????8b8d????????c745??ffffffff8b85????????c786????????00000000c786????????00000000c746??0e0000008946??8b54????85d274??8b028bca6a01ff108b85????????8b8d????????ffb5????????8974????8bcec746??00000000",
		"ai_dingle (normal)", 0, GRID_POOP, NO_VARIANT_REG, 0, ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EAX, 0, 0x190, NO_COND_JMP},

		{"8b0d????????8985????????8b81????????8985????????85f678??81fec00100007c??68????????6a03e8????????8b85????????83c40881febf0100000f87????????81bc??????????840300000f8f????????689c010000e8????????8bf083c40489b5????????8bcec745??1c000000",
		"ai_dingle (red champion)", 0, GRID_POOP, NO_VARIANT_REG, 1, ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EAX, 0, 0x107, NO_COND_JMP},

		// idx is being pulled from Entity_NPC._projectileDelay because it gets moved to a register AFTER we patch
		{"8b0d????????8bbb",
		"Entity_NPC_Gideon::SpawnLadder", 0, GRID_STAIRS, NO_VARIANT_REG, 1, ASMPatch::Registers::EBX, 0xba4, ASMPatch::Registers::EAX, 0, 0xa5, NO_COND_JMP},

		{"8b0d????????8945??8b81????????8945",
		"Entity_NPC_Raglich::UpdateAI_Arm", 0, GRID_PIT, NO_VARIANT_REG, 0, ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EAX, 0, 0xa5, NO_COND_JMP},

		// overwriting the move of the seed to ebx should be inconsequential
		// this fires AFTER the existing GridEntity is destroyed! custom handling will be needed to fix this
		{"8bd885f678",
		"Entity_NPC_Singe::DoExplosiveFart", 0, GRID_PIT, NO_VARIANT_REG, 0, ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EAX, 0, 0xcb, NO_COND_JMP},

		{"8945??85ff78??81ffc00100007c??68????????6a03e8????????83c40881ffbf01000077",
		"make_wall (crawlspace ladder)", 0, GRID_DECORATION, NO_VARIANT_REG, 10, ASMPatch::Registers::EDI, 0, ASMPatch::Registers::EAX, 0, 0x92, NO_COND_JMP},

		{"8945??85ff78??81ffc00100007c??68????????6a03e8????????83c40885ff",
		"make_wall (crawlspace gravity)", 0, GRID_GRAVITY, NO_VARIANT_REG, 0, ASMPatch::Registers::EDI, 0, ASMPatch::Registers::EAX, 0, 0x9a, NO_COND_JMP},

		{"8b80????????8945??85ff",
		"update_card_against_humanity", 0, GRID_POOP, ASMPatch::Registers::EBP, -0x14, ASMPatch::Registers::EDI, 0, ASMPatch::Registers::EDX, 0, 0x152, NO_COND_JMP},

		// we jmp to a place that sets al to 0 if SpawnGridEntity fails
		{"8b75??85f678??81fec0010000",
		"TrySpawnSanguineBondSpikes", 0, GRID_SPIKES, NO_VARIANT_REG, 100, ASMPatch::Registers::EBP, -0x14, ASMPatch::Registers::EBX, 0, 0x98, -0x16e},

		{"8b4d??8945??85c978",
		"BombTearFlagEffects #1", 0, GRID_PIT, NO_VARIANT_REG, 0, ASMPatch::Registers::EBP, -0x4c, ASMPatch::Registers::EAX, 0, 0xb3, NO_COND_JMP},

		{"8985????????85f678??81fec00100007c??68????????6a03e8????????83c40885f60f88????????81fec0010000",
		"BombTearFlagEffects #2 (giga bomb)", 0, GRID_PIT, NO_VARIANT_REG, 0, ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EAX, 0, 0xb6, NO_COND_JMP},

		// seems like this inline jumps partway through to a second inline, hence the big jmp
		{"81febf0100000f87????????50",
		"BombDamage (isaac's room staircase)", 0, GRID_STAIRS, NO_VARIANT_REG, 0, ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EBP, -0x20, 0x19a, NO_COND_JMP},

		{"81febf01000077??50",
		"BombDamage (barren room trapdoor)", 0, GRID_TRAPDOOR, NO_VARIANT_REG, 0, ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EBP, -0x20, 0x85, NO_COND_JMP},

		{"8b0d????????8b7e??8945",
		"TrySpawnLadder", 0, GRID_STAIRS, NO_VARIANT_REG, 0, ASMPatch::Registers::ESI, 0x24, ASMPatch::Registers::EAX, 0, 0xa2, NO_COND_JMP},

		{"8985????????8b85????????85c078",
		"Room::Init #1 (random decorations)", 0, GRID_DECORATION, NO_VARIANT_REG, 0, ASMPatch::Registers::EBP, -0x518, ASMPatch::Registers::EAX, 0, 0xd2, NO_COND_JMP},

		{"8985????????85f678??81fec00100007c??68????????6a03e8????????83c4088b85",
		"Room::Init #2 (crawlspace gravity)", 0, GRID_GRAVITY, NO_VARIANT_REG, 0, ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EAX, 0, 0xa9, NO_COND_JMP},

		{"85d278??81fac00100007c",
		"Room::Init #3 (random greed decorations)", 0, GRID_DECORATION, NO_VARIANT_REG, 10, ASMPatch::Registers::EDX, 0, ASMPatch::Registers::ESI, 0, 0xc0, NO_COND_JMP},

		{"8945??85f678??81fec0010000",
		"TrySpawnSecretShop", 0, GRID_STAIRS, NO_VARIANT_REG, 2, ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EAX, 0, 0x93, NO_COND_JMP},

		PressurePlateGoldPoopReward.Build(),
		PressurePlatePoopReward.Build(),
	}};

	return patches;
}

const std::array<GridEntitySpawnPatchInfo, 23>& GetGridEntitySpawnPatches()
{
    static std::array<GridEntitySpawnPatchInfo, 23> patches = build_patches();
    return patches;
}