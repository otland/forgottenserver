local mType = Game.createMonsterType("Askarak Prince")
local monster = {}
monster.description = "an askarak prince"
monster.experience = 1700
monster.outfit = {
	lookType = 419
}

monster.health = 2600
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 13957
monster.speed = 240
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
	{text = "DEATH TO THE SHABRUAK!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 86, chance = 50000},
	{id = "platinum coin", maxCount = 4, chance = 35333},
	{id = "strong mana potion", chance = 14285},
	{id = "strong health potion", chance = 14285},
	{id = "small emerald", maxCount = 5, chance = 12500},
	{id = "brown mushroom", maxCount = 5, chance = 6666},
	{id = "stealth ring", chance = 4545},
	{id = "mastermind potion", chance = 1315},
	{id = "springsprout rod", chance = 950},
	{id = "magic sulphur", chance = 740},
	{id = "terra mantle", chance = 840},
	{id = "demonic finger", chance = 420},
	{id = "giant sword", chance = 320}
}

monster.attacks = {
	{name = "melee",  attack = 83, skill = 75, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -70, maxDamage = -250, range = 7, radius = 6, shootEffect = 15, effect = 9},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -100, maxDamage = -200, length = 4, spread = 3, radius = 6, shootEffect = 15, effect = 9},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, target = true, duration = 15000, speedChange = -700, radius = 1, shootEffect = 15, effect = 14}
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