local mType = Game.createMonsterType("Incineron")
local monster = {}
monster.description = "incineron"
monster.experience = 3500
monster.outfit = {
	lookType = 243
}

monster.health = 7000
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 6324
monster.speed = 260
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 9
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

monster.attacks = {
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 100, interval = 2000, minDamage = 0, maxDamage = -1000, length = 8, spread = 1, effect = 7},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 100, interval = 2000, minDamage = 0, maxDamage = -395, range = 7, radius = 7, shootEffect = 4, effect = 7}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20}
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
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)