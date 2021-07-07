local mType = Game.createMonsterType("glooth brigand")
local monster = {}

monster.name = "Glooth Brigand"
monster.description = "a glooth brigand"
monster.experience = 1900
monster.outfit = {
	lookType = 137,
	lookHead = 114,
	lookBody = 114,
	lookLegs = 110,
	lookFeet = 114,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 2400
monster.maxHealth = 2400
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 24257
monster.speed = 280
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
	chance = 2
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
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 25},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 20}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, skill = 90, attack = 60, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -200, range = 7, ShootEffect = CONST_ANI_ARROW}
}

monster.defenses = {
	defense = 31,
	armor = 23,
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_HEALING, minDamage = 200, maxDamage = 250, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = 7412, chance = 50},
	{id = 7419, chance = 100},
	{id = 7632, chance = 1500},
	{id = 23554, chance = 960},
	{id = 23551, chance = 980},
	{id = 23574, chance = 9830, maxCount = 2},
	{id = 23550, chance = 1000},
	{id = 24183, chance = 3100},
	{id = 23549, chance = 1000},
	{id = 23514, chance = 2480},
	{id = 23529, chance = 1000},
	{id = 23517, chance = 2500},
	{id = 2148, chance = 100000, maxCount = 100},
	{id = 7590, chance = 3490},
	{id = 8472, chance = 3540},
	{id = 2155, chance = 200},
	{id = 23538, chance = 160},
	{id = 7386, chance = 150},
	{id = 2152, chance = 79920, maxCount = 3},
	{id = 23536, chance = 640},
	{id = 24185, chance = 8070},
	{id = 7887, chance = 400},
	{id = 7886, chance = 490},
	{id = 7903, chance = 710},
	{id = 7885, chance = 500},
	{id = 7884, chance = 600},
	{id = 8473, chance = 3450}
}

mType:register(monster)
