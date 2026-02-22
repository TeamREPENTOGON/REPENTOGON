#include <filesystem>
#include <fstream>
#include <sstream>
#include <variant>

#include <Windows.h>
#include <glad/glad.h>

#include "ASMPatcher.hpp"
#include "../Patches/ASMPatches.h"
#include "HookSystem.h"
#include "Log.h"
#include "SigScan.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "LuaRender.h"
#include "../ShaderLoader.h"
#include "../Utils/ImageUtils.hpp"
#include "../Utils/ShaderUtils.hpp"

using LuaRender::LuaImage;
using LuaRender::LuaTransformer;
using LuaRender::Transformation;

LuaRender::ContextQueue LuaRender::RenderContextQueue;
LuaRender::RenderContext LuaRender::ElementsRenderContext;
LuaRender::RenderContext LuaRender::VerticesRenderContext;

static constexpr bool EnableCustomRendering = false;

// ============================================================================
// Utils

template <typename Func>
static void RenderToSurface(KAGE_Graphics_ImageBase& surface, Func&& renderFn)
{
	auto& manager = g_KAGE_Graphics_Manager;

	Rendering::PushCurrentRenderTarget();
	manager.SetCurrentRenderTarget(&surface, false);
	
	renderFn();

	manager.Present();
	Rendering::RestorePreviousRenderTarget();
}

static void fill_shader_vertices(float* vertexBuffer, const KAGE_Graphics_ShaderBase& shader, lua_State* L, int tableIdx, std::vector<const char*>& errorFields)
{
	lua::LuaStackProtector protector(L);
	tableIdx = lua_absindex(L, tableIdx);

	auto fill_single_float = [&](const char* name, size_t attributeOffset, size_t vertexSize)
	{
		lua_getfield(L, tableIdx, name);
		if (!lua_isnumber(L, -1))
		{
			errorFields.push_back(name);
			lua_pop(L, 1);
			return;
		}

		float data = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);

		for (size_t i = 0; i < 4; i++)
		{
			vertexBuffer[attributeOffset + (i * vertexSize)] = data;
		}
	};

	auto fill_vector = [&](const char* name, size_t attributeOffset, size_t vectorSize, size_t vertexSize)
	{
		lua_getfield(L, tableIdx, name);
		if (!lua_istable(L, -1))
		{
			errorFields.push_back(name);
			lua_pop(L, 1); // pop table field
			return;
		}

		float* data = (float*)alloca(vectorSize * sizeof(float));

		for (size_t i = 0; i < vectorSize; i++)
		{
			lua_rawgeti(L, -1, i + 1);
			if (!lua_isnumber(L, -1))
			{
				errorFields.push_back(name);
				lua_pop(L, 2);
				return;
			}

			data[i] = (float)lua_tonumber(L, -1);
			lua_pop(L, 1);
		}

		lua_pop(L, 1);

		for (size_t i = 0; i < 4; i++)
		{
			std::memcpy(&vertexBuffer[attributeOffset + (i * vertexSize)], data, vectorSize * sizeof(float));
		}
	};

	KAGE_Graphics_VertexAttributeDescriptor* attributes = shader._vertexAttributes;
	size_t numAttributes = shader._numVertexAttributes;
	size_t vertexSize = ShaderUtils::GetVertexSize(attributes, numAttributes);
	size_t attributeOffset = 0;
	for (size_t i = 0; i < numAttributes; i++)
	{
		auto& attribute = attributes[i];

		size_t formatSize = 0;
		switch (attribute.format)
		{
		case (uint32_t)eVertexAttributeFormat::FLOAT:
			formatSize = 1;
			fill_single_float(attribute.name, attributeOffset, vertexSize);
			break;
		case (uint32_t)eVertexAttributeFormat::VEC_2:
			formatSize = 2;
			fill_vector(attribute.name, attributeOffset, formatSize, vertexSize);
			break;
		case (uint32_t)eVertexAttributeFormat::VEC_3:
			formatSize = 3;
			fill_vector(attribute.name, attributeOffset, formatSize, vertexSize);
			break;
		case (uint32_t)eVertexAttributeFormat::VEC_4:
			formatSize = 4;
			fill_vector(attribute.name, attributeOffset, formatSize, vertexSize);
			break;
		default:
			formatSize = ShaderUtils::GetFormatSize(attribute.format);
			break;
		}

		attributeOffset += formatSize;
	}
}

static RenderMatrix get_render_matrix(lua_State* L, int idx)
{
	if (!lua_istable(L, idx))
	{
		luaL_argerror(L, idx, "render matrix is not a table");
	}

	RenderMatrix matrix;

	int luaMatrix = lua_absindex(L, idx);
	lua_rawgeti(L, luaMatrix, 1);
	lua_rawgeti(L, luaMatrix, 2);
	
	int xTransform = lua_absindex(L, -2);
	int yTransform = lua_absindex(L, -1);

	if (!lua_istable(L, xTransform) || !lua_istable(L, yTransform))
	{
		luaL_argerror(L, luaMatrix, "render matrix row is not a table");
	}

	auto assign_matrix_field = [](float& field, lua_State* L, int luaMatrix, int row, int columnIdx)
	{
		lua_rawgeti(L, row, columnIdx);
		if (!lua_isnumber(L, -1))
		{
			luaL_argerror(L, luaMatrix, "render matrix element is not a number!");
		}

		field = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);
	};

	assign_matrix_field(matrix.a, L, luaMatrix, xTransform, 1);
	assign_matrix_field(matrix.b, L, luaMatrix, xTransform, 2);
	assign_matrix_field(matrix.tx, L, luaMatrix, xTransform, 3);

	assign_matrix_field(matrix.c, L, luaMatrix, yTransform, 1);
	assign_matrix_field(matrix.d, L, luaMatrix, yTransform, 2);
	assign_matrix_field(matrix.ty, L, luaMatrix, yTransform, 3);

	lua_pop(L, 2); // pop xTransform, yTransform
	
	return matrix;
}

// ===========================================================================
// Shader
namespace LuaShader {
	struct Userdata {
		static constexpr char* MT = "Shader";
		KAGE_Graphics_Shader* shader = nullptr;

		Userdata(KAGE_Graphics_Shader* shader)
			: shader(shader) {
		}
	};

	static Userdata* GetUserdata(lua_State* L, int idx) {
		return lua::GetRawUserdata<Userdata*>(L, idx, Userdata::MT);
	}

	static Userdata* NewUserdata(lua_State* L, KAGE_Graphics_Shader* shader)
	{
		Userdata* userdata = new (lua_newuserdata(L, sizeof(Userdata))) Userdata(shader);
		luaL_setmetatable(L, Userdata::MT);
		return userdata;
	}

	static void RegisterUserdataClass(lua_State* L) {
		luaL_Reg functions[] = {
			{ NULL, NULL }
		};

		lua::RegisterNewClass(L, Userdata::MT, Userdata::MT, functions);
	}
}

// ============================================================================
// Image

LuaImage* LuaRender::GetLuaImage(lua_State* L, int idx) {
	return lua::GetRawUserdata<LuaImage*>(L, idx, LuaRender::ImageMT);
}

LUA_FUNCTION(lua_Image_gc) {
	LuaImage* image = LuaRender::GetLuaImage(L);
	image->image.DecrRef();
	return 0;
}

LUA_FUNCTION(Lua_Image_GetWidth)
{
	LuaImage* luaImage = LuaRender::GetLuaImage(L, 1);
	lua_pushinteger(L, luaImage->image.image->GetWidth());
	return 1;
}

LUA_FUNCTION(Lua_Image_GetHeight)
{
	LuaImage* luaImage = LuaRender::GetLuaImage(L, 1);
	lua_pushinteger(L, luaImage->image.image->GetHeight());
	return 1;
}

LUA_FUNCTION(Lua_Image_GetPaddedWidth)
{
	LuaImage* luaImage = LuaRender::GetLuaImage(L, 1);
	lua_pushinteger(L, luaImage->image.image->GetPaddedWidth());
	return 1;
}

LUA_FUNCTION(Lua_Image_GetPaddedHeight)
{
	LuaImage* luaImage = LuaRender::GetLuaImage(L, 1);
	lua_pushinteger(L, luaImage->image.image->GetPaddedHeight());
	return 1;
}

LUA_FUNCTION(Lua_Image_GetName)
{
	LuaImage* luaImage = LuaRender::GetLuaImage(L, 1);
	lua_pushstring(L, luaImage->image.image->_name);
	return 1;
}

LUA_FUNCTION(Lua_Image_Render)
{
	LuaImage* luaImage = LuaRender::GetLuaImage(L, 1);
	SourceQuad* sourceQuad = LuaRender::GetSourceQuad(L, 2);
	DestinationQuad* destQuad = LuaRender::GetDestQuad(L, 3);
	KColor color = *lua::GetLuabridgeUserdata<KColor*>(L, 4, lua::Metatables::KCOLOR, "KColor");

	auto& image = *luaImage->image.image;

	auto& shader = *__ptr_g_AllShaders[ShaderType::SHADER_COLOR_OFFSET];
	float* vertexBuffer = ImageUtils::SubmitQuadForShader(image, shader, *sourceQuad, *destQuad, ImageUtils::QuadColor(color));
	if (vertexBuffer)
	{
		ShaderUtils::ColorOffset::FillVertices(vertexBuffer, image, ColorMod());
	}

	return 0;
}

LUA_FUNCTION(Lua_Image_RenderWithShader)
{
	LuaImage* luaImage = LuaRender::GetLuaImage(L, 1);
	SourceQuad* sourceQuad = LuaRender::GetSourceQuad(L, 2);
	DestinationQuad* destQuad = LuaRender::GetDestQuad(L, 3);
	KColor color = *lua::GetLuabridgeUserdata<KColor*>(L, 4, lua::Metatables::KCOLOR, "KColor");
	LuaShader::Userdata* luaShader = LuaShader::GetUserdata(L, 5);
	if (!lua_istable(L, 6))
	{
		return luaL_typeerror(L, 6, lua_typename(L, LUA_TTABLE));
	}

	auto& shader = *luaShader->shader;
	if (!shader._initialized)
	{
		// shader was shutdown or never successfully compiled.
		return luaL_argerror(L, 5, "invalid shader used");
	}

	auto& image = *luaImage->image.image;

	float* vertexBuffer = ImageUtils::SubmitQuadForShader(image, shader, *sourceQuad, *destQuad, ImageUtils::QuadColor(color));
	if (!vertexBuffer)
	{
		return 0;
	}

	// no reserve, since the error path should be a rare and non desirable occurrence.
	std::vector<const char*> errorFields;
	fill_shader_vertices(vertexBuffer, shader, L, 6, errorFields);

	if (!errorFields.empty())
	{
		std::string errorMessage = "some fields were not properly set :";
		for (size_t i = 0; i < errorFields.size(); i++)
		{
			const char* separator = i == 0 ? " " : ", ";
			errorMessage += std::string(separator) + errorFields[i];
		}

		// field not being setup correctly is purely the caller's fault, so even tho we can safely continue it's better to error.
		return luaL_argerror(L, 6, errorMessage.c_str());
	}

	return 0;
}

LUA_FUNCTION(Lua_Image_GetTexelRegion)
{
	LuaImage* luaImage = LuaRender::GetLuaImage(L, 1);
	int x = (int)luaL_checkinteger(L, 2);
	int y = (int)luaL_checkinteger(L, 3);
	uint32_t width = (uint32_t)luaL_checkinteger(L, 4);
	uint32_t height = (uint32_t)luaL_checkinteger(L, 5);

	auto& image = *luaImage->image.image;
	uint32_t size = width * height * 4; // (RGBA)
	luaL_Buffer buffer;
	luaL_buffinit(L, &buffer);

	uint8_t* dst = (uint8_t*)luaL_prepbuffsize(&buffer, size);
	image.GetTexelRegion(x, y, width, height, dst);

	luaL_addsize(&buffer, size);
	luaL_pushresult(&buffer);
	return 1;
}

static void RegisterImageClass(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetWidth", Lua_Image_GetWidth },
		{ "GetHeight", Lua_Image_GetHeight },
		{ "GetPaddedWidth", Lua_Image_GetPaddedWidth },
		{ "GetPaddedHeight", Lua_Image_GetPaddedHeight },
		{ "GetName", Lua_Image_GetName },
		{ "GetTexelRegion", Lua_Image_GetTexelRegion },
		{ "Render", Lua_Image_Render },
		{ "RenderWithShader", Lua_Image_RenderWithShader },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, "Image", "Image", functions, lua_Image_gc);
}

// ===========================================================================
// SurfaceRenderController

// Used for functions that only make sense during a Surface render operation
namespace LuaSurfaceRenderController {
	struct Userdata {
		static constexpr char* MT = "SurfaceRenderController";
		bool valid = true;

		Userdata() = default;
	};
	
	static Userdata* get_userdata(lua_State* L, int idx) {
		return lua::GetRawUserdata<Userdata*>(L, idx, Userdata::MT);
	}

	static Userdata* get_valid_surface_render_controller(lua_State* L, int idx) {
		Userdata* controller = get_userdata(L, idx);
		if (!controller->valid)
		{
			luaL_error(L, "This surface render controller has already been applied and cannot be used again");
		}
	
		return controller;
	}

	LUA_FUNCTION(lua_clear)
	{
		Userdata* controller = get_valid_surface_render_controller(L, 1);
		g_KAGE_Graphics_Manager.Clear();
		return 0;
	}

	static Userdata* NewUserdata(lua_State* L)
	{
		Userdata* userdata = new (lua_newuserdata(L, sizeof(Userdata))) Userdata;
		luaL_setmetatable(L, Userdata::MT);
		return userdata;
	}

	static void RegisterUserdataClass(lua_State* L) {
		luaL_Reg functions[] = {
			{ "Clear", lua_clear },
			{ NULL, NULL }
		};
		lua::RegisterNewClass(L, Userdata::MT, Userdata::MT, functions);
	}
}

// ============================================================================
// Transformer

LuaTransformer* LuaRender::GetTransformer(lua_State* L, int idx) {
	return lua::GetRawUserdata<LuaTransformer*>(L, idx, LuaRender::TransformerMT);
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
	KColor mod = *lua::GetLuabridgeUserdata<KColor*>(L, 5, lua::Metatables::KCOLOR, "KColor");

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
	KColor mod = *lua::GetLuabridgeUserdata<KColor*>(L, 5, lua::Metatables::KCOLOR, "KColor");
	KColor mod2 = *lua::GetLuabridgeUserdata<KColor*>(L, 5, lua::Metatables::KCOLOR, "KColor");
	KColor mod3 = *lua::GetLuabridgeUserdata<KColor*>(L, 5, lua::Metatables::KCOLOR, "KColor");
	KColor mod4 = *lua::GetLuabridgeUserdata<KColor*>(L, 5, lua::Metatables::KCOLOR, "KColor");

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

	RenderToSurface(*transformer->_output.image, [&](){
		// __ptr_g_KAGE_Graphics_Manager->Clear();
		for (Transformation& transformation : transformer->_transformations) {
			KAGE_Graphics_ImageBase* image = transformation._input.image;
			image->Render(transformation._source, transformation._dest, transformation._color1, transformation._color2, transformation._color3, transformation._color4);
		}
	});

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
	lua::RegisterNewClass(L, LuaRender::TransformerMT, LuaRender::TransformerMT, functions);
}

// ============================================================================
// Quads

// DestinationQuad is used as the generic quad.
static DestinationQuad* get_quad(lua_State* L, int idx)
{
	DestinationQuad* ud = (DestinationQuad*)luaL_testudata(L, idx, LuaRender::SourceQuadMT);
	if (ud)
	{
		return ud;
	}

	ud = (DestinationQuad*)luaL_checkudata(L, idx, LuaRender::DestinationQuadMT);
	return ud;
}

SourceQuad* LuaRender::GetSourceQuad(lua_State* L, int idx) {
	return (SourceQuad*)luaL_checkudata(L, idx, LuaRender::SourceQuadMT);
}

DestinationQuad* LuaRender::GetDestQuad(lua_State* L, int idx) {
	return (DestinationQuad*)luaL_checkudata(L, idx, LuaRender::DestinationQuadMT);
}

LUA_FUNCTION(Lua_DestinationQuad_Copy)
{
	DestinationQuad* quad = LuaRender::GetDestQuad(L, 1);

	DestinationQuad* result = (DestinationQuad*)lua_newuserdata(L, sizeof(DestinationQuad));
	luaL_setmetatable(L, LuaRender::DestinationQuadMT);
	*result = *quad;

	return 1;
}

LUA_FUNCTION(Lua_SourceQuad_Copy)
{
	SourceQuad* quad = LuaRender::GetSourceQuad(L, 1);

	SourceQuad* result = (SourceQuad*)lua_newuserdata(L, sizeof(SourceQuad));
	luaL_setmetatable(L, LuaRender::SourceQuadMT);
	*result = *quad;

	return 1;
}

LUA_FUNCTION(lua_Quad_GetTopLeft) {
	DestinationQuad* quad = get_quad(L, 1);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), quad->_topLeft);
	return 1;
}

LUA_FUNCTION(lua_Quad_GetTopRight) {
	DestinationQuad* quad = get_quad(L, 1);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), quad->_topRight);
	return 1;
}

LUA_FUNCTION(lua_Quad_GetBottomLeft) {
	DestinationQuad* quad = get_quad(L, 1);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), quad->_bottomLeft);
	return 1;
}

LUA_FUNCTION(lua_Quad_GetBottomRight) {
	DestinationQuad* quad = get_quad(L, 1);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), quad->_bottomRight);
	return 1;
}

LUA_FUNCTION(lua_Quad_SetTopLeft) {
	DestinationQuad* quad = get_quad(L, 1);
	quad->_topLeft = *lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	return 0;
}

LUA_FUNCTION(lua_Quad_SetTopRight) {
	DestinationQuad* quad = get_quad(L, 1);
	quad->_topRight = *lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	return 0;
}

LUA_FUNCTION(lua_Quad_SetBottomLeft) {
	DestinationQuad* quad = get_quad(L, 1);
	quad->_bottomLeft = *lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	return 0;
}

LUA_FUNCTION(lua_Quad_SetBottomRight) {
	DestinationQuad* quad = get_quad(L, 1);
	quad->_bottomRight = *lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	return 0;
}

LUA_FUNCTION(Lua_Quad_Translate)
{
	DestinationQuad* quad = get_quad(L, 1);
	Vector offset = *lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	
	quad->Translate(offset);

	return 0;
}

LUA_FUNCTION(Lua_Quad_Scale)
{
	DestinationQuad* quad = get_quad(L, 1);
	Vector scale = *lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	Vector anchor = *lua::GetLuabridgeUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");

	quad->Scale(scale, anchor);

	return 0;
}

LUA_FUNCTION(Lua_Quad_Rotate)
{
	DestinationQuad* quad = get_quad(L, 1);
	float rotation = (float)luaL_checknumber(L, 2);
	Vector anchor = *lua::GetLuabridgeUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");

	quad->RotateDegrees(anchor, rotation);

	return 0;
}

LUA_FUNCTION(Lua_Quad_Shear)
{
	DestinationQuad* quad = get_quad(L, 1);
	Vector shear = *lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	Vector anchor = *lua::GetLuabridgeUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");

	quad->Shear(shear, anchor);

	return 0;
}

LUA_FUNCTION(Lua_Quad_ApplyMatrix)
{
	DestinationQuad* quad = get_quad(L, 1);
	if (!lua_istable(L, 2))
	{
		luaL_typeerror(L, 2, lua_typename(L, LUA_TTABLE));
	}
	RenderMatrix matrix = get_render_matrix(L, 2);
	Vector anchor = *lua::GetLuabridgeUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");

	quad->ApplyMatrix(matrix, anchor);

	return 0;
}

LUA_FUNCTION(Lua_Quad_Flip)
{
	DestinationQuad* quad = get_quad(L, 1);
	bool flipX = lua_isnoneornil(L, 2) ? true : lua_toboolean(L, 2);
	bool flipY = lua_isnoneornil(L, 3) ? true : lua_toboolean(L, 3);

	if (flipX)
	{
		quad->FlipX();
	}

	if (flipY)
	{
		quad->FlipY();
	}

	return 0;
}

LUA_FUNCTION(Lua_DestQuad_ToString)
{
	DestinationQuad* quad = LuaRender::GetDestQuad(L, 1);

	lua_pushfstring(L, "[DestQuad: TopLeft %f %f | TopRight %f %f | BottomLeft %f %f | BottomRight %f %f]",
		quad->_topLeft.x, quad->_topLeft.y,
		quad->_topRight.x, quad->_topRight.y,
		quad->_bottomLeft.x, quad->_bottomLeft.y,
		quad->_bottomRight.x, quad->_bottomRight.y);

	return 1;
}

LUA_FUNCTION(Lua_SourceQuad_IsUVSpace)
{
	SourceQuad* quad = LuaRender::GetSourceQuad(L, 1);
	lua_pushboolean(L, quad->_coordinateSpace == SourceQuad::eCoordinateSpace::NORMALIZED_UV);
	return 1;
}

LUA_FUNCTION(Lua_SourceQuad_ConvertToPixelSpace)
{
	SourceQuad* quad = LuaRender::GetSourceQuad(L, 1);
	LuaImage* luaImage = LuaRender::GetLuaImage(L, 2);

	quad->ConvertToPixelSpace(*luaImage->image.image);

	return 0;
}

LUA_FUNCTION(Lua_SourceQuad_ConvertToUVSpace)
{
	SourceQuad* quad = LuaRender::GetSourceQuad(L, 1);
	LuaImage* luaImage = LuaRender::GetLuaImage(L, 2);

	quad->ConvertToUVSpace(*luaImage->image.image);

	return 0;
}

LUA_FUNCTION(Lua_SourceQuad_ToString)
{
	SourceQuad* quad = LuaRender::GetSourceQuad(L, 1);

	lua_pushfstring(L, "[SourceQuad: TopLeft %f %f | TopRight %f %f | BottomLeft %f %f | BottomRight %f %f | UV %s]",
		quad->_topLeft.x, quad->_topLeft.y,
		quad->_topRight.x, quad->_topRight.y,
		quad->_bottomLeft.x, quad->_bottomLeft.y,
		quad->_bottomRight.x, quad->_bottomRight.y,
		quad->_coordinateSpace == SourceQuad::eCoordinateSpace::NORMALIZED_UV ? "true" : "false");

	return 1;
}

static void RegisterQuadClasses(lua_State* L) {
	luaL_Reg destinationQuadFunctions[] = {
		{ "Copy", Lua_DestinationQuad_Copy },
		{ "GetTopLeft", lua_Quad_GetTopLeft },
		{ "GetTopRight", lua_Quad_GetTopRight },
		{ "GetBottomLeft", lua_Quad_GetBottomLeft },
		{ "GetBottomRight", lua_Quad_GetBottomRight },
		{ "SetTopLeft", lua_Quad_SetTopLeft },
		{ "SetTopRight", lua_Quad_SetTopRight },
		{ "SetBottomLeft", lua_Quad_SetBottomLeft },
		{ "SetBottomRight", lua_Quad_SetBottomRight },
		{ "Translate", Lua_Quad_Translate },
		{ "Scale", Lua_Quad_Scale },
		{ "Rotate", Lua_Quad_Rotate },
		{ "Shear", Lua_Quad_Shear },
		{ "ApplyMatrix", Lua_Quad_ApplyMatrix },
		{ "Flip", Lua_Quad_Flip },
		{ "__tostring", Lua_DestQuad_ToString },
		{ NULL, NULL }
	};

	luaL_Reg sourceQuadFunctions[] = {
		{ "Copy", Lua_SourceQuad_Copy },
		{ "IsUVSpace", Lua_SourceQuad_IsUVSpace },
		{ "ConvertToPixelSpace", Lua_SourceQuad_ConvertToPixelSpace },
		{ "ConvertToUVSpace", Lua_SourceQuad_ConvertToUVSpace },
		{ "__tostring", Lua_SourceQuad_ToString },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, LuaRender::DestinationQuadMT, LuaRender::DestinationQuadMT, destinationQuadFunctions);

	lua::RegisterNewClass(L, LuaRender::SourceQuadMT, LuaRender::SourceQuadMT, sourceQuadFunctions);
	luaL_getmetatable(L, LuaRender::SourceQuadMT);
	luaL_setmetatable(L, LuaRender::DestinationQuadMT);
	lua_pop(L, 1);
}

static void FillQuad(lua_State* L, DestinationQuad& quad, int startIdx) {
	quad._topLeft = *lua::GetLuabridgeUserdata<Vector*>(L, startIdx + 0, lua::Metatables::VECTOR, "Vector");
	quad._topRight = *lua::GetLuabridgeUserdata<Vector*>(L, startIdx + 1, lua::Metatables::VECTOR, "Vector");
	quad._bottomLeft = *lua::GetLuabridgeUserdata<Vector*>(L, startIdx + 2, lua::Metatables::VECTOR, "Vector");
	quad._bottomRight = *lua::GetLuabridgeUserdata<Vector*>(L, startIdx + 3, lua::Metatables::VECTOR, "Vector");
}

LUA_FUNCTION(lua_SourceQuad_new) {
	SourceQuad quad;
	FillQuad(L, quad, 1);
	bool uvSpace = lua_toboolean(L, 5);
	quad._coordinateSpace = uvSpace ? SourceQuad::eCoordinateSpace::NORMALIZED_UV : SourceQuad::eCoordinateSpace::PIXEL;

	SourceQuad* result = (SourceQuad*)lua_newuserdata(L, sizeof(SourceQuad));
	luaL_setmetatable(L, LuaRender::SourceQuadMT);
	memcpy(result, &quad, sizeof(SourceQuad));
	return 1;
}

LUA_FUNCTION(lua_DestinationQuad_new) {
	DestinationQuad quad;
	FillQuad(L, quad, 1);

	DestinationQuad* result = (DestinationQuad*)lua_newuserdata(L, sizeof(DestinationQuad));
	luaL_setmetatable(L, LuaRender::DestinationQuadMT);
	memcpy(result, &quad, sizeof(DestinationQuad));
	return 1;
}

// ============================================================================
// Custom rendering

/* VertexBuffer and associated classes and functions breakdown.
 *
 * Access to vertex buffers through two distinct classes: NativeVertexBuffer, CustomVertexBuffer.
 * NativeVertexBuffer used for the vertex buffer coming from the game. Can only alter existing vertices.
 * CustomVertexBuffer used for custom rendering. Arbitrary number of vertices, free choice of shader etc.
 *
 * VertexDescriptor: class describing the vertex buffer. Every attribute with its type and name
 * in the vertex shader.
 *
 * Vertex: class representing a vertex. Generated on the fly to contain a structure mirroring the
 * vertex shader.
 *
 * Elements: class representing the GL_ELEMENT_ARRAY_BUFFER (note that the game never explicitly binds
 * this buffer and simply gives data to glDrawElements).
 *
 * Renderer.ShaderType: Lua translation of the ShaderType C++ enum as a table.
 * Renderer.GLSLType: Lua translation of the GLSLType C++ enum as a table.
 *
 * GLSLValue: abstract root class of all GLSL values in Lua.
 *	- Floats must be lifted
 *
 * GLSLFloat <: GLSLValue: represents a float.
 * GLSLVecN <: GLSLValue: represents a vector of N components.
 * GLSLMatN <: GLSLValue: represents a matrix of N*N components.
 *
 * Shader: represents a shader, storing the id of the shader program.
 *
 * Pipeline: represents a sequence of transformations.
 *
 * Lua methods of Stage:
 *	- CustomVertexBuffer Stage:GetVertexBuffer()
 *
 * Lua methods of Pipeline:
 *	- VertexBuffer Projectile:NewPass(int nb_vertices, Shader shader, Elements elements)
 *		- The VertexDescriptor is extracted from the shader itself
 *	- void Projectile:AddOriginalRenderingPass()
 *		- Can be done multiple times. Adds a special pass that performs the original render operation.
 *	- (C++) The stages are stored in the pipeline in a sequence
 *
 * Lua methods of Renderer:
 *	- static Shader Renderer.Shader(string vertex_shader, string fragment_shader,
 *	VertexDescriptor descriptor)
 *	- static Pipeline Renderer.Pipeline()
 *	- static VertexDescriptor Renderer.VertexDescriptor()
 *	- static Elements Renderer.Elements(int nb_vertices)
 *	- static GLSLValue Renderer.FloatToGLSL(float f)
 *	- static GLSLVecN Renderer.VecN(float x1, ..., float xn)
 *	- static GLSLMatN Renderer.MatN(GLSLVecN row1, ..., GLSLVecN rown)
 *
 * Lua attributes of GLSLFloat:
 *	- <writable> float value
 *
 * Lua attributes of GLSLVecN:
 *	- <writable> float <component1>
 *	- ...
 *	- <writable> float <componentN>
 *
 * Lua attributes of GLSLMatN:
 *	- <writable> GLSLVecN <row1>
 *	- ...
 *	- <writable> GLSLVecN <rowN>
 *
 * Lua methods of Shader:
 *	- void Shader:BindUniform(string name, GLSLValue value)
 *	- (C++) The bindings are stored in a map
 *
 * Lua methods of VertexDescriptor:
 *	- void VertexDescriptor:AddAttribute(GLSLType type, string name)
 *	- __len metamethod, returns the number of attributes
 *	- mixed VertexDescriptor.__meta.__index(string name) :
 *		- If name coerces to an integer, return the attribute at that position, raise an error if OOB
 *		- Otherwise, attempt to call a method of that name
 *
 * Lua methods and attribute of Vertex:
 *	- <writable> GLSLValue attribute1
 *  - ...
 *  - <writable> GLSLValue attributeN
 *
 * Lua methods of NativeVertexBuffer:
 *	- <writable> Vertex NativeVertexBuffer:GetVertex(int id)
 *		- Errors if id out of bounds compared to the max value in indices
 *  - <read-only> ShaderType NativeVertexBuffer:GetShader()
 *	- <read-only> Elements NativeVertexBuffer:GetElements()
 *  - <read-only> VertexDescriptor NativeVertexBuffer:GetDescriptor()
 *
 * Lua methods of CustomVertexBuffer:
 *	- <writable> Vertex CustomVertexBuffer:GetVertex(int id)
 *		- Errors if the id out of bounds compared to the value given when the buffer was created
 *
 * Typical use is as follows:
 *	- Create a Pipeline
 *	- As long as stages are needed
 *		- If the stage should perform the original rendering:
 *			- Call AddOriginalRenderingPass()
 *		- Else
 *			- Use NewStage() to create a new stage and get a vertex buffer with a shader and elements array bound
 *			- Configure the vertex buffer as needed
 *	- Return the Pipeline object
 *
 * A more complete use (on a single pass) would be as follows :
 *	- Create a VertexDescriptor using Renderer.VertexDescriptor
 *	- For each attribute, call AddAttribute on the VBD
 *	- Create a shader using Renderer.Shader(vertexShader, fragmentShader, descriptor)
 *	- For each uniform of the shader, bind it using BindUniform(name, value)
 *	- Create a Pipeline using Renderer.Pipeline
 *	- Create an Elements using Renderer.Elements(nb_total_vertices)
 *	- Configure the Elements object using the [] operator
 *	- Create a new pass in the Pipeline using NewPass(nb_local_vertices, shader, elements)
 *	- Configure the vertex buffer of the pass using GetVertex as needed
 *	- Return the Pipeline object
 */
namespace GL {
	enum GLSLType {
		GLSL_FLOAT,
		GLSL_VEC2,
		GLSL_VEC3,
		GLSL_VEC4,
		GLSL_MAX
	};

	struct GLFloat;
	struct GLVec2;
	struct GLVec3;
	struct GLVec4;
	struct GLMat2;
	struct GLMat3;
	struct GLMat4;

	struct GLSLValue {
		static inline bool CheckKey(lua_State* L, const char* key) {
			if (lua_isstring(L, 2)) {
				return !strcmp(lua_tostring(L, 2), key);
			}

			return luaL_error(L, "Invalid type for field access");
		}

		static inline void AssertKey(lua_State* L, const char* key) {
			if (!CheckKey(L, key)) {
				luaL_error(L, "Invalid field name");
			}
		}

		static inline int CheckIntKey(lua_State* L) {
			if (lua_isinteger(L, 2)) {
				return (int) lua_tointegerx(L, 2, NULL);
			}

			return luaL_error(L, "Invalid type for row access");
		}

		static inline void AssertBadIntKey(lua_State* L, int key) {
			luaL_error(L, "Invalid row index %d", key);
		}

		template<typename T>
		static T* GetWithAlt(lua_State* L, int index, const char* mt) {
			char altMt[100];
			strcpy(altMt, mt);
			strcpy(altMt + strlen(mt), "Ptr");

			if (lua_isuserdata(L, index)) {
				lua_getmetatable(L, index);
				luaL_getmetatable(L, mt);

				if (lua_rawequal(L, -1, -2)) {
					lua_pop(L, 2);
					return (T*)lua_touserdata(L, index);
				}
				else {
					lua_pop(L, 1);
					luaL_getmetatable(L, altMt);
					if (lua_rawequal(L, -1, -2)) {
						lua_pop(L, 2);
						return *(T**)lua_touserdata(L, index);
					}
				}

				luaL_error(L, "Invalid metatable");
			}

			luaL_error(L, "Expected userdata of type %s", mt);
			return nullptr;
		}

		template<typename T>
		static inline T* AssertVec(lua_State* L);
		
		template<>
		static inline GLVec2* AssertVec<GLVec2>(lua_State* L) {
			return GetWithAlt<GLVec2>(L, 3, LuaRender::GLVec2MT);
		}

		template<>
		static inline GLVec3* AssertVec<GLVec3>(lua_State* L) {
			return GetWithAlt<GLVec3>(L, 3, LuaRender::GLVec3MT);
		}

		template<>
		static inline GLVec4* AssertVec<GLVec4>(lua_State* L) {
			return GetWithAlt<GLVec4>(L, 3, LuaRender::GLVec4MT);
		}

		template<typename T>
		static inline T* Get(lua_State* L);

		template<>
		static inline GLFloat* Get<GLFloat>(lua_State* L) {
			return lua::GetRawUserdata<GLFloat*>(L, 1, LuaRender::GLFloatMT);
		}

		template<>
		static inline GLVec2* Get<GLVec2>(lua_State* L) {
			return GetWithAlt<GLVec2>(L, 1, LuaRender::GLVec2MT);
		}

		template<>
		static inline GLVec3* Get<GLVec3>(lua_State* L) {
			return GetWithAlt<GLVec3>(L, 1, LuaRender::GLVec3MT);
		}

		template<>
		static inline GLVec4* Get<GLVec4>(lua_State* L) {
			return GetWithAlt<GLVec4>(L, 1, LuaRender::GLVec4MT);
		}

		template<>
		static inline GLMat2* Get<GLMat2>(lua_State* L) {
			return lua::GetRawUserdata<GLMat2*>(L, 1, LuaRender::GLMat2MT);
		}

		template<>
		static inline GLMat3* Get<GLMat3>(lua_State* L) {
			return lua::GetRawUserdata<GLMat3*>(L, 1, LuaRender::GLMat3MT);
		}

		template<>
		static inline GLMat4* Get<GLMat4>(lua_State* L) {
			return lua::GetRawUserdata<GLMat4*>(L, 1, LuaRender::GLMat4MT);
		}

		static inline float AssertFloat(lua_State* L) {
			if (!lua_isnumber(L, 3)) {
				luaL_error(L, "Invalid type of assignment");
			}

			return (float) lua_tonumber(L, 3);
		}

		static constexpr const char* ValueKey = "value";
		static constexpr const char* XKey = "x";
		static constexpr const char* YKey = "y";
		static constexpr const char* ZKey = "z";
		static constexpr const char* WKey = "w";

		template<typename T>
		static void InitGLSLMetatable(lua_State* L, const char* name) {
			luaL_newmetatable(L, name);
			lua_pushstring(L, "__index");
			lua_pushcfunction(L, T::index);
			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, T::newIndex);
			lua_rawset(L, -5);
			lua_rawset(L, -3);
			lua_pop(L, 1);
		}

		static void InitMetatables(lua_State* L) {
			InitGLSLMetatable<GLFloat>(L, LuaRender::GLFloatMT);
			InitGLSLMetatable<GLVec2>(L, LuaRender::GLVec2MT);
			InitGLSLMetatable<GLVec3>(L, LuaRender::GLVec3MT);
			InitGLSLMetatable<GLVec4>(L, LuaRender::GLVec4MT);
			//InitGLSLMetatable<GLVec2>(L, LuaRender::GLVec2PtrMT);
			//InitGLSLMetatable<GLVec3>(L, LuaRender::GLVec3PtrMT);
			//InitGLSLMetatable<GLVec4>(L, LuaRender::GLVec4PtrMT);
			InitGLSLMetatable<GLMat2>(L, LuaRender::GLMat2MT);
			InitGLSLMetatable<GLMat3>(L, LuaRender::GLMat3MT);
			InitGLSLMetatable<GLMat4>(L, LuaRender::GLMat4MT);
		}

		virtual void Bind(int location) = 0;
	};

	struct GLFloat : public GLSLValue {
		float f;
		float* ptr;

		GLFloat() {
			ptr = &f;
		}

		GLFloat(float val) : GLFloat() {
			f = val;
		}

		GLFloat(float* src) : ptr(src) {

		}

		GLFloat(GLFloat const& other) : ptr(&f) {
			*ptr = *other.ptr;
		}

		GLFloat& operator=(GLFloat const& other) {
			if (this == &other)
				return *this;

			*ptr = *other.ptr;
			return *this;
		}

		LUA_FUNCTION(index) {
			GLFloat* value = Get<GLFloat>(L);
			AssertKey(L, ValueKey);
			lua_pushnumber(L, *(value->ptr));
			return 1;
		}

		LUA_FUNCTION(newIndex) {
			GLFloat* value = Get<GLFloat>(L);
			AssertKey(L, ValueKey);
			*(value->ptr) = AssertFloat(L);
			return 0;
		}

		void Bind(int location) override {
			glUniform1f(location, *ptr);
		}
	};

	struct GLVec2 : public GLSLValue {
		float* ptr;
		float data[2];

		GLVec2() : ptr(data) {

		}

		GLVec2(float x, float y) : GLVec2() {
			data[0] = x;
			data[1] = y;
		}

		GLVec2(float* src) : ptr(src) {

		}

		GLVec2(const GLVec2& other) : ptr(data) {
			memcpy(ptr, other.ptr, sizeof(float) * 2);
		}

		GLVec2& operator=(GLVec2 const& other) {
			if (this == &other)
				return *this;

			memcpy(ptr, other.ptr, sizeof(float) * 2);
			return *this;
		}

		void push(lua_State* L) {
			lua::place<GLVec2>(L, LuaRender::GLVec2MT, ptr);
		}

		LUA_FUNCTION(index) {
			GLVec2* value = Get<GLVec2>(L);
			if (CheckKey(L, XKey)) {
				lua_pushnumber(L, value->ptr[0]);
			}
			else {
				AssertKey(L, YKey);
				lua_pushnumber(L, value->ptr[1]);
			}

			return 1;
		}

		LUA_FUNCTION(newIndex) {
			GLVec2* value = Get<GLVec2>(L);
			if (CheckKey(L, XKey)) {
				value->ptr[0] = AssertFloat(L);
			}
			else {
				AssertKey(L, YKey);
				value->ptr[1] = AssertFloat(L);
			}

			return 0;
		}

		void Bind(int location) override {
			glUniform2fv(location, 2, data);
		}
	};

	struct GLVec3 : public GLSLValue {
		float* ptr;
		float data[3];

		GLVec3() : ptr(data) {

		}

		GLVec3(float x, float y, float z) : GLVec3() {
			data[0] = x;
			data[1] = y;
			data[2] = z;
		}

		GLVec3(float* src) : ptr(src) {

		}

		GLVec3(GLVec3 const& other) : ptr(data) {
			memcpy(ptr, other.ptr, sizeof(float) * 3);
		}

		GLVec3& operator=(GLVec3 const& other) {
			if (this == &other)
				return *this;

			memcpy(ptr, other.ptr, sizeof(float) * 3);
			return *this;
		}

		void Bind(int location) override {
			glUniform3fv(location, 3, data);
		}

		void push(lua_State* L) {
			lua::place<GLVec3>(L, LuaRender::GLVec3MT, ptr);
		}

		LUA_FUNCTION(index) {
			GLVec3* value = Get<GLVec3>(L);
			if (CheckKey(L, XKey)) {
				lua_pushnumber(L, value->ptr[0]);
			}
			else if (CheckKey(L, YKey)) {
				lua_pushnumber(L, value->ptr[1]);
			}
			else {
				AssertKey(L, ZKey);
				lua_pushnumber(L, value->ptr[2]);
			}

			return 1;
		}

		LUA_FUNCTION(newIndex) {
			GLVec3* value = Get<GLVec3>(L);
			if (CheckKey(L, XKey)) {
				value->ptr[0] = AssertFloat(L);
			}
			else if (CheckKey(L, YKey)) {
				value->ptr[1] = AssertFloat(L);
			}
			else {
				AssertKey(L, ZKey);
				value->ptr[2] = AssertFloat(L);
			}

			return 0;
		}
	};

	struct GLVec4 : public GLSLValue {
		float* ptr;
		float data[4];

		GLVec4() : ptr(data) {

		}

		GLVec4(float x, float y, float z, float w) : GLVec4() {
			data[0] = x;
			data[1] = y;
			data[2] = z;
			data[3] = w;
		}

		GLVec4(float* src) : ptr(src) {

		}

		GLVec4(const GLVec4& other) : ptr(data) {
			memcpy(ptr, other.ptr, sizeof(float) * 4);
		}

		GLVec4& operator=(GLVec4 const& other) {
			if (this == &other)
				return *this;

			memcpy(ptr, other.ptr, sizeof(float) * 4);
			return *this;
		}

		void Bind(int location) override {
			glUniform4fv(location, 4, data);
		}

		void push(lua_State* L) {
			lua::place<GLVec4>(L, LuaRender::GLVec4MT, ptr);
		}

		LUA_FUNCTION(index) {
			GLVec4* value = Get<GLVec4>(L);
			if (CheckKey(L, XKey)) {
				lua_pushnumber(L, value->ptr[0]);
			}
			else if (CheckKey(L, YKey)) {
				lua_pushnumber(L, value->ptr[1]);
			}
			else if (CheckKey(L, ZKey)) {
				lua_pushnumber(L, value->ptr[2]);
			}
			else {
				AssertKey(L, WKey);
				lua_pushnumber(L, value->ptr[3]);
			}

			return 1;
		}

		LUA_FUNCTION(newIndex) {
			GLVec4* value = Get<GLVec4>(L);
			if (CheckKey(L, XKey)) {
				value->ptr[0] = AssertFloat(L);
			}
			else if (CheckKey(L, YKey)) {
				value->ptr[1] = AssertFloat(L);
			}
			else if (CheckKey(L, ZKey)) {
				value->ptr[2] = AssertFloat(L);
			}
			else {
				AssertKey(L, WKey);
				value->ptr[3] = AssertFloat(L);
			}

			return 0;
		}
	};

	struct GLMat2 : public GLSLValue {
		float data[4];
		GLVec2 row1, row2;

		GLMat2() : row1(data), row2(data + 2) {

		}

		void Bind(int location) override {
			glUniformMatrix2fv(location, 4, GL_TRUE, data);
		}

		LUA_FUNCTION(index) {
			GLMat2* mat = Get<GLMat2>(L);
			switch (int key = CheckIntKey(L)) {
			case 0:
				mat->row1.push(L);
				break;

			case 1:
				mat->row2.push(L);
				break;

			default:
				AssertBadIntKey(L, key);
			}

			return 1;
		}

		LUA_FUNCTION(newIndex) {
			GLMat2* mat = Get<GLMat2>(L);
			GLVec2 vec = *AssertVec<GLVec2>(L);
			switch (int key = CheckIntKey(L)) {
			case 0:
				mat->row1 = vec;
				break;

			case 1:
				mat->row2 = vec;
				break;

			default:
				AssertBadIntKey(L, key);
			}

			return 0;
		}
	};

	struct GLMat3 : public GLSLValue {
		float data[9];
		GLVec3 row1, row2, row3;

		GLMat3() : row1(data), row2(data + 3), row3(data + 6) {

		}

		void Bind(int location) override {
			glUniformMatrix3fv(location, 9, GL_TRUE, data);
		}

		LUA_FUNCTION(index) {
			GLMat3* mat = Get<GLMat3>(L);
			switch (int key = CheckIntKey(L)) {
			case 0:
				mat->row1.push(L);
				break;

			case 1:
				mat->row2.push(L);
				break;

			case 2:
				mat->row3.push(L);
				break;

			default:
				AssertBadIntKey(L, key);
			}

			return 1;
		}

		LUA_FUNCTION(newIndex) {
			GLMat3* mat = Get<GLMat3>(L);
			GLVec3 vec = *AssertVec<GLVec3>(L);
			switch (int key = CheckIntKey(L)) {
			case 0:
				mat->row1 = vec;
				break;

			case 1:
				mat->row2 = vec;
				break;

			case 2:
				mat->row3 = vec;
				break;

			default:
				AssertBadIntKey(L, key);
			}

			return 0;
		}
	};

	struct GLMat4 : public GLSLValue {
		float data[16];
		GLVec4 row1, row2, row3, row4;

		GLMat4() : row1(data), row2(data + 4), row3(data + 8), row4(data + 12) {

		}

		void Bind(int location) override {
			glUniformMatrix4fv(location, 16, GL_TRUE, data);
		}

		LUA_FUNCTION(index) {
			GLMat4* mat = Get<GLMat4>(L);
			switch (int key = CheckIntKey(L)) {
			case 0:
				mat->row1.push(L);
				break;

			case 1:
				mat->row2.push(L);
				break;

			case 2:
				mat->row3.push(L);
				break;

			case 3:
				mat->row4.push(L);
				break;

			default:
				AssertBadIntKey(L, key);
			}

			return 1;
		}

		LUA_FUNCTION(newIndex) {
			GLMat4* mat = Get<GLMat4>(L);
			GLVec4 vec = *AssertVec<GLVec4>(L);

			switch (int key = CheckIntKey(L)) {
			case 0:
				mat->row1 = vec;
				break;

			case 1:
				mat->row2 = vec;
				break;

			case 2:
				mat->row3 = vec;
				break;

			case 3:
				mat->row4 = vec;
				break;

			default:
				AssertBadIntKey(L, key);
			}

			return 0;
		}
	};

	static GL::GLMat4 ProjectionMatrix;

	class VertexDescriptor {
	public:
		VertexDescriptor() {
			_size = 0;
		}

		void AddAttribute(GLSLType type, std::string&& name) {
			// _attributes.push_back(VertexAttributeDescriptor(type, std::move(name)));
			_offsets[name] = std::make_tuple(_size, type);
			size_t size;

			switch (type) {
			case GLSL_FLOAT:
				size = 1;
				break;

			case GLSL_VEC2:
				size = 2;
				break;

			case GLSL_VEC3:
				size = 3;
				break;

			case GLSL_VEC4:
				size = 4;
				break;

			default:
				throw std::runtime_error("Invalid GLSLType value");
			}

			_size += size;
			_attributeSize.push_back(type);
		}

		uint32_t GetSize() const {
			return _size;
		}

		LUA_FUNCTION(Lua_AddAttribute) {
			VertexDescriptor* descriptor = lua::GetRawUserdata<VertexDescriptor*>(L, 1, LuaRender::VertexDescriptorMT);
			int type = (int) luaL_checkinteger(L, 2);
			if (type < 0 || type >= GLSLType::GLSL_MAX) {
				return luaL_error(L, "Invalid attribute type %d", type);
			}
			std::string name(luaL_checkstring(L, 3));
			descriptor->AddAttribute((GLSLType)type, std::move(name));
			return 0;
		}

		static void InitMetatable(lua_State* L) {
			luaL_Reg funcs[] = {
				{ "AddAttribute", Lua_AddAttribute },
				{ NULL, NULL }
			};

			lua::RegisterNewClass(L, LuaRender::VertexDescriptorMT, LuaRender::VertexDescriptorMT, funcs);
		}

		std::map<std::string, std::tuple<size_t, GLSLType>> const& GetOffsets() const {
			return _offsets;
		}

		std::vector<GLSLType> const& GetTypes() const {
			return _attributeSize;
		}

	private:
		std::map<std::string, std::tuple<size_t, GLSLType>> _offsets;
		std::vector<GLSLType> _attributeSize;
		uint32_t _size;
	};

	struct Elements {
		Elements(uint32_t n, uint32_t nVertices) : _nVertices(nVertices) {
			_indices.resize(n);
		}

		Elements(const void* indices, GLuint type, size_t size, uint32_t nVertices) : _nativeIndices(indices), _nativeType(type), _nativeSize(size), _nVertices(nVertices) {

		}

		uint32_t Get(uint32_t index) const {
			if (_nativeIndices) {
				if (index >= _nativeSize) {
					throw std::out_of_range("Invalid index");
				}

				switch (_nativeType) {
				case GL_UNSIGNED_BYTE:
					return *((uint8_t*)_nativeIndices + index);

				case GL_UNSIGNED_SHORT:
					return *((uint16_t*)_nativeIndices + index);

				case GL_UNSIGNED_INT:
					return *((uint32_t*)_nativeIndices + index);

				default:
					throw std::runtime_error("Invalid gl type");
				}
			}
			else {
				return _indices.at(index);
			}
		}

		void Set(uint32_t index, uint32_t value) {
			if (_nativeIndices) {
				if (index > _nativeSize) {
					throw std::out_of_range("Invalid index");
				}

				if (value > _nVertices) {
					throw std::logic_error("Invalid value");
				}

				switch (_nativeType) {
				case GL_UNSIGNED_BYTE:
					*((uint8_t*)_nativeIndices + index) = (uint8_t)value;

				case GL_UNSIGNED_SHORT:
					*((uint16_t*)_nativeIndices + index) = (uint16_t)value;

				case GL_UNSIGNED_INT:
					*((uint32_t*)_nativeIndices + index) = (uint32_t)value;

				default:
					throw std::runtime_error("Invalid gl type");
				}
			}
			else {
				_indices.at(index) = value;
			}
		}

		uint32_t Max() const {
			if (_nativeIndices) {
				return _nativeSize - 1;
			}
			else {
				return _indices.size() - 1;
			}
		}

		LUA_FUNCTION(Lua_index) {
			Elements** ud = lua::GetRawUserdata<Elements**>(L, 1, LuaRender::ElementsArrayMT);
			Elements* elements = *ud;
			uint32_t index = (uint32_t) luaL_checkinteger(L, 2);

			try {
				lua_pushinteger(L, elements->Get(index));
			}
			catch (std::out_of_range& ) {
				return luaL_error(L, "Invalid index %u, max: %u", index, elements->Max());
			}

			return 1;
		}

		LUA_FUNCTION(Lua_newindex) {
			Elements** ud = lua::GetRawUserdata<Elements**>(L, 1, LuaRender::ElementsArrayMT);
			Elements* elements = *ud;
			uint32_t index = (uint32_t) luaL_checkinteger(L, 2);
			uint32_t value = (uint32_t) luaL_checkinteger(L, 3);

			try {
				elements->Set(index, value);
			}
			catch (std::out_of_range& ) {
				return luaL_error(L, "Invalid index %u, max: %u", index, elements->Max());
			}
			catch (std::logic_error& ) {
				return luaL_error(L, "Invalid value %u for index %u, max: %u", value, index, elements->_nVertices);
			}

			return 0;
		}

		LUA_FUNCTION(Lua_len) {
			Elements** ud = lua::GetRawUserdata<Elements**>(L, 1, LuaRender::ElementsArrayMT);
			lua_pushinteger(L, (*ud)->Max() + 1);
			return 1;
		}

		static void InitMetatable(lua_State* L) {
			luaL_newmetatable(L, LuaRender::ElementsArrayMT);
			lua::TableAssoc(L, "__index", Lua_index);
			lua::TableAssoc(L, "__newindex", Lua_newindex);
			lua::TableAssoc(L, "__len", Lua_len);
			lua_pop(L, 1);
		}

		std::vector<int> _indices;
		uint32_t _nVertices;

		// Used to interface with the native elements array
		const void* _nativeIndices = nullptr;
		GLuint _nativeType = 0;
		size_t _nativeSize = 0;
	};

	struct Vertex {
		Vertex(float* base, VertexDescriptor const* descriptor) {
			_data = base;
			_offsets = &descriptor->GetOffsets();
		}

		inline std::tuple<size_t, GLSLType> GetAttribute(std::string const& name) {
			auto iter = _offsets->find(name);
			if (iter == _offsets->end()) {
				return std::make_tuple(0, GLSLType::GLSL_MAX);
			}

			return iter->second;
		}

		LUA_FUNCTION(Lua_Dump) {
			Vertex* vertex = lua::GetRawUserdata<Vertex*>(L, 1, LuaRender::VertexMT);
			lua_newtable(L);
			for (auto const& [name, data] : *vertex->_offsets) {
				auto const& [offset, type] = data;
				lua_pushstring(L, name.c_str());
				lua_newtable(L);
				lua::TableAssoc(L, "Offset", (int)offset);
				lua::TableAssoc(L, "Type", (int)type);
				lua_rawset(L, -3);
			}
			return 1;
		}

		LUA_FUNCTION(Lua_index) {
			Vertex* vertex = lua::GetRawUserdata<Vertex*>(L, 1, LuaRender::VertexMT);
			const char* name = luaL_checkstring(L, 2);
			auto [offset, type] = vertex->GetAttribute(name);
			float* data = vertex->_data + offset;

			if (type == GLSL_MAX) {
				// return luaL_error(L, "Invalid attribute name %s", name);
				luaL_getmetatable(L, LuaRender::VertexMT);
				lua_pushstring(L, name);
				int type = lua_rawget(L, -2);
				
				if (type == LUA_TNIL) {
					luaL_error(L, "No field or method with name %s\n", name);
				}

				return 1;
			}

			switch (type) {
			case GLSL_FLOAT:
				lua::place<GLFloat>(L, LuaRender::GLFloatMT, data);
				break;

			case GLSL_VEC2:
				lua::place<GLVec2>(L, LuaRender::GLVec2MT, data);
				break;

			case GLSL_VEC3:
				lua::place<GLVec3>(L, LuaRender::GLVec3MT, data);
				break;

			case GLSL_VEC4:
				lua::place<GLVec4>(L, LuaRender::GLVec4MT, data);
				break;

			default:
				return luaL_error(L, "Invalid GLSLType %d\n", data);
			}

			return 1;
		}

		template<typename T>
		static void AssignAttribute(lua_State* L, Vertex* vertex, size_t offset, const char* mt) {
			T(vertex->_data + offset) = *lua::GetRawUserdata<T*>(L, 3, mt);
		}

		LUA_FUNCTION(Lua_newindex) {
			Vertex* vertex = lua::GetRawUserdata<Vertex*>(L, 1, LuaRender::VertexMT);
			const char* name = luaL_checkstring(L, 2);

			auto [offset, type] = vertex->GetAttribute(name);

			if (!offset) {
				return luaL_error(L, "Invalid attribute name %s", name);
			}

			switch (type) {
			case GLSL_FLOAT:
				AssignAttribute<GLFloat>(L, vertex, offset, LuaRender::GLFloatMT);
				break;

			case GLSL_VEC2:
				AssignAttribute<GLVec2>(L, vertex, offset, LuaRender::GLVec2MT);
				break;

			case GLSL_VEC3:
				AssignAttribute<GLVec3>(L, vertex, offset, LuaRender::GLVec3MT);
				break;

			case GLSL_VEC4:
				AssignAttribute<GLVec4>(L, vertex, offset, LuaRender::GLVec4MT);
				break;

			default:
				throw std::runtime_error("Invalid GLSLType value");
			}

			return 0;
		}

		static void InitMetatable(lua_State* L) {
			luaL_newmetatable(L, LuaRender::VertexMT);
			lua_pushstring(L, "__index");
			lua_pushcfunction(L, Lua_index);
			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, Lua_newindex);
			lua_rawset(L, -5);
			lua_rawset(L, -3);

			luaL_Reg funcs[] = {
				{ "Dump", Lua_Dump },
				{ NULL, NULL }
			};
			luaL_setfuncs(L, funcs, 0);
			lua_pop(L, 1);
		}

		std::map<std::string, std::tuple<size_t, GLSLType>> const* _offsets;
		float* _data;
	};

	struct VertexBuffer {
		float* _data = nullptr;
		VertexDescriptor const* _descriptor = nullptr;
		size_t _nVertices = 0;
		Elements _elements;
		bool _needRelease = true;

		VertexBuffer(float* data, ShaderType shader, size_t nVertices, const void* indices, size_t nElements, GLuint type) :
			_data(data), _nVertices(nVertices), _elements(indices, type, nElements, nVertices) {
			SetDescriptor(shader);
		}

		VertexBuffer(size_t nVertices, size_t nElements, VertexDescriptor const& descriptor) : 
			_nVertices(nVertices), _descriptor(&descriptor), _elements(nElements, nVertices) {
			_data = new float[nVertices * _descriptor->GetSize()];
		}

		~VertexBuffer() {
			if (_needRelease) {
				// FILE* f = fopen("repentogon.log", "a");
				// fprintf(f, "Freeing vertex buffer at %p (_data = %p)\n", this, _data);
				// fclose(f);
				delete[] _data;
			}
		}

		VertexBuffer* Slice(int start, int end, int nElements, VertexDescriptor const* descriptor);

		void Bind();

		void SetDescriptor(ShaderType shader);

		void GetVertex(lua_State* L, size_t index) {
			if (index > _nVertices) {
				luaL_error(L, "Invalid index %u in vertex buffer (max %u)", index, _nVertices);
			}

			float* target = _data + index * _descriptor->GetSize();
			lua::place<Vertex>(L, LuaRender::VertexMT, target, _descriptor);

			/* FILE* f = fopen("repentogon.log", "a");
			fprintf(f, "[OpenGL] Getting vertex %d from vertex buffer %p. Base is at %p, vertex is at %p (descriptor size = %d)\n", index, this, _data, target, _descriptor->GetSize());
			fclose(f); */
		}

		LUA_FUNCTION(Lua_GetVertex) {
			VertexBuffer** buffer = lua::GetRawUserdata<VertexBuffer**>(L, 1, LuaRender::VertexBufferMT);
			(*buffer)->GetVertex(L, (size_t) luaL_checkinteger(L, 2));
			return 1;
		}

		LUA_FUNCTION(Lua_GetElements) {
			VertexBuffer** buffer = lua::GetRawUserdata<VertexBuffer**>(L, 1, LuaRender::VertexBufferMT);
			Elements** ud = (Elements**)lua_newuserdata(L, sizeof(void*));
			*ud = &(*buffer)->_elements;
			luaL_setmetatable(L, LuaRender::ElementsArrayMT);
			return 1;
		}

		LUA_FUNCTION(Lua_gc) {
			VertexBuffer** buffer = lua::GetRawUserdata<VertexBuffer**>(L, 1, LuaRender::VertexBufferMT);
			/* FILE* f = fopen("repentogon.log", "a");
			fprintf(f, "Lua GC freeing vertex buffer at %p\n", *buffer);
			fclose(f); */
			delete *buffer;
			return 0;
		}

		LUA_FUNCTION(Lua_len) {
			VertexBuffer** buffer = lua::GetRawUserdata<VertexBuffer**>(L, 1, LuaRender::VertexBufferMT);
			lua_pushinteger(L, (*buffer)->_nVertices + 1);
			return 1;
		}

		static void InitMetatable(lua_State* L) {
			luaL_Reg funcs[] = {
				{ "GetVertex", Lua_GetVertex },
				{ "GetElements", Lua_GetElements },
				{ "__len", Lua_len },
				{ NULL, NULL },
			};

			lua::RegisterNewClass(L, LuaRender::VertexBufferMT, LuaRender::VertexBufferMT, funcs, Lua_gc);
		}
	};

	class Shader {
	public:
		// Deep copy the descriptor to let the lifetime of the original Lua object
		// expire.
		Shader(std::filesystem::path const& vertexShaderPath, std::filesystem::path const& fragmentShaderPath,
			VertexDescriptor* descriptor) : _descriptor(*descriptor) {
			char buffer[4096];
			GetCurrentDirectory(4096, buffer);

			int vertexShader = InitShader(vertexShaderPath, true);
			int fragmentShader = InitShader(fragmentShaderPath, false);

			int program = glCreateProgram();
			glAttachShader(program, vertexShader);
			glAttachShader(program, fragmentShader);
			glLinkProgram(program);

			int success;
			glGetProgramiv(program, GL_LINK_STATUS, &success);
			if (!success) {
				char buffer[4096];
				glGetProgramInfoLog(program, 4096, NULL, buffer);
				std::ostringstream err;
				err << "Error while linking shader from vertex shader " << vertexShaderPath << " and fragment shader " << fragmentShaderPath << ": " << buffer;
				throw std::runtime_error(err.str());
			}

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			_program = program;
		}

		~Shader() {

		}

		void Use() {
			glUseProgram(_program);
		}

		VertexDescriptor const& GetDescriptor() const {
			return _descriptor;
		}

		int GetProgram() const {
			return _program;
		}

		void BindUniform(std::string const& name, GLSLValue* value) {
			int location = glGetUniformLocation(_program, name.c_str());
			if (location == -1) {
				std::ostringstream err;
				err << "Invalid uniform name " << name << " for shader program " << _program;
				throw std::runtime_error(err.str());
			}

			value->Bind(location);
		}

		static void Lua_BindUniform(lua_State* L, const char* mt) {
			Shader** shader = lua::GetRawUserdata<Shader**>(L, 1, LuaRender::GLShaderMT);
			std::string name(luaL_checkstring(L, 2));
			GLSLValue* value = lua::GetRawUserdata<GLSLValue*>(L, 3, mt);
			(*shader)->BindUniform(name, value);
		}

		LUA_FUNCTION(Lua_BindFloat) {
			Lua_BindUniform(L, LuaRender::GLFloatMT);
			return 0;
		}

		LUA_FUNCTION(Lua_BindVec2) {
			Lua_BindUniform(L, LuaRender::GLVec2MT);
			return 0;
		}

		LUA_FUNCTION(Lua_BindVec3) {
			Lua_BindUniform(L, LuaRender::GLVec3MT);
			return 0;
		}

		LUA_FUNCTION(Lua_BindVec4) {
			Lua_BindUniform(L, LuaRender::GLVec4MT);
			return 0;
		}

		LUA_FUNCTION(Lua_BindMat2) {
			Lua_BindUniform(L, LuaRender::GLMat2MT);
			return 0;
		}

		LUA_FUNCTION(Lua_BindMat3) {
			Lua_BindUniform(L, LuaRender::GLMat3MT);
			return 0;
		}

		LUA_FUNCTION(Lua_BindMat4) {
			Lua_BindUniform(L, LuaRender::GLMat4MT);
			return 0;
		}

		static void InitMetatable(lua_State* L) {
			luaL_Reg funcs[] = {
				{ "BindFloat", Lua_BindFloat },
				{ "BindVec2", Lua_BindVec2 },
				{ "BindVec3", Lua_BindVec3 },
				{ "BindVec4", Lua_BindVec4 },
				{ "BindMat2", Lua_BindMat2 },
				{ "BindMat3", Lua_BindMat3 },
				{ "BindMat4", Lua_BindMat4 },
				{ NULL, NULL}
			};

			lua::RegisterNewClass(L, LuaRender::GLShaderMT, LuaRender::GLShaderMT, funcs);
		}

		// Shaders outlive everything once registered. Store them in static memory
		static std::vector<std::unique_ptr<Shader>> _shaders;

	private:
		int InitShader(std::filesystem::path const& path, bool vertex) {
			unsigned int shader = glCreateShader(vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
			std::string shaderType;
			if (vertex) {
				shaderType = "vertex";
			}
			else {
				shaderType = "fragment";
			}

			std::ifstream shaderStream(path);
			if (!shaderStream.good()) {
				std::ostringstream err;
				err << "Error while opening " << shaderType << " shader file " << path;
				throw std::runtime_error(err.str());
			}

			std::stringstream src;
			src << shaderStream.rdbuf();
			std::string asString = src.str();
			const char* shaderSrc = asString.c_str();
			glShaderSource(shader, 1, &shaderSrc, NULL);
			glCompileShader(shader);

			int success;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				std::ostringstream err;
				char buffer[4096];
				glGetShaderInfoLog(shader, 4096, NULL, buffer);
				err << "Error while compiling " << shaderType << " shader from file " << path << ": " << buffer;
				throw std::runtime_error(err.str());
			}

			return shader;
		}

		int _program;
		VertexDescriptor _descriptor;
	};

	std::vector<std::unique_ptr<Shader>> Shader::_shaders;

	struct Transformation {
		Shader* _shader;
		VertexBuffer* _buffer;
	};

	class Pipeline {
	public:
		VertexBuffer* NewPass(size_t nbVertices, Shader* shader, uint32_t nElements) {
			VertexBuffer* buffer = new VertexBuffer(nbVertices, nElements, shader->GetDescriptor());
			Transformation trans;
			trans._buffer = buffer;
			trans._shader = shader;
			_sequence.push_back(trans);
			return buffer;
		}

		void Render() {

		}

		LUA_FUNCTION(Lua_NewPass) {
			Pipeline* pipeline = lua::GetRawUserdata<Pipeline*>(L, 1, LuaRender::PipelineMT);
			size_t nbVertices = (size_t) luaL_checkinteger(L, 2);
			Shader** shader = lua::GetRawUserdata<Shader**>(L, 3, LuaRender::GLShaderMT);
			size_t nbElements = (size_t) luaL_checkinteger(L, 4);

			VertexBuffer* buffer = pipeline->NewPass(nbVertices, *shader, nbElements);
			VertexBuffer** ud = (VertexBuffer**)lua_newuserdata(L, sizeof(buffer));
			*ud = buffer;
			luaL_setmetatable(L, LuaRender::VertexBufferMT);
			return 1;
		}

		static void InitMetatable(lua_State* L) {
			luaL_Reg funcs[] = {
				{ "NewPass", Lua_NewPass },
				{ NULL, NULL }
			};

			lua::RegisterNewClass(L, LuaRender::PipelineMT, LuaRender::PipelineMT, funcs);
		}

	private:
		std::vector<Transformation> _sequence;
	};

	class ContextLuaVisitor {
	public:
		ContextLuaVisitor(lua_State* _L) : L(_L) {

		}

		void operator()(Entity_Bomb* b) {
			Expose(b, LuaRender::RENDER_CTX_ENTITY_BOMB, lua::Metatables::ENTITY_BOMB);
		}

		void operator()(Entity_Effect* e) {
			Expose(e, LuaRender::RENDER_CTX_ENTITY_EFFECT, lua::Metatables::ENTITY_EFFECT);
		}

		void operator()(Entity_Familiar* f) {
			Expose(f, LuaRender::RENDER_CTX_ENTITY_FAMILIAR, lua::Metatables::ENTITY_FAMILIAR);
		}

		void operator()(Entity_Knife* k) {
			Expose(k, LuaRender::RENDER_CTX_ENTITY_KNIFE, lua::Metatables::ENTITY_KNIFE);
		}

		void operator()(Entity_Laser* l) {
			Expose(l, LuaRender::RENDER_CTX_ENTITY_LASER, lua::Metatables::ENTITY_LASER);
		}

		void operator()(Entity_NPC* n) {
			Expose(n, LuaRender::RENDER_CTX_ENTITY_NPC, lua::Metatables::ENTITY_NPC);
		}

		void operator()(Entity_Pickup* p) {
			Expose(p, LuaRender::RENDER_CTX_ENTITY_PICKUP, lua::Metatables::ENTITY_PICKUP);
		}

		void operator()(Entity_Player* p) {
			Expose(p, LuaRender::RENDER_CTX_ENTITY_PLAYER, lua::Metatables::ENTITY_PLAYER);
		}

		void operator()(Entity_Projectile* p) {
			Expose(p, LuaRender::RENDER_CTX_ENTITY_PROJECTILE, lua::Metatables::ENTITY_PROJECTILE);
		}

		void operator()(Entity_Slot* s) {
			OpenTable();
			PushType(LuaRender::RENDER_CTX_ENTITY_SLOT);
			lua_pushstring(L, "Data");
			lua::luabridge::UserdataPtr::push(L, s, lua::metatables::EntitySlotMT);
			lua_rawset(L, -3);
			CloseTable();
		}

		void operator()(Entity_Tear* t) {
			Expose(t, LuaRender::RENDER_CTX_ENTITY_TEAR, lua::Metatables::ENTITY_TEAR);
		}

		void operator()(GridEntity_Rock* r) {
			Expose(r, LuaRender::RENDER_CTX_GRIDENTITY_ROCK, lua::Metatables::GRID_ENTITY_ROCK);
		}

		void operator()(AnimationState* s) {
			Expose(s->_animation, LuaRender::RENDER_CTX_ANIMATION_STATE, lua::Metatables::SPRITE);
		}

		void operator()(AnimationLayer* l) {
			OpenTable();
			PushType(LuaRender::RENDER_CTX_ANIMATION_LAYER);
			lua_pushstring(L, "Data");
			lua_pushinteger(L, l->GetLayerID());
			lua_rawset(L, -3);
			CloseTable();
		}
	private:
		void OpenTable() {
			lua_pushinteger(L, i);
			lua_newtable(L);
			++i;
		}

		void CloseTable() {
			lua_rawset(L, -3);
		}

		void PushType(LuaRender::ContextType ctx) {
			lua::TableAssoc(L, "Type", ctx);
		}

		void Expose(void* p, LuaRender::ContextType ctx, lua::Metatables mt) {
			OpenTable();
			lua::TableAssoc(L, "Type", ctx);
			lua_pushstring(L, "Data");
			lua::luabridge::UserdataPtr::push(L, p, mt);
			lua_rawset(L, -3);
			CloseTable();
		}

		lua_State* L; // Table at the top
		int i = 1; // Index
	};

	class RenderOperationContext {
	public:
		RenderOperationContext(LuaRender::ContextArray* context) : _context(context) {

		}

		void Push(lua_State* L, int index) {
			if ((size_t) index >= _context->size()) {
				lua_pushnil(L);
				lua_pushnil(L);
				return;
			}

			LuaRender::ContextQueue const& queue = (*_context)[index];
			lua_pushinteger(L, index);
			lua_newtable(L);

			ContextLuaVisitor visitor(L);
			for (LuaRender::Context const& ctx : queue) {
				std::visit(visitor, ctx);
			}
		}

		LUA_FUNCTION(Lua__pairs_iterator) {
			RenderOperationContext* context = (RenderOperationContext*)lua_topointer(L, 1);
			if (lua_isnil(L, 2)) {
				context->Push(L, 0);
			}
			else {
				int index = (int) luaL_checkinteger(L, 2);
				context->Push(L, index + 1);
			}
			return 2;
		}

		LUA_FUNCTION(Lua__pairs) {
			/* for vars in exprlist do body end
			 * explist is evaluated to produce four values
			 *	- iterator function (pairs -> next)
			 *  - state (pairs -> t)
			 *	- initial value for control variable (first variable in vars; pairs -> nil)
			 *	- closing value (pairs -> none)
			 * 
			 * During each iteration, Lua calls the iterator function with (state, control variable)
			 * The call to the iterator function produces the values assigned to vars
			 * Looping stops when control variable becomes nil
			 */
			RenderOperationContext* context = lua::GetRawUserdata<RenderOperationContext*>(L, 1, LuaRender::ContextMT);
			lua_pushcfunction(L, Lua__pairs_iterator);
			lua_pushlightuserdata(L, context);
			lua_pushnil(L);
			return 3;
		}

		static void InitMetatable(lua_State* L) {
			luaL_Reg functions[] = {
				{ "__pairs", Lua__pairs },
				{ NULL, NULL }
			};

			lua::RegisterNewClass(L, LuaRender::ContextMT, LuaRender::ContextMT, functions);
		}
	private:
		LuaRender::ContextArray* _context;
	};

	class RenderSet {
	public:
		LUA_FUNCTION(Lua_SliceSingle) {
			VertexBuffer** buffer = lua::GetRawUserdata<VertexBuffer**>(L, 1, LuaRender::VertexBufferMT);
			Shader** shader = lua::GetRawUserdata<Shader**>(L, 2, LuaRender::GLShaderMT);
			int nElements = (int) luaL_checkinteger(L, 3);

			int first = (int) luaL_checkinteger(L, 4);
			int second = (int) luaL_optinteger(L, 5, -1);

			if (second != -1 && second < first) {
				return luaL_error(L, "Invalid slice range %d:%d", first, second);
			}

			VertexBuffer* slice = (*buffer)->Slice(first, second, nElements, &(*shader)->GetDescriptor());
			return 0;
		}

		LUA_FUNCTION(Lua_SlicePipeline) {
			return 0;
		}

		static void InitMetatable(lua_State* L) {
			luaL_Reg funcs[] = {
				{ "SliceSingle", Lua_SliceSingle },
				{ "SlicePipeline", Lua_SlicePipeline },
				{ NULL, NULL }
			};

			lua::RegisterNewClass(L, LuaRender::RenderSetMT, LuaRender::RenderSetMT, funcs);
		}

	private:
		std::vector<std::variant<Pipeline, Transformation>> _operations;
	};

	VertexDescriptor ColorOffsetDescriptor, ColorOffsetChampionDescriptor, PixelationDescriptor, BloomDescriptor,
		ColorCorrectionDescriptor, HQ4XDescriptor, ShockwaveDescriptor, OldTVDescriptor, WaterDescriptor,
		HallucinationDescriptor, ColorModDescriptor, WaterV2Descriptor, BackgroundDescriptor,
		WaterOverlayDescriptor, DizzyDescriptor, HeatWaveDescriptor, MirrorDescriptor;

	static size_t GetNumberOfVertices(GLuint type, const void* elements, uint32_t nElements) {
		switch (type) {
		case GL_UNSIGNED_BYTE:
			return *std::max_element((uint8_t*)elements, (uint8_t*)elements + nElements);

		case GL_UNSIGNED_SHORT:
			return *std::max_element((uint16_t*)elements, (uint16_t*)elements + nElements);

		case GL_UNSIGNED_INT:
			return *std::max_element((uint32_t*)elements, (uint32_t*)elements + nElements);

		default:
			return 0;
		}
	}

	void VertexBuffer::SetDescriptor(ShaderType shader) {
		switch (shader) {
		case SHADER_COLOR_OFFSET:
		case SHADER_COLOR_OFFSET_DOGMA:
		case SHADER_COLOR_OFFSET_GOLD:
			_descriptor = &ColorOffsetDescriptor;
			break;

		case SHADER_COLOR_OFFSET_CHAMPION:
			_descriptor = &ColorOffsetChampionDescriptor;
			break;

		case SHADER_PIXELATION:
			_descriptor = &PixelationDescriptor;
			break;

		case SHADER_BLOOM:
			_descriptor = &BloomDescriptor;
			break;

		case SHADER_COLOR_CORRECTION:
			_descriptor = &ColorCorrectionDescriptor;
			break;

		case SHADER_HQ4X:
			_descriptor = &HQ4XDescriptor;
			break;

		case SHADER_SHOCKWAVE:
			_descriptor = &ShockwaveDescriptor;
			break;

		case SHADER_OLDTV:
			_descriptor = &OldTVDescriptor;
			break;

		case SHADER_WATER:
			_descriptor = &WaterDescriptor;
			break;

		case SHADER_HALLUCINATION:
			_descriptor = &HallucinationDescriptor;
			break;

		case SHADER_COLOR_MOD:
			_descriptor = &ColorModDescriptor;
			break;

		case SHADER_WATER_V2:
			_descriptor = &WaterV2Descriptor;
			break;

		case SHADER_BACKGROUND:
			_descriptor = &BackgroundDescriptor;
			break;

		case SHADER_WATER_OVERLAY:
			_descriptor = &WaterOverlayDescriptor;
			break;

		case SHADER_DIZZY:
			_descriptor = &DizzyDescriptor;
			break;

		case SHADER_MIRROR:
			_descriptor = &MirrorDescriptor;
			break;

		case SHADER_HEAT_WAVE:
			_descriptor = &HeatWaveDescriptor;
			break;

		default:
			break;
		}

		_needRelease = false;
	}

	static float ProjectionWidth = 0, ProjectionHeight = 0;

	// Initialize the constant part of the projection matrix
	static void EarlyInitProjectionMatrix() {
		memset(ProjectionMatrix.data, 0, sizeof(float) * 16);
		ProjectionMatrix.row1.ptr[3] = -1.f;
		ProjectionMatrix.row2.ptr[3] = -1.f;
		ProjectionMatrix.row3.ptr[2] = -0.001f;
		ProjectionMatrix.row3.ptr[3] = -0.f;
		ProjectionMatrix.row4.ptr[3] = 1.f;
	}

	// Initialize the projection matrix. If the width and height have not changed since the last 
	// initialization, this does nothing.
	static void InitProjectionMatrix() {
		if (ProjectionWidth == g_OrthographicWidth && ProjectionHeight == g_OrthographicHeight) {
			return;
		}

		ProjectionMatrix.row1.ptr[0] = 2 / g_OrthographicWidth;
		ProjectionMatrix.row2.ptr[1] = 2 / g_OrthographicHeight;
	}

	void VertexBuffer::Bind() {
		if (!_descriptor)
			return;

		int i = 0;
		float* bufferOffset = _data;
		for (GLSLType type: _descriptor->GetTypes()) {
			GLint size = 0;
			GLsizei stride = _descriptor->GetSize() * sizeof(float);

			switch (type) {
			case GLSL_FLOAT:
				size = 1;
				break;

			case GLSL_VEC2:
				size = 2;
				break;

			case GLSL_VEC3:
				size = 3;
				break;

			case GLSL_VEC4:
				size = 4;
				break;

			default:
				throw std::runtime_error("Argh");
			}

			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, stride, bufferOffset);
			bufferOffset += size;
			++i;
		}
	}

	VertexBuffer* VertexBuffer::Slice(int first, int last, int nElements, VertexDescriptor const* descriptor) {
		if (first < last && last != -1) {
			std::ostringstream err;
			err << "Invalid slice range " << first << ":" << last << std::endl;
			throw std::runtime_error(err.str());
		}

		VertexBuffer* buffer = new VertexBuffer(last - first + 1, nElements, *descriptor);
		return buffer;
	}
}

namespace GL {
	bool GLADInitialized = false;
	HMODULE OpenGLLibrary;
}

void* load_fn(const char* name) {
	void* addr = wglGetProcAddress(name);
	if (!addr) {
		addr = GetProcAddress(GL::OpenGLLibrary, name);
	}
	ZHL::Log("[OpenGL] %s -> %p\n", name, addr);
	return addr;
}

struct ContextDataVisitor {
	std::string operator()(Entity_Bomb* p) {
		return Base("EntityBomb", p);
	}

	std::string operator()(Entity_Effect* p) {
		return Base("EntityEffect", p);
	}

	std::string operator()(Entity_Familiar* p) {
		return Base("EntityFamiliar", p);
	}

	std::string operator()(Entity_Knife* p) {
		return Base("EntityKnife", p);
	}

	std::string operator()(Entity_Laser* p) {
		return Base("EntityLaser", p);
	}

	std::string operator()(Entity_NPC* p) {
		return Base("EntityNPC", p);
	}

	std::string operator()(Entity_Pickup* p) {
		return Base("EntityPickup", p);
	}

	std::string operator()(Entity_Player* p) {
		return Base("EntityPlayer", p);
	}

	std::string operator()(Entity_Projectile* p) {
		return Base("EntityProjectile", p);
	}

	std::string operator()(Entity_Slot* p) {
		return Base("EntitySlot", p);
	}

	std::string operator()(Entity_Tear* p) {
		return Base("EntityTear", p);
	}

	std::string operator()(GridEntity_Rock* p) {
		return Base("GridEntityRock", p);
	}

	std::string operator()(AnimationState* p) {
		return Base("AnimationState", p);
	}

	std::string operator()(AnimationLayer* layer) {
		std::ostringstream str;
		str << "AnimationLayer (" << layer << ", id = " << layer->GetLayerID() << ")" << std::endl;
		return str.str();
	}

	std::string Base(const char* name, void* p) {
		std::ostringstream str;
		str << name << " (" << p << ")" << std::endl;
		return str.str();
	}
};

void __stdcall LuaPreDrawElements(KAGE_Graphics_RenderBatch* descriptor, GLenum mode, GLsizei count, GLenum type, const void* indices) {
	float* vertexBuffer = (float*)descriptor->_vertexBuffer.GetBase();
	size_t size = 0;
	switch (type) {
	case GL_UNSIGNED_BYTE:
		size = 1;
		break;

	case GL_UNSIGNED_SHORT:
		size = 2;
		break;

	case GL_UNSIGNED_INT:
		size = 4;
		break;

	default:
		throw std::runtime_error("Bad.");
	}

	const int callbackId = 1136;
	if (CallbackState.test(callbackId - 1000) && __ptr_g_KAGE_Graphics_Manager->currentRenderTarget != 0) {
		GL::InitProjectionMatrix();

		int currentShader; 
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentShader);

		int shaderId = -1;
		if (currentShader != 0) {
			for (int i = 0; i < SHADER_MAX; ++i) {
				if (__ptr_g_AllShaders[i]->GetShaderId() == currentShader) {
					shaderId = i;
					break;
				}
			}
		}

		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua::LuaCaller caller(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		caller.push(callbackId).pushnil();
		GL::VertexBuffer* buffer = new GL::VertexBuffer(vertexBuffer, (ShaderType)shaderId, GL::GetNumberOfVertices(type, indices, count), indices, count, type);
		GL::VertexBuffer** bufferUd = (GL::VertexBuffer**)caller.pushUd(sizeof(buffer), LuaRender::VertexBufferMT);
		*bufferUd = buffer;
		caller.push(shaderId);

		auto vertexBufferDescriptor = &descriptor->_vertexBuffer;
		auto it = LuaRender::VerticesRenderContext.find(vertexBufferDescriptor);
		if (it != LuaRender::VerticesRenderContext.end()) {
			// ZHL::Log("Found %d contexts for vertex buffer %p (from descriptor %p)\n", it->second.size(), vertexBufferDescriptor, descriptor);
			caller.pushUd<GL::RenderOperationContext>(LuaRender::ContextMT, &it->second);
		}
		else {
			// ZHL::Log("No context found for vertex buffer %p (from descriptor %p\n", vertexBufferDescriptor, descriptor);
			caller.pushnil();
		}

		// If the return value is a pipeline, override the default rendering.
		lua::LuaResults results = caller.call(1);
		if (!results) {
			switch (lua_type(L, -1)) {
			case LUA_TNUMBER:
			{
				int retShaderId = (int) lua_tointeger(L, -1);
				if (retShaderId < 0 || retShaderId >= ShaderType::SHADER_MAX) {
					if (retShaderId == -1)
						break;

					bool ok = false;
					for (std::unique_ptr<GL::Shader> const& shader : GL::Shader::_shaders) {
						if (retShaderId == shader->GetProgram()) {
							shader->Use();
							buffer->Bind();
							ok = true;
							break;
						}
					}

					if (!ok) {
						ZHL::Log("[FATAL] Invalid shader ID %d (neither a native shader, nor a registered shader)\n", retShaderId);
					}
				}
				else {
					glUseProgram(__ptr_g_AllShaders[retShaderId]->GetShaderId());
					buffer->Bind();
				}
				break;
			}

			case LUA_TUSERDATA:
				lua_getmetatable(L, -1);
				luaL_getmetatable(L, LuaRender::GLShaderMT);
				if (lua_rawequal(L, -1, -2)) {
					lua_pop(L, 2);
					GL::Shader* shader = *lua::GetRawUserdata<GL::Shader**>(L, -1, LuaRender::GLShaderMT);
					shader->Use();
					auto loc = glGetUniformLocation(shader->GetProgram(), "Transform");
					glUniformMatrix4fv(loc, 1, GL_TRUE, GL::ProjectionMatrix.data);
					buffer->Bind();
				}
				else {
					lua_pop(L, 1);
					luaL_getmetatable(L, LuaRender::PipelineMT);
					if (lua_rawequal(L, -1, -2)) {
						lua_pop(L, 2);
						GL::Pipeline* pipeline = lua::GetRawUserdata<GL::Pipeline*>(L, -1, LuaRender::PipelineMT);
						pipeline->Render();
					}
					else {
						lua_pop(L, 2);
						ZHL::Log("[ERROR] Invalid return value for MC_PRE_OPENGL_RENDER\n");
					}
				}
				break;

			case LUA_TNIL:
			case LUA_TNONE:
				break;

			default:
			{
				ZHL::Log("[ERROR] Invalid return value for MC_PRE_OPENGL_RENDER\n");
				break;
			}
			}
		}
	}

	glDrawElements(mode, count, type, indices);

	LuaRender::ElementsRenderContext[&descriptor->_indexBuffer].clear();
	LuaRender::VerticesRenderContext[&descriptor->_vertexBuffer].clear();
}

static void RegisterCustomRenderMetatables(lua_State* L) {
	lua::LuaStackProtector protector(L);

	GL::RenderSet::InitMetatable(L);
	GL::RenderOperationContext::InitMetatable(L);
	GL::Shader::InitMetatable(L);
	GL::GLSLValue::InitMetatables(L);
	GL::VertexDescriptor::InitMetatable(L);
	GL::Pipeline::InitMetatable(L);
	GL::VertexBuffer::InitMetatable(L);
	GL::Vertex::InitMetatable(L);
	GL::Elements::InitMetatable(L);
}

// ============================================================================
// Renderer

LUA_FUNCTION(lua_Renderer_LoadImage) {
	const char* path = luaL_checkstring(L, 1);
	KAGE_SmartPointer_ImageBase image;
	Manager::LoadImage(&image, path, __ptr_g_VertexAttributeDescriptor_Position, false);

	if (!image.image) {
		return luaL_error(L, "Image %s does not exist", path);
	}

	LuaImage* ud = new (lua_newuserdata(L, sizeof(LuaImage))) LuaImage;
	memset(ud, 0, sizeof(LuaImage));
	luaL_setmetatable(L, LuaRender::ImageMT);
	ud->image = image;
	return 1;
}

LUA_FUNCTION(Lua_Renderer_CreateImage) {
	uint32_t width = (uint32_t)luaL_checkinteger(L, 1);
	uint32_t height = (uint32_t)luaL_checkinteger(L, 2);
	const char* name = luaL_checkstring(L, 3);
	KColor color = KColor(0.0, 0.0, 0.0, 0.0);

	// prevent name clashes with real images (since these are added to the cache for some reason, even though they are never loaded)
	std::string trueName = REPENTOGON::StringFormat("%s.procedural", name);
	KAGE_SmartPointer_ImageBase pointer = KAGE_Graphics_ImageManager::CreateProceduralImage(width, height, trueName.c_str(), color);

	if (!pointer.image) {
		return luaL_error(L, "Unable to create Image");
	}

	LuaImage* ud = new (lua_newuserdata(L, sizeof(LuaImage))) LuaImage;
	memset(ud, 0, sizeof(LuaImage));
	luaL_setmetatable(L, LuaRender::ImageMT);
	ud->image = pointer;
	return 1;
}

LUA_FUNCTION(Lua_Renderer_RenderToImage) {
	LuaImage* luaImage = LuaRender::GetLuaImage(L);
	auto* image = luaImage->image.image;
	if ((image->_flags & (uint64_t)eImageFlag::PROCEDURAL) == 0)
	{
		return luaL_error(L, "Cannot use a non procedural image as the render target");
	}

	luaL_checktype(L, 2, LUA_TFUNCTION);
	int renderFn = lua_absindex(L, 2);

	lua::LuaStackProtector protector(L);
	auto* renderController = LuaSurfaceRenderController::NewUserdata(L);
	int renderControllerIdx = lua_absindex(L, -1);

	RenderToSurface(*image, [&](){
		lua_pushvalue(L, renderFn);
		lua::LuaResults results = lua::LuaCaller(L)
			.pushvalue(renderControllerIdx)
			.call(0);

		if (results.getResultCode() != LUA_OK)
		{
			if (lua_isstring(L, -1))
			{
				const char* msg = lua_tostring(L, -1);
				g_Game->GetConsole()->PrintError(REPENTOGON::StringFormat("An error occurred while Rendering to Surface \"%s\": %s\n", image->_name, msg));
			}
			else
			{
				g_Game->GetConsole()->PrintError(REPENTOGON::StringFormat("An error occurred while Rendering to Surface \"%s\"\n", image->_name));
			}
		}
		
		renderController->valid = false;
	});

	lua_pop(L, 1); // pop renderController

	return 0;
}

LUA_FUNCTION(lua_Renderer_StartTransformation) {
	LuaImage* luaImage = LuaRender::GetLuaImage(L);
	auto& pointer = luaImage->image;
	if ((pointer.image->_flags & (uint64_t)eImageFlag::PROCEDURAL) == 0)
	{
		return luaL_error(L, "Cannot use a non procedural image as the render target");
	}

	LuaTransformer* transformer = new (lua_newuserdata(L, sizeof(LuaTransformer))) LuaTransformer;
	transformer->_valid = true;
	luaL_setmetatable(L, LuaRender::TransformerMT);
	transformer->_output = pointer;
	return 1;
}

LUA_FUNCTION(Lua_Renderer_GLShader) {
	std::string vertexShader(luaL_checkstring(L, 1));
	std::string fragmentShader(luaL_checkstring(L, 2));
	GL::VertexDescriptor* descriptor = lua::GetRawUserdata<GL::VertexDescriptor*>(L, 3, LuaRender::VertexDescriptorMT);

	GL::Shader* shader;
	try {
		shader = new GL::Shader(vertexShader, fragmentShader, descriptor);
		std::unique_ptr<GL::Shader> ptr(shader);
		GL::Shader** ud = (GL::Shader**)lua_newuserdata(L, sizeof(shader));
		luaL_setmetatable(L, LuaRender::GLShaderMT);
		*ud = shader;
		GL::Shader::_shaders.push_back(std::move(ptr));
	}
	catch (std::runtime_error& err) {
		const char* error = err.what();
		ZHL::Log("%s\n", error);
		luaL_error(L, "Error while creating shader: %s", error);
	}
	
	return 1;
}

LUA_FUNCTION(Lua_Renderer_LoadShader)
{
	std::string path = luaL_checkstring(L, 1);
	if (!lua_istable(L, 2))
	{
		return luaL_typeerror(L, 2, lua_typename(L, LUA_TTABLE));
	}
	
	int descTbl = lua_absindex(L, 2);

	size_t numAttributes = (size_t)lua_rawlen(L, descTbl);
	std::vector<KAGE_Graphics_VertexAttributeDescriptor> descriptor;
	descriptor.reserve(numAttributes + 1); // + 1 for terminator

	// lua table to descriptor
	for (size_t i = 0; i < numAttributes; i++)
	{
		auto& attributeDesc = descriptor.emplace_back();

		int type = lua_rawgeti(L, descTbl, i + 1);
		if (!lua_istable(L, -1))
		{
			lua_pop(L, 1);
			return luaL_error(L, "vertex attribute %i: table expected", i + 1);
		}

		lua_rawgeti(L, -1, 1);
		if (!lua_isstring(L, -1))
		{
			lua_pop(L, 2);
			return luaL_error(L, "vertex attribute %i name: string expected", i + 1);
		}
		attributeDesc.name = lua_tostring(L, -1);
		lua_pop(L, 1); // pop name

		lua_rawgeti(L, -1, 2);
		if (!lua_isinteger(L, -1))
		{
			lua_pop(L, 2);
			return luaL_error(L, "vertex attribute %i format: integer expected", i + 1);
		}
		int format = (int)lua_tointeger(L, -1);
		if (!(1 <= format && format <= 8))
		{
			lua_pop(L, 2);
			return luaL_error(L, "vertex attribute %i format: invalid format", i + 1);
		}
		attributeDesc.format = format;
		lua_pop(L, 1); // pop format

		lua_pop(L, 1); // pop attribute
	}

	auto& terminator = descriptor.emplace_back();
	terminator.name = "";
	terminator.format = (size_t)eVertexAttributeFormat::TERMINATOR;

	KAGE_Graphics_Shader* shader = ShaderLoader::LoadShader(path, descriptor.data());
	if (!shader)
	{
		luaL_error(L, "Unable to load shader \"%s\"", path);
	}

	// confirm correct vertex descriptor
	if (!ShaderUtils::UsesVertexDescriptor(*shader, descriptor.data(), descriptor.size() - 1))
	{
		luaL_error(L, "Incorrect VertexDescriptor for \"%s\"", path);
	}

	LuaShader::NewUserdata(L, shader);
	
	return 1;
}

LUA_FUNCTION(Lua_Renderer_GetShaderByType)
{
	int shaderType = (int)luaL_checkinteger(L, 1);

	if (!(0 <= shaderType && shaderType < ShaderType::SHADER_MAX))
	{
		return luaL_argerror(L, 2, "invalid shader type");
	}

	LuaShader::Userdata* ud = LuaShader::NewUserdata(L, __ptr_g_AllShaders[shaderType]);
	return 1;
}

LUA_FUNCTION(Lua_Renderer_VertexDescriptor) {
	lua::place<GL::VertexDescriptor>(L, LuaRender::VertexDescriptorMT);
	return 1;
}

LUA_FUNCTION(Lua_Renderer_Pipeline) {
	lua::place<GL::Pipeline>(L, LuaRender::PipelineMT);
	return 1;
}

LUA_FUNCTION(Lua_Renderer_Vec2) {
	float x = (float) luaL_optnumber(L, 1, 0);
	float y = (float) luaL_optnumber(L, 2, 0);
	lua::place<GL::GLVec2>(L, LuaRender::GLVec2MT, x, y);
	return 1;
}

LUA_FUNCTION(Lua_Renderer_Vec3) {
	float x = (float) luaL_optnumber(L, 1, 0);
	float y = (float) luaL_optnumber(L, 2, 0);
	float z = (float) luaL_optnumber(L, 3, 0);
	lua::place<GL::GLVec3>(L, LuaRender::GLVec3MT, x, y, z);
	return 1;
}

LUA_FUNCTION(Lua_Renderer_Vec4) {
	float x = (float) luaL_optnumber(L, 1, 0);
	float y = (float) luaL_optnumber(L, 2, 0);
	float z = (float) luaL_optnumber(L, 3, 0);
	float w = (float) luaL_optnumber(L, 4, 0);
	lua::place<GL::GLVec4>(L, LuaRender::GLVec4MT, x, y, z, w);
	return 1;
}

LUA_FUNCTION(Lua_Renderer_ProjectionMatrix) {
	GL::GLMat4* matrix = lua::place<GL::GLMat4>(L, LuaRender::GLMat4MT);
	memcpy(matrix->data, GL::ProjectionMatrix.data, sizeof(float) * 16);
	return 1;
}

LUA_FUNCTION(Lua_Renderer_RenderSet) {
	lua::place<GL::RenderSet>(L, LuaRender::RenderSetMT);
	return 1;
}

LUA_FUNCTION(Lua_Renderer_GetPixelationAmount)
{
	lua_pushnumber(L, g_ANM2_PixelationAmount);
	return 1;
}

LUA_FUNCTION(Lua_Renderer_GetClipPaneNormal)
{
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), g_ANM2_ClipPaneNormal);
	return 1;
}

LUA_FUNCTION(Lua_Renderer_GetClipPaneThreshold)
{
	lua_pushnumber(L, g_ANM2_ClipPaneThreshold);
	return 1;
}

// ============================================================================
// Hooks

HOOK_METHOD(KAGE_Graphics_GraphicsBufferObject, Allocate, (uint32_t n) -> void*) {
	void* result = super(n);
	if (!EnableCustomRendering) {
		return result;
	}

	// ZHL::Log("Allocating memory in descriptor %p. Amount is %u, base is located at %p, result is at %p\n", this, n, GetBase(), result);

	ptrdiff_t diff = (ptrdiff_t)result - (ptrdiff_t)GetBase();
	if (diff < 0) {
		std::ostringstream err;
		err << "[FATAL] Difference between buffer new data and buffer base cannot be negative" << std::endl;
		ZHL::Log(err.str().c_str());
		throw std::runtime_error(err.str());
	}

	uint32_t offset = diff / elementSize;

	if (n == LuaRender::ELEMENT_BUFFER) {
		LuaRender::ElementsRenderContext[this].push_back(LuaRender::RenderContextQueue);
	}
	else if (n == LuaRender::VERTEX_BUFFER) {
		LuaRender::VerticesRenderContext[this].push_back(LuaRender::RenderContextQueue);
	}
	else {
		std::ostringstream err;
		err << "[FATAL] Unknown memory pool allocation size " << n << std::endl;
		ZHL::Log(err.str().c_str());
		throw std::runtime_error(err.str());
	}
	return result;
}

HOOK_METHOD(GridEntity_Rock, Render, (Vector& offset) -> void) {
	LuaRender::ScopedContext<GridEntity_Rock*> context(this);
	super(offset);
}

HOOK_METHOD(AnimationState, Render, (Vector const& position, Vector const& topLeftClamp, Vector const& bottomRightClamp) -> void) {
	LuaRender::ScopedContext<AnimationState*> context(this);
	super(position, topLeftClamp, bottomRightClamp);
}

HOOK_METHOD(AnimationLayer, RenderFrame, (Vector const& position, int unk, Vector const& topLeftClamp, Vector const& bottomRightClamp, ANM2* animation) -> void) {
	LuaRender::ScopedContext<AnimationLayer*> context(this);
	super(position, unk, topLeftClamp, bottomRightClamp, animation);
}

HOOK_METHOD(Entity_NPC, Render, (Vector* offset) -> void) {
	LuaRender::ScopedContext<Entity_NPC*> context(this);
	super(offset);
}

HOOK_METHOD(Entity_Player, Render, (Vector* offset) -> void) {
	LuaRender::ScopedContext<Entity_Player*> context(this);
	super(offset);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	if (!GL::GLADInitialized) {
		GL::OpenGLLibrary = LoadLibrary("opengl32.dll");
		if (!GL::OpenGLLibrary || !gladLoadGLLoader(load_fn)) {
			abort();
		}
		GL::GLADInitialized = true;

		//glEnable(GL_DEBUG_OUTPUT);
		//glDebugMessageCallback(GL::OpenGLErrorCallback, 0);
	}

	lua_State* L = _state;
	lua::LuaStackProtector protector(L);
	RegisterImageClass(L);
	LuaShader::RegisterUserdataClass(L);
	LuaSurfaceRenderController::RegisterUserdataClass(L);
	RegisterTransformerClass(L);
	RegisterQuadClasses(L);
	RegisterCustomRenderMetatables(L);

	lua_register(L, "SourceQuad", lua_SourceQuad_new);
	lua_register(L, "DestinationQuad", lua_DestinationQuad_new);

	lua_newtable(L);

	luaL_Reg renderFunctions[] = {
		{ "LoadImage", lua_Renderer_LoadImage },
		{ "CreateImage", Lua_Renderer_CreateImage },
		{ "RenderToImage", Lua_Renderer_RenderToImage },
		{ "StartTransformation", lua_Renderer_StartTransformation },
		// { "GLShader", Lua_Renderer_GLShader },
		{ "GetShaderByType", Lua_Renderer_GetShaderByType },
		{ "LoadShader", Lua_Renderer_LoadShader },
		// { "Vec2", Lua_Renderer_Vec2 },
		// { "Vec3", Lua_Renderer_Vec3 },
		// { "Vec4", Lua_Renderer_Vec4 },
		// { "ProjectionMatrix", Lua_Renderer_ProjectionMatrix },
		// { "Pipeline", Lua_Renderer_Pipeline },
		// { "VertexDescriptor", Lua_Renderer_VertexDescriptor},
		// { "RenderSet", Lua_Renderer_RenderSet },
		{ "GetPixelationAmount", Lua_Renderer_GetPixelationAmount },
		{ "GetClipPaneNormal", Lua_Renderer_GetClipPaneNormal },
		{ "GetClipPaneThreshold", Lua_Renderer_GetClipPaneThreshold },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, renderFunctions, 0);

	// GLSLType table
	lua_pushstring(L, "GLSLType");
	lua_newtable(L);
	lua::TableAssoc(L, "Float", GL::GLSLType::GLSL_FLOAT);
	lua::TableAssoc(L, "Vec2", GL::GLSLType::GLSL_VEC2);
	lua::TableAssoc(L, "Vec3", GL::GLSLType::GLSL_VEC3);
	lua::TableAssoc(L, "Vec4", GL::GLSLType::GLSL_VEC4);
	lua_rawset(L, -3);

	lua_pushstring(L, "ShaderType");
	lua_newtable(L);
	lua::TableAssoc(L, "SHADER_COLOR_OFFSET", SHADER_COLOR_OFFSET);
	lua::TableAssoc(L, "SHADER_PIXELATION", SHADER_PIXELATION);
	lua::TableAssoc(L, "SHADER_BLOOM", SHADER_BLOOM);
	lua::TableAssoc(L, "SHADER_COLOR_CORRECTION", SHADER_COLOR_CORRECTION);
	lua::TableAssoc(L, "SHADER_HQ4X", SHADER_HQ4X);
	lua::TableAssoc(L, "SHADER_SHOCKWAVE", SHADER_SHOCKWAVE);
	lua::TableAssoc(L, "SHADER_OLDTV", SHADER_OLDTV);
	lua::TableAssoc(L, "SHADER_WATER", SHADER_WATER);
	lua::TableAssoc(L, "SHADER_HALLUCINATION", SHADER_HALLUCINATION);
	lua::TableAssoc(L, "SHADER_COLOR_MOD", SHADER_COLOR_MOD);
	lua::TableAssoc(L, "SHADER_COLOR_OFFSET_CHAMPION", SHADER_COLOR_OFFSET_CHAMPION);
	lua::TableAssoc(L, "SHADER_WATER_V2", SHADER_WATER_V2);
	lua::TableAssoc(L, "SHADER_BACKGROUND", SHADER_BACKGROUND);
	lua::TableAssoc(L, "SHADER_WATER_OVERLAY", SHADER_WATER_OVERLAY);
	lua::TableAssoc(L, "SHADER_UNK", SHADER_UNK);
	lua::TableAssoc(L, "SHADER_COLOR_OFFSET_DOGMA", SHADER_COLOR_OFFSET_DOGMA);
	lua::TableAssoc(L, "SHADER_COLOR_OFFSET_GOLD", SHADER_COLOR_OFFSET_GOLD);
	lua::TableAssoc(L, "SHADER_DIZZY", SHADER_DIZZY);
	lua::TableAssoc(L, "SHADER_HEAT_WAVE", SHADER_HEAT_WAVE);
	lua::TableAssoc(L, "SHADER_MIRROR", SHADER_MIRROR);
	lua_rawset(L, -3);

	lua_pushstring(L, "VertexAttributeFormat");
	lua_newtable(L);
	lua::TableAssoc(L, "FLOAT", (int)eVertexAttributeFormat::FLOAT);
	lua::TableAssoc(L, "VEC2", (int)eVertexAttributeFormat::VEC_2);
	lua::TableAssoc(L, "VEC3", (int)eVertexAttributeFormat::VEC_3);
	lua::TableAssoc(L, "VEC4", (int)eVertexAttributeFormat::VEC_4);
	lua::TableAssoc(L, "POSITION", (int)eVertexAttributeFormat::POSITION);
	lua::TableAssoc(L, "COLOR", (int)eVertexAttributeFormat::COLOR);
	lua::TableAssoc(L, "TEX_COORD", (int)eVertexAttributeFormat::TEX_COORD);
	lua_rawset(L, -3);

	lua_setglobal(L, "Renderer");
}

// ============================================================================
// Globals

namespace LuaRender {
	void PatchglDrawElements() {
		// Call to glDrawElements in the second loop in Manager::apply_frame_images
		SigScan secondLoopScan("ff15????????8b0d????????ff75f8ff75ec8b0157");
		// Call to glDrawElements in ImageBase::apply_image
		SigScan applyImageScan("ff15????????8b0d????????ff75e8ff75f48b0157");

		secondLoopScan.Scan();
		applyImageScan.Scan();

		void* secondLoop = secondLoopScan.GetAddress();
		void* applyImage = applyImageScan.GetAddress();

		ASMPatch secondLoopPatch, applyImagePatch;
		secondLoopPatch.Push(ASMPatch::Registers::ESI, 0); // render descriptor
		secondLoopPatch.AddInternalCall(LuaPreDrawElements);
		secondLoopPatch.AddRelativeJump((char*)secondLoop + 0x6);

		applyImagePatch.Push(ASMPatch::Registers::ESI); // render descriptor
		applyImagePatch.AddInternalCall(LuaPreDrawElements);
		applyImagePatch.AddRelativeJump((char*)applyImage + 0x6);

		if (EnableCustomRendering) {
			sASMPatcher.PatchAt(secondLoop, &secondLoopPatch);
			sASMPatcher.PatchAt(applyImage, &applyImagePatch);
		}
	}

#define PAD(D, T, N) D.AddAttribute(T, N)

	void InitShaders() {
		using namespace GL; 
		PAD(ColorOffsetDescriptor, GLSL_VEC3, "Position");
		PAD(ColorOffsetDescriptor, GLSL_VEC4, "Color");
		PAD(ColorOffsetDescriptor, GLSL_VEC2, "TexCoord");
		PAD(ColorOffsetDescriptor, GLSL_VEC4, "ColorizeIn");
		PAD(ColorOffsetDescriptor, GLSL_VEC3, "ColorOffsetIn");
		PAD(ColorOffsetDescriptor, GLSL_VEC2, "TextureSize");
		PAD(ColorOffsetDescriptor, GLSL_FLOAT, "PixelationAmount");
		PAD(ColorOffsetDescriptor, GLSL_VEC3, "ClipPlane");

		PAD(ColorOffsetChampionDescriptor, GLSL_VEC3, "Position");
		PAD(ColorOffsetChampionDescriptor, GLSL_VEC4, "Color");
		PAD(ColorOffsetChampionDescriptor, GLSL_VEC2, "TexCoord");
		PAD(ColorOffsetChampionDescriptor, GLSL_VEC4, "ColorizeIn");
		PAD(ColorOffsetChampionDescriptor, GLSL_VEC3, "ColorOffsetIn");
		PAD(ColorOffsetChampionDescriptor, GLSL_VEC2, "TextureSize");
		PAD(ColorOffsetChampionDescriptor, GLSL_FLOAT, "PixelationAmount");
		PAD(ColorOffsetChampionDescriptor, GLSL_VEC3, "ClipPlane");
		PAD(ColorOffsetChampionDescriptor, GLSL_VEC4, "ChampionColorIn");

		PAD(PixelationDescriptor, GLSL_VEC3, "Position");
		PAD(PixelationDescriptor, GLSL_VEC4, "Color");
		PAD(PixelationDescriptor, GLSL_VEC2, "TexCoord");
		PAD(PixelationDescriptor, GLSL_FLOAT, "PixelationAmount");
		PAD(PixelationDescriptor, GLSL_VEC4, "ScreenSize");

		PAD(BloomDescriptor, GLSL_VEC3, "Position");
		PAD(BloomDescriptor, GLSL_VEC4, "Color");
		PAD(BloomDescriptor, GLSL_VEC2, "TexCoord");
		PAD(BloomDescriptor, GLSL_FLOAT, "BloomAmount");
		PAD(BloomDescriptor, GLSL_VEC2, "Ratio");

		PAD(ColorCorrectionDescriptor, GLSL_VEC3, "Position");
		PAD(ColorCorrectionDescriptor, GLSL_VEC2, "TexCoord");
		PAD(ColorCorrectionDescriptor, GLSL_FLOAT, "Exposure");
		PAD(ColorCorrectionDescriptor, GLSL_FLOAT, "Gamma");

		PAD(HQ4XDescriptor, GLSL_VEC3, "Position");
		PAD(HQ4XDescriptor, GLSL_VEC2, "TexCoord");
		PAD(HQ4XDescriptor, GLSL_VEC3, "ScreenSizePow2");

		PAD(ShockwaveDescriptor, GLSL_VEC3, "Position");
		PAD(ShockwaveDescriptor, GLSL_VEC4, "Color");
		PAD(ShockwaveDescriptor, GLSL_VEC2, "TexCoord");
		PAD(ShockwaveDescriptor, GLSL_VEC4, "Shockwave1");
		PAD(ShockwaveDescriptor, GLSL_VEC4, "Shockwave2");
		PAD(ShockwaveDescriptor, GLSL_VEC2, "Ratio");

		PAD(OldTVDescriptor, GLSL_VEC3, "Position");
		PAD(OldTVDescriptor, GLSL_VEC2, "TexCoord");
		PAD(OldTVDescriptor, GLSL_FLOAT, "Time");
		PAD(OldTVDescriptor, GLSL_FLOAT, "Amount");
		PAD(OldTVDescriptor, GLSL_VEC4, "ScreenSize");

		PAD(WaterDescriptor, GLSL_VEC3, "Position");
		PAD(WaterDescriptor, GLSL_VEC4, "Color");
		PAD(WaterDescriptor, GLSL_VEC2, "TexCoord");
		PAD(WaterDescriptor, GLSL_FLOAT, "Time");
		PAD(WaterDescriptor, GLSL_FLOAT, "Amount");
		PAD(WaterDescriptor, GLSL_VEC2, "Ratio");
		PAD(WaterDescriptor, GLSL_VEC2, "Offset");
		PAD(WaterDescriptor, GLSL_VEC2, "PlayerPos");
		PAD(WaterDescriptor, GLSL_VEC2, "PlayerVel");

		PAD(HallucinationDescriptor, GLSL_VEC3, "Position");
		PAD(HallucinationDescriptor, GLSL_VEC2, "TexCoord");
		PAD(HallucinationDescriptor, GLSL_FLOAT, "Amount");
		PAD(HallucinationDescriptor, GLSL_VEC4, "ScreenSize");

		PAD(ColorModDescriptor, GLSL_VEC3, "Position");
		PAD(ColorModDescriptor, GLSL_VEC4, "Color");
		PAD(ColorModDescriptor, GLSL_VEC2, "TexCoord");
		PAD(ColorModDescriptor, GLSL_FLOAT, "Amount");
		PAD(ColorModDescriptor, GLSL_FLOAT, "Brightness");
		PAD(ColorModDescriptor, GLSL_FLOAT, "Contrast");

		PAD(WaterV2Descriptor, GLSL_VEC3, "Position");
		PAD(WaterV2Descriptor, GLSL_VEC4, "Color");
		PAD(WaterV2Descriptor, GLSL_VEC2, "TexCoord");
		PAD(WaterV2Descriptor, GLSL_FLOAT, "Time");
		PAD(WaterV2Descriptor, GLSL_VEC3, "ColorMul");
		PAD(WaterV2Descriptor, GLSL_VEC2, "CurrentDir");
		PAD(WaterV2Descriptor, GLSL_VEC2, "TextureSize");
		PAD(WaterV2Descriptor, GLSL_FLOAT, "PixelationAmount");

		PAD(BackgroundDescriptor, GLSL_VEC3, "Position");
		PAD(BackgroundDescriptor, GLSL_VEC4, "Color");
		PAD(BackgroundDescriptor, GLSL_VEC2, "TexCoord");
		PAD(BackgroundDescriptor, GLSL_VEC2, "WrapSize");

		PAD(WaterOverlayDescriptor, GLSL_VEC3, "Position");
		PAD(WaterOverlayDescriptor, GLSL_VEC4, "Color");
		PAD(WaterOverlayDescriptor, GLSL_VEC2, "TexCoord");
		PAD(WaterOverlayDescriptor, GLSL_VEC4, "Ratio");
		PAD(WaterOverlayDescriptor, GLSL_FLOAT, "Amount");
		PAD(WaterOverlayDescriptor, GLSL_FLOAT, "PixelationAmount");

		PAD(DizzyDescriptor, GLSL_VEC3, "Position");
		PAD(DizzyDescriptor, GLSL_VEC4, "Color");
		PAD(DizzyDescriptor, GLSL_VEC2, "TexCoord");
		PAD(DizzyDescriptor, GLSL_VEC2, "TextureSize");
		PAD(DizzyDescriptor, GLSL_VEC4, "Ratio");
		PAD(DizzyDescriptor, GLSL_FLOAT, "Time");
		PAD(DizzyDescriptor, GLSL_FLOAT, "Amount");
		PAD(DizzyDescriptor, GLSL_FLOAT, "PixelationAmount");

		PAD(MirrorDescriptor, GLSL_VEC3, "Position");
		PAD(MirrorDescriptor, GLSL_VEC4, "Color");
		PAD(MirrorDescriptor, GLSL_VEC2, "TexCoord");
		PAD(MirrorDescriptor, GLSL_VEC2, "TextureSize");
		PAD(MirrorDescriptor, GLSL_VEC4, "Ratio");
		PAD(MirrorDescriptor, GLSL_FLOAT, "Time");
		PAD(MirrorDescriptor, GLSL_FLOAT, "Amount");
		PAD(MirrorDescriptor, GLSL_FLOAT, "PixelationAmount");

		PAD(HeatWaveDescriptor, GLSL_VEC3, "Position");
		PAD(HeatWaveDescriptor, GLSL_VEC4, "Color");
		PAD(HeatWaveDescriptor, GLSL_VEC2, "TexCoord");
		PAD(HeatWaveDescriptor, GLSL_VEC4, "Ratio");
		PAD(HeatWaveDescriptor, GLSL_FLOAT, "Amount");
		PAD(HeatWaveDescriptor, GLSL_FLOAT, "PixelationAmount");

		GL::EarlyInitProjectionMatrix();
	}
#undef PAD
}