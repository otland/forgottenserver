local mType = Game.createMonsterType("Carniphila")
local monster = {}
monster.description = "a carniphila"
monster.experience = 150
monster.outfit = {
	lookType = 120
}

monster.health = 255
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 6047
monster.speed = 170
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
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 40, chance = 60000},
	{id = "dark mushroom", chance = 7692},
	{id = "carniphila seeds", chance = 4166},
	{id = "shadow herb", chance = 880},
	{id = "corncob", chance = 890},
	{id = "seeds", chance = 490},
	{id = "sling herb", maxCount = 2, chance = 446},
	{id = "carrot on a stick", chance = 170}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -60, maxDamage = -95, range = 7, shootEffect = 15, effect = 9},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = -800, range = 7, shootEffect = 15, effect = 9},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -40, maxDamage = -130, radius = 3, shootEffect = 15, effect = 21}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_ICEDAMAGE, percent = 35},
	{type = COMBAT_FIREDAMAGE, percent = -20}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)