function Party.broadcastPartyLoot(self, text)
	self:getLeader():sendTextMessage(MESSAGE_LOOT, text)
	local membersList = self:getMembers()
	for i = 1, #membersList do
		local player = membersList[i]
		if player then
			player:sendTextMessage(MESSAGE_LOOT, text)
		end
	end
end
