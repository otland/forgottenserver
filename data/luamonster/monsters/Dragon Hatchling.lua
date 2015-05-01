local mType = Game.createMonsterType("Dragon Hatchling")
local monster = {}
monster.description = "a dragon hatchling"
monster.experience = 185
monster.outfit = {
	lookType = 271
}

monster.health = 380
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7621
monster.speed = 170
monster.runHealth = 20
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Fchu?", yell = false},
	{text = "Rooawwrr", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 55, chance = 67500},
	{id = "dragon ham", chance = 61000},
	{id = "health potion", chance = 400},
	{id = "dragon's tail", chance = 4300}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 45, minDamage = 0, maxDamage = -55, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -60, maxDamage = -90, length = 5, spread = 2, effect = 7},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -30, maxDamage = -55, target = true, range = 7, radius = 4, shootEffect = 4, effect = 7}
}

monster.defenses = {
	defense = 10,
    armor = 10,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 8, maxDamage = 33, shootEffect = 4, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 75},
	{type = COMBAT_ICEDAMAGE, percent = -10}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)