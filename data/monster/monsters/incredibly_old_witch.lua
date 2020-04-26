local mType = Game.createMonsterType("incredibly old witch")
local monster = {}

monster.name = "Incredibly Old Witch"
monster.description = "an incredibly old witch"
monster.experience = 0
monster.outfit = {
	lookType = 54,
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
monster.race = "blood"
monster.corpse = 20535
monster.speed = 202
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 5000,
	chance = 20
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = true,
	boss = false,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
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
	{text = "Feel the wrath of the witch!", yell = false},
	{text = "Oh how you will regret to have distubed me!", yell = false},
	{text = "Everyone is so stupid!", yell = false},
	{text = "Stupid people!", yell = false},
	{text = "I will teach them all to leave me alone!", yell = false}
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
	{name ="outfit", interval = 2000, chance = 12, monster = "rat", duration = 60000, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_TELEPORT},
	{name ="outfit", interval = 2000, chance = 12, monster = "chicken", duration = 60000, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_TELEPORT},
	{name ="outfit", interval = 2000, chance = 12, monster = "green frog", duration = 60000, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_TELEPORT},
	{name ="outfit", interval = 2000, chance = 12, monster = "bug", duration = 60000, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_TELEPORT},
	{name ="outfit", interval = 2000, chance = 12, monster = "pig", duration = 60000, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_TELEPORT},
	{name ="outfit", interval = 2000, chance = 12, monster = "kongra", duration = 60000, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_TELEPORT},
	{name ="outfit", interval = 2000, chance = 12, monster = "dog", duration = 60000, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_TELEPORT},
	{name ="outfit", interval = 2000, chance = 12, monster = "skunk", duration = 60000, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_TELEPORT},
	{name ="outfit", interval = 2000, chance = 12, monster = "donkey", duration = 60000, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_TELEPORT}
}

monster.defenses = {
	defense = 5,
	armor = 10
}

monster.loot = {
}

mType:register(monster)
