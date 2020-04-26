local mType = Game.createMonsterType("herald of gloom")
local monster = {}

monster.name = "Herald of Gloom"
monster.description = "a herald of gloom"
monster.experience = 450
monster.outfit = {
	lookType = 320,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 350
monster.maxHealth = 350
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 9915
monster.speed = 260
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 0,
	chance = 8
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
	{text = "The powers of light are waning.", yell = true},
	{text = "You will join us in eternal night!", yell = true},
	{text = "The shadows will engulf the world.", yell = true}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 20},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -90, effect = CONST_ME_DRAWBLOOD},
	{name ="speed", interval = 3000, chance = 10, speed = {min = -600, max = -600}, duration = 30000, range = 7, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 24, type = COMBAT_HOLYDAMAGE, minDamage = -90, maxDamage = -170, range = 4, ShootEffect = CONST_ANI_SMALLHOLY}
}

monster.defenses = {
	defense = 55,
	armor = 50,
	{name ="speed", interval = 1000, chance = 15, speed = {min = 200, max = 200}, duration = 20000, effect = CONST_ME_MAGIC_RED},
	{name ="invisible", interval = 5000, chance = 20, duration = 5000, effect = CONST_ME_MAGIC_RED},
	{name ="outfit", interval = 1500, chance = 20, monster = "nightstalker", duration = 6000, effect = CONST_ME_MAGIC_BLUE},
	{name ="outfit", interval = 1500, chance = 10, monster = "werewolf", duration = 6000, effect = CONST_ME_MAGIC_BLUE},
	{name ="outfit", interval = 1500, chance = 10, monster = "the count", duration = 6000, effect = CONST_ME_MAGIC_BLUE},
	{name ="outfit", interval = 1500, chance = 10, monster = "grim reaper", duration = 6000, effect = CONST_ME_MAGIC_BLUE},
	{name ="outfit", interval = 1500, chance = 10, monster = "tarantula", duration = 6000, effect = CONST_ME_MAGIC_BLUE},
	{name ="outfit", interval = 1500, chance = 10, monster = "ferumbras", duration = 6000, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "midnight shard", chance = 1886}
}

mType:register(monster)
