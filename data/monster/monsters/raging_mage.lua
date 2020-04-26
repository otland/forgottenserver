local mType = Game.createMonsterType("raging mage")
local monster = {}

monster.name = "Raging Mage"
monster.description = "a raging mage"
monster.experience = 3250
monster.outfit = {
	lookType = 416,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 3500
monster.maxHealth = 3500
monster.runHealth = 300
monster.race = "blood"
monster.corpse = 13834
monster.speed = 200
monster.summonCost = 0
monster.maxSummons = 1

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
	boss = true,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 80,
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
	{text = "Behold the all permeating powers I draw from this gate!!", yell = false},
	{text = "ENERGY!!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -25},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 15}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -78, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 35, type = COMBAT_ENERGYDAMAGE, minDamage = -100, maxDamage = -200, range = 7, radius = 5, target = true, ShootEffect = CONST_ANI_ENERGYBALL, effect = CONST_ME_ENERGYHIT},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_MANADRAIN, minDamage = -100, maxDamage = -200, range = 7},
	{name ="energyfield", interval = 2000, chance = 15, range = 7, radius = 2, target = true, ShootEffect = CONST_ANI_ENERGY},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_ENERGYDAMAGE, minDamage = -101, maxDamage = -300, length = 8, spread = 0, effect = CONST_ME_BIGCLOUDS}
}

monster.defenses = {
	defense = 25,
	armor = 25
}

monster.summons = {
	{name = "golden servant", chance = 50, interval = 2000}
}

monster.loot = {
	{id = "ring of the sky", chance = 1830},
	{id = "small sapphire", chance = 1830, maxCount = 5},
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 47000, maxCount = 69},
	{id = "platinum coin", chance = 77400, maxCount = 8},
	{id = "stealth ring", chance = 4880},
	{id = "mind stone", chance = 9760},
	{id = "boots of haste", chance = 610},
	{id = "dark mushroom", chance = 3600, maxCount = 3},
	{id = "skull helmet", chance = 610},
	{id = "red piece of cloth", chance = 31100},
	{id = "assassin star", chance = 1830, maxCount = 7},
	{id = "bullseye potion", chance = 6710},
	{id = "great mana potion", chance = 23170, maxCount = 5},
	{id = "great health potion", chance = 26830, maxCount = 5},
	{id = "focus cape", chance = 4880},
	{id = "spellbook of mind control", chance = 610},
	{id = "royal tapestry", chance = 1220},
	{id = "luminous orb", chance = 4270},
	{id = 13940, chance = 1220}
}

mType:register(monster)
