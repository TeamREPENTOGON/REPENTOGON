#include <variant>

#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "LuaRender.h"

using LuaRender::LuaImage;
using LuaRender::LuaTransformer;
using LuaRender::Transformation;

// ============================================================================
// Renderer

LUA_FUNCTION(lua_Renderer_LoadImage) {
	const char* path = luaL_checkstring(L, 1);
	ReferenceCounter_ImageBase image;
	Manager::LoadImage(&image, path, __ptr_g_VertexAttributeDescriptor_Position, false);

	if (!image.image) {
		image.DecrRef();
		return luaL_error(L, "Image %s does not exist", path);
	}

	LuaImage* ud = new (lua_newuserdata(L, sizeof(LuaImage))) LuaImage;
	memset(ud, 0, sizeof(LuaImage));
	luaL_setmetatable(L, LuaRender::ImageMT);
	ud->image = image;
	image.DecrRef();
	return 1;
}

LUA_FUNCTION(lua_Renderer_StartTransformation) {
	LuaImage* image = LuaRender::GetLuaImage(L);
	LuaTransformer* transformer = new (lua_newuserdata(L, sizeof(LuaTransformer))) LuaTransformer;
	transformer->_valid = true;
	luaL_setmetatable(L, LuaRender::TransformerMT);
	transformer->_output = image->image;
	return 1;
}

// ============================================================================
// Image

LuaImage* LuaRender::GetLuaImage(lua_State* L, int idx) {
	return lua::GetUserdata<LuaImage*>(L, idx, LuaRender::ImageMT);
}

LUA_FUNCTION(lua_Image_gc) {
	LuaImage* image = LuaRender::GetLuaImage(L);
	image->image.DecrRef();
	return 0;
}

static void RegisterImageClass(lua_State* L) {
	luaL_Reg functions[] = {
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, "Image", "Image", functions, lua_Image_gc);
}

// ============================================================================
// Transformer

LuaTransformer* LuaRender::GetTransformer(lua_State* L, int idx) {
	return lua::GetUserdata<LuaTransformer*>(L, idx, LuaRender::TransformerMT);
}

LUA_FUNCTION(lua_Transformer_gc) {
	LuaTransformer* transformer = LuaRender::GetTransformer(L);
	for (Transformation& transformation : transformer->_transformations) {
		transformation._input.DecrRef();
	}

	transformer->_output.DecrRef();
}

LUA_FUNCTION(lua_Transformer_Render) {
	LuaTransformer* transformer = LuaRender::GetTransformer(L);
	if (!transformer->_valid) {
		return luaL_error(L, "No operations allowed after a transformer has been applied");
	}
	LuaImage* image = LuaRender::GetLuaImage(L, 2);
	SourceQuad* source = LuaRender::GetSourceQuad(L, 3);
	DestinationQuad* dest = LuaRender::GetDestQuad(L, 4);
	KColor mod = *lua::GetUserdata<KColor*>(L, 5, lua::Metatables::KCOLOR, "KColor");

	Transformation trans;
	trans._input = image->image;
	trans._source = *source;
	trans._dest = *dest;
	trans._color1 = trans._color2 = trans._color3 = trans._color4 = mod;

	transformer->_transformations.push_back(trans);
	return 0;
}

LUA_FUNCTION(lua_Transformer_RenderEx) {
	LuaTransformer* transformer = LuaRender::GetTransformer(L);
	if (!transformer->_valid) {
		return luaL_error(L, "No operations allowed after a transformed has been applied");
	}
	LuaImage* image = LuaRender::GetLuaImage(L, 2);
	SourceQuad* source = LuaRender::GetSourceQuad(L, 3);
	DestinationQuad* dest = LuaRender::GetDestQuad(L, 4);
	KColor mod = *lua::GetUserdata<KColor*>(L, 5, lua::Metatables::KCOLOR, "KColor");
	KColor mod2 = *lua::GetUserdata<KColor*>(L, 5, lua::Metatables::KCOLOR, "KColor");
	KColor mod3 = *lua::GetUserdata<KColor*>(L, 5, lua::Metatables::KCOLOR, "KColor");
	KColor mod4 = *lua::GetUserdata<KColor*>(L, 5, lua::Metatables::KCOLOR, "KColor");

	Transformation trans;
	trans._input = image->image;
	trans._source = *source;
	trans._dest = *dest;
	trans._color1 = mod;
	trans._color2 = mod2;
	trans._color3 = mod3;
	trans._color4 = mod4;

	transformer->_transformations.push_back(trans);

	return 0;
}

LUA_FUNCTION(lua_Transformer_Apply) {
	LuaTransformer* transformer = LuaRender::GetTransformer(L);
	if (!transformer->_valid) {
		return luaL_error(L, "No operations allowed after a transformer had been applied");
	}
	Rendering::PushCurrentRenderTarget();
	__ptr_g_KAGE_Graphics_Manager->SetCurrentRenderTarget(transformer->_output.image, false);
	// __ptr_g_KAGE_Graphics_Manager->Clear();
	for (Transformation& transformation : transformer->_transformations) {
		KAGE_Graphics_ImageBase* image = transformation._input.image;
		image->Render(transformation._source, transformation._dest, transformation._color1, transformation._color2, transformation._color3, transformation._color4);
	}
	__ptr_g_KAGE_Graphics_Manager->Present();
	Rendering::RestorePreviousRenderTarget();
	transformer->_valid = false;
	return 0;
}

LUA_FUNCTION(lua_Transformer_IsValid) {
	LuaTransformer* transformer = LuaRender::GetTransformer(L);
	lua_pushboolean(L, transformer->_valid);
	return 1;
}

static void RegisterTransformerClass(lua_State* L) {
	luaL_Reg functions[] = {
		{ "Render", lua_Transformer_Render },
		{ "RenderEx", lua_Transformer_RenderEx },
		{ "Apply", lua_Transformer_Apply },
		{ "IsValid", lua_Transformer_IsValid },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, "Transformer", "Transformer", functions);
}

// ============================================================================
// Quads

SourceQuad* LuaRender::GetSourceQuad(lua_State* L, int idx) {
	return (SourceQuad*)luaL_checkudata(L, idx, LuaRender::SourceQuadMT);
}

DestinationQuad* LuaRender::GetDestQuad(lua_State* L, int idx) {
	return (DestinationQuad*)luaL_checkudata(L, idx, LuaRender::DestinationQuadMT);
}

typedef std::variant<SourceQuad*, DestinationQuad*> QuadVar;

static QuadVar GetQuad(lua_State* L, int idx = 1) {
	if (SourceQuad* source = (SourceQuad*)luaL_testudata(L, 1, LuaRender::SourceQuadMT)) {
		return source;
	}
	else if (DestinationQuad* destination = (DestinationQuad*)luaL_checkudata(L, 1, LuaRender::DestinationQuadMT)) {
		return destination;
	}
	else {
		luaL_error(L, "Expected either SourceQuad or DestinationQuad");
		return (SourceQuad*)nullptr;
	}
}

static bool IsSource(QuadVar const& quad) {
	return std::holds_alternative<SourceQuad*>(quad);
}

static SourceQuad* AsSource(QuadVar const& quad) {
	return std::get<SourceQuad*>(quad);
}

static DestinationQuad* AsDest(QuadVar const& quad) {
	return std::get<DestinationQuad*>(quad);
}

#define QUAD_GET(slot, var) static Vector* QuadGet##slot##(QuadVar const& quad) {\
	if (IsSource(quad)) {\
		return &AsSource(quad)->##var##; \
	}\
	else {\
		return &AsDest(quad)->##var##;\
	}\
}

QUAD_GET(TopLeft, _topLeft)
QUAD_GET(TopRight, _topRight)
QUAD_GET(BottomLeft, _bottomLeft)
QUAD_GET(BottomRight, _bottomRight)

static void PushQuadComponent(lua_State* L, Vector* (*fn)(QuadVar const&)) {
	Vector* component = fn(GetQuad(L));
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), *component);
}

LUA_FUNCTION(lua_Quad_GetTopLeft) {
	PushQuadComponent(L, QuadGetTopLeft);
	return 1;
}

LUA_FUNCTION(lua_Quad_GetTopRight) {
	PushQuadComponent(L, QuadGetTopRight);
	return 1;
}

LUA_FUNCTION(lua_Quad_GetBottomLeft) {
	PushQuadComponent(L, QuadGetBottomLeft);
	return 1;
}

LUA_FUNCTION(lua_Quad_GetBottomRight) {
	PushQuadComponent(L, QuadGetBottomRight);
	return 1;
}

static void SetQuadComponent(lua_State* L, Vector* (*fn)(QuadVar const&)) {
	Vector* component = fn(GetQuad(L));
	*component = lua::GetUserdata<Vector>(L, 2, lua::Metatables::VECTOR, "Vector");
}

LUA_FUNCTION(lua_Quad_SetTopLeft) {
	SetQuadComponent(L, QuadGetTopLeft);
	return 0;
}

LUA_FUNCTION(lua_Quad_SetTopRight) {
	SetQuadComponent(L, QuadGetTopRight);
	return 0;
}

LUA_FUNCTION(lua_Quad_SetBottomLeft) {
	SetQuadComponent(L, QuadGetBottomLeft);
	return 0;
}

LUA_FUNCTION(lua_Quad_SetBottomRight) {
	SetQuadComponent(L, QuadGetBottomRight);
	return 0;
}

static void RegisterQuadClasses(lua_State* L) {
	luaL_Reg quadFunctions[] = {
		{ "GetTopLeft", lua_Quad_GetTopLeft },
		{ "GetTopRight", lua_Quad_GetTopRight },
		{ "GetBottomLeft", lua_Quad_GetBottomLeft },
		{ "GetBottomRight", lua_Quad_GetBottomRight },
		{ "SetTopLeft", lua_Quad_SetTopLeft },
		{ "SetTopLeft", lua_Quad_SetTopRight },
		{ "SetBottomLeft", lua_Quad_SetBottomLeft },
		{ "SetBottomRight", lua_Quad_SetBottomRight },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, LuaRender::QuadMT, LuaRender::QuadMT, quadFunctions);
	
	// Missing convert_to_coordinate_space
	luaL_Reg sourceQuadFunctions[] = {
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, LuaRender::SourceQuadMT, LuaRender::SourceQuadMT, sourceQuadFunctions);
	luaL_getmetatable(L, LuaRender::SourceQuadMT);
	luaL_setmetatable(L, LuaRender::QuadMT);
	lua_pop(L, 1);

	luaL_Reg destinationQuadFunctions[] = {
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, LuaRender::DestinationQuadMT, LuaRender::DestinationQuadMT, destinationQuadFunctions);
	luaL_getmetatable(L, LuaRender::DestinationQuadMT);
	luaL_setmetatable(L, LuaRender::QuadMT);
	lua_pop(L, 1);
}

static void FillQuad(lua_State* L, void* quad) {
	char* addr = (char*)quad;
	for (int i = 1; i <= lua_gettop(L); ++i) {
		*(Vector*)addr = *lua::GetUserdata<Vector*>(L, i, lua::Metatables::VECTOR, "Vector");
		addr += sizeof(Vector);
	}

	for (int i = lua_gettop(L) + 1; i <= 4; ++i) {
		Vector* vect = (Vector*)addr;
		vect->x = vect->y = 0;
		addr += sizeof(Vector);
	}
}

LUA_FUNCTION(lua_SourceQuad_new) {
	SourceQuad quad;
	quad.__coordinateSpaceEnum = 0;
	FillQuad(L, &quad);
	SourceQuad* result = (SourceQuad*)lua_newuserdata(L, sizeof(SourceQuad));
	luaL_setmetatable(L, LuaRender::SourceQuadMT);
	memcpy(result, &quad, sizeof(SourceQuad));
	return 1;
}

LUA_FUNCTION(lua_DestinationQuad_new) {
	DestinationQuad quad;
	FillQuad(L, &quad);
	DestinationQuad* result = (DestinationQuad*)lua_newuserdata(L, sizeof(DestinationQuad));
	luaL_setmetatable(L, LuaRender::DestinationQuadMT);
	memcpy(result, &quad, sizeof(DestinationQuad));
	return 1;
}

// ============================================================================
// Lua engine

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua_State* L = _state;
	lua::LuaStackProtector protector(L);
	RegisterImageClass(L);
	RegisterTransformerClass(L);
	RegisterQuadClasses(L);

	lua_register(L, "SourceQuad", lua_SourceQuad_new);
	lua_register(L, "DestinationQuad", lua_DestinationQuad_new);

	lua_newtable(L);

	luaL_Reg renderFunctions[] = {
		{ "LoadImage", lua_Renderer_LoadImage },
		{ "StartTransformation", lua_Renderer_StartTransformation },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, renderFunctions, 0);
	lua_setglobal(L, "Renderer");
}