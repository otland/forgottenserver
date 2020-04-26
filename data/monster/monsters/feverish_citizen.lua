local mType = Game.createMonsterType("feverish citizen")
local monster = {}

monster.name = "Feverish Citizen"
monster.description = "a feverish citizen"
monster.experience = 30
monster.outfit = {
	lookType = 425,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 2,
	lookMount = 0
}

monster.health = 125
monster.maxHealth = 125
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20395
monster.speed = 185
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
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
	{text = "Aaaaargh!", yell = false},
	{text = "I am the king of the world!", yell = false},
	{text = "Die Ferumbras!", yell = false},
	{text = "Tigerblood is running through my veins!", yell = false},
	{text = "You! It's you again!", yell = false},
	{text = "Stand still you tasty morsel!", yell = false},
	{text = "<giggle>", yell = false},
	{text = "Burn heretic! Burn!", yell = false},
	{text = "Harrr!", yell = false},
	{text = "This is Venore!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 80},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -18, effect = CONST_ME_DRAWBLOOD},
	{name ="drunk", interval = 2000, chance = 15, drunkenness = 25, duration = 3000, length = 3, spread = 2, effect = CONST_ME_POISONAREA}
}

monster.defenses = {
	defense = 15,
	armor = 15,
	{name ="outfit", interval = 2000, chance = 1, monster = "bog raider", duration = 5000, radius = 3, target = false, effect = CONST_ME_GREEN_RINGS}
}

monster.loot = {
	{id = "gold coin", chance = 30300, maxCount = 35},
	{id = 2230, chance = 4920},
	{id = "worm", chance = 25730, maxCount = 3},
	{id = "ominous piece of cloth", chance = 1660},
	{id = "dubious piece of cloth", chance = 1720},
	{id = "voluminous piece of cloth", chance = 2290},
	{id = "obvious piece of cloth", chance = 2006},
	{id = "ludicrous piece of cloth", chance = 2060},
	{id = "luminous piece of cloth", chance = 2290},
	{id = "plague mask", chance = 100},
	{id = "plague bell", chance = 100}
}

mType:register(monster)
