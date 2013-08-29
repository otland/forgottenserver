_lastCid = nil
_lastCreature = nil
_lastPid = nil
_lastPlayer = nil

function _Creature(cid)
	if _lastPid == cid then
		return _lastPlayer
	end

	if _lastCid ~= cid then
		_lastCid = cid
		_lastCreature = Creature(cid)
	end
	return _lastCreature
end

function _Player(cid)
	if _lastPid ~= cid then
		_lastPid = cid
		_lastPlayer = Player(cid)
	end
	return _lastPlayer
end

function pushThing(thing)
	local t = {uid = 0, itemid = 0, type = 0, actionid = 0}
	if thing ~= nil then
		if thing:isItem() then
			t.uid = thing:getUniqueId()
			t.itemid = thing:getId()
			if thing:hasSubType() then
				t.type = thing:getSubType()
			end
			t.actionid = thing:getActionId()
		elseif thing:isCreature() then
			t.uid = thing:getId()
			t.itemid = 1
			if thing:isPlayer() then
				t.type = 1
			elseif thing:isMonster() then
				t.type = 2
			else
				t.type = 3
			end
		end
	end
	return t
end

function isCreature(cid) return _Creature(cid) ~= nil end
function isPlayer(cid) return _Player(cid) ~= nil end
function isMonster(cid) return Monster(cid) ~= nil end
function isNpc(cid) return Npc(cid) ~= nil end
function isItem(uid) return Item(uid) ~= nil end
function isContainer(uid) return Container(uid) ~= nil end

function getCreatureName(cid) local c = _Creature(cid) return c ~= nil and c:getName() or false end
function getCreatureHealth(cid) local c = _Creature(cid) return c ~= nil and c:getHealth() or false end
function getCreatureMaxHealth(cid) local c = _Creature(cid) return c ~= nil and c:getMaxHealth() or false end
function getCreaturePosition(cid) local c = _Creature(cid) return c ~= nil and c:getPosition() or false end

getCreaturePos = getCreaturePosition

function doCreatureAddHealth(cid, health) local c = _Creature(cid) return c ~= nil and c:addHealth(health) or false end
function doRemoveCreature(cid) local c = _Creature(cid) return c ~= nil and c:remove() or false end
function doCreatureSetLookDir(cid, direction) local c = _Creature(cid) return c ~= nil and c:setDirection(direction) or false end
function doCreatureSay(cid, text, type, ...) local c = _Creature(cid) return c ~= nil and c:say(text, type, ...) or false end

doSetCreatureDirection = doCreatureSetLookDir

function registerCreatureEvent(cid, name) local c = _Creature(cid) return c ~= nil and c:registerEvent(name) or false end
function unregisterCreatureEvent(cid, name) local c = _Creature(cid) return c ~= nil and c:unregisterEvent(name) or false end

function getPlayerByName(name) local p = Player(name) return p ~= nil and p:getId() or false end
function getIPByPlayerName(name) local p = Player(name) return p ~= nil and p:getIp() or false end
function getPlayerGUID(cid) local p = _Player(cid) return p ~= nil and p:getGuid() or false end
function getPlayerIp(cid) local p = _Player(cid) return p ~= nil and p:getIp() or false end
function getPlayerAccountType(cid) local p = _Player(cid) return p ~= nil and p:getAccountType() or false end
function getPlayerLastLoginSaved(cid) local p = _Player(cid) return p ~= nil and p:getLastLoginSaved() or false end
function getPlayerName(cid) local p = _Player(cid) return p ~= nil and p:getName() or false end
function getPlayerFreeCap(cid) local p = _Player(cid) return p ~= nil and p:getFreeCapacity() or false end
function getPlayerPosition(cid) local p = _Player(cid) return p ~= nil and p:getPosition() or false end
function getPlayerMagLevel(cid) local p = _Player(cid) return p ~= nil and p:getMagicLevel() or false end
function getPlayerAccess(cid) local p = _Player(cid) return p ~= nil and p:getGroup():getAccess() or false end
function getPlayerSkill(cid, skillId) local p = _Player(cid) return p ~= nil and p:getSkillLevel(skillId) or false end
function getPlayerMana(cid) local p = _Player(cid) return p ~= nil and p:getMana() or false end
function getPlayerMaxMana(cid) local p = _Player(cid) return p ~= nil and p:getMaxMana() or false end
function getPlayerLevel(cid) local p = _Player(cid) return p ~= nil and p:getLevel() or false end
function getPlayerTown(cid) local p = _Player(cid) return p ~= nil and p:getTown():getId() or false end
function getPlayerVocation(cid) local p = _Player(cid) return p ~= nil and p:getVocation():getId() or false end
function getPlayerSoul(cid) local p = _Player(cid) return p ~= nil and p:getSoul() or false end
function getPlayerSex(cid) local p = _Player(cid) return p ~= nil and p:getSex() or false end
function getPlayerStorageValue(cid, key) local p = _Player(cid) return p ~= nil and p:getStorageValue(key) or false end
function getPlayerBalance(cid) local p = _Player(cid) return p ~= nil and p:getBankBalance() or false end
function getPlayerMoney(cid) local p = _Player(cid) return p ~= nil and p:getMoney() or false end
function getPlayerGroupId(cid) local p = _Player(cid) return p ~= nil and p:getGroup():getId() or false end
function getPlayerLookDir(cid) local p = _Player(cid) return p ~= nil and p:getDirection() or false end
function getPlayerLight(cid) local p = _Player(cid) return p ~= nil and p:getLight() or false end
function getPlayerDepotItems(cid, depotId) local p = _Player(cid) return p ~= nil and p:getDepotItems(depotId) or false end
function getPlayerSkullType(cid) local p = _Player(cid) return p ~= nil and p:getSkull() or false end
function getPlayerLossPercent(cid) local p = _Player(cid) return p ~= nil and p:getDeathPenalty() or false end
function getPlayerMount(cid, mountId) local p = _Player(cid) return p ~= nil and p:hasMount(mountId) or false end
function getPlayerPremiumDays(cid) local p = _Player(cid) return p ~= nil and p:getPremiumDays() or false end
function getPlayerBlessing(cid, blessing) local p = _Player(cid) return p ~= nil and p:hasBlessing(blessing) or false end
function getPlayerGuildId(cid)
	local player = _Player(cid)
	if player == nil then
		return false
	end

	local guild = player:getGuild()
	if guild == nil then
		return false
	end
	return guild:getId()
end
function getPlayerGuildLevel(cid) local p = _Player(cid) return p ~= nil and p:getGuildLevel() or false end
function getPlayerGuildName(cid)
	local player = _Player(cid)
	if player == nil then
		return false
	end

	local guild = player:getGuild()
	if guild == nil then
		return false
	end
	return guild:getName()
end
function getPlayerGuildRank(cid)
	local player = _Player(cid)
	if player == nil then
		return false
	end

	local guild = player:getGuild()
	if guild == nil then
		return false
	end

	local rank = guild:getRankByLevel(player:getGuildLevel())
	return rank ~= nil and rank.name or false
end
function getPlayerGuildNick(cid) local p = _Player(cid) return p ~= nil and p:getGuildNick() or false end
function getPlayerMasterPos(cid) local p = _Player(cid) return p ~= nil and p:getTown():getTemplePosition() or false end
function getPlayerItemCount(cid, itemId, ...) local p = _Player(cid) return p ~= nil and p:getItemCount(itemId, ...) or false end
function getPlayerSlotItem(cid, slot)
	local player = _Player(cid)
	if player == nil then
		return pushThing(nil)
	end
	return pushThing(player:getSlotItem(slot))
end
function getPlayerItemById(cid, deepSearch, itemId, ...)
	local player = _Player(cid)
	if player == nil then
		return pushThing(nil)
	end
	return pushThing(player:getItemById(itemId, deepSearch, ...))
end

getPlayerAccountBalance = getPlayerBalance
getIPByName = getIPByPlayerName

function setPlayerStorageValue(cid, key, value) local p = _Player(cid) return p ~= nil and p:setStorageValue(key, value) or false end
function doPlayerSetBalance(cid, balance) local p = _Player(cid) return p ~= nil and p:setBankBalance(balance) or false end
function doPlayerAddMoney(cid, money) local p = _Player(cid) return p ~= nil and p:addMoney(money) or false end
function doPlayerRemoveMoney(cid, money) local p = _Player(cid) return p ~= nil and p:removeMoney(money) or false end
function doPlayerAddSoul(cid, soul) local p = _Player(cid) return p ~= nil and p:addSoul(soul) or false end
function doPlayerSetVocation(cid, vocation) local p = _Player(cid) return p ~= nil and p:setVocation(Vocation(vocation)) or false end
function doPlayerSetTown(cid, town) local p = _Player(cid) return p ~= nil and p:setTown(Town(town)) or false end
function setPlayerGroupId(cid, groupId) local p = _Player(cid) return p ~= nil and p:setGroup(Group(groupId)) or false end
function doPlayerSetSex(cid, sex) local p = _Player(cid) return p ~= nil and p:setSex(sex) or false end
function doShowTextDialog(cid, itemId, text) local p = _Player(cid) return p ~= nil and p:showTextDialog(itemId, text) or false end
function doPlayerAddItemEx(cid, uid, ...) local p = _Player(cid) return p ~= nil and p:addItemEx(Item(uid), ...) or false end
function doPlayerAddPremiumDays(cid, days) local p = _Player(cid) return p ~= nil and p:addPremiumDays(days) or false end
function doPlayerRemovePremiumDays(cid, days) local p = _Player(cid) return p ~= nil and p:removePremiumDays(days) or false end
function doPlayerAddBlessing(cid, blessing) local p = _Player(cid) return p ~= nil and p:addBlessing(blessing) or false end

function getTownId(townName) local t = Town(townName) return t ~= nil and t:getId() or false end
function getTownName(townId) local t = Town(townId) return t ~= nil and t:getName() or false end
function getTownTemplePosition(townId) local t = Town(townId) return t ~= nil and t:getTemplePosition() or false end

function doSetItemActionId(uid, actionId) local i = Item(uid) return i ~= nil and i:setActionId(actionId) or false end
function doTransformItem(uid, newItemId, ...) local i = Item(uid) return i ~= nil and i:transform(newItemId, ...) or false end

function getContainerSize(uid) local c = Container(uid) return c ~= nil and c:getSize() or false end
function getContainerCap(uid) local c = Container(uid) return c ~= nil and c:getCapacity() or false end
function getContainerItem(uid, slot)
	local container = Container(uid)
	if container == nil then
		return pushThing(nil)
	end
	return pushThing(container:getItem(slot))
end

function doSendMagicEffect(pos, magicEffect, ...) return Position(pos):sendMagicEffect(magicEffect, ...) end
function doSendDistanceShoot(fromPos, toPos, distanceEffect, ...) return Position(fromPos):sendDistanceEffect(toPos, distanceEffect, ...) end
function isSightClear(fromPos, toPos, floorCheck) return Position(fromPos):isSightClear(toPos, floorCheck) end

function getPromotedVocation(vocationId)
	local vocation = Vocation(vocationId)
	if vocation == nil then
		return 0
	end

	local promotedVocation = vocation:getPromotion()
	if promotedVocation == nil then
		return 0
	end
	return promotedVocation:getId()
end

function getGuildId(guildName)
	local resultId = db.query("SELECT `id` FROM `guilds` WHERE `name` = " .. db.escapeString(guildName))
	if resultId == false then
		return false
	end

	local guildId = result.getDataInt(resultId, "id")
	result.free(resultId)
	return guildId
end

function getHouseName(houseId) local h = House(houseId) return h ~= nil and h:getName() or false end
function getHouseOwner(houseId) local h = House(houseId) return h ~= nil and h:getOwnerGuid() or false end
function getHouseEntry(houseId) local h = House(houseId) return h ~= nil and h:getExitPosition() or false end
function getHouseTown(houseId) local h = House(houseId) if h == nil then return false end local t = h:getTown() return t ~= nil and t:getId() or false end
function getHouseTilesSize(houseId) local h = House(houseId) return h ~= nil and h:getTileCount() or false end

function isItemStackable(itemId) return ItemType(itemId):isStackable() end
function isItemRune(itemId) return ItemType(itemId):isRune() end
function isItemDoor(itemId) return ItemType(itemId):isDoor() end
function isItemContainer(itemId) return ItemType(itemId):isContainer() end
function isItemFluidContainer(itemId) return ItemType(itemId):isFluidContainer() end
function isItemMovable(itemId) return ItemType(itemId):isMovable() end

isItemMoveable = isItemMovable

function getItemName(itemId) return ItemType(itemId):getName() end
function getItemWeight(itemId, ...) return ItemType(itemId):getWeight(...) end
function getItemDescriptions(itemId)
	local itemType = ItemType(itemId)
	return {
		name = itemType:getName(),
		plural = itemType:getPluralName(),
		article = itemType:getArticle(),
		description = itemType:getDescription()
	}
end
function getItemIdByName(name)
	local id = ItemType(name):getId()
	if id == 0 then
		return false
	end
	return id
end
function getContainerCapById(itemId) return ItemType(itemId):getCapacity() end

function queryTileAddThing(thing, pos, ...) local t = Tile(pos) return t ~= nil and t:queryAdd(thing, ...) or false end
