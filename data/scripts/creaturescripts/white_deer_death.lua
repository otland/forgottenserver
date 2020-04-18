local config = {
	{chance = 30, monster = "Enraged White Deer", message = "The white deer summons all his strength and turns to fight!"},
	{chance = 100, monster = "Desperate White Deer", message = "The white deer desperately tries to escape!"}
}

local creatureevent = CreatureEvent("WhiteDeerDeath")

function creatureevent.onDeath(creature, corpse, lasthitkiller, mostdamagekiller, lasthitunjustified, mostdamageunjustified)
	local targetMonster = creature:getMonster()
	if not targetMonster or targetMonster:getMaster() then
		return true
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
	return true
end

creatureevent:register()
