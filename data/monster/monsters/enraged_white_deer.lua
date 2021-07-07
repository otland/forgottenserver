local mType = Game.createMonsterType("enraged white deer")
local monster = {}

monster.name = "Enraged White Deer"
monster.description = "an enraged white deer"
monster.experience = 165
monster.outfit = {
	lookType = 400,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 255
monster.maxHealth = 255
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 13513
monster.speed = 210
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 5000,
	chance = 0
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = true,
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
	{text = "*wheeze*", yell = false},
	{text = "ROOOAAARR!!", yell = false},
	{text = "*sniff*", yell = false},
	{text = "*bell*", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
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
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -45, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 15,
	armor = 15,
	{name ="combat", interval = 4000, chance = 15, type = COMBAT_HEALING, minDamage = 10, maxDamage = 40, effect = CONST_ME_MAGIC_BLUE}
}

monster.events = {
	"WhiteDeerScouts"
}

monster.loot = {
	{id = "ham", chance = 19850, maxCount = 3},
	{id = "white deer antlers", chance = 19520},
	{id = "white deer skin", chance = 20280}
}

mType:register(monster)
