#include "IsaacRepentance.h"

static bool IsValidLayerID(ANM2* anm2, int id) {
	return (id >= 0 && (const unsigned int)id + 1 <= anm2->GetLayerCount());
}

static void SetLayerWrapModes(BeamRenderer* beam, int layerID) {
	beam->_anm2.GetLayer(layerID)->_wrapSMode = 0;
	beam->_anm2.GetLayer(layerID)->_wrapTMode = 1;
}

MOD_EXPORT void L_Beam_Init(BeamRenderer* beam, ANM2* sprite, int layerID, bool useOverlay, bool unk) {
	new (beam) BeamRenderer(layerID, useOverlay, unk);
	beam->_anm2.construct_from_copy(sprite);
	SetLayerWrapModes(beam, layerID);
}

MOD_EXPORT void L_Beam_Destroy(BeamRenderer* beam) {
	beam->_anm2.destructor();
	beam->~BeamRenderer();
	memset(beam, 0, sizeof(BeamRenderer));
}

MOD_EXPORT void L_Beam_Add(BeamRenderer* beam, Point* point) {
	beam->_points.push_back(*point);
}

MOD_EXPORT int L_Beam_Render(BeamRenderer* beam, bool clearPoints) {
	int error = -1;

	if (beam->_points.size() < 2) {
		error = 0;
	}
	else if (beam->_useOverlayData && beam->_anm2._overlayAnimState._animData == nullptr) {
		error = 1;
	}
	else if (!beam->_useOverlayData && beam->_anm2._animState._animData == nullptr) {
		error = 2;
	}
	else if (!IsValidLayerID(beam->GetANM2(), beam->_layer)) {
		error = 3;
	}
	else {
		g_BeamRenderer->Begin(beam->GetANM2(), beam->_layer, beam->_useOverlayData, beam->_unkBool);

		for (auto it = beam->_points.begin(); it != beam->_points.end(); ++it) {
			Vector posBuffer;
			if (it->_worldSpace)
				LuaEngine::Isaac_WorldToScreen(&posBuffer, &it->_pos);
			else
				posBuffer = it->_pos;
			g_BeamRenderer->Add(&it->_pos, &it->_color, it->_width, it->_spritesheetCoordinate);
		}

		g_BeamRenderer->End();
	}

	if (clearPoints) {
		beam->_points.clear();
	}

	return error;
}

MOD_EXPORT void L_Beam_SetSprite(BeamRenderer* beam, ANM2* sprite) {
	beam->_anm2.destructor();
	beam->_anm2.construct_from_copy(sprite);
}

MOD_EXPORT void L_Beam_SetLayer(BeamRenderer* beam, int layerID) {
	beam->_layer = layerID;
	SetLayerWrapModes(beam, layerID);
}

MOD_EXPORT unsigned int L_Beam_GetPointCount(BeamRenderer* beam) {
	return beam->_points.size();
}

MOD_EXPORT void L_Beam_GetPoints(BeamRenderer* beam, Point* out) {
	for (const Point& point : beam->_points) {
		*out++ = point;
	}
}

MOD_EXPORT void L_Beam_SetPoints(BeamRenderer* beam, Point* points, unsigned int count) {
	beam->_points.assign(points, points + count);
}