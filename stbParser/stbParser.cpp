#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <vector>

#include "stbParser.hpp"

namespace stb {

	void parseStb(std::string const& path, std::vector<Room>& rooms) {
		FILE* f = fopen(path.c_str(), "rb");
		if (!f) {
			return;
		}

		char header[5];
		fread(header, sizeof(char), 4, f);
		header[4] = '\0';

		if (strcmp(header, "STB1")) {
			return;
		}

		uint32_t nbRooms;
		fread(&nbRooms, sizeof(nbRooms), 1, f);

		for (uint32_t i = 0; i < nbRooms; ++i) {
			uint32_t type, variant, subtype;
			uint8_t difficulty;
			uint16_t nameLen;

			fread(&type, sizeof(type), 1, f);
			fread(&variant, sizeof(variant), 1, f);
			fread(&subtype, sizeof(subtype), 1, f);
			fread(&difficulty, sizeof(difficulty), 1, f);
			fread(&nameLen, sizeof(nameLen), 1, f);

			char* name = (char*)malloc(nameLen + 1);
			if (!name) {
				printf("Out of memory\n");
				return;
			}

			fread(name, sizeof(char), nameLen, f);
			name[nameLen] = '\0';

			float weight;
			uint8_t width, height, shape, numDoors;
			uint16_t numEnts;

			fread(&weight, sizeof(weight), 1, f);
			fread(&width, sizeof(width), 1, f);
			fread(&height, sizeof(height), 1, f);
			fread(&shape, sizeof(shape), 1, f);
			fread(&numDoors, sizeof(numDoors), 1, f);
			fread(&numEnts, sizeof(numEnts), 1, f);

			width += 2;
			height += 2;

			if (shape == 0) {
				printf("Invalid room shape %d for room %d (%s), W = %d, H = %d\n", shape, variant, name, width, height);
				shape = 1;
			}

			printf("Processing room %d.%d.%d (%s), with weight = %f, dims = %d * %d, shape = %d, ndoors = %d, nentities = %d\n", type, variant, subtype, name, weight, width, height, shape, numDoors, numEnts);

			std::vector<Door> doors;
			doors.reserve(numDoors);

			for (uint8_t j = 0; j < numDoors; ++j) {
				int16_t x, y;
				bool exists;

				fread(&x, sizeof(x), 1, f);
				fread(&y, sizeof(y), 1, f);
				fread(&exists, sizeof(exists), 1, f);

				doors.push_back({ x, y, exists });
			}

			// std::cout << (int)height << ", " << (int)width << std::endl;

			Room room;
			room.name = std::string(name);
			free(name);
			room.difficulty = difficulty;
			room.weight = weight;
			room.type = type;
			room.variant = variant;
			room.subtype = subtype;
			room.shape = shape;
			room.doors = std::move(doors);

			room.shapeData = &(shapeDatas[shape]);
			if (room.shapeData->baseShape) {
				room.baseShapeData = &(shapeDatas[*room.shapeData->baseShape]);
			}

			uint16_t realWidth = std::get<0>(room.GetDims());
			uint16_t gridLen = room.GridLen();

			printf("Found realWidth = %d, gridLen = %d\n", realWidth, gridLen);
			fflush(stdout);

			room.entities.resize(gridLen);

			for (uint16_t j = 0; j < numEnts; ++j) {
				int16_t x, y;
				uint8_t stacked;

				fread(&x, sizeof(x), 1, f);
				fread(&y, sizeof(y), 1, f);
				fread(&stacked, sizeof(stacked), 1, f);

				++x;
				++y;

				uint16_t gridIndex = Room::GridIndex(x, y, realWidth);
				if (gridIndex > gridLen) {
					printf("Discarding current entity due to invalid position in room %d (%s; x = %d, y = %d)\n", variant, room.name.c_str(), x - 1, y - 1);
					continue;
				}

				std::vector<Entity>& entities = room.entities[gridIndex];

				for (uint8_t k = 0; k < stacked; ++k) {
					uint16_t etype, evariant, esubtype;
					float weight;

					fread(&etype, sizeof(etype), 1, f);
					fread(&evariant, sizeof(evariant), 1, f);
					fread(&esubtype, sizeof(esubtype), 1, f);
					fread(&weight, sizeof(weight), 1, f);

					entities.push_back({ etype, evariant, esubtype, weight });
				}
			}

			room.print();
			rooms.push_back(room);
		}
	}

	uint16_t Room::GridIndex(uint16_t x, uint16_t y, uint16_t w) {
		return y * w + x;
	}

	Dims Room::GetDims() const {
		if (baseShapeData) {
			return baseShapeData->dims;
		} else {
			return shapeData->dims;
		}
	}

	uint16_t Room::GridLen() const {
		auto [x, y] = GetDims();
		return x * y;
	}

	void Room::print() const {
		printf("Room %d.%d.%d (%s)\n", type, variant, subtype, name.c_str());
		printf("\tDifficulty = %d, weight = %f, shape = %d\n", difficulty, weight, shape);
		printf("\tDoors = {\n");
		for (Door const& door : doors) {
			printf("\t\t{ %d, %d } (%d)\n", door.x, door.y, door.exists);
		}
		printf("\t}\n");
		printf("\tEntities = {\n");
		uint16_t index = 0;
		for (std::vector<Entity> const& stacked: entities) {
			if (stacked.size() == 0) {
				++index;
				continue;
			}

			if (stacked.size() == 1) {
				Entity const& entity = stacked[0];
				printf("\t\t%d = { %d.%d.%d, %f }\n", index, entity.type, entity.variant, entity.subtype, entity.weight);
			} else {
				printf("\t\t%d = {\n", index);
				for (Entity const& entity : stacked) {
					printf("\t\t\t{ %d.%d.%d, %f }\n", entity.type, entity.variant, entity.subtype, entity.weight);
				}
				printf("\t\t}\n");
			}
			++index;
		}
		printf("\t}\n");
	}

	ShapeData shapeDatas[13] = {
		// 0, null
		{
			std::nullopt, // Base Shape
			{ 0, 0 } // Dims
		},
		// 1, 1 * 1
		{
			std::nullopt,
			{ 15, 9 }
		},
		// 2, 1 * 0.5
		{
			1,
			{ 15, 5 }
		},
		// 3, 0.5 * 1
		{
			1,
			{ 7, 9 }
		},
		// 4, 1 x 2
		{
			std::nullopt,
			{ 15, 16 }
		},
		// 5, 0.5 * 2
		{
			4,
			{ 7, 16 }
		},
		// 6, 2 * 1
		{
			std::nullopt,
			{ 28, 9 }
		},
		// 7, 2 * 0.5
		{
			6,
			{ 28, 5 }
		},
		// 8, 2 * 2
		{
			std::nullopt,
			{ 28, 16 }
		},
		// 9, Mirrored L
		{
			8,
			{ 28, 16 }
		},
		// 10, L
		{
			8,
			{ 28, 16 }
		},
		// 11, Mirrored r
		{
			8,
			{ 28, 16 }
		},
		// 12, r
		{
			8,
			{ 28, 16 }
		}
	};
}