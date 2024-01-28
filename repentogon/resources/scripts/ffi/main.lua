libzhl = ffi.load("libzhl")

package.path = package.path .. ';resources/scripts/ffi/?.lua'

pcall(require("Isaac"))
pcall(require("Console"))

print = function(str) 
	Isaac.DebugString(str)
	Isaac.ConsoleOutput(str)
end

package = nil
ffi = nil