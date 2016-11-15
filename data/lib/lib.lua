-- Core API functions implemented in Lua
dofile('data/lib/core/core.lua')

-- Compatibility library for our old Lua API
dofile('data/lib/compat/compat.lua')

-- Json impplementation in Liua
JSON = (loadfile "data/lib/other/JSON.lua")()

-- Redis API implemented in Lua
dofile('data/lib/other/redis.lua')
