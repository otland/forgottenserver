local mType = Game.createMonsterType("Jagged Earth Elemental")
local monster = {}
monster.description = "a jagged earth elemental"
monster.experience = 1300
monster.outfit = {
	lookType = 285
}

monster.health = 1500
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 8933
monster.speed = 280
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

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Stomp STOMP", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 27000},
	{id = "gold coin", maxCount = 90, chance = 27000},
	{id = "iron ore", maxCount = 2, chance = 800},
	{id = "natural soil", chance = 9000},
	{id = "seeds", chance = 1600},
	{id = "small emerald", maxCount = 2, chance = 3750},
	{id = "some leaves", chance = 10000},
	{id = "twigs", chance = 18000},
	{id = "gold coin", maxCount = 10, chance = 1500}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 1000, minDamage = -100, maxDamage = -250, length = 6, effect = 45},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 11, interval = 1000, minDamage = 0, maxDamage = -200, target = true, range = 7, radius = 6, shootEffect = 39, effect = 21}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -25}
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