local mType = Game.createMonsterType("the mutated pumpkin")
local monster = {}

monster.name = "The Mutated Pumpkin"
monster.description = "The Mutated Pumpkin"
monster.experience = 30000
monster.outfit = {
	lookType = 292,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 500000
monster.maxHealth = 500000
monster.runHealth = 0
monster.race = "venom"
monster.corpse = 8960
monster.speed = 330
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 5000,
	chance = 25
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
	{text = "I had the Halloween Hare for breakfast!", yell = false},
	{text = "Your soul will be mine...wait, wrong line", yell = false},
	{text = "Trick or treat? I saw death!", yell = false},
	{text = "No wait! Don't kill me! It's me, your friend!", yell = false},
	{text = "Bunnies, bah! I'm the real thing!", yell = false},
	{text = "Muahahahaha!", yell = false},
	{text = "I've come to avenge all those mutilated pumpkins!", yell = false},
	{text = "Wait until I get you!", yell = false},
	{text = "Fear the spirit of Halloween!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -300, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_MANADRAIN, minDamage = 0, maxDamage = -12, length = 5, spread = 2, effect = CONST_ME_HITBYPOISON},
	{name ="drunk", interval = 2000, chance = 10, drunkenness = 25, duration = 5000, radius = 4, target = true, effect = CONST_ME_STUN}
}

monster.defenses = {
	defense = 30,
	armor = 30,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 2000, maxDamage = 2900, effect = CONST_ME_HITBYPOISON}
}

monster.loot = {
	{id = "yummy gummy worm", chance = 100000, maxCount = 10},
	{id = "pumpkin", chance = 100000, maxCount = 5},
	{id = 6492, chance = 300},
	{id = "skeleton decoration", chance = 300},
	{id = "spiderwebs", chance = 250},
	{id = 9006, chance = 250}
}

mType:register(monster)
