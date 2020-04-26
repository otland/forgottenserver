local mType = Game.createMonsterType("parrot")
local monster = {}

monster.name = "Parrot"
monster.description = "a parrot"
monster.experience = 0
monster.outfit = {
	lookType = 217,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 25
monster.maxHealth = 25
monster.runHealth = 25
monster.race = "blood"
monster.corpse = 6056
monster.speed = 320
monster.summonCost = 250
monster.maxSummons = 0

monster.changeTarget = {
	interval = 5000,
	chance = 0
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = false,
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
	{text = "You advanshed, you advanshed!", yell = false},
	{text = "Neeewbiiieee!", yell = false},
	{text = "Screeech!", yell = false},
	{text = "Hunterrr ish PK!", yell = false},
	{text = "BR? PL? SWE?", yell = false},
	{text = "Hope you die and loooosh it!", yell = false},
	{text = "You powerrrrrrabuserrrrr!", yell = false},
	{text = "You are corrrrupt! Corrrrupt!", yell = false},
	{text = "Tarrrrp?", yell = false},
	{text = "Blesshhh my stake! Screeech!", yell = false},
	{text = "Leeave orrr hunted!!", yell = false},
	{text = "Shhtop whining! Rraaah!", yell = false},
	{text = "I'm heeerrre! Screeeech!", yell = false}
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
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -5, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 2,
	armor = 2
}

monster.loot = {
}

mType:register(monster)
