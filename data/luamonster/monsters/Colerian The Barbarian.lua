local mType = Game.createMonsterType("Colerian The Barbarian")
local monster = {}
monster.description = "colerian the barbarian"
monster.experience = 90
monster.outfit = {
	lookFeet = 43,
	lookLegs = 115,
	lookHead = 76,
	lookType = 253,
	lookBody = 115
}

monster.health = 265
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7349
monster.speed = 190
monster.maxSummons = 0

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

monster.voices = {
	interval = 2000,
	chance = 5,
	{text = "Flee, coward!", yell = false},
	{text = "You will lose!", yell = false},
	{text = "Yeehaawh", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 20, minDamage = 0, maxDamage = -60, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 80, interval = 1000, minDamage = 0, maxDamage = -40, target = true, range = 5, radius = 1, shootEffect = 24}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -20}
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