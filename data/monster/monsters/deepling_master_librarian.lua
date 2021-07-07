local mType = Game.createMonsterType("deepling master librarian")
local monster = {}

monster.name = "Deepling Master Librarian"
monster.description = "a deepling master librarian"
monster.experience = 1900
monster.outfit = {
	lookType = 443,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1700
monster.maxHealth = 1700
monster.runHealth = 250
monster.race = "blood"
monster.corpse = 15211
monster.speed = 190
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
	pushable = true,
	boss = false,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 60,
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
	{text = "Mmmmmoooaaaaaahaaa!!!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -210, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ICEDAMAGE, minDamage = -260, maxDamage = -450, length = 10, spread = 3, effect = CONST_ME_ICETORNADO},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_DROWNDAMAGE, minDamage = -150, maxDamage = -280, radius = 4, target = true, effect = CONST_ME_BUBBLES},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_LIFEDRAIN, minDamage = -80, maxDamage = -140, range = 7, effect = CONST_ME_SMALLPLANTS},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_MANADRAIN, minDamage = -60, maxDamage = -140, range = 7, effect = CONST_ME_SMALLPLANTS},
	{name ="deepling spellsinger skill reducer", interval = 2000, chance = 10, range = 5}
}

monster.defenses = {
	defense = 20,
	armor = 20,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 40, maxDamage = 80, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "small sapphire", chance = 8440, maxCount = 3},
	{id = "gold coin", chance = 100000, maxCount = 100},
	{id = "platinum coin", chance = 90000, maxCount = 3},
	{id = "life ring", chance = 3200},
	{id = 2667, chance = 9090, maxCount = 2},
	{id = "fish fin", chance = 1950},
	{id = "eye of a deepling", chance = 9380},
	{id = "deepling staff", chance = 3130},
	{id = "necklace of the deep", chance = 1330},
	{id = "spellsinger's seal", chance = 25000},
	{id = "key to the Drowned Library", chance = 20000},
	{id = "deepling filet", chance = 20000},
	{id = "ornate crossbow", chance = 39}
}

mType:register(monster)
