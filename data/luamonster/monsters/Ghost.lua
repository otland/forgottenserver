local mType = Game.createMonsterType("Ghost")
local monster = {}
monster.description = "a ghost"
monster.experience = 120
monster.outfit = {
	lookType = 48
}

monster.health = 150
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 5993
monster.speed = 160
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
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
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Huh!", yell = false},
	{text = "Shhhhhh", yell = false},
	{text = "Buuuuuh", yell = false}
}

monster.loot = {
    {id = "morning star", chance = 10610},
	{id = "combat knife", chance = 7002},
	{id = "shadow herb", chance = 14400},
	{id = "cape", chance = 8800},
	{id = "book", chance = 1310},
	{id = "white piece of cloth", chance = 1940},
	{id = "ancient shield", chance = 860},
	{id = "stealth ring", chance = 180},
	{id = "ghostly tissue", chance = 1870}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 30, minDamage = 0, maxDamage = -80, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -20, maxDamage = -45, range = 1, effect = 14}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = true},
	{type = "bleed", condition = true, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)