local mType = Game.createMonsterType("hydra")
local monster = {}

monster.name = "Hydra"
monster.description = "a hydra"
monster.experience = 2100
monster.outfit = {
	lookType = 121,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 2350
monster.maxHealth = 2350
monster.runHealth = 300
monster.race = "blood"
monster.corpse = 6048
monster.speed = 206
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
	{text = "FCHHHHH", yell = true},
	{text = "HISSSS", yell = true}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE , percent = 30},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -270, effect = CONST_ME_DRAWBLOOD},
	{name ="speed", interval = 2000, chance = 25, speed = {min = -700, max = -700}, duration = 15000, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_GREEN_RINGS},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ICEDAMAGE, minDamage = -100, maxDamage = -250, length = 8, spread = 3, effect = CONST_ME_LOSEENERGY},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ICEDAMAGE, minDamage = -80, maxDamage = -155, ShootEffect = CONST_ANI_SMALLICE},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_EARTHDAMAGE, minDamage = -66, maxDamage = -320, length = 8, spread = 3, effect = CONST_ME_CARNIPHILA}
}

monster.defenses = {
	defense = 27,
	armor = 27,
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_HEALING, minDamage = 260, maxDamage = 407, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "gold coin", chance = 34000, maxCount = 100},
	{id = "gold coin", chance = 34000, maxCount = 100},
	{id = "gold coin", chance = 20000, maxCount = 46},
	{id = "ham", chance = 60000, maxCount = 4},
	{id = "platinum coin", chance = 49000, maxCount = 3},
	{id = "hydra head", chance = 10120},
	{id = "small sapphire", chance = 5000},
	{id = "cucumber", chance = 4780},
	{id = "ring of healing", chance = 1190},
	{id = "knight armor", chance = 1000},
	{id = "hydra egg", chance = 930},
	{id = "stone skin amulet", chance = 900},
	{id = "warrior helmet", chance = 890},
	{id = "life crystal", chance = 520},
	{id = "strong mana potion", chance = 470},
	{id = "royal helmet", chance = 220},
	{id = "medusa shield", chance = 220},
	{id = "boots of haste", chance = 110}
}

mType:register(monster)
