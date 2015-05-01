local mType = Game.createMonsterType("Earth Overlord")
local monster = {}
monster.description = "an earth overlord"
monster.experience = 2800
monster.outfit = {
	lookType = 285
}

monster.health = 4000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 8933
monster.speed = 330
monster.runHealth = 1
monster.maxSummons = 0

monster.changeTarget = {
    interval = 20000,
	chance = 30
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
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "gold coin", maxCount = 68, chance = 100000},
	{id = "mother soil", chance = 100000},
	{id = "lump of earth", chance = 33333},
	{id = "platinum coin", maxCount = 3, chance = 33333},
	{id = "shiny stone", chance = 8333},
	{id = "terra mantle", chance = 1923},
	{id = "coal", chance = 552}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 90, minDamage = 0, maxDamage = -400, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 1000, minDamage = 0, maxDamage = -800, length = 7, effect = 45},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 9, interval = 1000, minDamage = 0, maxDamage = -490, radius = 6, effect = 51},
	{name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, duration = 4000, speedChange = -750, range = 7, effect = 14}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)