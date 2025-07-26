local charmStatus = {
	UNLOCKED = 1,
	LOCKED = 0,
}

function Player.isCharmUnlocked(self, charmId)
	if (self:getStorageValue(PlayerStorageKeys.charmsUnlocked + charmId) or 0) == charmStatus.UNLOCKED then
		return charmStatus.UNLOCKED
	end
	return charmStatus.LOCKED
end

function Player.addCharmPoints(self, value)
	local points = self:getCharmPoints() or 0
	return self:setStorageValue(PlayerStorageKeys.charmPoints, points + value)
end

function Player.removeCharmPoints(self, value)
	local points = self:getCharmPoints() or 0

	if points < value then
		return false
	end
	return self:setStorageValue(PlayerStorageKeys.charmPoints, points - value)
end

function Player.unlockCharm(self, charmId)
	self:setStorageValue(PlayerStorageKeys.charmsUnlocked + charmId, charmStatus.UNLOCKED)
end

function Player.removeCharm(self, charmId)
	self:setStorageValue(PlayerStorageKeys.charmsUnlocked + charmId, charmStatus.LOCKED)
end

function Player.setCharmMonster(self, charmId, raceId)
	if self:isCharmUnlocked(charmId) == charmStatus.LOCKED then
		return false
	end
	return self:setStorageValue(PlayerStorageKeys.charmsMonster + charmId, raceId)
end

function Player.getCharmPoints(self)
	return math.max(0, self:getStorageValue(PlayerStorageKeys.charmPoints) or 0)
end
function Player.getCharmMonster(self, charmId)
	return math.max(0, self:getStorageValue(PlayerStorageKeys.charmsMonster + charmId) or 0)
end