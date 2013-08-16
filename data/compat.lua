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

function getCreatureName(cid) return _Creature(cid):getName() end
function getCreatureHealth(cid) return _Creature(cid):getHealth() end
function getCreatureMaxHealth(cid) return _Creature(cid):getMaxHealth() end
function getCreaturePosition(cid) return _Creature(cid):getPosition() end

getCreaturePos = getCreaturePosition

function doCreatureAddHealth(cid, health) return _Creature(cid):addHealth(health) end
function doRemoveCreature(cid) return _Creature(cid):remove() end
function doCreatureSetLookDir(cid, direction) return _Creature(cid):setDirection(direction) end

doSetCreatureDirection = doCreatureSetLookDir

function getPlayerName(cid) return _Player(cid):getName() end
function getPlayerPosition(cid) return _Player(cid):getPosition() end
function getPlayerMagLevel(cid) return _Player(cid):getMagicLevel() end
function getPlayerAccess(cid) return _Player(cid):getGroup():getAccess() end
function getPlayerSkill(cid, skillId) return _Player(cid):getSkillLevel(skillId) end
function getPlayerMana(cid) return _Player(cid):getMana() end
function getPlayerMaxMana(cid) return _Player(cid):getMaxMana() end
function getPlayerLevel(cid) return _Player(cid):getLevel() end
function getPlayerTown(cid) return _Player(cid):getTown() end
function getPlayerVocation(cid) return _Player(cid):getVocation() end
function getPlayerSoul(cid) return _Player(cid):getSoul() end
function getPlayerSex(cid) return _Player(cid):getSex() end
function getPlayerStorageValue(cid, key) return _Player(cid):getStorageValue(key) end
function getPlayerBalance(cid) return _Player(cid):getBankBalance() end
function getPlayerMoney(cid) return _Player(cid):getMoney() end
function getPlayerGroupId(cid) return _Player(cid):getGroup():getId() end
function getPlayerLookDir(cid) return _Player(cid):getDirection() end
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

getPlayerAccountBalance = getPlayerBalance

function setPlayerStorageValue(cid, key, value) return _Player(cid):setStorageValue(key, value) end
function doPlayerSetBalance(cid, balance) return _Player(cid):setBankBalance(balance) end
function doPlayerAddMoney(cid, money) return _Player(cid):addMoney(money) end
function doPlayerRemoveMoney(cid, money) return _Player(cid):removeMoney(money) end
function doPlayerAddSoul(cid, soul) return _Player(cid):addSoul(soul) end
function doPlayerSetVocation(cid, vocation) return _Player(cid):setVocation(vocation) end
function doPlayerSetTown(cid, town) return _Player(cid):setTown(town) end
function setPlayerGroupId(cid, groupId) return _Player(cid):setGroup(Group(groupId)) end
function doPlayerSetSex(cid, sex) return _Player(cid):setSex(sex) end
function doShowTextDialog(cid, itemId, text) return _Player(cid):showTextDialog(cid, itemId, text) end
