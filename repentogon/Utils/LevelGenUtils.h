#pragma once
#include <cstdint>
#include <vector>

struct RoomCoords {
	uint32_t col;
	uint32_t row;

	RoomCoords(uint32_t col, uint32_t row);

	bool IsValid() const;

	int ToGridIndex() const;

	bool operator==(RoomCoords const& other) const;
};

std::vector<RoomCoords> GetOccupiedCoords(RoomCoords& base_coords, int shape);

std::vector<RoomCoords> GetForbiddenNeighbors(RoomCoords& base_coords, int shape, int doors);