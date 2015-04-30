local mType = Game.createMonsterType("Golgordan")
local monster = {}
monster.description = "golgordan"
monster.experience = 10000
monster.outfit = {
	lookFeet = 114,
	lookLegs = 105,
	lookHead = 108,
	lookType = 12,
	lookBody = 100
}

monster.health = 40000
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 6068
monster.speed = 350
monster.maxSummons = 0

monster.changeTarget = {
    interval = 7000,
	chance = 10
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
	staticAttack = 85
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Latrivan your fool", yell = true}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 105, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 15, interval = 3000, minDamage = -250, maxDamage = -850, length = 8, effect = 18},
	{name = "condition", type = CONDITION_POISON,  chance = 11, interval = 1000, minDamage = -300, maxDamage = -350, length = 5, spread = 2, effect = 21},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 8, interval = 1000, minDamage = -50, maxDamage = -955, radius = 6, effect = 35},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -50, maxDamage = -200, target = true, radius = 7, effect = 7}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 15},
	{type = COMBAT_FIREDAMAGE, percent = -5}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)