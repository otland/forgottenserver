local event = Event()

event.onCreatureDeath = function(self, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
	if not self:isMonster() then
		return
	end

	local mType = self:getType()
	if mType:monsterName() ~= "scarab" then
		return
	end

	if math.random(100) < 10 then
		Game.createMonster("Scorpion", self:getPosition())
		self:say("Horestis' curse spawns a vengeful scorpion from the body!", TALKTYPE_MONSTER_SAY)
	end
end

event:register()
