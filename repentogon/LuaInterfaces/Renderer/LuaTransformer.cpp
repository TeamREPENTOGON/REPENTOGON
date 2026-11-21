#include <new>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../LuaRender.h"

using LuaRender::LuaTransformer;
using LuaRender::Transformation;

MOD_EXPORT bool L_Transformer_Init(LuaTransformer* transformer, KAGE_SmartPointer_ImageBase* output) {
	new (transformer) LuaTransformer();
	if ((output->image->_flags & (uint64_t)eImageFlag::PROCEDURAL) == 0) {
		transformer->_valid = false;
		return false;
	}
	transformer->_output = *output;
	transformer->_valid = true;
	return true;
}

MOD_EXPORT void L_Transformer_Destroy(LuaTransformer* transformer) {
	transformer->~LuaTransformer();
	memset(transformer, 0, sizeof(LuaTransformer));
}

MOD_EXPORT void L_Transformer_Render(LuaTransformer* transformer, KAGE_SmartPointer_ImageBase* image, SourceQuad* source, DestinationQuad* dest,
	KColor* color1, KColor* color2, KColor* color3, KColor* color4) {
	Transformation trans;
	trans._input = *image;
	trans._source = *source;
	trans._dest = *dest;
	trans._color1 = *color1;
	trans._color2 = *color2;
	trans._color3 = *color3;
	trans._color4 = *color4;
	transformer->_transformations.push_back(trans);
}

MOD_EXPORT void L_Transformer_Apply(LuaTransformer* transformer) {
	Rendering::PushCurrentRenderTarget();
	g_KAGE_Graphics_Manager.SetCurrentRenderTarget(transformer->_output.image, false);

	for (Transformation& transformation : transformer->_transformations) {
		KAGE_Graphics_ImageBase* image = transformation._input.image;
		image->Render(transformation._source, transformation._dest, transformation._color1, transformation._color2, transformation._color3, transformation._color4);
	}

	g_KAGE_Graphics_Manager.Present();
	Rendering::RestorePreviousRenderTarget();

	transformer->_valid = false;
}
