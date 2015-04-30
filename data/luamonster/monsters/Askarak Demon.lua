local mType = Game.createMonsterType("Askarak Demon")
local monster = {}
monster.description = "an askarak demon"
monster.experience = 900
monster.outfit = {
	lookType = 420
}

monster.health = 1500
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 13815
monster.speed = 220
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
	{text = "POWER TO THE ASKARAK!", yell = false},
	{text = "GREEN WILL RULE!", yell = false},
	{text = "GREEN IS MEAN!", yell = false},
	{text = "ONLY WE ARE TRUE DEMONS!", yell = false},
	{text = "RED IS MAD", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 35, chance = 40000},
	{id = "small emerald", maxCount = 4, chance = 6250},
	{id = "strong mana potion", chance = 5263},
	{id = "strong health potion", chance = 5263},
	{id = "assassin star", maxCount = 5, chance = 4761},
	{id = "brown mushroom", maxCount = 5, chance = 3846},
	{id = "piggy bank", chance = 1052},
	{id = "energy ring", chance = 961},
	{id = "springsprout rod", chance = 512},
	{id = "mastermind potion", chance = 431},
	{id = "terra legs", chance = 123},
	{id = "magic sulphur", chance = 102}
}

monster.attacks = {
	{name = "melee",  attack = 57, skill = 40, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -20, maxDamage = -60, range = 7, radius = 6, shootEffect = 15, effect = 9},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -75, maxDamage = -140, length = 4, spread = 3, radius = 6, shootEffect = 15, effect = 9},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -130, maxDamage = -170, length = 4, radius = 6, shootEffect = 15, effect = 9},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, duration = 15000, speedChange = -600, radius = 1, shootEffect = 15, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 50},
	{type = COMBAT_ICEDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = -30}
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