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

using LuaRender::LuaImage;
using LuaRender::LuaTransformer;
using LuaRender::Transformation;

LuaRender::ContextQueue LuaRender::RenderContextQueue;
LuaRender::RenderContext LuaRender::ElementsRenderContext;
LuaRender::RenderContext LuaRender::VerticesRenderContext;

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
	lua::RegisterNewClass(L, LuaRender::TransformerMT, LuaRender::TransformerMT, functions);
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
	*component = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
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
		{ "SetTopRight", lua_Quad_SetTopRight },
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
			return lua::GetUserdata<GLFloat*>(L, 1, LuaRender::GLFloatMT);
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
			return lua::GetUserdata<GLMat2*>(L, 1, LuaRender::GLMat2MT);
		}

		template<>
		static inline GLMat3* Get<GLMat3>(lua_State* L) {
			return lua::GetUserdata<GLMat3*>(L, 1, LuaRender::GLMat3MT);
		}

		template<>
		static inline GLMat4* Get<GLMat4>(lua_State* L) {
			return lua::GetUserdata<GLMat4*>(L, 1, LuaRender::GLMat4MT);
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
			VertexDescriptor* descriptor = lua::GetUserdata<VertexDescriptor*>(L, 1, LuaRender::VertexDescriptorMT);
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
			Elements** ud = lua::GetUserdata<Elements**>(L, 1, LuaRender::ElementsArrayMT);
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
			Elements** ud = lua::GetUserdata<Elements**>(L, 1, LuaRender::ElementsArrayMT);
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
			Elements** ud = lua::GetUserdata<Elements**>(L, 1, LuaRender::ElementsArrayMT);
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
			Vertex* vertex = lua::GetUserdata<Vertex*>(L, 1, LuaRender::VertexMT);
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
			Vertex* vertex = lua::GetUserdata<Vertex*>(L, 1, LuaRender::VertexMT);
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
			T(vertex->_data + offset) = *lua::GetUserdata<T*>(L, 3, mt);
		}

		LUA_FUNCTION(Lua_newindex) {
			Vertex* vertex = lua::GetUserdata<Vertex*>(L, 1, LuaRender::VertexMT);
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
			VertexBuffer** buffer = lua::GetUserdata<VertexBuffer**>(L, 1, LuaRender::VertexBufferMT);
			(*buffer)->GetVertex(L, (size_t) luaL_checkinteger(L, 2));
			return 1;
		}

		LUA_FUNCTION(Lua_GetElements) {
			VertexBuffer** buffer = lua::GetUserdata<VertexBuffer**>(L, 1, LuaRender::VertexBufferMT);
			Elements** ud = (Elements**)lua_newuserdata(L, sizeof(void*));
			*ud = &(*buffer)->_elements;
			luaL_setmetatable(L, LuaRender::ElementsArrayMT);
			return 1;
		}

		LUA_FUNCTION(Lua_gc) {
			VertexBuffer** buffer = lua::GetUserdata<VertexBuffer**>(L, 1, LuaRender::VertexBufferMT);
			/* FILE* f = fopen("repentogon.log", "a");
			fprintf(f, "Lua GC freeing vertex buffer at %p\n", *buffer);
			fclose(f); */
			delete *buffer;
			return 0;
		}

		LUA_FUNCTION(Lua_len) {
			VertexBuffer** buffer = lua::GetUserdata<VertexBuffer**>(L, 1, LuaRender::VertexBufferMT);
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
			Shader** shader = lua::GetUserdata<Shader**>(L, 1, LuaRender::ShaderMT);
			std::string name(luaL_checkstring(L, 2));
			GLSLValue* value = lua::GetUserdata<GLSLValue*>(L, 3, mt);
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

			lua::RegisterNewClass(L, LuaRender::ShaderMT, LuaRender::ShaderMT, funcs);
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
			Pipeline* pipeline = lua::GetUserdata<Pipeline*>(L, 1, LuaRender::PipelineMT);
			size_t nbVertices = (size_t) luaL_checkinteger(L, 2);
			Shader** shader = lua::GetUserdata<Shader**>(L, 3, LuaRender::ShaderMT);
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
			RenderOperationContext* context = lua::GetUserdata<RenderOperationContext*>(L, 1, LuaRender::ContextMT);
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
			VertexBuffer** buffer = lua::GetUserdata<VertexBuffer**>(L, 1, LuaRender::VertexBufferMT);
			Shader** shader = lua::GetUserdata<Shader**>(L, 2, LuaRender::ShaderMT);
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

void __stdcall LuaPreDrawElements(KAGE_Graphics_RenderDescriptor* descriptor, GLenum mode, GLsizei count, GLenum type, const void* indices) {
	float* vertexBuffer = (float*)descriptor->vertices.GetBase();
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

	int callbackId = 1136;
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

		auto vertexBufferDescriptor = &descriptor->vertices;
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
				luaL_getmetatable(L, LuaRender::ShaderMT);
				if (lua_rawequal(L, -1, -2)) {
					lua_pop(L, 2);
					GL::Shader* shader = *lua::GetUserdata<GL::Shader**>(L, -1, LuaRender::ShaderMT);
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
						GL::Pipeline* pipeline = lua::GetUserdata<GL::Pipeline*>(L, -1, LuaRender::PipelineMT);
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

	LuaRender::ElementsRenderContext[&descriptor->elements].clear();
	LuaRender::VerticesRenderContext[&descriptor->vertices].clear();
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

LUA_FUNCTION(Lua_Renderer_Shader) {
	std::string vertexShader(luaL_checkstring(L, 1));
	std::string fragmentShader(luaL_checkstring(L, 2));
	GL::VertexDescriptor* descriptor = lua::GetUserdata<GL::VertexDescriptor*>(L, 3, LuaRender::VertexDescriptorMT);

	GL::Shader* shader;
	try {
		shader = new GL::Shader(vertexShader, fragmentShader, descriptor);
		std::unique_ptr<GL::Shader> ptr(shader);
		GL::Shader** ud = (GL::Shader**)lua_newuserdata(L, sizeof(shader));
		luaL_setmetatable(L, LuaRender::ShaderMT);
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

// ============================================================================
// Hooks

HOOK_METHOD(KAGE_Memory_MemoryPoolDescriptor, Allocate, (uint32_t n) -> void*) {
	void* result = super(n);
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
	RegisterTransformerClass(L);
	RegisterQuadClasses(L);
	RegisterCustomRenderMetatables(L);

	lua_register(L, "SourceQuad", lua_SourceQuad_new);
	lua_register(L, "DestinationQuad", lua_DestinationQuad_new);

	lua_newtable(L);

	luaL_Reg renderFunctions[] = {
		{ "LoadImage", lua_Renderer_LoadImage },
		{ "StartTransformation", lua_Renderer_StartTransformation },
		{ "Shader", Lua_Renderer_Shader },
		{ "Vec2", Lua_Renderer_Vec2 },
		{ "Vec3", Lua_Renderer_Vec3 },
		{ "Vec4", Lua_Renderer_Vec4 },
		{ "ProjectionMatrix", Lua_Renderer_ProjectionMatrix },
		{ "Pipeline", Lua_Renderer_Pipeline },
		{ "VertexDescriptor", Lua_Renderer_VertexDescriptor},
		{ "RenderSet", Lua_Renderer_RenderSet },
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

		// sASMPatcher.PatchAt(secondLoop, &secondLoopPatch);
		// sASMPatcher.PatchAt(applyImage, &applyImagePatch);
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