local mType = Game.createMonsterType("undead gladiator")
local monster = {}

monster.name = "Undead Gladiator"
monster.description = "an undead gladiator"
monster.experience = 800
monster.outfit = {
	lookType = 306,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1000
monster.maxHealth = 1000
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 9823
monster.speed = 180
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
	{text = "Let's battle it out in a duel!", yell = false},
	{text = "Bring it!", yell = false},
	{text = "I'll fight here in eternity and beyond.", yell = false},
	{text = "I will not give up!", yell = false},
	{text = "Another foolish adventurer who tries to beat me.", yell = false}
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
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 80},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -250, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -135, range = 7, ShootEffect = CONST_ANI_WHIRLWINDSWORD}
}

monster.defenses = {
	defense = 45,
	armor = 45,
	{name ="invisible", interval = 2000, chance = 15, duration = 3000, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "gold coin", chance = 44000, maxCount = 100},
	{id = "gold coin", chance = 50500, maxCount = 48},
	{id = "stealth ring", chance = 30},
	{id = "protection amulet", chance = 2200},
	{id = "two handed sword", chance = 1900},
	{id = "throwing star", chance = 15700, maxCount = 18},
	{id = 2419, chance = 11280},
	{id = "knight axe", chance = 280},
	{id = "plate armor", chance = 1700},
	{id = "brass armor", chance = 4700},
	{id = "brass legs", chance = 5500},
	{id = "dark helmet", chance = 1460},
	{id = "crusader helmet", chance = 100},
	{id = "plate legs", chance = 2444},
	{id = "meat", chance = 15000, maxCount = 2},
	{id = "hunting spear", chance = 4200},
	{id = "flask of warrior's sweat", chance = 210},
	{id = "health potion", chance = 350},
	{id = "belted cape", chance = 5000},
	{id = "broken gladiator shield", chance = 5200}
}

mType:register(monster)
