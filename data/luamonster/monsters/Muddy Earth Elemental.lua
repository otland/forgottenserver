local mType = Game.createMonsterType("Muddy Earth Elemental")
local monster = {}
monster.description = "a muddy earth elemental"
monster.experience = 450
monster.outfit = {
	lookType = 301
}

monster.health = 650
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 8933
monster.speed = 260
monster.runHealth = 1
monster.maxSummons = 0

monster.changeTarget = {
    interval = 20000,
	chance = 50
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 80
}

monster.loot = {
    {id = "gold coin", maxCount = 80, chance = 24500},
	{id = "gold coin", maxCount = 47, chance = 24500},
	{id = "small stone", maxCount = 3, chance = 40000},
	{id = "some leaves", chance = 22000},
	{id = "natural soil", chance = 3750}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 30, minDamage = 0, maxDamage = -160, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -25, maxDamage = -155, target = true, range = 7, radius = 2, effect = 45},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 1000, minDamage = 0, maxDamage = -26, length = 6, effect = 35}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 20},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -25},
	{type = COMBAT_DEATHDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = 20}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)