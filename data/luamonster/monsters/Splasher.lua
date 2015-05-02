local mType = Game.createMonsterType("Splasher")
local monster = {}
monster.description = "a splasher"
monster.experience = 500
monster.outfit = {
	lookType = 72
}

monster.health = 1000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6064
monster.speed = 520
monster.runHealth = 40
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Qua hah tsh!", yell = false},
	{text = "Teech tsha tshul!", yell = false},
	{text = "Quara tsha Fach!", yell = false},
	{text = "Tssssha Quara!", yell = false},
	{text = "Blubber.", yell = false},
	{text = "Blup.", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 109, skill = 60, minDamage = 0, maxDamage = -382, interval = 1000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 8, interval = 2000, minDamage = -162, maxDamage = -228, range = 7, effect = 14},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 7, interval = 2000, minDamage = -106, maxDamage = -169, length = 8, spread = 3, effect = 17},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 9, interval = 2000, minDamage = -134, maxDamage = -148, length = 8, effect = 26},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 12, interval = 2000, minDamage = -101, maxDamage = -149, radius = 3, effect = 26}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 100, maxDamage = 120, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -25},
	{type = COMBAT_EARTHDAMAGE, percent = -15}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)