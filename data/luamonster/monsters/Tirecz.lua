local mType = Game.createMonsterType("Tirecz")
local monster = {}
monster.description = "tirecz"
monster.experience = 6000
monster.outfit = {
	lookType = 334
}

monster.health = 25000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 11103
monster.speed = 220
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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Hissss!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 72, minDamage = 0, maxDamage = -410, interval = 2000, effect = 0},
	{name = "invisible",  chance = 25, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 1, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 25, interval = 2000, minDamage = -120, maxDamage = -460, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -290, radius = 3, shootEffect = 4, effect = 4},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 30, interval = 3000, minDamage = -80, maxDamage = -345, length = 8, shootEffect = 4, effect = 12},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = -200, maxDamage = -370, radius = 7, shootEffect = 4, effect = 1}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = 30},
	{type = COMBAT_FIREDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE, percent = 30},
	{type = COMBAT_DEATHDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = 30}
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

monster.events = {
    "tirecz"
}

mType:register(monster)