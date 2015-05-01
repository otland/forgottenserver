local mType = Game.createMonsterType("Little Corym Charlatan")
local monster = {}
monster.description = "a little corym charlatan"
monster.experience = 40
monster.outfit = {
	lookLegs = 116,
	lookType = 532,
	lookBody = 97
}

monster.health = 90
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 19729
monster.speed = 170
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Squeeeeeeak!", yell = false},
	{text = "Must have it! Must have it!", yell = false},
	{text = "Gimme! Gimme!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 10, skill = 10, minDamage = 0, maxDamage = -10, interval = 2000, effect = 0}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)