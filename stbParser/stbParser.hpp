#pragma once

#define STB_EXPORT __declspec(dllexport)

#include <cstdint>

#include <array>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

namespace stb {

	typedef std::tuple<uint16_t, uint16_t> Coords;
	typedef Coords Dims;

	struct ShapeData {
		/* struct Wall {

		}; */

		/* std::vector<Coords> doors;
		std::array<std::vector<Coords>, 2> walls;
		std::optional<uint32_t> topLeft; */
		std::optional<uint8_t> baseShape;
		/* std::optional<uint16_t> mirrorX;
		std::optional<uint16_t> mirrorY; */
		Dims dims;
	};

	extern ShapeData shapeDatas[13];

	struct Door {
		int x;
		int y;
		bool exists;
	};

	struct Entity {
		uint32_t type;
		uint32_t variant;
		uint32_t subtype;
		float weight;
	};

	struct Room {
		std::string name;
		uint8_t difficulty = 0;
		float weight = 0;
		uint32_t type = 0;
		uint32_t variant = 0;
		uint32_t subtype = 0;
		uint8_t shape = 0;
		std::vector<Door> doors;
		std::vector<std::vector<Entity>> entities;
		ShapeData* shapeData = nullptr;
		ShapeData* baseShapeData = nullptr;

		Dims GetDims() const;
		uint16_t GridLen() const;

		static uint16_t GridIndex(uint16_t x, uint16_t y, uint16_t width);

		void print() const;
	};

	void STB_EXPORT parseStb(std::string const& path, std::vector<Room>& rooms);

}