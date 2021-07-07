local mType = Game.createMonsterType("ferumbras")
local monster = {}

monster.name = "Ferumbras"
monster.description = "Ferumbras"
monster.experience = 12000
monster.outfit = {
	lookType = 229,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 50000
monster.maxHealth = 50000
monster.runHealth = 2500
monster.race = "venom"
monster.corpse = 6078
monster.speed = 320
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
	boss = true,
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
	{text = "NOONE WILL STOP ME THIS TIME!", yell = true},
	{text = "THE POWER IS MINE!", yell = true},
	{text = "I returned from death and you dream about defeating me?", yell = false},
	{text = "Witness the first seconds of my eternal world domination!", yell = false},
	{text = "Even in my weakened state I will crush you all!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 90},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 90},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 20},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -350, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_MANADRAIN, minDamage = -300, maxDamage = -700, range = 7, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 3000, chance = 20, type = COMBAT_EARTHDAMAGE, minDamage = -250, maxDamage = -550, radius = 6, target = false, effect = CONST_ME_POISONAREA},
	{name ="combat", interval = 2000, chance = 18, type = COMBAT_ENERGYDAMAGE, minDamage = -200, maxDamage = -400, radius = 6, target = false, effect = CONST_ME_ENERGYHIT},
	{name ="combat", interval = 2000, chance = 16, type = COMBAT_MANADRAIN, minDamage = -225, maxDamage = -375, radius = 6, target = false, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 21, type = COMBAT_LIFEDRAIN, minDamage = -200, maxDamage = -450, radius = 6, target = false, effect = CONST_ME_POFF},
	{name ="effect", interval = 3000, chance = 20, minDamage = -200, maxDamage = -600, radius = 5, target = false, effect = CONST_ME_HITAREA},
	{name ="condition", interval = 3000, chance = 20, type = CONDITION_FIRE, startDamage = 0, tick = 9000, minDamage = -200, maxDamage = -800, range = 7, radius = 7, target = true, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA},
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_LIFEDRAIN, minDamage = -50, maxDamage = -250, length = 8, spread = 0, effect = CONST_ME_HITBYPOISON}
}

monster.defenses = {
	defense = 120,
	armor = 100,
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_HEALING, minDamage = 900, maxDamage = 1500, effect = CONST_ME_MAGIC_GREEN},
	{name ="speed", interval = 3000, chance = 30, speed = {min = 360, max = 360}, duration = 7000, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 4000, chance = 20, duration = 5000, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Demon", chance = 12, interval = 3000}
}

monster.loot = {
	{id = "Ferumbras' hat", chance = 100000},
	{id = "gold coin", chance = 97000, maxCount = 100},
	{id = "gold coin", chance = 97000, maxCount = 100},
	{id = "gold coin", chance = 33000, maxCount = 70},
	{id = "gold ingot", chance = 75000, maxCount = 2},
	{id = "great shield", chance = 26000},
	{id = "spellbook of lost souls", chance = 26000},
	{id = "golden armor", chance = 24000},
	{id = "golden legs", chance = 22000},
	{id = "spellbook of mind control", chance = 22000},
	{id = "velvet mantle", chance = 22000},
	{id = "demon shield", chance = 20000},
	{id = "divine plate", chance = 20000},
	{id = "magma legs", chance = 20000},
	{id = "tempest shield", chance = 20000},
	{id = "emerald bangle", chance = 18000},
	{id = "glacier kilt", chance = 18000},
	{id = "lightning legs", chance = 18000},
	{id = "phoenix shield", chance = 18000},
	{id = "spellbook of dark mysteries", chance = 18000},
	{id = "terra legs", chance = 18000},
	{id = "emerald sword", chance = 16000},
	{id = "havoc blade", chance = 16000},
	{id = "shadow sceptre", chance = 16000},
	{id = "small emerald", chance = 16000, maxCount = 100},
	{id = 7632, chance = 14000, maxCount = 5},
	{id = 7633, chance = 14000, maxCount = 5},
	{id = "magic plate armor", chance = 14000},
	{id = "mastermind shield", chance = 14000},
	{id = "runed sword", chance = 14000},
	{id = "spellscroll of prophecies", chance = 14000},
	{id = "chaos mace", chance = 12000},
	{id = "demonwing axe", chance = 12000},
	{id = "greenwood coat", chance = 12000},
	{id = "small sapphire", chance = 12000, maxCount = 98},
	{id = "white pearl", chance = 12000, maxCount = 88},
	{id = "haunted blade", chance = 10000},
	{id = "hellforged axe", chance = 10000},
	{id = "obsidian truncheon", chance = 10000},
	{id = "ornamented axe", chance = 10000},
	{id = "queen's sceptre", chance = 10000},
	{id = "small amethyst", chance = 10000, maxCount = 54},
	{id = "small topaz", chance = 10000, maxCount = 87},
	{id = "demonrage sword", chance = 8100},
	{id = "jade hammer", chance = 8100},
	{id = "platinum coin", chance = 8100, maxCount = 58},
	{id = "skullcrusher", chance = 8100},
	{id = "soul orb", chance = 8100, maxCount = 9},
	{id = "vile axe", chance = 8100},
	{id = "abyss hammer", chance = 6100},
	{id = "berserker", chance = 6100},
	{id = "black pearl", chance = 6100, maxCount = 83},
	{id = "bloody edge", chance = 6100},
	{id = "nightmare blade", chance = 6100},
	{id = "great axe", chance = 4000},
	{id = "impaler", chance = 4000},
	{id = "red tome", chance = 4000},
	{id = "small diamond", chance = 4000, maxCount = 82},
	{id = "small ruby", chance = 4000, maxCount = 92}
}

mType:register(monster)
