local mType = Game.createMonsterType("Acolyte of the Cult")
local monster = {}
monster.description = "an acolyte of the cult"
monster.experience = 300
monster.outfit = {
	lookFeet = 57,
	lookLegs = 121,
	lookHead = 114,
	lookType = 194,
	lookBody = 121
}

monster.health = 390
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20319
monster.speed = 200
monster.maxSummons = 1

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
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.summons = {
    {name = "Skeleton", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Praise the voodoo!", yell = false},
	{text = "Power to the cult!", yell = false},
	{text = "Feel the power of the cult!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 40, chance = 66940},
	{id = "rope belt", chance = 10420},
	{id = "cultish robe", chance = 8070},
	{id = "morning star", chance = 4990},
	{id = "pirate voodoo doll", chance = 1060},
	{id = "dragon necklace", subType = 200, chance = 1050},
	{id = "book", chance = 730},
	{id = "life ring", chance = 560},
	{id = "small emerald", chance = 550},
	{id = "music sheet", chance = 480},
	{id = "terra rod", chance = 250},
	{id = "broken key ring", chance = 60},
	{id = "cultish symbol", chance = 40}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 20, interval = 2000, minDamage = -60, maxDamage = -120, target = true, range = 7, radius = 1, shootEffect = 15, effect = 14},
	{name = "drunk",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 1, shootEffect = 31, effect = 40}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 40, maxDamage = 60, radius = 1, shootEffect = 31, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = 20}
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