local mType = Game.createMonsterType("groam")
local monster = {}

monster.name = "Groam"
monster.description = "Groam"
monster.experience = 180
monster.outfit = {
	lookType = 413,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 400
monster.maxHealth = 400
monster.runHealth = 60
monster.race = "blood"
monster.corpse = 13839
monster.speed = 560
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
	chance = 50
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = true,
	boss = true,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 95,
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
	{text = "Njaaarh!!", yell = false},
	{text = "Begjone, intrjuder!!", yell = false},
	{text = "Djon't djare stjare injo the eyes of the djeep!", yell = false},
	{text = "Ljeave this sjacred pljace while you cjan", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -20},
	{type = COMBAT_ENERGYDAMAGE, percent = -20},
	{type = COMBAT_EARTHDAMAGE, percent = -20},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -20}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -99, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 11, type = COMBAT_DROWNDAMAGE, minDamage = -15, maxDamage = -100, range = 5, ShootEffect = CONST_ANI_SPEAR, effect = CONST_ME_LOSEENERGY}
}

monster.defenses = {
	defense = 20,
	armor = 12
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 50},
	{id = "life ring", chance = 50000},
	{id = "hunting spear", chance = 25000, maxCount = 4},
	{id = 9808, chance = 14285}
}

mType:register(monster)
