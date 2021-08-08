local ec = EventCallback

ec.onDropLoot = function(self, corpse)
	if configManager.getNumber(configKeys.RATE_LOOT) == 0 then
		return
	end

	local player = Player(corpse:getCorpseOwner())
	local mType = self:getType()
	if not player or player:getStamina() > 840 then
		local monsterLoot = mType:getLoot()
		for i = 1, #monsterLoot do
			local item = corpse:createLootItem(monsterLoot[i])
			if not item then
				local playerLootedGems = Container.getGems()
				local playerBal = player:getTharianBankBalance()
				local playerNewBal = playerBal + playerLootedGems
				if playerLootedGems >= 1 then
					player:setTharianBankBalance(playerNewBal)
					local msg = ("Gems from %s:  %s gems"):format(mType:getNameDescription(), playerLootedGems)
					print(playerBal)
					sendChannelMessage(CHANNEL_LOOT, TALKTYPE_CHANNEL_Y, msg)
					Container.resetGems()
				end
			end
		end

		if player then
			local text = ("Loot of %s: %s"):format(mType:getNameDescription(), corpse:getContentDescription())
			local party = player:getParty()
			if party then
				party:broadcastPartyLoot(text)
				sendChannelMessage(CHANNEL_LOOT, TALKTYPE_CHANNEL_Y, text)
			else
				player:sendTextMessage(MESSAGE_LOOT, text)
				sendChannelMessage(CHANNEL_LOOT, TALKTYPE_CHANNEL_Y, text)
			end
		end
	else
		local text = ("Loot of %s: nothing (due to low stamina)"):format(mType:getNameDescription())
		local party = player:getParty()
		if party then
			party:broadcastPartyLoot(text)
			sendChannelMessage(CHANNEL_LOOT, TALKTYPE_CHANNEL_Y, text)
		else
			player:sendTextMessage(MESSAGE_LOOT, text)
			sendChannelMessage(CHANNEL_LOOT, TALKTYPE_CHANNEL_Y, text)
		end
	end
end

ec:register()
