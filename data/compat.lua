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

function isCreature(cid) return _Creature(cid) ~= nil end
function isPlayer(cid) return _Player(cid) ~= nil end
function isMonster(cid) return Monster(cid) ~= nil end
function isNpc(cid) return Npc(cid) ~= nil end
function isItem(uid) return Item(uid) ~= nil end
function isContainer(uid) return Container(uid) ~= nil end

function getCreatureName(cid) return _Creature(cid):getName() end
function getCreatureHealth(cid) return _Creature(cid):getHealth() end
function getCreatureMaxHealth(cid) return _Creature(cid):getMaxHealth() end
function getCreaturePosition(cid) return _Creature(cid):getPosition() end

getCreaturePos = getCreaturePosition

function doCreatureAddHealth(cid, health) return _Creature(cid):addHealth(health) end
function doRemoveCreature(cid) return _Creature(cid):remove() end
function doCreatureSetLookDir(cid, direction) return _Creature(cid):setDirection(direction) end
function doCreatureSay(cid, text, type, ...) return _Creature(cid):say(text, type, ...) end

doSetCreatureDirection = doCreatureSetLookDir

function registerCreatureEvent(cid, name) return _Creature(cid):registerEvent(name) end
function unregisterCreatureEvent(cid, name) return _Creature(cid):unregisterEvent(name) end

function getPlayerByName(name)
	local player = Player(name)
	if player == nil then
		return false
	end
	return player:getId()
end
function getIPByPlayerName(name)
	local player = Player(name)
	if player == nil then
		return false
	end
	return player:getIp()
end
function getPlayerGUID(cid) return _Player(cid):getGuid() end
function getPlayerIp(cid) return _Player(cid):getIp() end
function getPlayerAccountType(cid) return _Player(cid):getAccountType() end
function getPlayerLastLoginSaved(cid) return _Player(cid):getLastLoginSaved() end
function getPlayerName(cid) return _Player(cid):getName() end
function getPlayerFreeCap(cid) return _Player(cid):getFreeCapacity() end
function getPlayerPosition(cid) return _Player(cid):getPosition() end
function getPlayerMagLevel(cid) return _Player(cid):getMagicLevel() end
function getPlayerAccess(cid) return _Player(cid):getGroup():getAccess() end
function getPlayerSkill(cid, skillId) return _Player(cid):getSkillLevel(skillId) end
function getPlayerMana(cid) return _Player(cid):getMana() end
function getPlayerMaxMana(cid) return _Player(cid):getMaxMana() end
function getPlayerLevel(cid) return _Player(cid):getLevel() end
function getPlayerTown(cid) return _Player(cid):getTown():getId() end
function getPlayerVocation(cid) return _Player(cid):getVocation():getId() end
function getPlayerSoul(cid) return _Player(cid):getSoul() end
function getPlayerSex(cid) return _Player(cid):getSex() end
function getPlayerStorageValue(cid, key) return _Player(cid):getStorageValue(key) end
function getPlayerBalance(cid) return _Player(cid):getBankBalance() end
function getPlayerMoney(cid) return _Player(cid):getMoney() end
function getPlayerGroupId(cid) return _Player(cid):getGroup():getId() end
function getPlayerLookDir(cid) return _Player(cid):getDirection() end
function getPlayerLight(cid) return _Player(cid):getLight() end
function getPlayerDepotItems(cid, depotId) return _Player(cid):getDepotItems(depotId) end
function getPlayerSkullType(cid) return _Player(cid):getSkull() end
function getPlayerLossPercent(cid) return _Player(cid):getDeathPenalty() end
function getPlayerMount(cid, mountId) return _Player(cid):hasMount(mountId) end
function getPlayerPremiumDays(cid) return _Player(cid):getPremiumDays() end
function getPlayerBlessing(cid, blessing) return _Player(cid):hasBlessing(blessing) end
function getPlayerGuildId(cid)
	local guild = _Player(cid):getGuild()
	if guild == nil then
		return false
	end
	return guild:getId()
end
function getPlayerGuildLevel(cid) return _Player(cid):getGuildLevel() end
function getPlayerGuildName(cid)
	local guild = _Player(cid):getGuild()
	if guild == nil then
		return false
	end
	return guild:getName()
end
function getPlayerGuildRank(cid)
	local player = _Player(cid)
	local guild = player:getGuild()
	if guild == nil then
		return false
	end
	
	local rank = guild:getRankByLevel(player:getGuildLevel())
	if rank == nil then
		return false
	end
	return rank.name
end
function getPlayerGuildNick(cid) return _Player(cid):getGuildNick() end
function getPlayerMasterPos(cid) return _Player(cid):getTown():getTemplePosition() end
function getPlayerItemCount(cid, itemId, ...) return _Player(cid):getItemCount(itemId, ...) end

getPlayerAccountBalance = getPlayerBalance
getIPByName = getIPByPlayerName

function setPlayerStorageValue(cid, key, value) return _Player(cid):setStorageValue(key, value) end
function doPlayerSetBalance(cid, balance) return _Player(cid):setBankBalance(balance) end
function doPlayerAddMoney(cid, money) return _Player(cid):addMoney(money) end
function doPlayerRemoveMoney(cid, money) return _Player(cid):removeMoney(money) end
function doPlayerAddSoul(cid, soul) return _Player(cid):addSoul(soul) end
function doPlayerSetVocation(cid, vocation) return _Player(cid):setVocation(Vocation(vocation)) end
function doPlayerSetTown(cid, town) return _Player(cid):setTown(Town(town)) end
function setPlayerGroupId(cid, groupId) return _Player(cid):setGroup(Group(groupId)) end
function doPlayerSetSex(cid, sex) return _Player(cid):setSex(sex) end
function doShowTextDialog(cid, itemId, text) return _Player(cid):showTextDialog(itemId, text) end
function doPlayerAddItemEx(cid, uid, ...) return _Player(cid):addItemEx(Item(uid), ...) end
function doPlayerAddPremiumDays(cid, days) return _Player(cid):addPremiumDays(days) end
function doPlayerRemovePremiumDays(cid, days) return _Player(cid):removePremiumDays(days) end
function doPlayerAddBlessing(cid, blessing) return _Player(cid):addBlessing(blessing) end

function getTownId(townName) return Town(townName):getId() end
function getTownName(townId) return Town(townId):getName() end
function getTownTemplePosition(townId) return Town(townId):getTemplePosition() end

function doSetItemActionId(uid, actionId) return Item(uid):setActionId(actionId) end
function doTransformItem(uid, newItemId, ...) return Item(uid):transform(newItemId, ...) end

function getContainerSize(uid) return Container(uid):getSize() end
function getContainerCap(uid) return Container(uid):getCapacity() end

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
