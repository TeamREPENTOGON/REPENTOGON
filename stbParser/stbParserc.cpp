#include "stbParser.hpp"

int main() {
	std::vector<stb::Room> rooms;
	stb::parseStb("test.stb", rooms);
	return 0;
}