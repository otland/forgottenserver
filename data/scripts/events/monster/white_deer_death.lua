local config = {
	{chance = 30, monster = "Enraged White Deer", message = "The white deer summons all his strength and turns to fight!"},
	{chance = 100, monster = "Desperate White Deer", message = "The white deer desperately tries to escape!"}
}

local event = Event()

event.onCreatureDeath = function(self, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
	if not self:isMonster() then
		return
	end

	local mType = self:getType()
	if mType:monsterName() ~= "white deer" then
		return
	end

	local targetMonster = self:getMonster()
	if not targetMonster or targetMonster:getMaster() then
		return
	end

	local chance = math.random(100)
	for i = 1, #config do
		if chance <= config[i].chance then
			local spawnMonster = Game.createMonster(config[i].monster, targetMonster:getPosition(), true, true)
			if spawnMonster then
				spawnMonster:getPosition():sendMagicEffect(CONST_ME_TELEPORT)
				targetMonster:say(config[i].message, TALKTYPE_MONSTER_SAY)
			end
			break
		end
	end
end

event:register()
