function Monster:onDropLoot(corpse)
	EventCallback(EVENT_CALLBACK_ONDROPLOOT, self, corpse)
end

function Monster:onSpawn(position, startup, artificial)
	if hasEventCallback(EVENT_CALLBACK_ONSPAWN) then return EventCallback(EVENT_CALLBACK_ONSPAWN, self, position, startup, artificial) else return true end
end
