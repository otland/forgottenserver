local mType = Game.createMonsterType("Spectral Scum")
local monster = {}
monster.description = "a spectral scum"
monster.experience = 5
monster.outfit = {
	lookType = 195
}

monster.health = 190
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6070
monster.speed = 230
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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 2000,
	chance = 10,
	{text = "BBooze! More booze!, Harrr Harrr!", yell = false},
	{text = "Burp, Fart.", yell = false},
	{text = "Nothing better than plundering and getting palstered!", yell = false},
	{text = "Yarrr!, A pirate's unlife for me!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 55, skill = 0, interval = 2000, effect = 0},
	{name = "drunk",  chance = 10, interval = 5000, minDamage = 0, maxDamage = 0, range = 2, effect = 3}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)