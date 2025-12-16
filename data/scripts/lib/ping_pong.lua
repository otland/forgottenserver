function Player.getLastPing(self)
  return self:getStorageValue(PlayerStorageKeys.lastPing)
end

function Player.setLastPing(self, time)
  self:setStorageValue(PlayerStorageKeys.lastPing, time)
end

function Player.getLastPong(self)
  return self:getStorageValue(PlayerStorageKeys.lastPong)
end

function Player.setLastPong(self, time)
  self:setStorageValue(PlayerStorageKeys.lastPong, time)
end
