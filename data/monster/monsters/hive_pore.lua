local mType = Game.createMonsterType("hive pore")
local monster = {}

monster.name = "Hive Pore"
monster.description = "Hive Pore"
monster.experience = 0
monster.outfit = {
	lookTypeEx = 15467
}

monster.health = 1
monster.maxHealth = 1
monster.runHealth = 0
monster.race = "venom"
monster.corpse = 0
monster.speed = 0
monster.summonCost = 0
monster.maxSummons = 3

monster.changeTarget = {
	interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
	attackable = false,
	hostile = true,
	convinceable = false,
	pushable = false,
	boss = false,
	illusionable = false,
	canPushItems = false,
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
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 100},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 100},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 100},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
}

monster.defenses = {
	defense = 0,
	armor = 0,
	{name ="effect", interval = 30000, chance = 100, radius = 3, target = false, effect = CONST_ME_HITBYPOISON}
}

monster.summons = {
	{name = "Lesser Swarmer", chance = 100, interval = 30000},
	{name = "Lesser Swarmer", chance = 100, interval = 30000},
	{name = "Lesser Swarmer", chance = 100, interval = 30000}
}

monster.loot = {
}

mType:register(monster)
