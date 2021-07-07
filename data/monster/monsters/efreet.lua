local mType = Game.createMonsterType("efreet")
local monster = {}

monster.name = "Efreet"
monster.description = "an efreet"
monster.experience = 410
monster.outfit = {
	lookType = 103,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 550
monster.maxHealth = 550
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6032
monster.speed = 234
monster.summonCost = 0
monster.maxSummons = 2

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
	{text = "I grant you a deathwish!", yell = false},
	{text = "Good wishes are for fairytales", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 60},
	{type = COMBAT_EARTHDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = 90},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE , percent = -8},
	{type = COMBAT_DEATHDAMAGE , percent = 20}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -110, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_FIREDAMAGE, minDamage = -40, maxDamage = -110, range = 7, ShootEffect = CONST_ANI_FIRE},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ENERGYDAMAGE, minDamage = -30, maxDamage = -90, radius = 3, target = false, effect = CONST_ME_ENERGYHIT},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_ENERGYDAMAGE, minDamage = -65, maxDamage = -120, range = 7, ShootEffect = CONST_ANI_ENERGY, effect = CONST_ME_ENERGYHIT},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -650, max = -650}, duration = 15000, range = 7, effect = CONST_ME_MAGIC_RED},
	{name ="drunk", interval = 2000, chance = 10, drunkenness = 25, duration = 6000, range = 7, ShootEffect = CONST_ANI_ENERGY},
	{name ="outfit", interval = 2000, chance = 1, monster = "rat", duration = 4000, range = 7, effect = CONST_ME_MAGIC_BLUE},
	{name ="djinn electrify", interval = 2000, chance = 15, range = 5}
}

monster.defenses = {
	defense = 24,
	armor = 24,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 50, maxDamage = 80, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "green djinn", chance = 10, interval = 2000}
}

monster.loot = {
	{id = "gold coin", chance = 47000, maxCount = 100},
	{id = "gold coin", chance = 47000, maxCount = 30},
	{id = "royal spear", chance = 15570, maxCount = 3},
	{id = "strong mana potion", chance = 10500},
	{id = "pear", chance = 9390, maxCount = 5},
	{id = "jewelled belt", chance = 8540},
	{id = "small emerald", chance = 7200},
	{id = "heavy machete", chance = 5000},
	{id = "green piece of cloth", chance = 3000},
	{id = 1860, chance = 2200},
	{id = "noble turban", chance = 1130},
	{id = "magma monocle", chance = 420},
	{id = "wand of inferno", chance = 390},
	{id = "mystic turban", chance = 180},
	{id = "small oil lamp", chance = 180},
	{id = "green gem", chance = 150}
}

mType:register(monster)
