local mType = Game.createMonsterType("Tremorak")
local monster = {}
monster.description = "tremorak"
monster.experience = 1300
monster.outfit = {
	lookType = 285
}

monster.health = 10000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 8933
monster.speed = 290
monsters.runHealth = 1
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 5
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
	interval = 2000,
	chance = 9,
	{text = "STOMP STOMP!", yell = true}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 36, minDamage = 0, maxDamage = -115, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 16, interval = 2000, minDamage = 0, maxDamage = -255, radius = 7, effect = 35},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 16, interval = 2000, minDamage = 0, maxDamage = -405, length = 8, effect = 35},
	{name = "condition", type = CONDITION_POISON,  chance = 16, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 15, effect = 35}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 16, interval = 2000, minDamage = 75, maxDamage = 200, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 50},
	{type = COMBAT_ENERGYDAMAGE, percent = 85},
	{type = COMBAT_FIREDAMAGE, percent = -15},
	{type = COMBAT_HOLYDAMAGE, percent = 50},
	{type = COMBAT_DEATHDAMAGE, percent = 45},
	{type = COMBAT_ICEDAMAGE, percent = 20}
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