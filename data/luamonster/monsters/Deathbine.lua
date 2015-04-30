local mType = Game.createMonsterType("Deathbine")
local monster = {}
monster.description = "a deathbine"
monster.experience = 340
monster.outfit = {
	lookType = 120
}

monster.health = 525
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 6047
monster.speed = 240
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "carniphila seeds", chance = 100000},
	{id = "platinum coin", maxCount = 5, chance = 100000},
	{id = "shadow herb", chance = 100000},
	{id = "small emerald", maxCount = 4, chance = 100000},
	{id = "dark mushroom", chance = 50000},
	{id = "seeds", chance = 50000},
	{id = "terra amulet", subType = 200, chance = 50000},
	{id = "terra boots", chance = 50000},
	{id = "springsprout rod", chance = 50000},
	{id = "mandrake", chance = 5555},
	{id = "sweet smelling bait", chance = 2854}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 25, interval = 1000, minDamage = -60, maxDamage = -90, range = 7, shootEffect = 15, effect = 17},
	{name = "speed",  chance = 34, interval = 1000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = -850, range = 7, shootEffect = 15, effect = 17},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 12, interval = 1000, minDamage = -40, maxDamage = -130, radius = 3, shootEffect = 15, effect = 21}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_ICEDAMAGE, percent = 35},
	{type = COMBAT_FIREDAMAGE, percent = -20}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
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