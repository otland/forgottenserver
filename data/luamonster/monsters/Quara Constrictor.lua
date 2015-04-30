local mType = Game.createMonsterType("Quara Constrictor")
local monster = {}
monster.description = "a quara constrictor"
monster.experience = 250
monster.outfit = {
	lookType = 46
}

monster.health = 450
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6065
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
	{text = "Gaaahhh!", yell = false},
	{text = "Gluh! Gluh!", yell = false},
	{text = "Tssss!", yell = false},
	{text = "Boohaa!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 87500},
	{id = "quara tentacle", chance = 14520},
	{id = "longsword", chance = 7761},
	{id = "shrimp", maxCount = 5, chance = 5000},
	{id = "brass armor", chance = 5000},
	{id = "fish fin", chance = 430},
	{id = "small amethyst", chance = 2860}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 50, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -50, maxDamage = -90, radius = 3, effect = 10},
	{name = "quara constrictor freeze",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 10},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 10, interval = 2000, minDamage = -40, maxDamage = -70, range = 7, radius = 4, effect = 44},
	{name = "quara constrictor electrify",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 1, radius = 4, effect = 44}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -25},
	{type = COMBAT_EARTHDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)