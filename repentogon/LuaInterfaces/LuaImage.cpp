#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "LuaRender.h";
#include "../Utils/ImageUtils.hpp"

//TODO: We might be able to move a lot of this Lua side at some point.
extern "C" {
	__declspec(dllexport) void L_Image_DecrRef(KAGE_SmartPointer_ImageBase* imageBase) {
		imageBase->DecrRef();
	}

	__declspec(dllexport) int L_Image_GetHeight(KAGE_SmartPointer_ImageBase* imageBase) {
		return imageBase->image->GetHeight();
	}

	__declspec(dllexport) const char* L_Image_GetName(KAGE_SmartPointer_ImageBase* imageBase) {
		return imageBase->image->_name;
	}

	__declspec(dllexport) int L_Image_GetPaddedHeight(KAGE_SmartPointer_ImageBase* imageBase) {
		return imageBase->image->GetPaddedHeight();
	}

	__declspec(dllexport) int L_Image_GetPaddedWidth(KAGE_SmartPointer_ImageBase* imageBase) {
		return imageBase->image->GetPaddedWidth();
	}

	__declspec(dllexport) int L_Image_GetWidth(KAGE_SmartPointer_ImageBase* imageBase) {
		return imageBase->image->GetWidth();
	}

	__declspec(dllexport) void L_Image_GetTexelRegion(KAGE_SmartPointer_ImageBase* imageBase, int x, int y, uint32_t width, uint32_t height, void* buf) {
		imageBase->image->GetTexelRegion(x, y, width, height, buf);
	}

	__declspec(dllexport) void L_Image_Render(KAGE_SmartPointer_ImageBase* imageBase, SourceQuad* sourceQuad, DestinationQuad* destinationQuad, KColor* color, ColorMod* colorMod) {
		auto& image = *imageBase->image;

		auto& shader = *__ptr_g_AllShaders[ShaderType::SHADER_COLOR_OFFSET];
		float* vertexBuffer = ImageUtils::SubmitQuadForShader(image, shader, *sourceQuad, *destinationQuad, ImageUtils::QuadColor(*color));
		if (vertexBuffer) {
			ShaderUtils::ColorOffset::FillVertices(vertexBuffer, image, *colorMod);
		}
	}

	__declspec(dllexport) float* L_Image_SubmitQuadForShader(KAGE_SmartPointer_ImageBase* imageBase, SourceQuad* sourceQuad, DestinationQuad* destinationQuad, KColor* color, KAGE_Graphics_Shader* shader) {
		return ImageUtils::SubmitQuadForShader(*imageBase->image, *shader, *sourceQuad, *destinationQuad, ImageUtils::QuadColor(*color));
	}
}