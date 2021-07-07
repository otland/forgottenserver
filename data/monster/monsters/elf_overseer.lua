local mType = Game.createMonsterType("elf overseer")
local monster = {}

monster.name = "Elf Overseer"
monster.description = "an elf overseer"
monster.experience = 290
monster.outfit = {
	lookType = 159,
	lookHead = 21,
	lookBody = 76,
	lookLegs = 95,
	lookFeet = 116,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 500
monster.maxHealth = 500
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20596
monster.speed = 176
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = true,
	boss = false,
	illusionable = false,
	canPushItems = false,
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
	{text = "Do YOU want some more water, yes? YES?", yell = false},
	{text = "DOWN WITH THE FIRES OF IGNORANCE!!", yell = true}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE , percent = -5},
	{type = COMBAT_DEATHDAMAGE , percent = 10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -75, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_DROWNDAMAGE, minDamage = -50, maxDamage = -90, length = 6, spread = 0, effect = CONST_ME_LOSEENERGY},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_DROWNDAMAGE, minDamage = -40, maxDamage = -50, range = 4, radius = 2, target = true, effect = CONST_ME_LOSEENERGY}
}

monster.defenses = {
	defense = 15,
	armor = 15,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 50, maxDamage = 72, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "gold coin", chance = 51000, maxCount = 50},
	{id = "elvish talisman", chance = 20000},
	{id = "bread", chance = 14000, maxCount = 3},
	{id = "health potion", chance = 14000},
	{id = "battle shield", chance = 11000},
	{id = "holy orchid", chance = 8500},
	{id = "mana potion", chance = 8500},
	{id = "crystal sword", chance = 5700}
}

mType:register(monster)
