local mType = Game.createMonsterType("The Dark Dancer")
local monster = {}
monster.description = "the dark dancer"
monster.experience = 435
monster.outfit = {
	lookType = 58
}

monster.health = 855
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7349
monster.speed = 170
monster.maxSummons = 3

monster.changeTarget = {
    interval = 0,
	chance = 0
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
	staticAttack = 95
}

monster.summons = {
    {name = "Ghoul", chance = 20, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 5,
	{text = "I hope you like my voice!", yell = false}
}

monster.attacks = {
	{name = "condition", type = CONDITION_POISON,  chance = 100, interval = 2000, minDamage = 0, maxDamage = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 70, interval = 3000, minDamage = -60, maxDamage = -95, target = true, range = 5, radius = 1, shootEffect = 32, effect = 18},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 90, interval = 6000, minDamage = -60, maxDamage = -95, target = true, range = 5, radius = 1, shootEffect = 32, effect = 18},
	{name = "speed",  chance = 35, interval = 1, minDamage = 0, maxDamage = 0, target = true, duration = 10000, range = 5, radius = 1, shootEffect = 32, effect = 14},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 30, interval = 4000, minDamage = -2, maxDamage = -110, target = true, range = 5, radius = 1, shootEffect = 32, effect = 14}
}

monster.defenses = {
	defense = 12,
    armor = 11,
    {name = "combat", type = COMBAT_HEALING,  chance = 45, interval = 2000, minDamage = 75, maxDamage = 135, shootEffect = 32, effect = 13},
	{name = "invisible",  chance = 50, interval = 3000, minDamage = 0, maxDamage = 0, shootEffect = 32, effect = 13}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)