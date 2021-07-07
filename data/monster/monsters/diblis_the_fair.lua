local mType = Game.createMonsterType("diblis the fair")
local monster = {}

monster.name = "Diblis The Fair"
monster.description = "Diblis The Fair"
monster.experience = 1800
monster.outfit = {
	lookType = 287,
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
monster.race = "undead"
monster.corpse = 8937
monster.speed = 280
monster.summonCost = 0
monster.maxSummons = 4

monster.changeTarget = {
	interval = 5000,
	chance = 10
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
	{text = "I envy you to be slain by someone as beautiful as me.", yell = false},
	{text = "I will drain your ugly corpses of the last drop of blood.", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = -15},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -380, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 12, type = COMBAT_DEATHDAMAGE, minDamage = 0, maxDamage = -155, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_MORTAREA}
}

monster.defenses = {
	defense = 30,
	armor = 30,
	{name ="combat", interval = 1000, chance = 12, type = COMBAT_HEALING, minDamage = 100, maxDamage = 235, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 3000, chance = 25, duration = 6000, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Banshee", chance = 50, interval = 4500}
}

monster.loot = {
	{id = "black pearl", chance = 8900, maxCount = 2},
	{id = "gold coin", chance = 100000, maxCount = 99},
	{id = "platinum coin", chance = 50000, maxCount = 5},
	{id = "ring of healing", chance = 14111},
	{id = 2229, chance = 15000},
	{id = "vampire shield", chance = 2100},
	{id = "strong health potion", chance = 1500},
	{id = "spellbook of lost souls", chance = 300},
	{id = "vampire lord token", chance = 100000},
	{id = "scum bag", chance = 500}
}

mType:register(monster)
