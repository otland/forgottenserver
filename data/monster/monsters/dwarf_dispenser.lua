local mType = Game.createMonsterType("dwarf dispenser")
local monster = {}

monster.name = "Dwarf Dispenser"
monster.description = "a dwarf dispenser"
monster.experience = 0
monster.outfit = {
	lookTypeEx = 6116
}

monster.health = 100
monster.maxHealth = 100
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 0
monster.speed = 0
monster.summonCost = 0
monster.maxSummons = 5

monster.changeTarget = {
	interval = 5000,
	chance = 16
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
	healthHidden = true,
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

monster.defenses = {
	defense = 5,
	armor = 10
}

monster.summons = {
	{name = "Dwarf Henchman", chance = 55, interval = 1000}
}

monster.loot = {
}

mType:register(monster)
