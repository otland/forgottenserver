local mType = Game.createMonsterType("vile grandmaster")
local monster = {}

monster.name = "Vile Grandmaster"
monster.description = "a vile grandmaster"
monster.experience = 1500
monster.outfit = {
	lookType = 268,
	lookHead = 59,
	lookBody = 19,
	lookLegs = 95,
	lookFeet = 94,
	lookAddons = 1,
	lookMount = 0
}

monster.health = 1700
monster.maxHealth = 1700
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
	{text = "You'll make a fine trophy!", yell = false},
	{text = "Is that the best, they can throw at me?", yell = false},
	{text = "I've seen orcs tougher than you!", yell = false},
	{text = "I will end this now!", yell = false},
	{text = "Your gods won't help you!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 25},
	{type = COMBAT_EARTHDAMAGE, percent = 25},
	{type = COMBAT_FIREDAMAGE, percent = 25},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 50},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, skill = 94, attack = 50, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 22, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -225, range = 7, ShootEffect = CONST_ANI_WHIRLWINDSWORD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = -25, maxDamage = -125, radius = 4, target = false, effect = CONST_ME_GROUNDSHAKER}
}

monster.defenses = {
	defense = 33,
	armor = 27,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 220, maxDamage = 280, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = 12406, chance = 724},
	{id = 2148, chance = 100000, maxCount = 100},
	{id = 2148, chance = 60000, maxCount = 55},
	{id = 2152, chance = 73188, maxCount = 2},
	{id = 2121, chance = 7246},
	{id = 12466, chance = 8695},
	{id = 2666, chance = 7971},
	{id = 2147, chance = 2173},
	{id = 2146, chance = 2173},
	{id = 2391, chance = 1449},
	{id = 5911, chance = 2898},
	{id = 2487, chance = 1449},
	{id = 2491, chance = 1449},
	{id = 2392, chance = 2173},
	{id = 7591, chance = 9420},
	{id = 2488, chance = 724}
}

mType:register(monster)
