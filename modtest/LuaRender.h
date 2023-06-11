#pragma once

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
}