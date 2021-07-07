local mType = Game.createMonsterType("mahrdis")
local monster = {}

monster.name = "Mahrdis"
monster.description = "Mahrdis"
monster.experience = 3050
monster.outfit = {
	lookType = 90,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 3900
monster.maxHealth = 3900
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 6025
monster.speed = 340
monster.summonCost = 0
monster.maxSummons = 4

monster.changeTarget = {
	interval = 5000,
	chance = 8
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
	{text = "Ashes to ashes!", yell = false},
	{text = "Fire, Fire!", yell = false},
	{text = "The eternal flame demands its due!", yell = false},
	{text = "Burnnnnnnnnn!", yell = false},
	{text = "May my flames engulf you!", yell = false},
	{text = "This is why I'm hot.", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE , percent = -20},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -400, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 65, interval = 4000}},
	{name ="combat", interval = 1600, chance = 7, type = COMBAT_PHYSICALDAMAGE, minDamage = -60, maxDamage = -600, range = 1, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 1000, chance = 7, type = COMBAT_FIREDAMAGE, minDamage = -60, maxDamage = -600, range = 7, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA},
	{name ="speed", interval = 2000, chance = 13, speed = {min = -850, max = -850}, duration = 50000, range = 7, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 34, type = COMBAT_FIREDAMAGE, minDamage = -80, maxDamage = -800, radius = 3, target = false, effect = CONST_ME_EXPLOSIONAREA},
	{name ="firefield", interval = 1000, chance = 12, radius = 4, target = false, effect = CONST_ME_BLOCKHIT},
	{name ="condition", interval = 2000, chance = 13, type = CONDITION_FIRE, startDamage = 0, tick = 9000, minDamage = -50, maxDamage = -500, length = 8, spread = 3, effect = CONST_ME_EXPLOSIONHIT}
}

monster.defenses = {
	defense = 42,
	armor = 42,
	{name ="combat", interval = 2000, chance = 40, type = COMBAT_HEALING, minDamage = 150, maxDamage = 600, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Fire Elemental", chance = 30, interval = 2000}
}

monster.loot = {
	{id = "burning heart", chance = 100000},
	{id = "gold coin", chance = 30000, maxCount = 100},
	{id = "gold coin", chance = 30000, maxCount = 100},
	{id = "gold coin", chance = 30000, maxCount = 30},
	{id = "small ruby", chance = 12000, maxCount = 3},
	{id = "great health potion", chance = 10500},
	{id = "life ring", chance = 6700},
	{id = "fire axe", chance = 1050},
	{id = "red gem", chance = 600},
	{id = "holy falcon", chance = 150},
	{id = "mini mummy", chance = 110},
	{id = "phoenix shield", chance = 110}
}

mType:register(monster)
