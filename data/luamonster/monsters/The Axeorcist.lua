local mType = Game.createMonsterType("The Axeorcist")
local monster = {}
monster.description = "the axeorcist"
monster.experience = 3000
monster.outfit = {
	lookType = 8
}

monster.health = 5000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5980
monster.speed = 250
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 9
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
	chance = 9,
	{text = "DEESTRUCTIOON!", yell = true},
	{text = "Blood! Carnage! Muhahaha!", yell = true}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 45, minDamage = 0, maxDamage = -275, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 10}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)