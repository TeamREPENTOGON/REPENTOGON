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

	/* Custom rendering: contexts.
	 *
	 * In custom rendering, a context is information tied to a render operation
	 * that can be used to determine what is being rendered.
	 * 
	 * For instance, a context composed of an EntityNPC*, AnimationState* and 
	 * AnimationLayer* gives information on the specific EntityNPC being 
	 * rendered, its animation data and the specific layer of animation that is
	 * being rendered. This information is not available in the recorded render
	 * operation (in the vertex / elements buffer) because it is useless to 
	 * perform the actual render. However, it is crucial for modders because this 
	 * will allow them to identify what is being drawn by a specific render 
	 * operation.
	 * 
	 * The system works as follows: whenever the game calls the glDrawElements 
	 * function, it fires a callback called MC_PRE_OPENGL_RENDER. This callback
	 * takes as parameter the elements array given to OpenGL, the currently bound
	 * shader's ID and a RenderContext (see below). 
	 *	- The elements array has the same meaning as in C OpenGL: an array containing
	 * indices of vertices in a related vertex array. In the context of Isaac the 
	 * elements array always contains 6 elements: 2 pairs of 3 elements each, which
	 * define two triangles that compose a rectangle (everything in Isaac is rendered
	 * as rectangles made of two adjacent triangles).
	 *	- The shader ID is the result of the glCreateProgram call. Native game shaders
	 * are enumerated in a table in Lua. User made shaders have an ID that is exposed 
	 * in Lua when they are built using the Renderer.Shader() function.
	 *	- The RenderContext contains information on all render operations registered 
	 * in the elements array. Each entry in the RenderContext corresponds to 6 values
	 * in the elements array. Entry index 'i' corresponds to values from '6 * i' (inclusive)
	 * to 6 * (i + 1) (exclusive).
	 * 
	 * In order to determine the context associated with a given render operation, we 
	 * hook the relevant functions on the path to the call to KAGE::Graphics::ImageBase::apply_data.
	 * This function is used to register the data relating to a render operation inside
	 * a KAGE::Graphics::ImageBase object. To give an example of how to get a context for 
	 * the rendering of an EntityNPC, we hook:
	 *	- void EntityNPC::Render() 
	 *	- void AnimationState::Render()
	 *	- void AnimationLayer::Render()
	 * The call to EntityNPC::Render gives information as to which entity in particular 
	 * is being rendered. The call to AnimationState::Render gives information on the 
	 * ANM2 of the EntityNPC. The call to AnimationLayer::Render gives information on 
	 * the specific layer being rendered. AnimationLayer::Render calls Image***::Render
	 * which finally arrives in ImageBase::apply_data. The context for an EntityNPC 
	 * is therefore made of three subcontexts: EntityNPC, AnimationState and AnimationLayer.
	 * Naturally, because animations may have multiple layers, the complete context 
	 * for an EntityNPC is a set of three-tuples, one for every layer, as every layer is
	 * rendered separately.
	 * 
	 * We expose a global variable, RenderContextQueue that contains the different subcontexts
	 * associated with a render operation. Each hook adds a subcontext in the queue. 
	 * During the call to apply_data the entire queue is added in an array associated
	 * with the vertex buffer in which the data is written. Therefore, each operation that 
	 * writes in this vertex buffer can be associated to a given context queue.
	 * 
	 * Types quick breakdown:
	 *	- Context is variant defining which C++ types can be used as a context. Start 
	 * there if you want to add a new type of context. For instance, Entity_NPC* is 
	 * a context.
	 *	- ContextType is an enumeration that must give an integral ID to every context 
	 * in Context. This is used to identify the type of a context in Lua.
	 *	- ContextQueue is a queue of subcontexts (Context). For instance, the ContextQueue
	 * of an EntityNPC contains an Entity_NPC*, an AnimationState* and an AnimationLayer*
	 * once it is recorded.
	 *	- ScoppedContext is a helper class to register a Context in the global ContextQueue.
	 *	- ContextArray is an array of ContextQueues. It is associated with a vertex buffer
	 * to give context to all operations recorded in the buffer.
	 *	- RenderContext is a map from a vertex buffer to a ContextArray. 
	 * 
	 * Globals quick breakdown:
	 *	- RenderContextQueue: a ContextQueue containing all the subcontexts of the current
	 * Render operation. Its content is inserted in the ContextArray of the current vertex
	 * buffer.
	 *	- ElementsRenderContext: a RenderContext mapping every elements buffer in the game
	 * to its RenderContext. Used in the glDrawElements hook to find the context of the
	 * buffer being rendered.
	 *	- VerticesRenderContext: similar as ElementsRenderContext but for vertex buffers.
	 * 
	 * Why does it work ?
	 * The elephant in the room is "What happens if a contextless operation appears ?". For 
	 * all its flaws, Isaac has a consistent render system. If the game initialized a 
	 * render operation, it does not switch contexts. If an EntityNPC starts rendering, then
	 * an EntityBomb won't start rendering before the EntityNPC has been rendered. Vertex buffers
	 * also don't store unrelated data, because they are tied to images. In other words, 
	 * an elements buffer cannot contain data related to an EntityNPC and an EntityBomb at
	 * the same time. An elements buffer can contain data related to the same kind of 
	 * EntityNPC, for instance all gapers in a room, but it cannot contain data related to 
	 * Gappers and Monstro at the same time because they use different images and therefore
	 * different buffers.
	 * If an operation with an unknown context appears (for instance pre rendering the
	 * walls of a room), then the operation is not given a context, and, by construction,
	 * cannot be mixed with other contexts, because an operation in a different context
	 * would use different buffers.
	 */

	/* The differents contexts that are available when performing render. 
	 * This variant is used to enforce the type safety of what goes into a given
	 * RenderContext (see below, RenderContext and ScoppedContext).
	 * If you want to add a new context in the render system, add it here first.
	 */
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

	/* Lua identifier a specific type of Context. This is used by modders to check 
	 * the underlying type of the context they are manipulating. 
	 * 
	 * When a context is exposed to Lua, it contains a type field and a data field, 
	 * the type field is an integer value from this enumeration, the data field is 
	 * a Lua representation of the associated type in the Context variant.
	 */
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

	/* A ContextQueue represents all the contexts that were available when the
	 * vertices of a render operation were added in a vertex buffer.
	 * 
	 * For instance, when rendering an EntityNPC, there are calls to Render in 
	 * EntityNPC, AnimationState and AnimationLayer. This creates three contexts: 
	 * the specific instance of EntityNPC being renderer, its animation state, 
	 * and the animation layer that is being rendered. 
	 */
	typedef std::deque<Context> ContextQueue;

	/* Queue in which the render contexts of the current rendering operation are added. 
	 * Add hooks to *::Render in order to add contexts.
	 */
	extern ContextQueue RenderContextQueue;
	
	/* Helper class to add / remove contexts from the context queue. In a *::Render
	 * hook (or whatever function performs rendering), create a ScopedContext
	 * parameterized with the type of the context you want to add (for instance in 
	 * EntityNPC::Render the type of the context is EntityNPC*, its value is 'this').
	 * 
	 * The purpose of this class is to automate the popping of the context from the 
	 * queue once the render operation is over. This prevents the stack from becoming
	 * inconsistent and containing information that does not match the current render
	 * operation.
	 */
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

	/* Type of an array of ContextQueues. For a given vertex buffer, this is an array 
	 * containing the context of every operation inside the buffer.
	 */
	typedef std::vector<ContextQueue> ContextArray;

	/* Type of a map from a vertex buffer to a context array. This is used to associate
	 * an array of context queues to every vertex buffer used by the game. 
	 */
	typedef std::map<KAGE_Memory_MemoryPoolDescriptor*, ContextArray> RenderContext;

	/* Maps of the vertex/elements buffers to their corresponding context arrays */
	extern RenderContext ElementsRenderContext;
	extern RenderContext VerticesRenderContext;

	static constexpr const uint32_t ELEMENT_BUFFER = 6;
	static constexpr const uint32_t VERTEX_BUFFER = 4;
}