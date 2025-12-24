function Monster:onDropLoot(corpse)
	if Event.onMonsterDropLoot then
		Event.onMonsterDropLoot(self, corpse)
	end

	local player = Player(corpse:getCorpseOwner())
	if player then
		player:updateKillTracker(self, corpse)
	end
end

function Monster:onSpawn(position, startup, artificial)
	if Event.onMonsterSpawn then
		return Event.onMonsterSpawn(self, position, startup, artificial)
	end
	return true
end
