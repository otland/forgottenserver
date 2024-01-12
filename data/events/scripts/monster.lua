function Monster:onDropLoot(corpse)
	if hasEvent.onDropLoot then
		Event.onDropLoot(self, corpse)
	end
	local player = Player(corpse:getCorpseOwner())
	if player then
		player:updateKillTracker(self, corpse)
	end
end

function Monster:onSpawn(position, startup, artificial)
	if hasEvent.onSpawn then
		return Event.onSpawn(self, position, startup, artificial)
	end
	return true
end
