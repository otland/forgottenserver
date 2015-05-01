local mType = Game.createMonsterType("The Many")
local monster = {}
monster.description = "the many"
monster.experience = 4000
monster.outfit = {
	lookType = 121
}

monster.health = 4600
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6048
monster.speed = 260
monster.runHealth = 300
monster.maxSummons = 0

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
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "FCHHHHH", yell = false},
	{text = "HISSSS", yell = false}
}

monster.loot = {
    {id = "strong mana potion", maxCount = 2, chance = 100000},
	{id = "small sapphire", maxCount = 5, chance = 56325},
	{id = "gold ingot", maxCount = 3, chance = 47725},
	{id = "stone skin amulet", subType = 5, chance = 100000},
	{id = "egg of The Many", chance = 100000},
	{id = "sacred tree amulet", subType = 5, chance = 75500},
	{id = "medusa shield", chance = 75500},
	{id = "royal helmet", chance = 25500},
	{id = "warrior helmet", chance = 75500}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 80, minDamage = 0, maxDamage = -270, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 7, interval = 2000, minDamage = -65, maxDamage = -320, length = 8, spread = 3, effect = 46},
	{name = "speed",  chance = 8, interval = 2000, minDamage = 0, maxDamage = 0, target = true, duration = 15000, speedChange = -360, range = 7, radius = 4, shootEffect = 15, effect = 9},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 9, interval = 2000, minDamage = -100, maxDamage = -250, length = 8, spread = 3, shootEffect = 15, effect = 2},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 10, interval = 2000, minDamage = -70, maxDamage = -155, range = 7, shootEffect = 29, effect = 44}
}

monster.defenses = {
	defense = 35,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 30, interval = 2000, minDamage = 420, maxDamage = 500, shootEffect = 29, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 30},
	{type = COMBAT_ICEDAMAGE, percent = 50}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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