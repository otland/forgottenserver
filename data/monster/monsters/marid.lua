local mType = Game.createMonsterType("marid")
local monster = {}

monster.name = "Marid"
monster.description = "a marid"
monster.experience = 410
monster.outfit = {
	lookType = 104,
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
monster.corpse = 6033
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
	{text = "Simsalabim", yell = false},
	{text = "Feel the power of my magic, tiny mortal!", yell = false},
	{text = "Wishes can come true.", yell = false},
	{text = "Djinns will soon again be the greatest!", yell = false},
	{text = "Be careful what you wish.", yell = false}
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
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = -8}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -90, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ENERGYDAMAGE, minDamage = -100, maxDamage = -250, range = 7, ShootEffect = CONST_ANI_ENERGYBALL},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_LIFEDRAIN, minDamage = -30, maxDamage = -90, range = 7, effect = CONST_ME_MAGIC_RED},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -650, max = -650}, duration = 1500, range = 7, effect = CONST_ME_MAGIC_RED},
	{name ="drunk", interval = 2000, chance = 10, drunkenness = 25, duration = 6000, range = 7, ShootEffect = CONST_ANI_ENERGY},
	{name ="outfit", interval = 2000, chance = 1, monster = "rabbit", duration = 4000, range = 7, effect = CONST_ME_MAGIC_BLUE},
	{name ="djinn electrify", interval = 2000, chance = 15, range = 5},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_ENERGYDAMAGE, minDamage = -30, maxDamage = -90, radius = 3, target = false, effect = CONST_ME_ENERGYHIT}
}

monster.defenses = {
	defense = 24,
	armor = 24,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 50, maxDamage = 80, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "blue djinn", chance = 10, interval = 2000}
}

monster.loot = {
	{id = "gold coin", chance = 49000, maxCount = 100},
	{id = "gold coin", chance = 49000, maxCount = 29},
	{id = "blueberry", chance = 25000, maxCount = 25},
	{id = "royal spear", chance = 15500, maxCount = 3},
	{id = "strong mana potion", chance = 9800},
	{id = "jewelled belt", chance = 8000},
	{id = "small sapphire", chance = 6200},
	{id = "heavy machete", chance = 4700},
	{id = "blue piece of cloth", chance = 3750},
	{id = 1872, chance = 2560},
	{id = "seeds", chance = 2400},
	{id = "hailstorm rod", chance = 770},
	{id = "noble turban", chance = 530},
	{id = "mystic turban", chance = 290},
	{id = "magma monocle", chance = 260},
	{id = 2374, chance = 260},
	{id = "small oil lamp", chance = 130},
	{id = "blue gem", chance = 80}
}

mType:register(monster)
