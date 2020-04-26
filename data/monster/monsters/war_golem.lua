local mType = Game.createMonsterType("war golem")
local monster = {}

monster.name = "War Golem"
monster.description = "a war golem"
monster.experience = 2750
monster.outfit = {
	lookType = 326,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 4300
monster.maxHealth = 4300
monster.runHealth = 0
monster.race = "venom"
monster.corpse = 10005
monster.speed = 240
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
	{text = "Azerus barada nikto!", yell = false},
	{text = "Klonk klonk klonk", yell = false},
	{text = "Engaging Enemy!", yell = false},
	{text = "Threat level processed.", yell = false},
	{text = "Charging weapon systems!", yell = false},
	{text = "Auto repair in progress.", yell = false},
	{text = "The battle is joined!", yell = false},
	{text = "Termination initialized!", yell = false},
	{text = "Rrrtttarrrttarrrtta", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 25},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_EARTHDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = 15},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 30},
	{type = COMBAT_HOLYDAMAGE , percent = 50},
	{type = COMBAT_DEATHDAMAGE , percent = 25}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -550, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_ENERGYDAMAGE, minDamage = -165, maxDamage = -220, range = 7, ShootEffect = CONST_ANI_ENERGY},
	{name ="outfit", interval = 2000, chance = 1, monster = "skeleton", duration = 3000, range = 7},
	{name ="war golem electrify", interval = 2000, chance = 15, range = 1},
	{name ="war golem skill reducer", interval = 2000, chance = 10}
}

monster.defenses = {
	defense = 40,
	armor = 40,
	{name ="speed", interval = 2000, chance = 15, speed = {min = 300, max = 300}, duration = 5000, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 200, maxDamage = 250, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "gold coin", chance = 37500, maxCount = 90},
	{id = "gold coin", chance = 37500, maxCount = 80},
	{id = "life crystal", chance = 1000},
	{id = "club ring", chance = 810},
	{id = "dwarven ring", chance = 1210},
	{id = "two handed sword", chance = 5500},
	{id = "morning star", chance = 7000},
	{id = "epee", chance = 6400},
	{id = "plate shield", chance = 9000},
	{id = "battle shield", chance = 6500},
	{id = "steel boots", chance = 620},
	{id = "iron ore", chance = 1920},
	{id = "berserker", chance = 50},
	{id = "jade hammer", chance = 120},
	{id = "bonebreaker", chance = 770},
	{id = "berserk potion", chance = 900},
	{id = "great mana potion", chance = 8860},
	{id = "lightning boots", chance = 90},
	{id = "nail", chance = 5260, maxCount = 5},
	{id = "ultimate health potion", chance = 10080},
	{id = 9809, chance = 260},
	{id = 9978, chance = 1080},
	{id = "crystal of power", chance = 130}
}

mType:register(monster)
