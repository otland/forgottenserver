local mType = Game.createMonsterType("blood hand")
local monster = {}

monster.name = "Blood Hand"
monster.description = "a blood hand"
monster.experience = 750
monster.outfit = {
	lookType = 552,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 700
monster.maxHealth = 700
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 21257
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
	{text = "Blood for the dark god!", yell = false},
	{text = "Die, filth!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
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
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -158, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 80, interval = 4000}},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_LIFEDRAIN, minDamage = -50, maxDamage = -100, radius = 4, target = false, effect = CONST_ME_MAGIC_RED},
	{name ="speed", interval = 2000, chance = 10, speed = {min = -600, max = -600}, duration = 15000, radius = 4, target = true, effect = CONST_ME_BLOCKHIT},
	{name ="condition", interval = 2000, chance = 15, type = CONDITION_BLEEDING, startDamage = 0, tick = 4000, minDamage = -120, maxDamage = -160, radius = 6, target = false, effect = CONST_ME_HITAREA}
}

monster.defenses = {
	defense = 25,
	armor = 25,
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_HEALING, minDamage = 70, maxDamage = 90, effect = CONST_ME_MAGIC_BLUE},
	{name ="effect", interval = 2000, chance = 10, radius = 1, target = false, effect = CONST_ME_INSECTS}
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 30},
	{id = "necrotic rod", chance = 3000},
	{id = "boots of haste", chance = 210},
	{id = "skull staff", chance = 130},
	{id = "mystic turban", chance = 790},
	{id = "white piece of cloth", chance = 840},
	{id = "red piece of cloth", chance = 500},
	{id = "noble axe", chance = 10},
	{id = "strong mana potion", chance = 5590},
	{id = "spellbook of enlightenment", chance = 790},
	{id = "book of necromantic rituals", chance = 9340},
	{id = "rough red gem", chance = 710},
	{id = "lancet", chance = 10680},
	{id = "horoscope", chance = 7950},
	{id = "blood tincture in a vial", chance = 15460},
	{id = "incantation notes", chance = 8820},
	{id = "pieces of magic chalk", chance = 6120}
}

mType:register(monster)
