local mType = Game.createMonsterType("Barbarian Skullhunter")
local monster = {}
monster.description = "a barbarian skullhunter"
monster.experience = 85
monster.outfit = {
	lookFeet = 114,
	lookLegs = 77,
	lookType = 254,
	lookBody = 77
}

monster.health = 135
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20347
monster.speed = 220
monster.maxSummons = 0

monster.changeTarget = {
    interval = 60000,
	chance = 0
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
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "You will become my trophy.", yell = false},
	{text = "Fight harder, coward.", yell = false},
	{text = "Show that you are a worthy opponent.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 30, chance = 60000},
	{id = "torch", chance = 60500},
	{id = "brass helmet", chance = 19000},
	{id = "knife", chance = 15250},
	{id = "viking helmet", chance = 8000},
	{id = "scale armor", chance = 3850},
	{id = "skull", chance = 3000},
	{id = "crystal sword", chance = 90},
	{id = "health potion", chance = 930},
	{id = "brown piece of cloth", chance = 500},
	{id = "life ring", chance = 250},
	{id = "ragnir helmet", chance = 100},
	{id = "fur boots", chance = 120}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 20, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = -1},
	{type = COMBAT_ICEDAMAGE, percent = 50}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)