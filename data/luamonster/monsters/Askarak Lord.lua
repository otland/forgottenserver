local mType = Game.createMonsterType("Askarak Lord")
local monster = {}
monster.description = "an askarak lord"
monster.experience = 1200
monster.outfit = {
	lookType = 410
}

monster.health = 2100
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 13956
monster.speed = 230
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "WE RULE!", yell = false},
	{text = "RED IS MAD", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 90, chance = 50000},
	{id = "platinum coin", maxCount = 2, chance = 35333},
	{id = "strong mana potion", chance = 7692},
	{id = "strong health potion", chance = 6250},
	{id = "small emerald", maxCount = 5, chance = 5882},
	{id = "brown mushroom", maxCount = 5, chance = 5263},
	{id = "springsprout rod", chance = 1010},
	{id = "energy ring", chance = 606},
	{id = "mastermind potion", chance = 750},
	{id = "magic sulphur", chance = 620},
	{id = "assassin star", maxCount = 5, chance = 70},
	{id = "terra mantle", chance = 70},
	{id = "dreaded cleaver", chance = 30}
}

monster.attacks = {
	{name = "melee",  attack = 57, skill = 55, minDamage = 0, maxDamage = -186, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -40, maxDamage = -80, range = 7, radius = 6, shootEffect = 15, effect = 9},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -95, maxDamage = -180, length = 4, spread = 3, shootEffect = 15, effect = 9},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -130, maxDamage = -180, length = 4, shootEffect = 15, effect = 9},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, target = true, duration = 15000, speedChange = -650, radius = 1, shootEffect = 15, effect = 14}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 120, maxDamage = 160, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 50},
	{type = COMBAT_ICEDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)