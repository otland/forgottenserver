local mType = Game.createMonsterType("demon")
local monster = {}

monster.name = "Demon"
monster.description = "a demon"
monster.experience = 6000
monster.outfit = {
	lookType = 35,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 8200
monster.maxHealth = 8200
monster.runHealth = 0
monster.race = "fire"
monster.corpse = 5995
monster.speed = 256
monster.summonCost = 0
monster.maxSummons = 1

monster.changeTarget = {
	interval = 4000,
	chance = 20
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
	{text = "Your soul will be mine!", yell = true},
	{text = "MUHAHAHAHA!", yell = true},
	{text = "CHAMEK ATH UTHUL ARAK!", yell = true},
	{text = "I SMELL FEEEEAAAAAR!", yell = true},
	{text = "Your resistance is futile!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 25},
	{type = COMBAT_ENERGYDAMAGE, percent = 50},
	{type = COMBAT_EARTHDAMAGE, percent = 40},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = -12},
	{type = COMBAT_HOLYDAMAGE , percent = -12},
	{type = COMBAT_DEATHDAMAGE , percent = 20}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -500, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_MANADRAIN, minDamage = -30, maxDamage = -120, range = 7},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_FIREDAMAGE, minDamage = -150, maxDamage = -250, range = 7, radius = 7, target = false, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA},
	{name ="firefield", interval = 2000, chance = 10, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_FIRE},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = -300, maxDamage = -480, length = 8, spread = 0, effect = CONST_ME_PURPLEENERGY},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ENERGYDAMAGE, minDamage = -210, maxDamage = -300, range = 1, ShootEffect = CONST_ANI_ENERGY},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -700, max = -700}, duration = 30000, range = 7, radius = 1, target = true, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 44,
	armor = 44,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 80, maxDamage = 250, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 2000, chance = 15, speed = {min = 320, max = 320}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.summons = {
	{name = "fire elemental", chance = 10, interval = 2000}
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "platinum coin", chance = 100000, maxCount = 8},
	{id = "great mana potion", chance = 25000, maxCount = 3},
	{id = "great spirit potion", chance = 25000, maxCount = 3},
	{id = "demon horn", chance = 20000},
	{id = "demonic essence", chance = 20000},
	{id = "ultimate health potion", chance = 20000, maxCount = 3},
	{id = "fire mushroom", chance = 20000, maxCount = 6},
	{id = "assassin star", chance = 15263, maxCount = 10},
	{id = "small topaz", chance = 10000, maxCount = 5},
	{id = "small ruby", chance = 10000, maxCount = 5},
	{id = "small emerald", chance = 10000, maxCount = 5},
	{id = "small amethyst", chance = 10000, maxCount = 5},
	{id = "fire axe", chance = 4000},
	{id = "talon", chance = 3571},
	{id = "orb", chance = 2900},
	{id = "red gem", chance = 2900},
	{id = "ring of healing", chance = 2600},
	{id = "might ring", chance = 2500},
	{id = "stealth ring", chance = 2300},
	{id = "giant sword", chance = 2000},
	{id = "ice rapier", chance = 1800},
	{id = "golden sickle", chance = 1428},
	{id = "purple tome", chance = 1190},
	{id = "devil helmet", chance = 1204},
	{id = "gold ring", chance = 1010},
	{id = "demon shield", chance = 750},
	{id = "platinum amulet", chance = 710},
	{id = "mastermind shield", chance = 470},
	{id = "golden legs", chance = 400},
	{id = 7393, chance = 100},
	{id = "magic plate armor", chance = 80},
	{id = "demonrage sword", chance = 60}
}

mType:register(monster)
