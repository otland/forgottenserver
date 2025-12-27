local event = Event()

event.onCreatureDeath = function(self, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
	if not self:isMonster() then
		return
	end

	local mType = self:getType()
	if mType:monsterName() ~= "enraged white deer" and mType:monsterName() ~= "desperate white deer" then
		return
	end

	local targetMonster = self:getMonster()
	if not targetMonster or targetMonster:getMaster() then
		return
	end

	local chance = math.random(100)
	if chance <= 10 then
		for i = 1, 2 do
			local spawnMonster = Game.createMonster("Elf Scout", targetMonster:getPosition(), true, true)
			if spawnMonster then
				spawnMonster:getPosition():sendMagicEffect(CONST_ME_TELEPORT)
			end
		end
		targetMonster:say("The elves came too late to save the deer, however they might avenge it.", TALKTYPE_MONSTER_SAY)
	end
end

event:register()
