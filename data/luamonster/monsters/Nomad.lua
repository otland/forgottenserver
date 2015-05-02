local mType = Game.createMonsterType("Nomad")
local monster = {}
monster.description = "a nomad"
monster.experience = 60
monster.outfit = {
	lookFeet = 2,
	lookLegs = 22,
	lookHead = 114,
	lookType = 146,
	lookAddons = 2,
	lookBody = 20
}

monster.health = 160
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20466
monster.speed = 205
monster.runHealth = 15
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "We are the true sons of the desert!", yell = false},
	{text = "I will leave your remains to the vultures!", yell = false},
	{text = "We are swift as the wind of the desert!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 40, chance = 56250},
	{id = "axe", chance = 2730},
	{id = "mace", chance = 2120},
	{id = "brass armor", chance = 2350},
	{id = "steel shield", chance = 920},
	{id = "iron helmet", chance = 650},
	{id = "rope belt", chance = 6420},
	{id = "potato", maxCount = 3, chance = 4840},
	{id = "nomad parchment", chance = 200},
	{id = "dirty turban", chance = 2160}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 30, minDamage = 0, maxDamage = -80, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 1, effect = 25}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 20}
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