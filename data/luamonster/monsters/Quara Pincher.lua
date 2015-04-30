local mType = Game.createMonsterType("Quara Pincher")
local monster = {}
monster.description = "a quara pincher"
monster.experience = 1200
monster.outfit = {
	lookType = 77
}

monster.health = 1800
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6063
monster.speed = 210
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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Clank clank!", yell = false},
	{text = "Clap!", yell = false},
	{text = "Crrrk! Crrrk!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 49, chance = 50000},
	{id = "platinum coin", chance = 40000},
	{id = "quara pincers", chance = 14285},
	{id = "small ruby", maxCount = 2, chance = 7761},
	{id = "halberd", chance = 6861},
	{id = "shrimp", maxCount = 5, chance = 5245},
	{id = "fish fin", chance = 1600},
	{id = "warrior helmet", chance = 1350},
	{id = "great health potion", chance = 10630},
	{id = "crown armor", chance = 350},
	{id = "glacier robe", chance = 140},
	{id = "giant shrimp", chance = 80}
}

monster.attacks = {
	{name = "melee",  attack = 72, skill = 85, interval = 2000, effect = 0},
	{name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, duration = 3000, speedChange = -600, range = 1, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = -25},
	{type = COMBAT_EARTHDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)