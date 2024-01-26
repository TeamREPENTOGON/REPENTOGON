local ffi = require("ffi")

ffi.cdef[[
struct Vector {
	float x;
	float y;
};

void L_Console_Print(const char*);
unsigned int L_GetTime();
void L_Game_ClearDonationModGreed();
unsigned int L_Random(unsigned int max);
void L_Spawn(int, int, unsigned int, void*);
]]

libzhl = ffi.load("libzhl")

print = function(...)
	str = ""

	for i,v in ipairs({ ... }) do
       str = str .. tostring(v) .. " "
    end
    str = str .. "\n"

	libzhl.L_Console_Print(str)
end

function getName()
	libzhl.L_Console_Print("hello from FFI!\n")
	print(libzhl.L_GetTime())
	return "Hello from luajit again!"
end

function laserCollision(pos, damage)
	libzhl.L_Spawn(5, 100, libzhl.L_Random(5) + 1, pos)
end