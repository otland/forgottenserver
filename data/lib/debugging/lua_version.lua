if type(jit) == 'table' then
	logInfo('>> Using ' .. jit.version) --LuaJIT 2.0.2
else
	logInfo('>> Using ' .. _VERSION)
end
