local mType = Game.createMonsterType("yakchal")
local monster = {}

monster.name = "Yakchal"
monster.description = "Yakchal"
monster.experience = 4400
monster.outfit = {
	lookType = 149,
	lookHead = 9,
	lookBody = 0,
	lookLegs = 85,
	lookFeet = 85,
	lookAddons = 1,
	lookMount = 0
}

monster.health = 5750
monster.maxHealth = 5750
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20546
monster.speed = 200
monster.summonCost = 0
monster.maxSummons = 4

monster.changeTarget = {
	interval = 2000,
	chance = 5
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
	staticAttackChance = 50,
	targetDistance = 4,
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
	{text = "So you think you are cool?", yell = false},
	{text = "I hope it is not too cold for you! HeHeHe.", yell = false},
	{text = "Freeze!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 40},
	{type = COMBAT_FIREDAMAGE, percent = 50},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 26},
	{type = COMBAT_DEATHDAMAGE , percent = -10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -389, effect = CONST_ME_DRAWBLOOD},
	{name ="outfit", interval = 1000, chance = 10, item = 7172, duration = 8000, range = 7, effect = CONST_ME_MAGIC_BLUE},
	{name ="combat", interval = 2000, chance = 18, type = COMBAT_ICEDAMAGE, minDamage = 0, maxDamage = -430, radius = 4, target = true, ShootEffect = CONST_ANI_SMALLICE, effect = CONST_ME_ICEAREA},
	{name ="combat", interval = 3000, chance = 34, type = COMBAT_PHYSICALDAMAGE, minDamage = -200, maxDamage = -310, range = 7, radius = 3, target = true, ShootEffect = CONST_ANI_SNOWBALL},
	{name ="speed", interval = 1000, chance = 10, speed = {min = -600, max = -600}, duration = 20000, range = 7, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 20,
	armor = 15,
	{name ="combat", interval = 1000, chance = 25, type = COMBAT_HEALING, minDamage = 50, maxDamage = 100, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Ice Golem", chance = 13, interval = 1000}
}

monster.loot = {
	{id = "gold coin", chance = 10000, maxCount = 88},
	{id = "boots of haste", chance = 2000},
	{id = "dragon necklace", chance = 3250},
	{id = "skull staff", chance = 4500},
	{id = "crystal mace", chance = 4000},
	{id = "blue piece of cloth", chance = 5000},
	{id = "shard", chance = 100000},
	{id = "queen's sceptre", chance = 1500},
	{id = "berserk potion", chance = 1500},
	{id = "mastermind potion", chance = 1500},
	{id = "bullseye potion", chance = 1500},
	{id = "crystal sword", chance = 4000},
	{id = "pair of earmuffs", chance = 1000},
	{id = "glacier kilt", chance = 2650},
	{id = "glacier robe", chance = 3000},
	{id = "gold ingot", chance = 5000}
}

mType:register(monster)
