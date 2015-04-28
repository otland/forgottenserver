function monster_gc_memory()
	monster_gc = {}
	loadDirectory("/data/luamonster/monsters/")
end

monster_gc_memory()