function Monster:onDropLoot(corpse)
	if EventCallback:has(EVENT_CALLBACK_ONDROPLOOT) then
		EventCallback:get(EVENT_CALLBACK_ONDROPLOOT, self, corpse)
	end
end

function Monster:onSpawn(position, startup, artificial)
	if EventCallback:has(EVENT_CALLBACK_ONSPAWN) then
		return EventCallback:get(EVENT_CALLBACK_ONSPAWN, self, position, startup, artificial)
	else
		return true
	end
end
