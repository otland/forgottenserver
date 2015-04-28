function monster_gc_memory()
	if type(monster_gc) == "table" then
		for _, gc in pairs(monster_gc) do
			if gc then
				gc:delete()
			end
		end
		monster_gc = nil
	end
	collectgarbage("collect")
	monster_gc = {}
	loadDirectory("/data/lua monster/monsters/")
end

monster_gc_memory()