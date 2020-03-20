function Monster:onDropLoot(corpse)
	EventCallback(EVENT_CALLBACK_ONDROPLOOT, self, corpse)
end

function Monster:onSpawn(position, startup, artificial)
	return hasEventCallback(EVENT_CALLBACK_ONSPAWN) and EventCallback(EVENT_CALLBACK_ONSPAWN, self, position, startup, artificial) or true
end
