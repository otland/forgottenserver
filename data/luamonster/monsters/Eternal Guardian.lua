local mType = Game.createMonsterType("Eternal Guardian")
local monster = {}
monster.description = "a eternal guardian"
monster.experience = 1800
monster.outfit = {
	lookType = 345
}

monster.health = 2500
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 11300
monster.speed = 180
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
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
	interval = 5000,
	chance = 10,
	{text = "Zzrkrrch!", yell = false},
	{text = "<crackle>", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 99930},
	{id = "platinum coin", maxCount = 4, chance = 99540},
	{id = "small stone", maxCount = 10, chance = 30230},
	{id = "shiny stone", chance = 800},
	{id = "ancient stone", chance = 20020},
	{id = "spiked iron ball", chance = 9960},
	{id = "Zaoan halberd", chance = 1860},
	{id = "coal", chance = 400},
	{id = "iron ore", chance = 1700},
	{id = "clay lump", chance = 720},
	{id = "piece of marble rock", chance = 430},
	{id = "tower shield", chance = 820},
	{id = "guardian halberd", chance = 560},
	{id = "Zaoan sword", chance = 100}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 50, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 25},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = 70},
	{type = COMBAT_HOLYDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = 10}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
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