local mType = Game.createMonsterType("dwarf soldier")
local monster = {}

monster.name = "Dwarf Soldier"
monster.description = "a dwarf soldier"
monster.experience = 70
monster.outfit = {
	lookType = 71,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 135
monster.maxHealth = 135
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6014
monster.speed = 176
monster.summonCost = 360
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
	pushable = false,
	boss = false,
	illusionable = true,
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
	{text = "Hail Durin!", yell = false}
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
	{type = COMBAT_EARTHDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -70, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -60, range = 7, ShootEffect = CONST_ANI_BOLT}
}

monster.defenses = {
	defense = 9,
	armor = 9
}

monster.loot = {
	{id = "white mushroom", chance = 40000, maxCount = 2},
	{id = "bolt", chance = 38000, maxCount = 7},
	{id = "gold coin", chance = 28000, maxCount = 12},
	{id = "soldier helmet", chance = 12000},
	{id = 2554, chance = 10000},
	{id = "chain armor", chance = 8000},
	{id = "piercing bolt", chance = 4000, maxCount = 3},
	{id = "dwarven shield", chance = 3000},
	{id = "crossbow", chance = 3000},
	{id = "battle axe", chance = 2500},
	{id = "iron ore", chance = 300},
	{id = "axe ring", chance = 120}
}

mType:register(monster)
