local mType = Game.createMonsterType("morik the gladiator")
local monster = {}

monster.name = "Morik The Gladiator"
monster.description = "Morik The Gladiator"
monster.experience = 160
monster.outfit = {
	lookType = 131,
	lookHead = 57,
	lookBody = 57,
	lookLegs = 57,
	lookFeet = 57,
	lookAddons = 1,
	lookMount = 0
}

monster.health = 1235
monster.maxHealth = 1235
monster.runHealth = 50
monster.race = "blood"
monster.corpse = 20450
monster.speed = 200
monster.summonCost = 0
monster.maxSummons = 2

monster.changeTarget = {
	interval = 2000,
	chance = 0
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = false,
	boss = true,
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
	{text = "To be the one you'll have to beat the one!", yell = false},
	{text = "Where did I put my ultimate health potion again?", yell = false},
	{text = "I am the best!", yell = false},
	{text = "I'll take your ears as a trophy!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = 10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = -10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -200, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -110, radius = 3, target = false, effect = CONST_ME_HITAREA},
	{name ="drunk", interval = 2000, chance = 10, drunkenness = 25, duration = 4000, ShootEffect = CONST_ANI_WHIRLWINDCLUB, effect = CONST_ME_STUN}
}

monster.defenses = {
	defense = 22,
	armor = 20,
	{name ="speed", interval = 1000, chance = 9, speed = {min = 215, max = 215}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.summons = {
	{name = "Gladiator", chance = 10, interval = 2000}
}

monster.loot = {
	{id = "Morik's helmet", chance = 100000}
}

mType:register(monster)
