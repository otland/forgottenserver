local mType = Game.createMonsterType("blood priest")
local monster = {}

monster.name = "Blood Priest"
monster.description = "a blood priest"
monster.experience = 900
monster.outfit = {
	lookType = 553,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 820
monster.maxHealth = 820
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 21262
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
	{text = "Blood for the dark god!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE , percent = -5},
	{type = COMBAT_DEATHDAMAGE , percent = 5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -80, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 100, interval = 4000}},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_DEATHDAMAGE, minDamage = -60, maxDamage = -100, range = 7, ShootEffect = CONST_ANI_DEATH, effect = CONST_ME_SMALLCLOUDS},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_LIFEDRAIN, minDamage = -40, maxDamage = -60, radius = 4, target = false, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 3000, chance = 10, type = COMBAT_MANADRAIN, minDamage = -80, maxDamage = -130, range = 1, length = 7, spread = 0, effect = CONST_ME_HITAREA},
	{name ="condition", interval = 2000, chance = 5, type = CONDITION_BLEEDING, startDamage = 0, tick = 4000, minDamage = -160, maxDamage = -290, range = 1, radius = 1, target = true, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 30,
	armor = 30,
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_HEALING, minDamage = 80, maxDamage = 120, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "small ruby", chance = 3510, maxCount = 2},
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 71},
	{id = "boots of haste", chance = 120},
	{id = "skull staff", chance = 180},
	{id = "mystic turban", chance = 2170},
	{id = "white piece of cloth", chance = 3040},
	{id = "red piece of cloth", chance = 640},
	{id = "strong mana potion", chance = 6000},
	{id = "spellbook of warding", chance = 290},
	{id = "spellbook of mind control", chance = 180},
	{id = "underworld rod", chance = 470},
	{id = "book of necromantic rituals", chance = 13550},
	{id = "rough red gem", chance = 880},
	{id = "lancet", chance = 14410},
	{id = "horoscope", chance = 9270},
	{id = "blood tincture in a vial", chance = 14460},
	{id = "incantation notes", chance = 14660},
	{id = "pieces of magic chalk", chance = 7770}
}

mType:register(monster)
