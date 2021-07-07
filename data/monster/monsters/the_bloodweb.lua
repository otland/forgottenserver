local mType = Game.createMonsterType("the bloodweb")
local monster = {}

monster.name = "The Bloodweb"
monster.description = "The Bloodweb"
monster.experience = 1450
monster.outfit = {
	lookType = 263,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1750
monster.maxHealth = 1750
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 7344
monster.speed = 340
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 20000,
	chance = 8
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = false,
	boss = true,
	illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	staticAttackChance = 60,
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
	{text = "Screeech!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = -20},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -250, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 8, interval = 4000}},
	{name ="speed", interval = 2000, chance = 20, speed = {min = -850, max = -850}, duration = 8000, range = 7, radius = 7, target = false, effect = CONST_ME_POFF},
	{name ="combat", interval = 1000, chance = 25, type = COMBAT_ENERGYDAMAGE, minDamage = -60, maxDamage = -150, range = 7, ShootEffect = CONST_ANI_ENERGY, effect = CONST_ME_ENERGYHIT}
}

monster.defenses = {
	defense = 20,
	armor = 25,
	{name ="speed", interval = 3000, chance = 40, speed = {min = 380, max = 380}, duration = 80000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = 2169, chance = 3703},
	{id = "plate armor", chance = 100000},
	{id = "knight armor", chance = 7692},
	{id = "knight legs", chance = 5555},
	{id = "jewelled backpack", chance = 7692},
	{id = "spider silk", chance = 50000},
	{id = "party hat", chance = 1886},
	{id = "shard", chance = 3703},
	{id = "sniper arrow", chance = 100000, maxCount = 9},
	{id = "sapphire hammer", chance = 20000},
	{id = "strong mana potion", chance = 100000},
	{id = "glacier kilt", chance = 33333},
	{id = "glacier mask", chance = 33333},
	{id = "sai", chance = 20000}
}

mType:register(monster)
