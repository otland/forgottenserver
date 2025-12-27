local event = Event()

event.onCreatureDeath = function(self, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
	if not self:isMonster() then
		return
	end

	local mType = self:getType()
	if mType:monsterName() ~= "wild horse" then
		return
	end

	self:say("With its last strength the horse runs to safety.", TALKTYPE_MONSTER_SAY)
end

event:register()
