local mType = Game.createMonsterType("lost thrower")
local monster = {}

monster.name = "Lost Thrower"
monster.description = "a lost thrower"
monster.experience = 1200
monster.outfit = {
	lookType = 539,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1700
monster.maxHealth = 1700
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 19998
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
	illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
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
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 40},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -301, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -250, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_THROWINGSTAR, effect = CONST_ME_EXPLOSIONAREA},
	{name ="combat", interval = 2000, chance = 5, type = COMBAT_PHYSICALDAMAGE, range = 7, radius = 2, target = false, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_PHYSICALDAMAGE, minDamage = -150, maxDamage = -300, range = 7, radius = 2, target = true, ShootEffect = CONST_ANI_WHIRLWINDCLUB, effect = CONST_ME_STUN},
	{name ="drunk", interval = 2000, chance = 10, drunkenness = 25, duration = 6000, radius = 3, target = true, ShootEffect = CONST_ANI_WHIRLWINDAXE, effect = CONST_ME_EXPLOSIONAREA}
}

monster.defenses = {
	defense = 30,
	armor = 30,
	{name ="combat", interval = 2000, chance = 5, type = COMBAT_HEALING, minDamage = 100, maxDamage = 500, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 2000, chance = 10, duration = 5000, effect = CONST_ME_TELEPORT}
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 37},
	{id = "platinum coin", chance = 70000, maxCount = 2},
	{id = "brown mushroom", chance = 14500, maxCount = 2},
	{id = "iron ore", chance = 9470},
	{id = "great mana potion", chance = 13470},
	{id = "great health potion", chance = 14730},
	{id = 13757, chance = 12180},
	{id = "lost basher's spike", chance = 11410},
	{id = "buckle", chance = 850},
	{id = "holy ash", chance = 15670},
	{id = "broken throwing axe", chance = 1390},
	{id = "helmet of the lost", chance = 13560},
	{id = "lost bracers", chance = 14860},
	{id = "mad froth", chance = 11810},
	{id = "basalt fetish", chance = 6150},
	{id = "basalt figurine", chance = 7900}
}

mType:register(monster)
