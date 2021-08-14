function Monster:onDropLoot(corpse)
	if hasEventCallback(EVENT_CALLBACK_ONDROPLOOT) then
		EventCallback(EVENT_CALLBACK_ONDROPLOOT, self, corpse)
	end
end
