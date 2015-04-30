local mType = Game.createMonsterType("Fire Overlord")
local monster = {}
monster.description = "a fire overlord"
monster.experience = 2800
monster.outfit = {
	lookType = 243
}

monster.health = 4000
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 8964
monster.speed = 300
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "eternal flames", chance = 100000},
	{id = "fiery heart", chance = 100000},
	{id = "gold coin", maxCount = 75, chance = 50000},
	{id = "platinum coin", maxCount = 3, chance = 50000},
	{id = "magma coat", chance = 819}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 80, interval = 2000, effect = 0},
	{name = "firefield",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 4, shootEffect = 4},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 1000, minDamage = -300, maxDamage = -900, length = 1, spread = 3, radius = 4, shootEffect = 4, effect = 37},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 13, interval = 1000, minDamage = -200, maxDamage = -350, target = true, radius = 4, shootEffect = 4, effect = 7}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 1},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 80},
	{type = COMBAT_DEATHDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = -25}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)