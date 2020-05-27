local fields = {
	[1423] = {type = CONDITION_FIRE, list = DAMAGELIST_VARYING_PERIOD, stepDamage = 20, periodicDamage = 10, period = {10, 12}, rounds = 2}, -- campfire
	[1424] = {type = CONDITION_FIRE, list = DAMAGELIST_VARYING_PERIOD, stepDamage = 20, periodicDamage = 10, period = {10, 12}, rounds = 2}, -- campfire
	[1425] = {type = CONDITION_FIRE, list = DAMAGELIST_VARYING_PERIOD, stepDamage = 20, periodicDamage = 10, period = {10, 12}, rounds = 2}, -- campfire
	[1487] = {type = CONDITION_FIRE, list = DAMAGELIST_VARYING_PERIOD, stepDamage = 20, periodicDamage = 10, period = {10, 12}, rounds = 7}, -- fire field
	[1488] = {type = CONDITION_FIRE, list = DAMAGELIST_VARYING_PERIOD, stepDamage = 10, periodicDamage = 10, period = {10, 12}, rounds = 7}, -- fire field
	[1490] = {type = CONDITION_POISON, list = DAMAGELIST_LOGARITHMIC_DAMAGE, stepDamage = 5, periodicDamage = 5}, -- poison field
	[1491] = {type = CONDITION_ENERGY, list = DAMAGELIST_VARYING_PERIOD, stepDamage = 30, periodicDamage = 25, period = {8, 10}}, -- energy field
	[1492] = {type = CONDITION_FIRE, list = DAMAGELIST_VARYING_PERIOD, stepDamage = 20, periodicDamage = 10, period = {10, 12}, rounds = 7}, -- fire field
	[1493] = {type = CONDITION_FIRE, list = DAMAGELIST_VARYING_PERIOD, stepDamage = 10, periodicDamage = 10, period = {10, 12}, rounds = 7}, -- fire field
	[1495] = {type = CONDITION_ENERGY, list = DAMAGELIST_VARYING_PERIOD, stepDamage = 30, periodicDamage = 25, period = {8, 10}}, -- energy field
	[1496] = {type = CONDITION_POISON, list = DAMAGELIST_LOGARITHMIC_DAMAGE, stepDamage = 5, periodicDamage = 5}, -- poison field
	[1497] = {harmless = true}, -- magic wall
	[1498] = {harmless = true}, -- magic wall
	[1499] = {harmless = true}, -- rush wood
	[1500] = {type = CONDITION_FIRE, list = DAMAGELIST_VARYING_PERIOD, stepDamage = 20, periodicDamage = 10, period = {10, 12}, rounds = 7}, -- fire field
	[1501] = {type = CONDITION_FIRE, list = DAMAGELIST_VARYING_PERIOD, stepDamage = 10, periodicDamage = 10, period = {10, 12}, rounds = 7}, -- fire field
	[1503] = {type = CONDITION_POISON, list = DAMAGELIST_LOGARITHMIC_DAMAGE, stepDamage = 5, periodicDamage = 5}, -- poison gas
	[1504] = {type = CONDITION_ENERGY, list = DAMAGELIST_VARYING_PERIOD, stepDamage = 30, periodicDamage = 25, period = {8, 10}}, -- energy field
	[1506] = {type = CONDITION_FIRE, stepDamage = 300}, -- searing fire
	[1507] = {type = CONDITION_FIRE, stepDamage = 300}, -- searing fire
	[7465] = {type = CONDITION_FIRE, stepDamage = 300}, -- searing fire
	[7466] = {type = CONDITION_FIRE, stepDamage = 300}, -- searing fire
	[7467] = {type = CONDITION_FIRE, stepDamage = 300}, -- searing fire
	[7468] = {type = CONDITION_FIRE, stepDamage = 300}, -- searing fire
	[7469] = {type = CONDITION_FIRE, stepDamage = 300}, -- searing fire
	[7470] = {type = CONDITION_FIRE, stepDamage = 300}, -- searing fire
	[7471] = {type = CONDITION_FIRE, stepDamage = 300}, -- searing fire
	[7472] = {type = CONDITION_FIRE, stepDamage = 300}, -- searing fire
	[7473] = {type = CONDITION_FIRE, stepDamage = 300}, -- searing fire
	[10986] = {type = CONDITION_FIRE, list = DAMAGELIST_VARYING_PERIOD, stepDamage = 20, periodicDamage = 10, period = {10, 12}, rounds = 7}, -- fire field
	[10987] = {type = CONDITION_FIRE, list = DAMAGELIST_VARYING_PERIOD, stepDamage = 10, periodicDamage = 10, period = {10, 12}, rounds = 7}, -- fire field
	[10988] = {type = CONDITION_FIRE, list = DAMAGELIST_VARYING_PERIOD, stepDamage = 10, periodicDamage = 5, period = {10, 12}, rounds = 7}, -- fire field
	[11098] = {harmless = true}, -- magic wall (safe)
	[11099] = {harmless = true} -- rush wood (safe)
}

function onStepIn(creature, item, position, fromPosition)
	local field = fields[item:getId()]
	if field.harmless or item:getType():isBlocking() then
		if not creature:isInGhostMode() then
			item:remove()
		end
		return true
	end

	Creature.addDamageCondition(item:getOwner(), creature, field.type, field.list, {step = field.stepDamage, periodic = field.periodicDamage}, field.period, field.rounds, item)
	return true
end

function onAddItem(item, tile, position)
	local field, owner = fields[item:getId()], item:getOwner()
	for _, target in ipairs(Tile(position):getCreatures()) do
		Creature.addDamageCondition(owner, target, field.type, field.list, {step = field.stepDamage, periodic = field.periodicDamage}, field.period, field.rounds, item)
	end
	return true
end
