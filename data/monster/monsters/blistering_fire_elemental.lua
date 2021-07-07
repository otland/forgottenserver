local mType = Game.createMonsterType("blistering fire elemental")
local monster = {}

monster.name = "Blistering Fire Elemental"
monster.description = "a blistering fire elemental"
monster.experience = 1300
monster.outfit = {
	lookType = 242,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1500
monster.maxHealth = 1500
monster.runHealth = 0
monster.race = "fire"
monster.corpse = 8964
monster.speed = 230
monster.summonCost = 0
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
	pushable = false,
	boss = false,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
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
	{text = "FCHHHRRR", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 25},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -15},
	{type = COMBAT_HOLYDAMAGE , percent = 100},
	{type = COMBAT_DEATHDAMAGE , percent = 40}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -350, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 11, type = COMBAT_FIREDAMAGE, minDamage = -65, maxDamage = -510, length = 7, spread = 3},
	{name ="condition", interval = 1000, chance = 12, type = CONDITION_FIRE, startDamage = 0, tick = 9000, minDamage = -50, maxDamage = -200, radius = 6, target = false, effect = CONST_ME_FIREAREA},
	{name ="firefield", interval = 1000, chance = 15, range = 7, radius = 3, target = true, ShootEffect = CONST_ANI_FIRE}
}

monster.defenses = {
	defense = 20,
	armor = 20
}

monster.loot = {
	{id = "gold coin", chance = 44000, maxCount = 100},
	{id = "gold coin", chance = 44000, maxCount = 70},
	{id = "fiery heart", chance = 10000},
	{id = "glimmering soil", chance = 9500},
	{id = "small ruby", chance = 5000, maxCount = 3},
	{id = "wand of draconia", chance = 500},
	{id = 13757, chance = 500}
}

mType:register(monster)
