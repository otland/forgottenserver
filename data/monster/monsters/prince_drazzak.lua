local mType = Game.createMonsterType("prince drazzak")
local monster = {}

monster.name = "Prince Drazzak"
monster.description = "Prince Drazzak"
monster.experience = 210000
monster.outfit = {
	lookType = 12,
	lookHead = 77,
	lookBody = 78,
	lookLegs = 94,
	lookFeet = 97,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 330000
monster.maxHealth = 330000
monster.runHealth = 2000
monster.race = "fire"
monster.corpse = 6068
monster.speed = 480
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
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
	{text = "EVEN WITH ALL THAT TIME IN THE PRISON THAT WEAKENED ME, YOU ARE NO MATCH TO ME!", yell = true},
	{text = "DIE!", yell = true},
	{text = "SORCERERS MUST DIE!", yell = true},
	{text = "DRUIDS MUST DIE!", yell = true},
	{text = "PALADINS MUST DIE!", yell = true},
	{text = "KNIGHTS MUST DIE!", yell = true},
	{text = "GET OVER HERE!", yell = true},
	{text = "CRUSH THEM ALL!", yell = true},
	{text = "VARIPHOR WILL RULE!", yell = true},
	{text = "THEY WILL ALL PAY!", yell = true},
	{text = "NOT EVEN AEONS OF IMPRISONMENT WILL STOP ME!", yell = true},
	{text = "They used you fools to escape and they left ME behind!!??", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 30},
	{type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = 30},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 30},
	{type = COMBAT_HOLYDAMAGE , percent = 30},
	{type = COMBAT_DEATHDAMAGE , percent = 30}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, skill = 190, attack = 100, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 18, type = COMBAT_DEATHDAMAGE, minDamage = -1000, maxDamage = -3000, length = 8, spread = 0, effect = CONST_ME_MORTAREA}
}

monster.defenses = {
	defense = 65,
	armor = 55,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 1500, maxDamage = 3000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = 22396, chance = 100000, maxCount = 5},
	{id = 22598, chance = 1000, maxCount = 3},
	{id = 6500, chance = 100000, maxCount = 2},
	{id = 5954, chance = 50000},
	{id = 2152, chance = 100000, maxCount = 50},
	{id = 8473, chance = 100000, maxCount = 100},
	{id = 8472, chance = 100000, maxCount = 100},
	{id = 7590, chance = 100000, maxCount = 100},
	{id = 22613, chance = 25000},
	{id = 22608, chance = 2500},
	{id = 22611, chance = 25000},
	{id = 22612, chance = 25000},
	{id = 5741, chance = 2500},
	{id = 7417, chance = 2500},
	{id = 22610, chance = 7000},
	{id = 7418, chance = 1000},
	{id = 7893, chance = 1000},
	{id = 7632, chance = 5000},
	{id = 7633, chance = 5000}
}

mType:register(monster)
