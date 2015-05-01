local mType = Game.createMonsterType("Apprentice Sheng")
local monster = {}
monster.description = "apprentice sheng"
monster.experience = 100
monster.outfit = {
	lookType = 23
}

monster.health = 95
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5981
monster.speed = 170
monster.maxSummons = 2

monster.changeTarget = {
    interval = 5000,
	chance = 8
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.summons = {
    {name = "Hyena", chance = 100, interval = 10500}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "I will protect the secrets of my master!", yell = false},
	{text = "This isle will become ours alone!", yell = false},
	{text = "Kaplar!", yell = false}
}

monster.loot = {
    {id = "worm", maxCount = 10, chance = 50000},
	{id = "gold coin", maxCount = 20, chance = 100000},
	{id = "torch", chance = 10000},
	{id = "leather helmet", chance = 8000},
	{id = "leather legs", chance = 8000},
	{id = "minotaur leather", chance = 100000},
	{id = "combat knife", chance = 10000},
	{id = "the carrot of doom", maxCount = 5, chance = 20000},
	{id = "dead snake", chance = 10000},
	{id = "heavy magic missile rune", subType = 10, chance = 2857},
	{id = "taurus mace", chance = 1000}
}

monster.attacks = {
	{name = "melee",  attack = 10, skill = 15, minDamage = 0, maxDamage = -13, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 14, interval = 1000, minDamage = -15, maxDamage = -25, range = 7, shootEffect = 36, effect = 38},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -15, maxDamage = -45, target = true, range = 7, radius = 1, shootEffect = 4, effect = 7},
	{name = "energyfield",  chance = 8, interval = 1000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 1, shootEffect = 5, effect = 7},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 100, interval = 10500, minDamage = 0, maxDamage = 0, radius = 3, shootEffect = 5, effect = 14}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = true, combat = true},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)