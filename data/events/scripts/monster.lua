function Monster:onDropLoot(corpse)
	local player = Player(corpse:getCorpseOwner())
	if player then
		player:updateKillTracker(self, corpse)
	end
	if EventCallback.onDropLoot then
		EventCallback.onDropLoot(self, corpse)
	end
end

function Monster:onSpawn(position, startup, artificial)
	if EventCallback.onSpawn then
		return EventCallback.onSpawn(self, position, startup, artificial)
	end
	return true
end
