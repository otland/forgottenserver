local mType = Game.createMonsterType("sparkion")
local monster = {}

monster.name = "Sparkion"
monster.description = "a sparkion"
monster.experience = 1520
monster.outfit = {
	lookType = 877,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 2700
monster.maxHealth = 2700
monster.runHealth = 0
monster.race = "venom"
monster.corpse = 26044
monster.speed = 302
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 5000,
	chance = 5
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
	{text = "Zzing!", yell = false},
	{text = "Frizzle!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = -15},
	{type = COMBAT_FIREDAMAGE, percent = 15},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 90},
	{type = COMBAT_HOLYDAMAGE , percent = 5},
	{type = COMBAT_DEATHDAMAGE , percent = 5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -300, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_LIFEDRAIN, minDamage = -350, maxDamage = -700, length = 6, spread = 0, effect = CONST_ME_GROUNDSHAKER},
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_ENERGYDAMAGE, minDamage = -150, maxDamage = -250, range = 5, ShootEffect = CONST_ANI_ENERGY, effect = CONST_ME_ENERGYHIT},
	{name ="condition", interval = 2000, chance = 20, type = CONDITION_ENERGY, startDamage = 0, tick = 10000, minDamage = -300, maxDamage = -600, range = 6, radius = 4, target = true, ShootEffect = CONST_ANI_ENERGY, effect = CONST_ME_BLOCKHIT}
}

monster.defenses = {
	defense = 40,
	armor = 40
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 100},
	{id = "platinum coin", chance = 90000, maxCount = 3},
	{id = "sparkion claw", chance = 16000},
	{id = "energy drink", chance = 15000},
	{id = "energy bar", chance = 15000},
	{id = "sparkion stings", chance = 13000},
	{id = "sparkion legs", chance = 11000},
	{id = "sparkion tail", chance = 9700},
	{id = "great mana potion", chance = 9500, maxCount = 2},
	{id = "great spirit potion", chance = 9500, maxCount = 2},
	{id = "great health potion", chance = 9500, maxCount = 2},
	{id = "blue crystal splinter", chance = 7700},
	{id = "cyan crystal fragment", chance = 5300},
	{id = "small sapphire", chance = 4400, maxCount = 2},
	{id = "blue crystal shard", chance = 4000},
	{id = "blue gem", chance = 900},
	{id = "wand of cosmic energy", chance = 900},
	{id = 26187, chance = 400},
	{id = 26189, chance = 400},
	{id = 26185, chance = 400},
	{id = 26199, chance = 250},
	{id = 26198, chance = 250},
	{id = 26200, chance = 250}
}

mType:register(monster)
