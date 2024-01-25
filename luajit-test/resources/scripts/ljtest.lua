local ffi = require("ffi")

ffi.cdef[[
struct Vector {
	float x;
	float y;
};

void L_Console_Print(const char*);
void L_Spawn(int, int, unsigned int, void*);
unsigned int L_Random(unsigned int max);
unsigned int L_GetTime();
]]

local libzhl = ffi.load("libzhl")

function getName()
	libzhl.L_Console_Print("hello from FFI!\n")
	print(libzhl.L_GetTime())
	return "Hello from luajit again!"
end

function laserCollision(pos, damage)
	libzhl.L_Spawn(5, 100, libzhl.L_Random(5) + 1, pos)
end