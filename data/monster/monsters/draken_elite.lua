local mType = Game.createMonsterType("draken elite")
local monster = {}

monster.name = "Draken Elite"
monster.description = "a draken elite"
monster.experience = 4200
monster.outfit = {
	lookType = 362,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 5550
monster.maxHealth = 5550
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 12609
monster.speed = 250
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 5000,
	chance = 10
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = false,
	boss = false,
	illusionable = true,
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
	{text = "For ze emperor!", yell = false},
	{text = "You will die zhouzandz deazhz!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 40},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 30},
	{type = COMBAT_DEATHDAMAGE , percent = 30}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -354, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_FIREDAMAGE, minDamage = -240, maxDamage = -550, length = 4, spread = 3, effect = CONST_ME_EXPLOSIONHIT},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_FIREDAMAGE, minDamage = -200, maxDamage = -300, range = 7, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_EARTHDAMAGE, minDamage = -280, maxDamage = -410, radius = 4, target = true, effect = CONST_ME_POFF},
	{name ="monster soulfire", interval = 2000, chance = 10},
	{name ="condition", interval = 2000, chance = 10, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -250, maxDamage = -320, range = 7, ShootEffect = CONST_ANI_POISON}
}

monster.defenses = {
	defense = 45,
	armor = 45,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 510, maxDamage = 600, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "small diamond", chance = 2440, maxCount = 4},
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 47000, maxCount = 100},
	{id = "platinum coin", chance = 50360, maxCount = 8},
	{id = "meat", chance = 30175},
	{id = "magic sulphur", chance = 2100},
	{id = "assassin dagger", chance = 980},
	{id = "great mana potion", chance = 9340, maxCount = 3},
	{id = "ultimate health potion", chance = 9250, maxCount = 3},
	{id = "Zaoan armor", chance = 490},
	{id = "Zaoan helmet", chance = 150},
	{id = "Zaoan legs", chance = 770},
	{id = "Zaoan sword", chance = 490},
	{id = "elite draken mail", chance = 110},
	{id = "twiceslicer", chance = 910},
	{id = "draken sulphur", chance = 7600},
	{id = "draken wristbands", chance = 14030},
	{id = "broken draken mail", chance = 16930},
	{id = "broken slicer", chance = 24670},
	{id = "cobra crown", chance = 10},
	{id = "draken boots", chance = 600},
	{id = "snake god's wristguard", chance = 80},
	{id = "blade of corruption", chance = 20}
}

mType:register(monster)
