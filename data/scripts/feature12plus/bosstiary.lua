local handlerList = PacketHandler(0xAE)
function handlerList.onReceive(player, msg)
	player:sendBoostiaryMilestonesData()
	player:sendBosstiaryList()
end
handlerList:register()

local handlerSlot = PacketHandler(0xAF)
function handlerSlot.onReceive(player, msg)
	player:sendBoostiaryMilestonesData()
	player:sendBoostiarySlotsData()
	player:sendResourceBalance(RESOURCE_BANK_BALANCE, player:getBankBalance())
	player:sendResourceBalance(RESOURCE_GOLD_EQUIPPED, player:getMoney())
end
handlerSlot:register()

local handlerSet = PacketHandler(0xB0)
function handlerSet.onReceive(player, msg)
	local slot = msg:getByte()
	local bossId = msg:getU32()
	local isValidBoss = player:isValidBoostiaryBoss(bossId)
	local isGoldRemoved = true
	if bossId == 0 then
		isGoldRemoved = player:removeTotalMoney(player:getBosstiaryRemoveCost())
		player:incrementBosstiaryRemoveCounter()
	end

	if isValidBoss == true and isGoldRemoved == true then
		player:sendResourceBalance(RESOURCE_BANK_BALANCE, player:getBankBalance())
		player:sendResourceBalance(RESOURCE_GOLD_EQUIPPED, player:getMoney())
		player:setBosstiarySlotBoss(slot, bossId)
		player:sendBoostiarySlotsData()
	else
		player:popupFYI("You cannot set this boss to the selected slot!")
	end

end
handlerSet:register()

local playerCharm = CreatureEvent("___")
function playerCharm.onLogin(player)
	player:sendTextMessage(MESSAGE_STATUS_DEFAULT,
		string.format("%s %s\n%s", "Today's boosted boss:",
			Bosstiary.getBossById(Bosstiary.getTodayBoostedBoss().id).name,
			"Boosted bosses contain more loot and count more kills for your Bosstiary."))
	player:registerEvent("Bosstiary_onKill")
	player:resetBestiaryRemoveCountIfNeeded()
	player:setBosstiaryLastRemoveDate()
	return true
end
playerCharm:register()

local creatureevent = CreatureEvent("Bosstiary_onKill")
function creatureevent.onKill(player, target)
	local bossData = Bosstiary.getBossByName(target:getName())
	if not bossData then
		return true
	end

	local todayBoostedBossId = Bosstiary.getTodayBoostedBoss().id
	local todayBoostedBossName = Bosstiary.getBossById(todayBoostedBossId).name
	local killers = target:getKillers(true)
	if todayBoostedBossName == target:getName() then
		for _, killer in ipairs(killers) do
			killer:addBosstiaryKills(todayBoostedBossId, Bosstiary.getConst().TODAY_BOOSTED_BOSS_KILL_COUNT)
		end
	else
		for _, killer in ipairs(killers) do
			killer:addBosstiaryKills(bossData.id, 1)
		end
	end
	return true
end
creatureevent:register()

local function isEqItem(item)
	local it = ItemType(item.itemId)
	if it:isWeapon() or it:isHelmet() or it:isArmor() or it:isLegs() or it:isBoots() or it:isQuiver() or it:isShield() or
		it:isNecklace() or it:isRing() or it:isTrinket() then
			return true
	end
	return false
end

local event = Event()
event.onDropLoot = function(self, corpse)
	if configManager.getNumber(configKeys.RATE_LOOT) == 0 then
		return
	end

	local player = Player(corpse:getCorpseOwner())
	local mType = self:getType()
	local doCreateLoot = false

	if not player or player:getStamina() > 840 then
		doCreateLoot = true
	end

	local bossData = Bosstiary.getBossByName(self:getName())
	if not bossData then
		return
	end

	local bonusRollChance = player:getBosstiaryBonusValue()
	if bossData.id == Bosstiary.getTodayBoostedBoss().id then
		bonusRollChance = Bosstiary.getConst().TODAY_BOOSTED_BOSS_KILL_BONUS
	end

	while bonusRollChance > 0 do
		local roll = math.random(1, 100)
		if roll <= bonusRollChance then
			if doCreateLoot then
				local monsterLoot = mType:getLoot()
				for i = 1, #monsterLoot do
					if isEqItem(monsterLoot[i]) then
						local item = corpse:createLootItem(monsterLoot[i], 1)
						if not item then
							print("[Warning] DropLoot: Could not add loot item to corpse.")
						end
					end
				end
			end
		end

		bonusRollChance = bonusRollChance - 100
	end
end

event:register(-1)
