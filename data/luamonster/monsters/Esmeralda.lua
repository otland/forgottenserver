local mType = Game.createMonsterType("Esmeralda")
local monster = {}
monster.description = "esmeralda"
monster.experience = 600
monster.outfit = {
	lookType = 305
}

monster.health = 800
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 9871
monster.speed = 245
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 2000,
	chance = 10,
	{text = "Fcccccchhhhhh", yell = false}
}

monster.loot = {
    {id = "platinum coin", maxCount = 4, chance = 68125},
	{id = "ring of healing", chance = 100000},
	{id = "tower shield", chance = 41400},
	{id = "halberd", chance = 30050},
	{id = "gold coin", maxCount = 100, chance = 58775},
	{id = "gold coin", maxCount = 19, chance = 58775},
	{id = "mutated rat tail", chance = 100000},
	{id = "epee", chance = 14150},
	{id = "knight armor", chance = 45950},
	{id = "stone herb", chance = 9600},
	{id = "terra mantle", chance = 2775}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 60, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 30, interval = 2000, minDamage = 0, maxDamage = -110, range = 7, shootEffect = 15},
	{name = "condition", type = CONDITION_POISON,  chance = 22, interval = 2000, minDamage = -100, maxDamage = -100, length = 8, shootEffect = 15, effect = 46},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 2000, minDamage = -80, maxDamage = -80, radius = 3, shootEffect = 15, effect = 17},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 25, interval = 2000, minDamage = 0, maxDamage = -110, range = 7, radius = 3, shootEffect = 15, effect = 1}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)