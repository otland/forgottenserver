local mType = Game.createMonsterType("draken abomination")
local monster = {}

monster.name = "Draken Abomination"
monster.description = "a draken abomination"
monster.experience = 3800
monster.outfit = {
	lookType = 357,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 6250
monster.maxHealth = 6250
monster.runHealth = 0
monster.race = "venom"
monster.corpse = 12623
monster.speed = 230
monster.summonCost = 0
monster.maxSummons = 2

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
	illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	staticAttackChance = 70,
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
	{text = "Ugggh!", yell = false},
	{text = "Gll", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE , percent = -5},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -420, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_FIREDAMAGE, minDamage = -310, maxDamage = -630, length = 4, spread = 3, effect = CONST_ME_EXPLOSIONHIT},
	{name ="draken abomination curse", interval = 2000, chance = 10, range = 5},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_DEATHDAMAGE, minDamage = -170, maxDamage = -370, length = 4, spread = 0, effect = CONST_ME_MORTAREA},
	{name ="drunk", interval = 2000, chance = 15, drunkenness = 25, duration = 9000, range = 7, radius = 4, target = false, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_PHYSICALDAMAGE, range = 7, radius = 3, target = false, effect = CONST_ME_HITBYPOISON}
}

monster.defenses = {
	defense = 30,
	armor = 30,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 650, maxDamage = 700, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Death Blob", chance = 10, interval = 2000, max = 2}
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 47000, maxCount = 98},
	{id = "platinum coin", chance = 50590, maxCount = 8},
	{id = "meat", chance = 50450, maxCount = 4},
	{id = "great mana potion", chance = 9950, maxCount = 3},
	{id = "terra hood", chance = 8730},
	{id = "great spirit potion", chance = 4905, maxCount = 3},
	{id = "ultimate health potion", chance = 9400, maxCount = 3},
	{id = "wand of voodoo", chance = 1020},
	{id = "small topaz", chance = 2900, maxCount = 4},
	{id = "Zaoan armor", chance = 470},
	{id = "Zaoan helmet", chance = 560},
	{id = "Zaoan legs", chance = 780},
	{id = "eye of corruption", chance = 12110},
	{id = "tail of corruption", chance = 6240},
	{id = "scale of corruption", chance = 10940},
	{id = "shield of corruption", chance = 10},
	{id = "draken boots", chance = 540},
	{id = "snake god's wristguard", chance = 10},
	{id = "bamboo leaves", chance = 360}
}

mType:register(monster)
