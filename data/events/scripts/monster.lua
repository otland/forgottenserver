function Monster:onDropLoot(corpse)
	if (configManager.getNumber(configKeys.RATE_LOOT) == 0) then
		corpse:decay()
		return
	end
	local player = Player(corpse:getCorpseOwner())
	local mType = self:getType()
	if not player or player:getStamina() > 840 then
		local monsterLoot = mType:getLoot()
		for it = 1, #monsterLoot do
			local loot = monsterLoot[it]
			local itemList = createLootItem(monsterLoot[it])
			if (#itemList > 0) then
				for i = 1, #itemList do
					local item = itemList[i]
					-- check containers
					if item:isContainer() then
						if not item:createLootContainer(loot.childLoot) then
							print('[Warning] DropLoot:', 'Could not create loot container.')
							item = nil
						end
					end
					if item then
						if corpse:addItemEx(item) ~= RETURNVALUE_NOERROR then
							print('[Warning] DropLoot:', 'Could not add loot item to corpse.')
						end
					else
						print('[Warning] DropLoot:', 'Item is nil.')
					end
				end
			end
		end
		if player then 
			local text = ("Loot of %s: %s"):format(mType:getNameDescription(), corpse:getContentDescription())
			local party = player:getParty()
			if party then
				party:broadcastPartyLoot(text)
			else
				player:sendTextMessage(MESSAGE_LOOT, text)
			end
		end
	else
		local text = ("Loot of %s: nothing (due to low stamina)"):format(mType:getNameDescription())
		local party = player:getParty()
		if party then
			party:broadcastPartyLoot(text)
		else
			player:sendTextMessage(MESSAGE_LOOT, text)
		end
	end
	corpse:decay()
end