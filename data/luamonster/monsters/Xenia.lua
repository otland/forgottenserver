local mType = Game.createMonsterType("Xenia")
local monster = {}
monster.description = "xenia"
monster.experience = 255
monster.outfit = {
	lookFeet = 95,
	lookLegs = 115,
	lookHead = 95,
	lookType = 137,
	lookBody = 115
}

monster.health = 290
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20542
monster.speed = 176
monster.runHealth = 10
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Stand still!", yell = false},
	{text = "One more head for me!", yell = false},
	{text = "Head off!", yell = false}
}

monster.loot = {
    {id = "worm", maxCount = 10, chance = 50000},
	{id = "gold coin", maxCount = 45, chance = 100000},
	{id = "chain armor", chance = 6666},
	{id = "crystal necklace", chance = 2500},
	{id = "double axe", chance = 3333},
	{id = "chain helmet", chance = 5000},
	{id = "red apple", maxCount = 4, chance = 10000},
	{id = "machete", chance = 10000},
	{id = "meat", maxCount = 3, chance = 20000},
	{id = "small ruby", maxCount = 2, chance = 2500},
	{id = "small diamond", maxCount = 2, chance = 2500},
	{id = "protection amulet", subType = 250, chance = 1000},
	{id = "plate armor", chance = 1333}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -50, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 1000, minDamage = -28, maxDamage = -42, range = 7, shootEffect = 1}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -6},
	{type = COMBAT_DEATHDAMAGE, percent = -7},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE, percent = 7},
	{type = COMBAT_FIREDAMAGE, percent = 10}
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