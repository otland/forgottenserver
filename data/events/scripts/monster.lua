-- Rarity Animations
local rare_popup = true
local rare_effect = true
local rare_effect_id = CONST_ME_STUN

function Monster:onDropLoot(corpse)
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
				print('[Warning] DropLoot:', 'Could not add loot item to corpse.')
			end
		end

		-- Apply rarity chance to corpse contents and apply animation
		if rollRarity(corpse) > 0 then -- If a rare item was rolled, play animation
				if rare_popup then
					local spectators = Game.getSpectators(corpse:getPosition(), false, true, 7, 7, 5, 5)
					for i = 1, #spectators do
						spectators[i]:say(rare_text, TALKTYPE_MONSTER_SAY, false, spectators[i], corpse:getPosition())
					end
				end
			if rare_effect then
				corpse:getPosition():sendMagicEffect(rare_effect_id)
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
end

function Monster:onSpawn(position, startup, artificial)
    self:registerEvent("rollHealth")
    self:registerEvent("rollMana")
   return true
end