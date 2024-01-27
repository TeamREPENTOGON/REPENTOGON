Isaac calls some functions that aren't implemented in Lua 5.1. In order to work with LuaJIT, we need to implement those missing imports so the game works properly.

Some functions were already implemented in LuaJIT through separate PRs. For everything else, I've taken some functions from https://github.com/lunarmodules/lua-compat-5.3 and put them here.

This is far from a proper solution. Some of these reimplementations run on the Lua side which is hopefully not too bad since we're using LuaJIT. We can hopefully replace these with stubs if we ever get to a point where the entire API has been moved over to FFI.