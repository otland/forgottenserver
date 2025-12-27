local event = Event()

event.onCreatureThink = function(self, interval)
	if not self:isPlayer() then
		return
	end

	if Game.getWorldType() == WORLD_TYPE_PVP_ENFORCED then
		return true
	end

	local skullTime = self:getSkullTime()
	if skullTime <= 0 then
		return true
	end

	local elapsed = interval / 1000
	skullTime = math.max(0, skullTime - elapsed)

	self:setSkullTime(skullTime)

	if skullTime > 0 then
		return true
	end

	if self:hasCondition(CONDITION_INFIGHT) then
		return true
	end

	local skull = self:getSkull()
	if skull == SKULL_RED or skull == SKULL_BLACK then
		self:setSkull(SKULL_NONE)
	end
	return true
end

event:register()
