local mType = Game.createMonsterType("Fallen Mooh'tah Master Ghar")
local monster = {}
monster.description = "fallen mooh'tah master ghar"
monster.experience = 4400
monster.outfit = {
	lookType = 29
}

monster.health = 8000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7349
monster.speed = 190
monster.maxSummons = 0

monster.changeTarget = {
    interval = 0,
	chance = 0
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
	staticAttack = 95
}

monster.voices = {
	interval = 2000,
	chance = 5,
	{text = "I will finish you!", yell = false},
	{text = "You are no match for a master of the Mooh'Tha!", yell = false},
	{text = "I might be fallen but you will fall before me!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 150, skill = 60, minDamage = 0, maxDamage = -525, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 30, interval = 6000, minDamage = -80, maxDamage = -300, length = 8, spread = 3, effect = 7},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 45, interval = 3000, minDamage = -80, maxDamage = -400, target = true, radius = 5, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 30, interval = 4000, minDamage = -80, maxDamage = -270, shootEffect = 11, effect = 18},
	{name = "condition", type = CONDITION_POISON,  chance = 40, interval = 4500, minDamage = -10, maxDamage = -200, range = 10, shootEffect = 15, effect = 18},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 30, interval = 5000, minDamage = -60, maxDamage = -300, length = 8, spread = 3, shootEffect = 15, effect = 21}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)