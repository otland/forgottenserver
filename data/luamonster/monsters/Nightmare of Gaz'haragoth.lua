local mType = Game.createMonsterType("Nightmare of Gaz'haragoth")
local monster = {}
monster.description = "nightmare of gaz'haragoth"
monster.experience = 5700
monster.outfit = {
	lookType = 587
}

monster.health = 9400
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 22497
monster.speed = 270
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
	{text = "Knorrrr!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 30000},
	{id = "platinum coin", maxCount = 10, chance = 33333},
	{id = "great mana potion", maxCount = 4, chance = 3100},
	{id = "iron ore", maxCount = 2, chance = 2195},
	{id = "ultimate health potion", maxCount = 2, chance = 2195},
	{id = "prismatic bolt", maxCount = 8, chance = 366},
	{id = "black pearl", chance = 866},
	{id = "white pearl", chance = 866},
	{id = "wand of defiance", chance = 1766},
	{id = "glacial rod", chance = 1066},
	{id = "knight legs", chance = 444},
	{id = "crystal of power", chance = 1066},
	{id = "giant sword", chance = 422},
	{id = "shiny stone", chance = 1032},
	{id = "magic sulphur", chance = 632},
	{id = "crown armor", chance = 632},
	{id = "crystal crossbow", chance = 603},
	{id = "crystalline sword", chance = 603}
}

monster.attacks = {
	{name = "melee",  attack = 90, skill = 90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 1800, minDamage = -300, maxDamage = -685, length = 7, effect = 45},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 9, interval = 2000, minDamage = -250, maxDamage = -590, radius = 6, effect = 51}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = true, combat = true},
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