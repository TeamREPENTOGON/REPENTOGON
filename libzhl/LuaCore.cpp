#include <map>
#include <sstream>
#include <stdexcept>

#include <lua.hpp>

#include "LuaCore.h"
#include "HookSystem_private.h"
#include "SigScan.h"

namespace lua {
	std::map<Metatables, void*> _metatables;
	std::map<std::string, Metatables> _metatable_idx_from_name;
	bool _metatable_idx_from_name_initialized = false;

	class MetatableAlreadyRegisteredException : public std::exception {
	public:
		MetatableAlreadyRegisteredException(Metatables metatable) : _metatable(metatable) {
			snprintf(_err, 256, "Metatable %d already registed\n", (int)_metatable);
		}

		const char* what() const override {
			return _err;
		}

	private:
		Metatables _metatable;
		char _err[256];
	};

	class UnregistedMetatableException : public std::exception {
	public:
		UnregistedMetatableException(Metatables metatable) : _metatable(metatable) {
			snprintf(_err, 256, "Attempt to get unregistered metatable %d\n", (int)_metatable);
		}

		const char* what() const override {
			return _err;
		}

	private:
		Metatables _metatable;
		char _err[256];
	};

	class UnknownMetatableNameException : public std::exception {
	public:
		UnknownMetatableNameException(std::string const& name) {
			_err = new char[256 + name.size()];
		}

		~UnknownMetatableNameException() {
			delete[] _err;
		}

		const char* what() const override {
			return _err;
		}

	private:
		char* _err;
	};

	void UnloadMetatables() {
		_metatables.clear();
	}

	void PushMetatable(lua_State* L, Metatables metatable) {
		auto iter = _metatables.find(metatable);

		if (iter == _metatables.end()) {
			throw UnregistedMetatableException(metatable);
		}

		lua_rawgetp(L, LUA_REGISTRYINDEX, _metatables[metatable]);
	}

	void* GetMetatableKey(Metatables metatable) {
		return _metatables[metatable];
	}

	void RegisterMetatable(Metatables metatable, void* key) {
		if (_metatables.find(metatable) != _metatables.end()) {
			throw MetatableAlreadyRegisteredException(metatable);
		}

		_metatables[metatable] = key;
	}

	void RegisterNewClass(lua_State* L, const char* name, const char* metaname, luaL_Reg* functions, lua_CFunction gc) {
		luaL_newmetatable(L, metaname);
		lua_pushstring(L, "__index");
		lua_pushvalue(L, -2);
		lua_rawset(L, -3);

		lua_pushstring(L, "__type");
		lua_pushstring(L, name);
		lua_rawset(L, -3);

		lua_pushstring(L, "__class");
		lua_pushstring(L, name);
		lua_rawset(L, -3);

		if (gc) {
			lua_pushstring(L, "__gc");
			lua_pushcfunction(L, gc);
			lua_rawset(L, -3);
		}

		luaL_setfuncs(L, functions, 0);
		lua_pop(L, 1);
	}

	static void InitMetatableIdxFromName() {
		_metatable_idx_from_name["PillEffect"] = Metatables::PILL_EFFECT;
		_metatable_idx_from_name["Spawn"] = Metatables::SPAWN;
		_metatable_idx_from_name["EntityTear"] = Metatables::ENTITY_TEAR;
		_metatable_idx_from_name["HUD"] = Metatables::HUD;
		_metatable_idx_from_name["Seeds"] = Metatables::SEEDS;
		_metatable_idx_from_name["Room"] = Metatables::ROOM;
		_metatable_idx_from_name["ActiveItemDesc"] = Metatables::ACTIVE_ITEM_DESC;
		_metatable_idx_from_name["GridEntityDesc"] = Metatables::GRID_ENTITY_DESC;
		_metatable_idx_from_name["Entity"] = Metatables::ENTITY;
		_metatable_idx_from_name["EntityBomb"] = Metatables::ENTITY_BOMB;
		_metatable_idx_from_name["Config"] = Metatables::CONFIG;
		_metatable_idx_from_name["VectorList"] = Metatables::VECTOR_LIST;
		_metatable_idx_from_name["MusicManager"] = Metatables::MUSIC_MANAGER;
		_metatable_idx_from_name["EffectList"] = Metatables::EFFECT_LIST;
		_metatable_idx_from_name["ItemConfigList"] = Metatables::ITEM_CONFIG_LIST;
		_metatable_idx_from_name["EntityKnife"] = Metatables::ENTITY_KNIFE;
		_metatable_idx_from_name["Level"] = Metatables::LEVEL;
		_metatable_idx_from_name["PillConfigList"] = Metatables::PILL_CONFIG_LIST;
		_metatable_idx_from_name["RoomDescriptor"] = Metatables::ROOM_DESCRIPTOR;
		_metatable_idx_from_name["EntityRef"] = Metatables::ENTITY_REF;
		_metatable_idx_from_name["GridEntityPressurePlate"] = Metatables::GRID_ENTITY_PRESSURE_PLATE;
		_metatable_idx_from_name["TemporaryEffect"] = Metatables::TEMPORARY_EFFECT;
		_metatable_idx_from_name["EntityEffect"] = Metatables::ENTITY_EFFECT;
		_metatable_idx_from_name["RNG"] = Metatables::RNG;
		_metatable_idx_from_name["EntityPlayer"] = Metatables::ENTITY_PLAYER;
		_metatable_idx_from_name["Font"] = Metatables::FONT;
		_metatable_idx_from_name["EntityPickup"] = Metatables::ENTITY_PICKUP;
		_metatable_idx_from_name["Costume"] = Metatables::COSTUME;
		_metatable_idx_from_name["EntityList"] = Metatables::ENTITY_LIST;
		_metatable_idx_from_name["TemporaryEffects"] = Metatables::_TEMPORARY_EFFECTS;
		_metatable_idx_from_name["BitSet128"] = Metatables::BITSET_128;
		_metatable_idx_from_name["ArrayProxy_RoomDescriptor"] = Metatables::ARRAY_PROXY_ROOM_DESCRIPTOR;
		_metatable_idx_from_name["Game"] = Metatables::GAME;
		_metatable_idx_from_name["ProjectileParams"] = Metatables::PROJECTILE_PARAMS;
		_metatable_idx_from_name["GridEntityRock"] = Metatables::GRID_ENTITY_ROCK;
		_metatable_idx_from_name["EntityNPC"] = Metatables::ENTITY_NPC;
		_metatable_idx_from_name["EntityProjectile"] = Metatables::ENTITY_PROJECTILE;
		_metatable_idx_from_name["Color"] = Metatables::COLOR;
		_metatable_idx_from_name["GridEntityTNT"] = Metatables::GRID_ENTITY_TNT;
		_metatable_idx_from_name["EntityFamiliar"] = Metatables::ENTITY_FAMILIAR;
		_metatable_idx_from_name["KColor"] = Metatables::KCOLOR;
		_metatable_idx_from_name["CostumeConfigList"] = Metatables::COSTUME_CONFIG_LIST;
		_metatable_idx_from_name["RoomConfigEntries"] = Metatables::ROOM_CONFIG_ENTRIES;
		_metatable_idx_from_name["QueueItemData"] = Metatables::QUEUE_ITEM_DATA;
		_metatable_idx_from_name["intValues"] = Metatables::INT_VALUES;
		_metatable_idx_from_name["Entry"] = Metatables::ENTRY;
		_metatable_idx_from_name["ItemPool"] = Metatables::ITEM_POOL;
		_metatable_idx_from_name["Item"] = Metatables::ITEM;
		_metatable_idx_from_name["GridEntitySpikes"] = Metatables::GRID_ENTITY_SPIKES;
		_metatable_idx_from_name["GridEntity"] = Metatables::GRID_ENTITY;
		_metatable_idx_from_name["RoomConfigSpawns"] = Metatables::ROOM_CONFIG_SPAWNS;
		_metatable_idx_from_name["RoomConfig_Room"] = Metatables::ROOM_CONFIG_ROOM;
		_metatable_idx_from_name["EntityPtr"] = Metatables::ENTITY_PTR;
		_metatable_idx_from_name["PosVel"] = Metatables::POS_VEL;
		_metatable_idx_from_name["PathFinder"] = Metatables::PATHFINDER;
		_metatable_idx_from_name["TearParams"] = Metatables::TEAR_PARAMS;
		_metatable_idx_from_name["GridEntityPoop"] = Metatables::GRID_ENTITY_POOP;
		_metatable_idx_from_name["Card"] = Metatables::CARD;
		_metatable_idx_from_name["GridEntityPit"] = Metatables::GRID_ENTITY_PIT;
		_metatable_idx_from_name["Sprite"] = Metatables::SPRITE;
		_metatable_idx_from_name["GridEntityDoor"] = Metatables::GRID_ENTITY_DOOR;
		_metatable_idx_from_name["EntityLaser"] = Metatables::ENTITY_LASER;
		_metatable_idx_from_name["SFXManager"] = Metatables::SFX_MANAGER;
		_metatable_idx_from_name["CardConfigList"] = Metatables::CARD_CONFIG_LIST;
		_metatable_idx_from_name["Vector"] = Metatables::VECTOR;

		_metatable_idx_from_name["const PillEffect"] = Metatables::CONST_PILL_EFFECT;
		_metatable_idx_from_name["const Spawn"] = Metatables::CONST_SPAWN;
		_metatable_idx_from_name["const EntityTear"] = Metatables::CONST_ENTITY_TEAR;
		_metatable_idx_from_name["const HUD"] = Metatables::CONST_HUD;
		_metatable_idx_from_name["const Seeds"] = Metatables::CONST_SEEDS;
		_metatable_idx_from_name["const Room"] = Metatables::CONST_ROOM;
		_metatable_idx_from_name["const ActiveItemDesc"] = Metatables::CONST_ACTIVE_ITEM_DESC;
		_metatable_idx_from_name["const GridEntityDesc"] = Metatables::CONST_GRID_ENTITY_DESC;
		_metatable_idx_from_name["const Entity"] = Metatables::CONST_ENTITY;
		_metatable_idx_from_name["const EntityBomb"] = Metatables::CONST_ENTITY_BOMB;
		_metatable_idx_from_name["const Config"] = Metatables::CONST_CONFIG;
		_metatable_idx_from_name["const VectorList"] = Metatables::CONST_VECTOR_LIST;
		_metatable_idx_from_name["const MusicManager"] = Metatables::CONST_MUSIC_MANAGER;
		_metatable_idx_from_name["const EffectList"] = Metatables::CONST_EFFECT_LIST;
		_metatable_idx_from_name["const ItemConfigList"] = Metatables::CONST_ITEM_CONFIG_LIST;
		_metatable_idx_from_name["const EntityKnife"] = Metatables::CONST_ENTITY_KNIFE;
		_metatable_idx_from_name["const Level"] = Metatables::CONST_LEVEL;
		_metatable_idx_from_name["const PillConfigList"] = Metatables::CONST_PILL_CONFIG_LIST;
		_metatable_idx_from_name["const RoomDescriptor"] = Metatables::CONST_ROOM_DESCRIPTOR;
		_metatable_idx_from_name["const EntityRef"] = Metatables::CONST_ENTITY_REF;
		_metatable_idx_from_name["const GridEntityPressurePlate"] = Metatables::CONST_GRID_ENTITY_PRESSURE_PLATE;
		_metatable_idx_from_name["const TemporaryEffect"] = Metatables::CONST_TEMPORARY_EFFECT;
		_metatable_idx_from_name["const EntityEffect"] = Metatables::CONST_ENTITY_EFFECT;
		_metatable_idx_from_name["const RNG"] = Metatables::CONST_RNG;
		_metatable_idx_from_name["const EntityPlayer"] = Metatables::CONST_ENTITY_PLAYER;
		_metatable_idx_from_name["const Font"] = Metatables::CONST_FONT;
		_metatable_idx_from_name["const EntityPickup"] = Metatables::CONST_ENTITY_PICKUP;
		_metatable_idx_from_name["const Costume"] = Metatables::CONST_COSTUME;
		_metatable_idx_from_name["const EntityList"] = Metatables::CONST_ENTITY_LIST;
		_metatable_idx_from_name["const TemporaryEffects"] = Metatables::_CONST_TEMPORARY_EFFECTS;
		_metatable_idx_from_name["const BitSet128"] = Metatables::CONST_BITSET_128;
		_metatable_idx_from_name["const ArrayProxy_RoomDescriptor"] = Metatables::CONST_ARRAY_PROXY_ROOM_DESCRIPTOR;
		_metatable_idx_from_name["const Game"] = Metatables::CONST_GAME;
		_metatable_idx_from_name["const ProjectileParams"] = Metatables::CONST_PROJECTILE_PARAMS;
		_metatable_idx_from_name["const GridEntityRock"] = Metatables::CONST_GRID_ENTITY_ROCK;
		_metatable_idx_from_name["const EntityNPC"] = Metatables::CONST_ENTITY_NPC;
		_metatable_idx_from_name["const EntityProjectile"] = Metatables::CONST_ENTITY_PROJECTILE;
		_metatable_idx_from_name["const Color"] = Metatables::CONST_COLOR;
		_metatable_idx_from_name["const GridEntityTNT"] = Metatables::CONST_GRID_ENTITY_TNT;
		_metatable_idx_from_name["const EntityFamiliar"] = Metatables::CONST_ENTITY_FAMILIAR;
		_metatable_idx_from_name["const KColor"] = Metatables::CONST_KCOLOR;
		_metatable_idx_from_name["const CostumeConfigList"] = Metatables::CONST_COSTUME_CONFIG_LIST;
		_metatable_idx_from_name["const RoomConfigEntries"] = Metatables::CONST_ROOM_CONFIG_ENTRIES;
		_metatable_idx_from_name["const QueueItemData"] = Metatables::CONST_QUEUE_ITEM_DATA;
		_metatable_idx_from_name["const intValues"] = Metatables::CONST_INT_VALUES;
		_metatable_idx_from_name["const Entry"] = Metatables::CONST_ENTRY;
		_metatable_idx_from_name["const ItemPool"] = Metatables::CONST_ITEM_POOL;
		_metatable_idx_from_name["const Item"] = Metatables::CONST_ITEM;
		_metatable_idx_from_name["const GridEntitySpikes"] = Metatables::CONST_GRID_ENTITY_SPIKES;
		_metatable_idx_from_name["const GridEntity"] = Metatables::CONST_GRID_ENTITY;
		_metatable_idx_from_name["const RoomConfigSpawns"] = Metatables::CONST_ROOM_CONFIG_SPAWNS;
		_metatable_idx_from_name["const RoomConfig_Room"] = Metatables::CONST_ROOM_CONFIG_ROOM;
		_metatable_idx_from_name["const EntityPtr"] = Metatables::CONST_ENTITY_PTR;
		_metatable_idx_from_name["const PosVel"] = Metatables::CONST_POS_VEL;
		_metatable_idx_from_name["const PathFinder"] = Metatables::CONST_PATHFINDER;
		_metatable_idx_from_name["const TearParams"] = Metatables::CONST_TEAR_PARAMS;
		_metatable_idx_from_name["const GridEntityPoop"] = Metatables::CONST_GRID_ENTITY_POOP;
		_metatable_idx_from_name["const Card"] = Metatables::CONST_CARD;
		_metatable_idx_from_name["const GridEntityPit"] = Metatables::CONST_GRID_ENTITY_PIT;
		_metatable_idx_from_name["const Sprite"] = Metatables::CONST_SPRITE;
		_metatable_idx_from_name["const GridEntityDoor"] = Metatables::CONST_GRID_ENTITY_DOOR;
		_metatable_idx_from_name["const EntityLaser"] = Metatables::CONST_ENTITY_LASER;
		_metatable_idx_from_name["const SFXManager"] = Metatables::CONST_SFX_MANAGER;
		_metatable_idx_from_name["const CardConfigList"] = Metatables::CONST_CARD_CONFIG_LIST;
		_metatable_idx_from_name["const Vector"] = Metatables::CONST_VECTOR;
	}

	Metatables GetMetatableIdxFromName(std::string const& name) {
		if (!_metatable_idx_from_name_initialized) {
			InitMetatableIdxFromName();
			_metatable_idx_from_name_initialized = true;
		}

		auto iter = _metatable_idx_from_name.find(name);
		if (iter == _metatable_idx_from_name.end()) {
			throw UnknownMetatableNameException(name);
		}

		return _metatable_idx_from_name[name];
	}

	void* TestUserdata(lua_State* L, int ud, lua::Metatables mt) {
		// s = ... userdata ... 
		void* p = lua_touserdata(L, ud);
		if (p != NULL) {
			lua::PushMetatable(L, mt); // ... userdata ... meta
			if (lua_getmetatable(L, ud)) { // ... userdata ... meta meta
				while (true) {
					if (!lua_rawequal(L, -1, -2)) {
						// Check parent metatable
						lua_pushstring(L, "__parent"); // ... userdata ... meta meta __parent
						int type = lua_rawget(L, -2); // ... userdata ... meta meta ?parent
						if (type != LUA_TTABLE) {
							// Pop the metatable we compare against, the metatable of the userdata
							// and its attempted parent
							lua_pop(L, 3);
							return NULL;
						}

						lua_remove(L, -2); // ... userdata ... meta parentmeta
					}
					else {
						lua_pop(L, 2);
						return p;
					}
				}
				// Proof that the while finishes in normal working conditions :
				// The else case of the if is trivial. Proof that the if iteself finishes
				// The if can not finish only if lua_rawget(L, -2) never produces something that is not a table.
				// This means there is an infinite chain of __parent, i.e. a loop.
				// Then this means the application is bugged and we are no longer in normal work conditions.
				// Otherwise, the chain of __parent is bounded and the inner if will eventually be entered, therefore the while finishes.

				// Pop the metatable we compare against and the metatable of the userdata
				lua_pop(L, 2);
				return p;
			}
			else {
				// Pop the metatable we compare against
				lua_pop(L, 1); // ... userdata ...
				return NULL;
			}
		}
		return NULL;
	}

	void* CheckUserdata(lua_State* L, int ud, lua::Metatables mt, std::string const& name) {
		void* p = TestUserdata(L, ud, mt);
		if (!p) {
			lua_getmetatable(L, ud);
			lua::PushMetatable(L, mt);
			std::string type = lua_typename(L, lua_type(L, ud));
			std::string err = name + " expected, got " + type;
			luaL_argerror(L, ud, err.c_str());
		}
		return p;
	}

	void RegisterFunction(lua_State *L, lua::Metatables mt, const char* name, lua_CFunction func) {
		lua::PushMetatable(L, mt);
		lua_pushstring(L, name);
		lua_pushcfunction(L, func);
		lua_rawset(L, -3);
		lua_pop(L, 1);
	}

	namespace luabridge {
		UserdataPtr::UserdataPtr(void* const p) {
			m_p = p;
		}

		void UserdataPtr::push(lua_State* L, void* const p, void const* const key) {
			if (p) {
				new (lua_newuserdata(L, sizeof(UserdataPtr))) UserdataPtr(p);
				lua_rawgetp(L, LUA_REGISTRYINDEX, key);
				lua_setmetatable(L, -2);
			}
			else {
				lua_pushnil(L);
			}
		}

		void UserdataPtr::push(lua_State* L, void* const p, const char* meta) {
			if (p) {
				new (lua_newuserdata(L, sizeof(UserdataPtr))) UserdataPtr(p);
				luaL_setmetatable(L, meta);
			} 
			else {
				lua_pushnil(L);
			}
		}

		void* identityKey;
		static VariableDefinition identityKeyDef("luabridge::IdentityKey", "5357FF15????????68(????????)", &identityKey);

		lua_CFunction indexMetaMethod;

		namespace index {
			static const HookSystem::ArgData* argdata = nullptr;
			static FunctionDefinition indexMetaMethodDef("luabrige::Namespace::ClassBase::indexMetaMethod", typeid(lua_CFunction), 
				"558bec83ec0c53568b7508576a0156ff15????????6a0256ff15", argdata, 1, 0, (void**)&indexMetaMethod);
		}

		lua_CFunction newIndexMetaMethod;
		namespace newIndex {
			static const HookSystem::ArgData* argdata = nullptr;
			static FunctionDefinition newIndexMetaMethodDef("luabridge::Namespace::ClassBase::newIndexMetaMethod", typeid(lua_CFunction),
				"558bec53568b7508576a0156ff15????????8b1d", argdata, 1, 0, (void**)&newIndexMetaMethod);
		}
	}

	namespace callbacks {
		int ToInteger(lua_State* L, int stackPosition) {
			lua_pushinteger(L, stackPosition);
			lua_gettable(L, -2);
			int res = (int)luaL_checkinteger(L, -1);
			lua_pop(L, 1);
			return res;
		}

		double ToNumber(lua_State* L, int stackPosition) {
			lua_pushinteger(L, stackPosition);
			lua_gettable(L, -2);
			double res = luaL_checknumber(L, -1);
			lua_pop(L, 1);
			return res;
		}

		bool ToBoolean(lua_State* L, int stackPosition) {
			lua_pushinteger(L, stackPosition);
			lua_gettable(L, -2);
			bool res = lua_toboolean(L, -1); //TODO add typechecking
			lua_pop(L, 1);
			return res;
		}

		const char* ToString(lua_State* L, int stackPosition) {
			lua_pushinteger(L, stackPosition);
			lua_gettable(L, -2);
			const char* res = luaL_checkstring(L, -1);
			lua_pop(L, 1);
			return res;
		}
	}

	bool luaL_optboolean(lua_State* L, int idx, bool default) {
		if (lua_gettop(L) < idx) {
			return default;
		}
		else {
			return lua_toboolean(L, idx);
		}
	}

	LuaCaller::LuaCaller(lua_State* L) : _L(L) { }

	LuaCaller& LuaCaller::push(bool x) {
		lua_pushboolean(_L, x);
		++_n;
		return *this;
	}

	LuaCaller& LuaCaller::push(lua_CFunction f, int n) {
		if (n != 0) {
			lua_pushcclosure(_L, f, n);
		}
		else {
			lua_pushcfunction(_L, f);
		}

		++_n;
		return *this;
	}

	LuaCaller& LuaCaller::pushfstring(const char* fmt, ...) {
		va_list va;
		va_start(va, fmt);
		return push(fmt, va);
	}

	LuaCaller& LuaCaller::push(lua_global_tag_t) {
		lua_pushglobaltable(_L);
		++_n;
		return *this;
	}

	LuaCaller& LuaCaller::push(void* p) {
		lua_pushlightuserdata(_L, p);
		++_n;
		return *this;
	}

	LuaCaller& LuaCaller::push(const char* s, size_t len) {
		if (len == 0) {
			lua_pushstring(_L, s);
		}
		else {
			lua_pushlstring(_L, s, len);
		}

		++_n;
		return *this;
	}

	LuaCaller& LuaCaller::pushnil() {
		lua_pushnil(_L);
		++_n;
		return *this;
	}

	LuaCaller& LuaCaller::pushvalue(int idx) {
		lua_pushvalue(_L, idx);
		++_n;
		return *this;
	}

	LuaCaller& LuaCaller::push(const char* fmt, va_list va) {
		lua_pushvfstring(_L, fmt, va);
		++_n;
		return *this;
	}

	LuaCaller& LuaCaller::push(void* ptr, Metatables meta) {
		luabridge::UserdataPtr::push(_L, ptr, GetMetatableKey(meta));
		++_n;
		return *this;
	}

	LuaCaller& LuaCaller::pushLuabridge(void* ptr, const char* meta)
	{
		luabridge::UserdataPtr::push(_L, ptr, meta);
		++_n;
		return *this;
	}

	void* LuaCaller::pushUd(size_t s, const char* mt) {
		void* result = lua_newuserdata(_L, s);
		luaL_setmetatable(_L, mt);
		++_n;
		return result;
	}

	LuaResults LuaCaller::call(int nresults) {
		int n = lua_gettop(_L) - _n - 1; // Expected amount after poping everything (number of params + function)
		int result = lua_pcall(_L, _n, nresults, 0);
		int results = lua_gettop(_L) - n; // How many results

		return LuaResults(_L, results, result);
	}

	LuaStackProtector::LuaStackProtector(lua_State* L, int n) : _L(L), _orig(lua_gettop(L)), _n(n) {

	}

	LuaStackProtector::~LuaStackProtector() {
		if (_L) {
			int n = lua_gettop(_L);
			if (n != _orig + _n) {
				std::ostringstream err;
				err << "Inconsistent Lua stack. Expected " << _orig + _n << " elements, got " << n << std::endl;
				FILE* f = fopen("repentogon.log", "a");
				fprintf(f, err.str().c_str());
				fclose(f);
				abort();
			}
		}
	}

	LuaStackProtector::LuaStackProtector(LuaStackProtector&& other) : _L(other._L), _n(other._n) {
		other._L = nullptr;
	}

	LuaStackProtector& LuaStackProtector::operator=(LuaStackProtector&& other) {
		_L = other._L;
		other._L = nullptr;
		return *this;
	}

	LuaResults::LuaResults(lua_State* L, int n, int valid) : _L(L), _n(n), _valid(valid) {

	}

	LuaResults::LuaResults(LuaResults&& other) : _L(other._L), _n(other._n), _valid(other._valid) {
		other._L = nullptr;
	}

	LuaResults& LuaResults::operator=(LuaResults&& other) {
		_L = other._L;
		_n = other._n;
		_valid = other._valid;
		other._L = nullptr;
		return *this;
	}

	LuaResults::~LuaResults() {
		// For consistency with lua_pcall returning 0 when everything is okay
		if (_L && !_valid) {
			lua_pop(_L, _n);
		}
	}

	LuaResults::operator bool() const {
		return _valid;
	}

	namespace metatables
	{
		const char* AnimationStateMT = "AnimationState";
		const char* NullFrameMT = "NullFrame";
		const char* EntitySlotMT = "EntitySlot";
		const char* HistoryMT = "History";
		const char* PlayerHUDMT = "PlayerHUD";
		const char* WeaponMT = "Weapon";
		const char* LevelGeneratorMT = "LevelGenerator";
		const char* LevelGeneratorRoomMT = "LevelGeneratorMT";
		const char* MinimapMT = "Minimap";
		const char* NightmareSceneMT = "NightmareScene";
		const char* CapsuleMT = "Capsule";
	}
}