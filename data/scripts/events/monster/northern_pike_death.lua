local event = Event()

event.onCreatureDeath = function(self, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
	if not self:isMonster() then
		return
	end

	local mType = self:getType()
	if mType:monsterName() ~= "northern pike" then
		return
	end

	if math.random(100) < 11 then
		Game.createMonster("Slippery Northern Pike", self:getPosition(), false, true)
	end
end

event:register()
