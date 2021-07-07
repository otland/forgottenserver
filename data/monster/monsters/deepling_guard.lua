local mType = Game.createMonsterType("deepling guard")
local monster = {}

monster.name = "Deepling Guard"
monster.description = "a deepling guard"
monster.experience = 2100
monster.outfit = {
	lookType = 442,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1900
monster.maxHealth = 1900
monster.runHealth = 20
monster.race = "blood"
monster.corpse = 15187
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
	canPushCreatures = false,
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
	{text = "QJELL NETA NA!!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = -20},
	{type = COMBAT_EARTHDAMAGE, percent = -20},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -400, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -200, range = 7, ShootEffect = CONST_ANI_WHIRLWINDSWORD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_DROWNDAMAGE, minDamage = -100, maxDamage = -150, range = 7, ShootEffect = CONST_ANI_SPEAR, effect = CONST_ME_LOSEENERGY}
}

monster.defenses = {
	defense = 35,
	armor = 35,
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_HEALING, minDamage = 100, maxDamage = 200, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "small sapphire", chance = 2890, maxCount = 3},
	{id = "gold coin", chance = 100000, maxCount = 90},
	{id = "gold coin", chance = 100000, maxCount = 90},
	{id = "platinum coin", chance = 70000, maxCount = 2},
	{id = "great mana potion", chance = 14285, maxCount = 3},
	{id = "great health potion", chance = 14285, maxCount = 3},
	{id = "heavy trident", chance = 1694},
	{id = "eye of a deepling", chance = 10000},
	{id = "deepling guard belt buckle", chance = 12500},
	{id = "deepling breaktime snack", chance = 16666},
	{id = "guardian axe", chance = 925},
	{id = "deepling claw", chance = 9090},
	{id = "foxtail", chance = 10},
	{id = "ornate crossbow", chance = 362},
	{id = "deepling backpack", chance = 333},
	{id = "deepling squelcher", chance = 751}
}

mType:register(monster)
