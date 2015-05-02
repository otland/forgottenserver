local mType = Game.createMonsterType("Bear")
local monster = {}
monster.description = "a bear"
monster.experience = 23
monster.outfit = {
	lookType = 16
}

monster.health = 80
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5975
monster.speed = 145
monster.runHealth = 15
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Grrrr", yell = false},
	{text = "Groar", yell = false}
}

monster.loot = {
    {id = "bear paw", chance = 2000},
	{id = "ham", maxCount = 3, chance = 20000},
	{id = "meat", maxCount = 4, chance = 39750},
	{id = "honeycomb", chance = 460}
}

monster.attacks = {
	{name = "melee",  attack = 10, skill = 40, minDamage = 0, maxDamage = -25, interval = 2000, effect = 0}
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