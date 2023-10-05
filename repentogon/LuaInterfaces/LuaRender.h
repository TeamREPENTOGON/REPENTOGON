#pragma once

#include <deque>
#include <map>
#include <variant>
#include <vector>

#include "libzhl.h"
#include "IsaacRepentance.h"

namespace LuaRender {
	struct LuaImage {
		ReferenceCounter_ImageBase image;
	};

	struct Transformation {
		ReferenceCounter_ImageBase _input;
		SourceQuad _source;
		DestinationQuad _dest;
		KAGE_Graphics_Color _color1;
		KAGE_Graphics_Color _color2;
		KAGE_Graphics_Color _color3;
		KAGE_Graphics_Color _color4;
	};

	struct LuaTransformer {
		ReferenceCounter_ImageBase _output;
		std::vector<Transformation> _transformations;
		bool _valid;
	};

	LuaImage* GetLuaImage(lua_State* L, int idx = 1);
	LuaTransformer* GetTransformer(lua_State* L, int idx = 1);
	SourceQuad* GetSourceQuad(lua_State* L, int idx = 1);
	DestinationQuad* GetDestQuad(lua_State* L, int idx = 1);

	constexpr static const char* ImageMT = "Image";
	constexpr static const char* TransformerMT = "Transformer";
	constexpr static const char* SourceQuadMT = "SourceQuad";
	constexpr static const char* DestinationQuadMT = "DestinationQuad";
	constexpr static const char* QuadMT = "Quad";

	constexpr static const char* VertexBufferMT = "VertexBuffer";
	constexpr static const char* ElementsArrayMT = "ElementsArray";
	constexpr static const char* ShaderMT = "Shader";
	constexpr static const char* GLFloatMT = "GLFloat";
	constexpr static const char* GLVec2MT = "GLVec2";
	constexpr static const char* GLVec3MT = "GLVec3";
	constexpr static const char* GLVec4MT = "GLVec4";
	// constexpr static const char* GLVec2PtrMT = "GLVec2Ptr";
	// constexpr static const char* GLVec3PtrMT = "GLVec3Ptr";
	// constexpr static const char* GLVec4PtrMT = "GLVec4Ptr";
	constexpr static const char* GLMat2MT = "GLMat2";
	constexpr static const char* GLMat3MT = "GLMat3";
	constexpr static const char* GLMat4MT = "GLMat4";
	constexpr static const char* VertexDescriptorMT = "VertexDescriptor";
	constexpr static const char* VertexMT = "Vertex";
	constexpr static const char* PipelineMT = "Pipeline";
	constexpr static const char* ContextMT = "Context";
	constexpr static const char* RenderSetMT = "RenderSet";

	void PatchglDrawElements();
	void InitShaders();

	struct Layer { unsigned int id;  };

	typedef std::variant <
		Entity_Bomb*,
		Entity_Effect*,
		Entity_Familiar*,
		Entity_Knife*,
		Entity_Laser*,
		Entity_NPC*,
		Entity_Pickup*,
		Entity_Player*,
		Entity_Projectile*,
		Entity_Slot*,
		Entity_Tear*,
		GridEntity_Rock*,
		AnimationState*,
		AnimationLayer*
	> Context;

	enum ContextType {
		RENDER_CTX_ENTITY_BOMB,
		RENDER_CTX_ENTITY_EFFECT,
		RENDER_CTX_ENTITY_FAMILIAR,
		RENDER_CTX_ENTITY_KNIFE,
		RENDER_CTX_ENTITY_LASER,
		RENDER_CTX_ENTITY_NPC,
		RENDER_CTX_ENTITY_PICKUP,
		RENDER_CTX_ENTITY_PLAYER,
		RENDER_CTX_ENTITY_PROJECTILE,
		RENDER_CTX_ENTITY_SLOT,
		RENDER_CTX_ENTITY_TEAR,
		RENDER_CTX_GRIDENTITY_ROCK,
		RENDER_CTX_ANIMATION_STATE,
		RENDER_CTX_ANIMATION_LAYER
	};

	typedef std::deque<Context> ContextQueue;

	extern ContextQueue RenderContextQueue;
	
	template<typename T>
	class ScopedContext {
	public:
		ScopedContext(T const& t) {
			static_assert(IsInPackV<T, Context>, "Invalid type in context stack");

			_n = RenderContextQueue.size();
			Context c = t;
			RenderContextQueue.push_back(c);
		}

		~ScopedContext() {
			if (RenderContextQueue.size() != _n + 1) {
				std::ostringstream err;
				err << "[FATAL] Inconsistent render context stack" << std::endl;
				ZHL::Log(err.str().c_str());
				abort(); // Cannot throw an exception in a destructor, abort instead
			}

			RenderContextQueue.pop_back();
		}

	private:
		size_t _n;
	};

	typedef std::vector<ContextQueue> ContextArray;
	typedef std::map<KAGE_Memory_MemoryPoolDescriptor*, ContextArray> RenderContext;

	extern RenderContext ElementsRenderContext;
	extern RenderContext VerticesRenderContext;

	static constexpr const uint32_t ELEMENT_BUFFER = 6;
	static constexpr const uint32_t VERTEX_BUFFER = 4;
}