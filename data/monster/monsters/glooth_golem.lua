local mType = Game.createMonsterType("glooth golem")
local monster = {}

monster.name = "Glooth Golem"
monster.description = "a glooth golem"
monster.experience = 2200
monster.outfit = {
	lookType = 600,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 2700
monster.maxHealth = 2700
monster.runHealth = 0
monster.race = "venom"
monster.corpse = 23343
monster.speed = 260
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
	chance = 4
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
	{text = "*slosh*", yell = false},
	{text = "*clank*", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 15},
	{type = COMBAT_DEATHDAMAGE , percent = 30}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, skill = 60, attack = 50, effect = CONST_ME_DRAWBLOOD},
	{name ="melee", interval = 2000, chance = 2, skill = 86, attack = 100, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_ENERGYDAMAGE, minDamage = -125, maxDamage = -245, range = 7, ShootEffect = CONST_ANI_ENERGY},
	{name ="war golem skill reducer", interval = 2000, chance = 16},
	{name ="war golem electrify", interval = 2000, chance = 9, range = 7}
}

monster.defenses = {
	defense = 35,
	armor = 27,
	{name ="speed", interval = 2000, chance = 13, speed = {min = 404, max = 404}, duration = 4000, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_HEALING, minDamage = 200, maxDamage = 250, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = 5880, chance = 1030},
	{id = 23554, chance = 1720},
	{id = 23541, chance = 1720},
	{id = 23536, chance = 1370},
	{id = 2148, chance = 100000, maxCount = 100},
	{id = 2148, chance = 60000, maxCount = 100},
	{id = 24124, chance = 4470},
	{id = 9690, chance = 690},
	{id = 23514, chance = 9970},
	{id = 2152, chance = 56010, maxCount = 4},
	{id = 23474, chance = 16840},
	{id = 8473, chance = 4470},
	{id = 7590, chance = 9280},
	{id = 23538, chance = 690},
	{id = 23550, chance = 3440},
	{id = 23549, chance = 1030},
	{id = 23551, chance = 690},
	{id = 2154, chance = 1030},
	{id = 9970, chance = 7560, maxCount = 4},
	{id = 2149, chance = 8590, maxCount = 4}
}

mType:register(monster)
