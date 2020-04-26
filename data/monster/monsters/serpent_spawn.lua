local mType = Game.createMonsterType("serpent spawn")
local monster = {}

monster.name = "Serpent Spawn"
monster.description = "a serpent spawn"
monster.experience = 3050
monster.outfit = {
	lookType = 220,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 3000
monster.maxHealth = 3000
monster.runHealth = 275
monster.race = "blood"
monster.corpse = 6061
monster.speed = 234
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
	staticAttackChance = 80,
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
	{text = "Sssssouls for the one", yell = false},
	{text = "HISSSS", yell = true},
	{text = "Tsssse one will risssse again", yell = false},
	{text = "I bring your deathhh, mortalssss", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -250, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_EARTHDAMAGE, minDamage = -80, maxDamage = -300, range = 7, ShootEffect = CONST_ANI_POISON},
	{name ="outfit", interval = 2000, chance = 1, item = 3976, duration = 3000, range = 7, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 2000, chance = 25, speed = {min = -850, max = -850}, duration = 12000, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_GREEN_RINGS},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = -200, maxDamage = -500, length = 8, spread = 0, effect = CONST_ME_SOUND_RED},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_EARTHDAMAGE, minDamage = -200, maxDamage = -500, length = 8, spread = 3, effect = CONST_ME_POISONAREA}
}

monster.defenses = {
	defense = 35,
	armor = 35,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 250, maxDamage = 500, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 2000, chance = 15, speed = {min = 340, max = 340}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "gold coin", chance = 32300, maxCount = 100},
	{id = "gold coin", chance = 32300, maxCount = 100},
	{id = "gold coin", chance = 32300, maxCount = 39},
	{id = "green mushroom", chance = 18200},
	{id = "snake skin", chance = 14800},
	{id = "small sapphire", chance = 12000},
	{id = "power bolt", chance = 6200},
	{id = "life ring", chance = 6050},
	{id = "energy ring", chance = 5900},
	{id = "golden mug", chance = 2870},
	{id = "mercenary sword", chance = 2070},
	{id = "great mana potion", chance = 2000},
	{id = "snakebite rod", chance = 930},
	{id = "winged tail", chance = 960},
	{id = "tower shield", chance = 860},
	{id = "noble axe", chance = 810},
	{id = "life crystal", chance = 780},
	{id = "strange helmet", chance = 630},
	{id = 4842, chance = 600},
	{id = "warrior helmet", chance = 560},
	{id = "crown armor", chance = 510},
	{id = "charmer's tiara", chance = 190},
	{id = "royal helmet", chance = 130},
	{id = "spellbook of mind control", chance = 100},
	{id = "swamplair armor", chance = 90}
}

mType:register(monster)
