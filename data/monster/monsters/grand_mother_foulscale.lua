local mType = Game.createMonsterType("grand mother foulscale")
local monster = {}

monster.name = "Grand Mother Foulscale"
monster.description = "Grand Mother Foulscale"
monster.experience = 1400
monster.outfit = {
	lookType = 34,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1850
monster.maxHealth = 1850
monster.runHealth = 400
monster.race = "blood"
monster.corpse = 5973
monster.speed = 180
monster.summonCost = 0
monster.maxSummons = 4

monster.changeTarget = {
	interval = 5000,
	chance = 8
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
	{text = "GROOAAARRR", yell = true},
	{text = "FCHHHHH", yell = true}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 80},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -170, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_FIREDAMAGE, minDamage = -45, maxDamage = -85, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA},
	{name ="combat", interval = 2000, chance = 8, type = COMBAT_FIREDAMAGE, minDamage = -90, maxDamage = -150, length = 8, spread = 3, effect = CONST_ME_FIREAREA}
}

monster.defenses = {
	defense = 20,
	armor = 27,
	{name ="combat", interval = 1000, chance = 17, type = COMBAT_HEALING, minDamage = 34, maxDamage = 66, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "dragon hatchlings", chance = 40, interval = 4000, max = 4}
}

monster.loot = {
	{id = "small diamond", chance = 500},
	{id = "gold coin", chance = 37500, maxCount = 70},
	{id = "gold coin", chance = 37500, maxCount = 50},
	{id = "life crystal", chance = 150},
	{id = "wand of inferno", chance = 1800},
	{id = "double axe", chance = 1333},
	{id = "longsword", chance = 5000},
	{id = "mace", chance = 21500},
	{id = "short sword", chance = 25000},
	{id = "serpent sword", chance = 500},
	{id = "broadsword", chance = 2000},
	{id = "dragon hammer", chance = 600},
	{id = "crossbow", chance = 10000},
	{id = "steel helmet", chance = 3000},
	{id = "steel shield", chance = 14000},
	{id = "dragon shield", chance = 500},
	{id = "burst arrow", chance = 4000, maxCount = 12},
	{id = "plate legs", chance = 2000},
	{id = "dragon ham", chance = 15500, maxCount = 3},
	{id = "green dragon leather", chance = 100000},
	{id = "green dragon scale", chance = 100000},
	{id = "dragonbone staff", chance = 650},
	{id = "strong health potion", chance = 1750}
}

mType:register(monster)
