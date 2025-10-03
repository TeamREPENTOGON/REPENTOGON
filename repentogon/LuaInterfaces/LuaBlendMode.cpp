#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

constexpr int FUNC_ADD = (int)eBlendEquation::FUNC_ADD;

constexpr int BLEND_ZERO = (int)eBlendFactor::ZERO;
constexpr int BLEND_ONE = (int)eBlendFactor::ONE;
constexpr int SRC_COLOR = (int)eBlendFactor::SRC_COLOR;
constexpr int DEST_COLOR = (int)eBlendFactor::DEST_COLOR;
constexpr int ONE_MINUS_SRC_ALPHA = (int)eBlendFactor::ONE_MINUS_SRC_ALPHA;

static void check_valid_blend_factor(lua_State* L, int idx, int blendFactor) {
	if (blendFactor < 0 || blendFactor >= (int)eBlendFactor::NUM_BLEND_FACTORS) {
		luaL_argerror(L, idx, "Invalid blend factor");
	}
}

static void check_valid_blend_equation(lua_State* L, int idx, int equation) {
	if (equation < 0 || equation >= (int)eBlendEquation::NUM_BLEND_EQUATIONS) {
		luaL_argerror(L, idx, "Invalid equation");
	}
}

//1
LUA_FUNCTION(Lua_GetRGBSourceFactor)
{
	BlendMode* mode = *lua::GetRawUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	lua_pushinteger(L, mode->_rgbSourceFactor);

	return 1;
}

LUA_FUNCTION(Lua_SetRGBSourceFactor)
{
	BlendMode* mode = *lua::GetRawUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	unsigned int blendFactor = (unsigned int)luaL_checkinteger(L, 2);
	check_valid_blend_factor(L, 2, blendFactor);
	mode->_rgbSourceFactor = blendFactor;
	return 0;
}

//2
LUA_FUNCTION(Lua_GetRGBDestinationFactor)
{
	BlendMode* mode = *lua::GetRawUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	lua_pushinteger(L, mode->_rgbDestFactor);

	return 1;
}

LUA_FUNCTION(Lua_SetRGBDestinationFactor)
{
	BlendMode* mode = *lua::GetRawUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	unsigned int blendFactor = (unsigned int)luaL_checkinteger(L, 2);
	check_valid_blend_factor(L, 2, blendFactor);
	mode->_rgbDestFactor = blendFactor;
	return 0;
}

//3
LUA_FUNCTION(Lua_GetAlphaSourceFactor)
{
	BlendMode* mode = *lua::GetRawUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	lua_pushinteger(L, mode->_alphaSourceFactor);

	return 1;
}

LUA_FUNCTION(Lua_SetAlphaSourceFactor)
{
	BlendMode* mode = *lua::GetRawUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	unsigned int blendFactor = (unsigned int)luaL_checkinteger(L, 2);
	check_valid_blend_factor(L, 2, blendFactor);
	mode->_alphaSourceFactor = blendFactor;
	return 0;
}

//4
LUA_FUNCTION(Lua_GetAlphaDestinationFactor)
{
	BlendMode* mode = *lua::GetRawUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	lua_pushinteger(L, mode->_alphaDestFactor);

	return 1;
}

LUA_FUNCTION(Lua_SetAlphaDestinationFactor)
{
	BlendMode* mode = *lua::GetRawUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	unsigned int blendFactor = (unsigned int)luaL_checkinteger(L, 2);
	check_valid_blend_factor(L, 2, blendFactor);
	mode->_alphaDestFactor = blendFactor;
	return 0;
}

LUA_FUNCTION(Lua_GetBlendEquation)
{
	BlendMode* mode = *lua::GetRawUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	lua_pushinteger(L, mode->_equation);

	return 1;
}

LUA_FUNCTION(Lua_SetBlendEquation)
{
	BlendMode* mode = *lua::GetRawUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	unsigned int equation = (unsigned int)luaL_checkinteger(L, 2);
	check_valid_blend_equation(L, 2, equation);
	mode->_equation = equation;
	return 0;
}

const BlendMode modes[5] = {
	BlendMode(FUNC_ADD, BLEND_ONE, BLEND_ZERO, BLEND_ONE, BLEND_ZERO), // CONSTANT / REPLACE
	BlendMode(FUNC_ADD, BLEND_ONE, ONE_MINUS_SRC_ALPHA, BLEND_ONE, ONE_MINUS_SRC_ALPHA), // NORMAL (Pre multiplied alpha)
	BlendMode(FUNC_ADD, BLEND_ONE, BLEND_ONE, BLEND_ONE, BLEND_ONE), // ADDITIVE (Pre multiplied alpha)
	BlendMode(FUNC_ADD, BLEND_ZERO, SRC_COLOR, BLEND_ZERO, SRC_COLOR), // MULTIPLICATIVE
	BlendMode(FUNC_ADD, DEST_COLOR, ONE_MINUS_SRC_ALPHA, DEST_COLOR, ONE_MINUS_SRC_ALPHA) // OVERLAY
};

LUA_FUNCTION(Lua_SetMode)
{
	BlendMode* mode = *lua::GetRawUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	if (lua_gettop(L) > 2) {
		int rgbSrc = (int)luaL_optnumber(L, 2, mode->_rgbSourceFactor);
		check_valid_blend_factor(L, 2, rgbSrc);

		int rgbDst = (int)luaL_optnumber(L, 3, mode->_rgbDestFactor);
		check_valid_blend_factor(L, 3, rgbDst);

		int alphaSrc = (int)luaL_optnumber(L, 4, mode->_alphaSourceFactor);
		check_valid_blend_factor(L, 4, alphaSrc);

		int alphaDst = (int)luaL_optnumber(L, 5, mode->_alphaDestFactor);
		check_valid_blend_factor(L, 5, alphaDst);

		int equation = (int)luaL_optnumber(L, 6, mode->_equation);
		check_valid_blend_equation(L, 6, equation);

		mode->_rgbSourceFactor = rgbSrc;
		mode->_rgbDestFactor = rgbDst;
		mode->_alphaSourceFactor = alphaSrc;
		mode->_alphaDestFactor = alphaDst;
		mode->_equation = equation;
	}
	else
	{
		int newMode = (int)luaL_checknumber(L, 2);

		//invalid modes are simply ignored
		if (newMode >= 0 && newMode <= 5) {
			*mode = modes[newMode];
		}
	}

	return 0;
}

static void RegisterBlendMode(lua_State* L) {

	// manual creation of BlendMode class to ensure __newindex definition and variable additions work correctly
	luaL_newmetatable(L, lua::metatables::BlendModeMT);
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, lua::luabridge::indexMetaMethod);
	lua_rawset(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, lua::luabridge::newIndexMetaMethod);
	lua_rawset(L, -3);

	lua_pushstring(L, "__name");
	lua_pushstring(L, "BlendMode");
	lua_rawset(L, -3);

	lua_pushstring(L, "__propget");
	lua_newtable(L);

	lua_pushstring(L, "RGBSourceFactor");
	lua_pushcfunction(L, Lua_GetRGBSourceFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "RGBDestinationFactor");
	lua_pushcfunction(L, Lua_GetRGBDestinationFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "AlphaSourceFactor");
	lua_pushcfunction(L, Lua_GetAlphaSourceFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "AlphaDestinationFactor");
	lua_pushcfunction(L, Lua_GetAlphaDestinationFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "Equation");
	lua_pushcfunction(L, Lua_GetBlendEquation);
	lua_rawset(L, -3);

	// depreciated
	lua_pushstring(L, "Flag1");
	lua_pushcfunction(L, Lua_GetRGBSourceFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "Flag2");
	lua_pushcfunction(L, Lua_GetRGBDestinationFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "Flag3");
	lua_pushcfunction(L, Lua_GetAlphaSourceFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "Flag4");
	lua_pushcfunction(L, Lua_GetAlphaDestinationFactor);
	lua_rawset(L, -3);

	lua_rawset(L, -3);

	// and repeat!
	lua_pushstring(L, "__propset");
	lua_newtable(L);

	lua_pushstring(L, "RGBSourceFactor");
	lua_pushcfunction(L, Lua_SetRGBSourceFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "RGBDestinationFactor");
	lua_pushcfunction(L, Lua_SetRGBDestinationFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "AlphaSourceFactor");
	lua_pushcfunction(L, Lua_SetAlphaSourceFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "AlphaDestinationFactor");
	lua_pushcfunction(L, Lua_SetAlphaDestinationFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "Equation");
	lua_pushcfunction(L, Lua_SetBlendEquation);
	lua_rawset(L, -3);

	// depreciated
	lua_pushstring(L, "Flag1");
	lua_pushcfunction(L, Lua_SetRGBSourceFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "Flag2");
	lua_pushcfunction(L, Lua_SetRGBDestinationFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "Flag3");
	lua_pushcfunction(L, Lua_SetAlphaSourceFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "Flag4");
	lua_pushcfunction(L, Lua_SetAlphaDestinationFactor);
	lua_rawset(L, -3);

	lua_rawset(L, -3);

	luaL_Reg functions[] = {
	{ "SetMode", Lua_SetMode },
	{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);

	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterBlendMode(_state);
}