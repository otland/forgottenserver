local mType = Game.createMonsterType("stone devourer")
local monster = {}

monster.name = "Stone Devourer"
monster.description = "a stone devourer"
monster.experience = 2900
monster.outfit = {
	lookType = 486,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 4200
monster.maxHealth = 4200
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 18375
monster.speed = 250
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
	{text = "Rumble!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 1},
	{type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 30},
	{type = COMBAT_HOLYDAMAGE , percent = 30},
	{type = COMBAT_DEATHDAMAGE , percent = 1}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -990, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_EARTHDAMAGE, minDamage = -230, maxDamage = -460, range = 7, ShootEffect = CONST_ANI_SMALLEARTH, effect = CONST_ME_STONES},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -650, range = 7, ShootEffect = CONST_ANI_LARGEROCK},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_LIFEDRAIN, minDamage = -150, maxDamage = -260, length = 5, spread = 0, effect = CONST_ME_STONES}
}

monster.defenses = {
	defense = 35,
	armor = 35
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 100},
	{id = "platinum coin", chance = 92000, maxCount = 7},
	{id = "stone skin amulet", chance = 2270},
	{id = "dwarven ring", chance = 2840},
	{id = "giant sword", chance = 570},
	{id = "crystal mace", chance = 850},
	{id = "war axe", chance = 920},
	{id = "sapphire hammer", chance = 1490},
	{id = "spiked squelcher", chance = 1490},
	{id = "glorious axe", chance = 3340},
	{id = "strong health potion", chance = 13840, maxCount = 2},
	{id = "strong mana potion", chance = 14900, maxCount = 2},
	{id = "great mana potion", chance = 15610, maxCount = 2},
	{id = "mana potion", chance = 15050, maxCount = 2},
	{id = "ultimate health potion", chance = 14410},
	{id = 8748, chance = 11360},
	{id = "ancient stone", chance = 12850},
	{id = "crystalline arrow", chance = 9940, maxCount = 10},
	{id = "green crystal splinter", chance = 6960},
	{id = "cyan crystal fragment", chance = 6810},
	{id = "stone nose", chance = 18679},
	{id = "crystalline spikes", chance = 16320}
}

mType:register(monster)
