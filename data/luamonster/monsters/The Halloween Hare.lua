local mType = Game.createMonsterType("The Halloween Hare")
local monster = {}
monster.description = "the halloween hare"
monster.experience = 0
monster.outfit = {
	lookType = 74
}

monster.health = 2000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 0
monster.speed = 150
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 95
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 2,
	staticAttack = 10
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "outfit",  chance = 6, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 4},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 4},
	{name = "outfit",  chance = 6, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 4},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 4},
	{name = "outfit",  chance = 6, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 4},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 4},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 4},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 4},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 4},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 4},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 4},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 4},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 4},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 4},
	{name = "outfit",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 4}
}

monster.defenses = {
	defense = 999,
    armor = 999,
    {name = "combat", type = COMBAT_HEALING,  chance = 50, interval = 1000, minDamage = 1500, maxDamage = 2000, radius = 3, effect = 13}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = true, combat = true},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = true},
	{type = "bleed", condition = true, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)