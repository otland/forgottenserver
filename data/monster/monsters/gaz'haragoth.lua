local mType = Game.createMonsterType("gaz'haragoth")
local monster = {}

monster.name = "Gaz'haragoth"
monster.description = "Gaz'haragoth"
monster.experience = 1000000
monster.outfit = {
	lookType = 591,
	lookHead = 0,
	lookBody = 94,
	lookLegs = 79,
	lookFeet = 79,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 350000
monster.maxHealth = 350000
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 22562
monster.speed = 400
monster.summonCost = 0
monster.maxSummons = 6

monster.changeTarget = {
	interval = 10000,
	chance = 20
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = false,
	boss = true,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 98,
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
	{text = "Gaz'haragoth begins to channel DEATH AND DOOM into the area! RUN!", yell = true},
	{text = "Minions! Follow my call!", yell = true},
	{text = "I've beaten tougher demons then you even know", yell = true},
	{text = "You puny humans will be my snacks!", yell = true},
	{text = "Gaz'haragoth calls down: DEATH AND DOOM!", yell = true}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 50},
	{type = COMBAT_ENERGYDAMAGE, percent = 50},
	{type = COMBAT_EARTHDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = 50},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE , percent = 50},
	{type = COMBAT_DEATHDAMAGE , percent = 50}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -5000, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 3000, chance = 35, type = COMBAT_ICEDAMAGE, minDamage = -900, maxDamage = -1100, range = 7, radius = 7, target = true, ShootEffect = CONST_ANI_ICE, effect = CONST_ME_ICEAREA},
	{name ="melee", interval = 1800, chance = 40, minDamage = 0, maxDamage = -1000, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 3000, chance = 30, type = COMBAT_FIREDAMAGE, minDamage = -4000, maxDamage = -6000, length = 8, spread = 0, effect = CONST_ME_HITBYFIRE},
	{name ="combat", interval = 2500, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = -200, maxDamage = -480, range = 7, radius = 5, target = false, effect = CONST_ME_MAGIC_GREEN}
}

monster.defenses = {
	defense = 65,
	armor = 55,
	{name ="combat", interval = 3000, chance = 35, type = COMBAT_HEALING, minDamage = 2500, maxDamage = 3500, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 4000, chance = 80, speed = {min = 500, max = 500}, duration = 6000, effect = CONST_ME_MAGIC_RED}
}

monster.summons = {
	{name = "Minion of Gaz'haragoth", chance = 33, interval = 4000, max = 3},
	{name = "Nightmare of Gaz'haragoth", chance = 33, interval = 4000, max = 3}
}

monster.loot = {
	{id = "platinum coin", chance = 10000, maxCount = 50},
	{id = "blue gem", chance = 10000, maxCount = 2},
	{id = "crystal coin", chance = 10000, maxCount = 6},
	{id = "life ring", chance = 6000},
	{id = "boots of haste", chance = 6000},
	{id = "red piece of cloth", chance = 6000},
	{id = "yellow piece of cloth", chance = 6000},
	{id = "demon horn", chance = 6000},
	{id = "demonic essence", chance = 10000, maxCount = 2},
	{id = "infernal bolt", chance = 6000},
	{id = "assassin star", chance = 10000, maxCount = 100},
	{id = "great mana potion", chance = 10000, maxCount = 100},
	{id = 7632, chance = 6000},
	{id = "magma coat", chance = 6000},
	{id = "great spirit potion", chance = 10000, maxCount = 100},
	{id = "ultimate health potion", chance = 10000, maxCount = 100},
	{id = "phoenix charm", chance = 6000},
	{id = "unity charm", chance = 6000},
	{id = "spiritual charm", chance = 6000},
	{id = "twin sun charm", chance = 6000},
	{id = "solitude charm", chance = 6000},
	{id = "violet crystal shard", chance = 10000, maxCount = 15},
	{id = "green crystal shard", chance = 10000, maxCount = 5},
	{id = "green crystal splinter", chance = 10000, maxCount = 10},
	{id = "blue crystal splinter", chance = 10000, maxCount = 15},
	{id = "cluster of solace", chance = 10000, maxCount = 14},
	{id = "dream matter", chance = 10000, maxCount = 3},
	{id = "crude umbral blade", chance = 5000},
	{id = "umbral blade", chance = 5000},
	{id = "umbral masterblade", chance = 4000},
	{id = "crude umbral slayer", chance = 5000},
	{id = "umbral slayer", chance = 5000},
	{id = "umbral master slayer", chance = 3000},
	{id = "crude umbral axe", chance = 5000},
	{id = "umbral axe", chance = 4000},
	{id = "umbral master axe", chance = 3000},
	{id = "crude umbral chopper", chance = 5000},
	{id = "umbral chopper", chance = 4000},
	{id = "umbral master chopper", chance = 3000},
	{id = "crude umbral mace", chance = 5000},
	{id = "umbral mace", chance = 4000},
	{id = "umbral master mace", chance = 3000},
	{id = "crude umbral hammer", chance = 5000},
	{id = "umbral hammer", chance = 4000},
	{id = "umbral master hammer", chance = 3000},
	{id = "crude umbral bow", chance = 5000},
	{id = "umbral bow", chance = 4000},
	{id = "umbral master bow", chance = 3000},
	{id = "crude umbral crossbow", chance = 5000},
	{id = "umbral crossbow", chance = 4000},
	{id = "umbral master crossbow", chance = 3000},
	{id = "crude umbral spellbook", chance = 5000},
	{id = "umbral spellbook", chance = 4000},
	{id = "umbral master spellbook", chance = 3000},
	{id = "unrealized dream", chance = 10000, maxCount = 3},
	{id = "nightmare horn", chance = 6000},
	{id = "dream warden mask", chance = 6000},
	{id = "psychedelic tapestry", chance = 6000},
	{id = "demonic tapestry", chance = 6000},
	{id = "eye pod", chance = 6000},
	{id = 22614, chance = 6000}
}

mType:register(monster)
