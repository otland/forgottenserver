local mType = Game.createMonsterType("renegade knight")
local monster = {}

monster.name = "Renegade Knight"
monster.description = "a renegade knight"
monster.experience = 1200
monster.outfit = {
	lookType = 268,
	lookHead = 97,
	lookBody = 113,
	lookLegs = 76,
	lookFeet = 98,
	lookAddons = 2,
	lookMount = 0
}

monster.health = 1450
monster.maxHealth = 1450
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20418
monster.speed = 210
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
	chance = 5
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = false,
	boss = false,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 90,
	targetDistance = 1,
	healthHidden = false,
	canWalkOnEnergy = false,
	canWalkOnFire = false,
	canWalkOnPoison = false
}

monster.light = {
	level = 0,
	color = 0
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "I'll teach you a lesson!", yell = false},
	{text = "Feel my steel!", yell = false},
	{text = "Take this!", yell = false},
	{text = "Your abilities are no match for mine!", yell = false},
	{text = "Let's see how good you are!", yell = false},
	{text = "A challenge at last!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 35},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 50},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, skill = 60, attack = 70, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = -25, maxDamage = -125, radius = 3, target = false, effect = CONST_ME_BLOCKHIT}
}

monster.defenses = {
	defense = 33,
	armor = 27,
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_HEALING, minDamage = 200, maxDamage = 250, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = 12406, chance = 1655},
	{id = 2148, chance = 100000, maxCount = 100},
	{id = 2152, chance = 25165, maxCount = 2},
	{id = 12466, chance = 4635},
	{id = 2666, chance = 7615},
	{id = 2377, chance = 5960},
	{id = 2391, chance = 993},
	{id = 5911, chance = 2317},
	{id = 2487, chance = 662},
	{id = 2491, chance = 331},
	{id = 7591, chance = 7284},
	{id = 2147, chance = 6622},
	{id = 2114, chance = 331},
	{id = 2488, chance = 331}
}

mType:register(monster)
