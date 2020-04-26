local mType = Game.createMonsterType("ironblight")
local monster = {}

monster.name = "Ironblight"
monster.description = "Ironblight"
monster.experience = 4400
monster.outfit = {
	lookType = 498,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 6600
monster.maxHealth = 6600
monster.runHealth = 260
monster.race = "undead"
monster.corpse = 17424
monster.speed = 220
monster.summonCost = 0
monster.maxSummons = 0

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
	boss = false,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 70,
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
	{text = "Clonk!", yell = false},
	{text = "Yowl!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 25},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 60},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 50}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -300, effect = CONST_ME_DRAWBLOOD},
	{name ="condition", interval = 2000, chance = 10, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -460, maxDamage = -480, radius = 6, target = false, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_ICEDAMAGE, minDamage = -260, maxDamage = -350, length = 7, spread = 0, ShootEffect = CONST_ANI_ICE, effect = CONST_ME_ICEATTACK},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_EARTHDAMAGE, minDamage = -180, maxDamage = -250, radius = 2, target = true, ShootEffect = CONST_ANI_GREENSTAR, effect = CONST_ME_BIGPLANTS},
	{name ="speed", interval = 2000, chance = 10, speed = {min = -800, max = -800}, duration = 30000, length = 5, spread = 3, effect = CONST_ME_BLOCKHIT}
}

monster.defenses = {
	defense = 35,
	armor = 35,
	{name ="invisible", interval = 2000, chance = 20, duration = 7000, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 99},
	{id = "small emerald", chance = 10890, maxCount = 3},
	{id = "small amethyst", chance = 15020, maxCount = 3},
	{id = "platinum coin", chance = 100000, maxCount = 8},
	{id = "red gem", chance = 3510},
	{id = "blue gem", chance = 480},
	{id = "twigs", chance = 620},
	{id = "epee", chance = 480},
	{id = "magic sulphur", chance = 830},
	{id = "sapphire hammer", chance = 480},
	{id = "great mana potion", chance = 17640},
	{id = "terra legs", chance = 1110},
	{id = "ultimate health potion", chance = 18330},
	{id = "composite hornbow", chance = 210},
	{id = "springsprout rod", chance = 1720},
	{id = "crystal of balance", chance = 4480},
	{id = "crystal of power", chance = 2480},
	{id = "war crystal", chance = 21230},
	{id = "shiny stone", chance = 15230},
	{id = "jade hat", chance = 210},
	{id = "glacial rod", chance = 960},
	{id = "green crystal shard", chance = 5720},
	{id = "brown crystal splinter", chance = 12610, maxCount = 2},
	{id = "red crystal fragment", chance = 10540},
	{id = "green crystal fragment", chance = 10540},
	{id = "crystalline spikes", chance = 18400}
}

mType:register(monster)
