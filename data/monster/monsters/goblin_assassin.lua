local mType = Game.createMonsterType("goblin assassin")
local monster = {}

monster.name = "Goblin Assassin"
monster.description = "a goblin assassin"
monster.experience = 52
monster.outfit = {
	lookType = 296,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 75
monster.maxHealth = 75
monster.runHealth = 15
monster.race = "blood"
monster.corpse = 6002
monster.speed = 200
monster.summonCost = 360
monster.maxSummons = 0

monster.changeTarget = {
	interval = 10000,
	chance = 0
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
	pushable = true,
	boss = false,
	illusionable = true,
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
	{text = "Goblin Powahhh!", yell = false},
	{text = "Me kill you!", yell = false},
	{text = "Me green menace!", yell = false},
	{text = "Gobabunga!", yell = false},
	{text = "Gooobliiiins!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 1},
	{type = COMBAT_DEATHDAMAGE , percent = -1}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -15, effect = CONST_ME_DRAWBLOOD},
	{name ="drunk", interval = 2000, chance = 10, drunkenness = 25, duration = 10000, ShootEffect = CONST_ANI_POISON},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -35, range = 6, ShootEffect = CONST_ANI_THROWINGKNIFE}
}

monster.defenses = {
	defense = 15,
	armor = 15,
	{name ="invisible", interval = 2000, chance = 10, duration = 2000, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 2000, chance = 15, speed = {min = 100, max = 100}, duration = 3000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "small stone", chance = 9900, maxCount = 3},
	{id = "gold coin", chance = 50000, maxCount = 9},
	{id = 2230, chance = 13000},
	{id = "mouldy cheese", chance = 6610},
	{id = "dagger", chance = 17000},
	{id = "short sword", chance = 8820},
	{id = "bone club", chance = 4770},
	{id = "leather helmet", chance = 13000},
	{id = "leather armor", chance = 7240},
	{id = "small axe", chance = 9800},
	{id = 2667, chance = 12400}
}

mType:register(monster)
