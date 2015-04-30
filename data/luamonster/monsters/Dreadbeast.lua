local mType = Game.createMonsterType("Dreadbeast")
local monster = {}
monster.description = "a dreadbeast"
monster.experience = 580
monster.outfit = {
	lookFeet = 50,
	lookLegs = 40,
	lookHead = 20,
	lookType = 101,
	lookBody = 30
}

monster.health = 515
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 3031
monster.speed = 200
monster.maxSummons = 0

monster.changeTarget = {
    interval = 60000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 80
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 20, interval = 2000, minDamage = -150, maxDamage = -250, target = true, radius = 1, shootEffect = 32, effect = 49},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 20, interval = 2000, minDamage = -150, maxDamage = -250, target = true, radius = 1, shootEffect = 32, effect = 48}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -25},
	{type = COMBAT_FIREDAMAGE, percent = 70}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
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