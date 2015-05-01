local mType = Game.createMonsterType("Orcus the Cruel")
local monster = {}
monster.description = "orcus the cruel"
monster.experience = 280
monster.outfit = {
	lookType = 59
}

monster.health = 480
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7349
monster.speed = 230
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

monster.attacks = {
	{name = "melee",  attack = 60, skill = 50, minDamage = 0, maxDamage = -180, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 40, interval = 2000, minDamage = 0, maxDamage = -70, target = true, range = 5, radius = 1, shootEffect = 9}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -20}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)