local mType = Game.createMonsterType("Crypt Defiler")
local monster = {}
monster.description = "a crypt defiler"
monster.experience = 70
monster.outfit = {
	lookFeet = 96,
	lookLegs = 61,
	lookHead = 115,
	lookType = 146,
	lookAddons = 3,
	lookBody = 115
}

monster.health = 180
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20359
monster.speed = 205
monster.runHealth = 15
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "I smell gold!", yell = false},
	{text = "You'll make a fine fake-mummy to be sold!", yell = false},
	{text = "Untold riches are awaiting me!", yell = false},
	{text = "I don't like competition", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 39, chance = 50900},
	{id = "axe", chance = 2730},
	{id = "mace", chance = 1800},
	{id = "brass armor", chance = 2240},
	{id = "steel shield", chance = 450},
	{id = "iron helmet", chance = 450},
	{id = "rope belt", chance = 4040},
	{id = "potato", maxCount = 3, chance = 4840},
	{id = "nomad parchment", chance = 220},
	{id = "dirty turban", chance = 1570}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 35, minDamage = 0, maxDamage = -90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -40, target = true, range = 7, radius = 1, shootEffect = 8}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_FIREDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
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