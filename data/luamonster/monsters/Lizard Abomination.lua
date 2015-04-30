local mType = Game.createMonsterType("Lizard Abomination")
local monster = {}
monster.description = "lizard abomination"
monster.experience = 1350
monster.outfit = {
	lookType = 364
}

monster.health = 20000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 12385
monster.speed = 220
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
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
	staticAttack = 90
}

monster.voices = {
	interval = 2000,
	chance = 10,
	{text = "NOOOO! NOW YOU HERETICS WILL FACE MY GODLY WRATH!", yell = true},
	{text = "RAAARRRR! I WILL DEVOL YOU!", yell = true},
	{text = "I WILL MAKE YOU ZHEE!", yell = true}
}

monster.attacks = {
	{name = "melee",  attack = 110, skill = 90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 40, interval = 2000, minDamage = 0, maxDamage = -980, radius = 3, effect = 17},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 50, interval = 2000, minDamage = -200, maxDamage = -300, length = 8, radius = 3, effect = 14},
	{name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, speedChange = -800, radius = 3, effect = 21}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 15},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = -10},
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