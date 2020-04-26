local mType = Game.createMonsterType("omruc")
local monster = {}

monster.name = "Omruc"
monster.description = "Omruc"
monster.experience = 2950
monster.outfit = {
	lookType = 90,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 4300
monster.maxHealth = 4300
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 6025
monster.speed = 370
monster.summonCost = 0
monster.maxSummons = 4

monster.changeTarget = {
	interval = 5000,
	chance = 8
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
	targetDistance = 4,
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
	{text = "Now chhhou shhhee me ... Now chhhou don't.", yell = false},
	{text = "Chhhhou are marked ashhh my prey.", yell = false},
	{text = "Psssst, I am over chhhere.", yell = false},
	{text = "Catchhhh me if chhhou can.", yell = false},
	{text = "Die!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE , percent = -10},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -120, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 65, interval = 4000}},
	{name ="combat", interval = 5000, chance = 20, type = COMBAT_LIFEDRAIN, minDamage = -100, maxDamage = -250, range = 1},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_EARTHDAMAGE, minDamage = -200, maxDamage = -500, ShootEffect = CONST_ANI_POISONARROW},
	{name ="combat", interval = 1000, chance = 20, type = COMBAT_FIREDAMAGE, minDamage = -120, maxDamage = -450, range = 3, ShootEffect = CONST_ANI_BURSTARROW, effect = CONST_ME_EXPLOSIONAREA},
	{name ="melee", interval = 3000, chance = 20, minDamage = -150, maxDamage = -500, effect = CONST_ME_DRAWBLOOD},
	{name ="speed", interval = 1000, chance = 25, speed = {min = -900, max = -900}, duration = 50000, range = 7, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 60,
	armor = 60,
	{name ="combat", interval = 1000, chance = 17, type = COMBAT_HEALING, minDamage = 100, maxDamage = 200, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 2000, chance = 14, duration = 8000, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Stalker", chance = 100, interval = 2000}
}

monster.loot = {
	{id = "crystal arrow", chance = 100000},
	{id = "gold coin", chance = 43000, maxCount = 100},
	{id = "gold coin", chance = 43000, maxCount = 60},
	{id = "red apple", chance = 76000, maxCount = 2},
	{id = "poison arrow", chance = 56000, maxCount = 20},
	{id = "burst arrow", chance = 47000, maxCount = 15},
	{id = "onyx arrow", chance = 32000, maxCount = 6},
	{id = "arrow", chance = 23000, maxCount = 25},
	{id = "power bolt", chance = 16000, maxCount = 5},
	{id = "great health potion", chance = 11000},
	{id = "small diamond", chance = 10000, maxCount = 3},
	{id = "mini mummy", chance = 2250},
	{id = "stealth ring", chance = 2250},
	{id = "boots of haste", chance = 1100}
}

mType:register(monster)
