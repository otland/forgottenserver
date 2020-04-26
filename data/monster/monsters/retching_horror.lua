local mType = Game.createMonsterType("retching horror")
local monster = {}

monster.name = "Retching Horror"
monster.description = "a retching horror"
monster.experience = 3900
monster.outfit = {
	lookType = 588,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 5300
monster.maxHealth = 5300
monster.runHealth = 0
monster.race = "fire"
monster.corpse = 22508
monster.speed = 250
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 10
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
	{text = "Wait for us, little maggot...", yell = false},
	{text = "We will devour you...", yell = false},
	{text = "My little beetles, go forth, eat, feast!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 15},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 20}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -400, effect = CONST_ME_DRAWBLOOD},
	{name ="drunk", interval = 2000, chance = 10, drunkenness = 25, duration = 5000, length = 4, spread = 3, effect = CONST_ME_MAGIC_GREEN},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -700, max = -700}, duration = 15000, radius = 4, target = true, ShootEffect = CONST_ANI_WHIRLWINDCLUB, effect = CONST_ME_STUN},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -110, radius = 4, target = false, effect = CONST_ME_HITAREA},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -200, radius = 1, target = true, ShootEffect = CONST_ANI_SNIPERARROW},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_FIREDAMAGE, minDamage = -200, maxDamage = -350, radius = 1, target = true, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREATTACK}
}

monster.defenses = {
	defense = 30,
	armor = 30
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 100},
	{id = "platinum coin", chance = 100000, maxCount = 9},
	{id = "fire sword", chance = 1240},
	{id = "crown shield", chance = 1700},
	{id = "tower shield", chance = 550},
	{id = "brown mushroom", chance = 14000, maxCount = 2},
	{id = "beastslayer axe", chance = 4260},
	{id = "mercenary sword", chance = 2500},
	{id = "spiked squelcher", chance = 410},
	{id = "great mana potion", chance = 12500},
	{id = "great health potion", chance = 15000},
	{id = "underworld rod", chance = 1800},
	{id = "wand of starstorm", chance = 960},
	{id = 22363, chance = 3700},
	{id = "goosebump leather", chance = 16400},
	{id = "pool of chitinous glue", chance = 14000}
}

mType:register(monster)
