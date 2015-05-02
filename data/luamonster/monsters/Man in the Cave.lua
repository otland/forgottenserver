local mType = Game.createMonsterType("Man in the Cave")
local monster = {}
monster.description = "man in the cave"
monster.experience = 555
monster.outfit = {
	lookFeet = 114,
	lookLegs = 95,
	lookHead = 95,
	lookType = 128,
	lookAddons = 2,
	lookBody = 116
}

monster.health = 485
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20446
monster.speed = 210
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.summons = {
    {name = "Monk", chance = 20, interval = 2000}
}

monster.voices = {
	interval = 2000,
	chance = 5,
	{text = "THE MONKS ARE MINE", yell = true},
	{text = "I will rope you up! All of you!", yell = false},
	{text = "You have been roped up!", yell = false},
	{text = "A MIC to rule them all!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 27, chance = 100000},
	{id = "rope", chance = 20000},
	{id = "rope", chance = 6666},
	{id = "rope", chance = 3333},
	{id = "brown bread", chance = 4000},
	{id = "brown piece of cloth", chance = 2000},
	{id = "shard", chance = 4000},
	{id = "mammoth fur cape", chance = 6666},
	{id = "mercenary sword", chance = 6666},
	{id = "fur cap", chance = 1000},
	{id = "amulet of loss", subType = 1, chance = 500}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 60, minDamage = 0, maxDamage = -105, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = -10, maxDamage = -95, range = 7, shootEffect = 10},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 100, interval = 1000, minDamage = -20, maxDamage = -30, range = 1, shootEffect = 10, effect = 14}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)