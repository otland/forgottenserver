local mType = Game.createMonsterType("Glitterscale")
local monster = {}
monster.description = "glitterscale"
monster.experience = 700
monster.outfit = {
	lookType = 34
}

monster.health = 1000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5973
monster.speed = 180
monster.runHealth = 300
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
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

monster.attacks = {
	{name = "melee",  attack = 40, skill = 50, minDamage = 0, maxDamage = -120, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 8, interval = 2000, minDamage = -90, maxDamage = -180, length = 8, spread = 3, effect = 7},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 12, interval = 2000, minDamage = -70, maxDamage = -120, target = true, range = 7, radius = 4, shootEffect = 4, effect = 7}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 80},
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