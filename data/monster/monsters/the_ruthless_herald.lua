local mType = Game.createMonsterType("the ruthless herald")
local monster = {}

monster.name = "The Ruthless Herald"
monster.description = "The Ruthless Herald"
monster.experience = 0
monster.outfit = {
	lookType = 237,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 100
monster.maxHealth = 100
monster.runHealth = 0
monster.race = "fire"
monster.corpse = 6364
monster.speed = 210
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
	chance = 95
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
	staticAttackChance = 50,
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
	{text = "I am so proud of my son Orshabaal.", yell = false},
	{text = "Have you heard, the imperor is marrying an imp named April? What a fool.", yell = false},
	{text = "The Ruthless Seven are going to make the easter bunny an honorary member!", yell = false},
	{text = "They are coming ... perhaps for YOU!", yell = false},
	{text = "Beware! The Ruthless Seven are coming!", yell = false},
	{text = "Killing me is imp-ossible, because I am imp-roved! I am immune to any imp-act!", yell = false},
	{text = "The one who kills me gets an imp-outfit.", yell = false},
	{text = "Hey you! I've heard that! You're first to die when the masters come.", yell = false},
	{text = "Nice to meet you. I am Harold, the ruthless herald.", yell = false},
	{text = "Have you seen my friend Harvey? I could swear he's somewhere around.", yell = false},
	{text = "My masters are on their way!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 100},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 100},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 100},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="outfit", interval = 2000, chance = 25, monster = "green frog", duration = 60000, radius = 3, target = true, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_TELEPORT},
	{name ="outfit", interval = 2000, chance = 25, monster = "pig", duration = 60000, radius = 3, target = true, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_TELEPORT},
	{name ="outfit", interval = 2000, chance = 25, monster = "skunk", duration = 60000, radius = 3, target = true, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_TELEPORT},
	{name ="outfit", interval = 2000, chance = 25, item = 2109, duration = 6000, radius = 3, target = true, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_TELEPORT}
}

monster.defenses = {
	defense = 5,
	armor = 10
}

monster.loot = {
}

mType:register(monster)
