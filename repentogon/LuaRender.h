#pragma once

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

	void PatchglDrawElements();
	void InitShaders();
}