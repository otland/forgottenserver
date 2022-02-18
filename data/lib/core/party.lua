function Party.broadcastPartyLoot(self, text)
	self:getLeader():sendTextMessage(MESSAGE_INFO_DESCR, text)
	local membersList = self:getMembers()
	for i = 1, #membersList do
		local player = membersList[i]
		if player then
			player:sendTextMessage(MESSAGE_INFO_DESCR, text)
		end
	end
end
