local mType = Game.createMonsterType("armadile")
local monster = {}

monster.name = "Armadile"
monster.description = "an armadile"
monster.experience = 2900
monster.outfit = {
	lookType = 487,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 3800
monster.maxHealth = 3800
monster.runHealth = 300
monster.race = "undead"
monster.corpse = 18378
monster.speed = 210
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
	illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 90,
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
	{text = "Creak!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 1},
	{type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 15},
	{type = COMBAT_HOLYDAMAGE , percent = 15},
	{type = COMBAT_DEATHDAMAGE , percent = 1}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -150, effect = CONST_ME_DRAWBLOOD},
	{name ="drunk", interval = 2000, chance = 15, drunkenness = 25, duration = 5000, radius = 4, target = true, effect = CONST_ME_FIREAREA},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_MANADRAIN, minDamage = -430, maxDamage = -550, range = 7, effect = CONST_ME_MAGIC_BLUE},
	{name ="condition", interval = 2000, chance = 15, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -200, maxDamage = -400, radius = 4, target = false, effect = CONST_ME_POISONAREA}
}

monster.defenses = {
	defense = 25,
	armor = 25,
	{name ="invisible", interval = 2000, chance = 15, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 100},
	{id = "gold coin", chance = 100000, maxCount = 98},
	{id = "platinum coin", chance = 95000, maxCount = 7},
	{id = 2169, chance = 1000},
	{id = "tower shield", chance = 620},
	{id = "titan axe", chance = 1540},
	{id = "bonebreaker", chance = 1150},
	{id = "strong health potion", chance = 14285, maxCount = 2},
	{id = "strong mana potion", chance = 15000, maxCount = 2},
	{id = "great mana potion", chance = 15920, maxCount = 2},
	{id = "great health potion", chance = 15000, maxCount = 2},
	{id = "mana potion", chance = 14285, maxCount = 3},
	{id = "terra boots", chance = 2850},
	{id = "crystalline armor", chance = 230},
	{id = "battle stone", chance = 12460},
	{id = 13757, chance = 6000},
	{id = "green crystal splinter", chance = 3920, maxCount = 2},
	{id = "green crystal fragment", chance = 5690, maxCount = 2},
	{id = "crystalline spikes", chance = 14540},
	{id = "drill bolt", chance = 7692, maxCount = 5},
	{id = "envenomed arrow", chance = 10230, maxCount = 10}
}

mType:register(monster)
