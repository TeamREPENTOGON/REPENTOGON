#pragma once
#include <cstdint>
#include <vector>
#include <IsaacRepentance.h>

std::vector<XY> GetOccupiedCoords(XY& base_coords, int shape);

std::vector<XY> GetForbiddenNeighbors(XY& base_coords, int shape, int doors);

int CalculateAutoDoors(const int* roomMap, XY& base_coords, int shape);
