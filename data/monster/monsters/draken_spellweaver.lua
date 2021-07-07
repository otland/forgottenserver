local mType = Game.createMonsterType("draken spellweaver")
local monster = {}

monster.name = "Draken Spellweaver"
monster.description = "a draken spellweaver"
monster.experience = 3100
monster.outfit = {
	lookType = 340,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 5000
monster.maxHealth = 5000
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 11316
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
	{text = "Kazzzzuuuum!!", yell = false},
	{text = "Fissziss!", yell = false},
	{text = "Zzzzzooom!!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE , percent = -5},
	{type = COMBAT_DEATHDAMAGE , percent = 75}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -252, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_FIREDAMAGE, minDamage = -240, maxDamage = -480, length = 4, spread = 3, effect = CONST_ME_EXPLOSIONHIT},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_FIREDAMAGE, minDamage = -100, maxDamage = -250, range = 7, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ENERGYDAMAGE, minDamage = -150, maxDamage = -300, range = 7, ShootEffect = CONST_ANI_ENERGY, effect = CONST_ME_ENERGYHIT},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_EARTHDAMAGE, minDamage = -200, maxDamage = -380, radius = 4, target = true, effect = CONST_ME_POFF},
	{name ="monster soulfire", interval = 2000, chance = 10},
	{name ="condition", interval = 2000, chance = 10, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -280, maxDamage = -360, ShootEffect = CONST_ANI_POISON}
}

monster.defenses = {
	defense = 25,
	armor = 25,
	{name ="invisible", interval = 2000, chance = 10, duration = 5000, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 270, maxDamage = 530, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "ring of the sky", chance = 370},
	{id = "small ruby", chance = 6910, maxCount = 5},
	{id = "gold coin", chance = 41000, maxCount = 100},
	{id = "gold coin", chance = 58000, maxCount = 100},
	{id = "platinum coin", chance = 25510, maxCount = 5},
	{id = "green gem", chance = 970},
	{id = "wand of inferno", chance = 1660},
	{id = "meat", chance = 30400},
	{id = "great mana potion", chance = 4970},
	{id = "focus cape", chance = 1450},
	{id = "Zaoan shoes", chance = 1980},
	{id = "weaver's wandtip", chance = 19790},
	{id = "draken trophy", chance = 10},
	{id = "spellweaver's robe", chance = 620},
	{id = "Zaoan robe", chance = 770},
	{id = "luminous orb", chance = 1980},
	{id = "draken sulphur", chance = 3930},
	{id = "harness", chance = 30},
	{id = "bamboo leaves", chance = 180}
}

mType:register(monster)
