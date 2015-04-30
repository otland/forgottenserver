local mType = Game.createMonsterType("Dracola")
local monster = {}
monster.description = "a dracola"
monster.experience = 7750
monster.outfit = {
	lookType = 231
}

monster.health = 15850
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6307
monster.speed = 410
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 5
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
	staticAttack = 95
}

monster.voices = {
	interval = 2000,
	chance = 5,
	{text = "DEATH CAN'T STOP MY HUNGER!", yell = true}
}

monster.loot = {
    {id = "platinum coin", maxCount = 8, chance = 20000},
	{id = "demonic essence", maxCount = 4, chance = 1000},
	{id = "Dracola's eye", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 85, skill = 85, interval = 2000, effect = 0},
	{name = "melee",  attack = 85, skill = 85, interval = 2000, effect = 1},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 3000, minDamage = 0, maxDamage = -800, range = 7, radius = 4, shootEffect = 32, effect = 18},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -120, maxDamage = -390, target = true, range = 7, radius = 4, shootEffect = 15, effect = 21},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 23, interval = 1000, minDamage = -50, maxDamage = -180, range = 7, radius = 4, shootEffect = 15, effect = 21},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 20, interval = 4000, minDamage = -500, maxDamage = -600, length = 8, spread = 3, radius = 4, shootEffect = 15, effect = 7},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -200, maxDamage = -685, length = 8, spread = 3, radius = 4, shootEffect = 15, effect = 21},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 20, interval = 3000, minDamage = -300, maxDamage = -600, length = 8, spread = 3, radius = 4, shootEffect = 15, effect = 15},
	{name = "condition", type = CONDITION_DROWN,  chance = 20, interval = 1000, minDamage = 0, maxDamage = 0, length = 8, spread = 3, radius = 4, shootEffect = 15, effect = 3}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)