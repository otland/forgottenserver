local mType = Game.createMonsterType("Azerus")
local monster = {}
monster.description = "azerus"
monster.experience = 6000
monster.outfit = {
	lookFeet = 81,
	lookLegs = 21,
	lookHead = 19,
	lookType = 309,
	lookBody = 96
}

monster.health = 7500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 9738
monster.speed = 320
monster.maxSummons = 10

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
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 85
}

monster.summons = {
    {name = "Rift Worm", chance = 30, interval = 3000}
}

monster.voices = {
	interval = 5000,
	chance = 12,
	{text = "The ultimate will finally consume this unworthy existence!", yell = false},
	{text = "My masters and I will tear down barriers and join the ultimate in its realm!", yell = false},
	{text = "The power of the Yalahari will all be mine!", yell = false},
	{text = "He who has returned from beyond has taught me secrets you can't even grasp!", yell = false},
	{text = "You can't hope to penetrate my shields!", yell = false},
	{text = "Do you really think you could beat me?", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -900, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -2000, range = 7, shootEffect = 5, effect = 3},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -800, target = true, range = 7, radius = 7, shootEffect = 4, effect = 41},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -524, range = 7, shootEffect = 32, effect = 18},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 1000, minDamage = -100, maxDamage = -500, length = 8, shootEffect = 32, effect = 33}
}

monster.defenses = {
	defense = 65,
    armor = 40,
    {name = "combat", type = COMBAT_HEALING,  chance = 11, interval = 1000, minDamage = 400, maxDamage = 900, shootEffect = 32, effect = 15}
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
	{type = "energy", condition = false, combat = false},
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