local mType = Game.createMonsterType("Crustacea Gigantica")
local monster = {}
monster.description = "a crustacea gigantica"
monster.experience = 1800
monster.outfit = {
	lookType = 383
}

monster.health = 1600
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 13331
monster.speed = 290
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
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Chrchrchr", yell = true},
	{text = "Klonklonk", yell = true},
	{text = "Chrrrrr", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "gold coin", maxCount = 43, chance = 100000},
	{id = "strong mana potion", chance = 14285},
	{id = "giant crab pincer", chance = 5000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = -60, maxDamage = -600, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 12},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = 10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)