local mType = Game.createMonsterType("Spite of the Emperor")
local monster = {}
monster.description = "a spite of the emperor"
monster.experience = 5600
monster.outfit = {
	lookType = 351
}

monster.health = 8000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 0
monster.speed = 410
monster.maxSummons = 2

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
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.summons = {
    {name = "Draken Warmaster", chance = 10, interval = 2000}
}

monster.attacks = {
	{name = "melee",  attack = 115, skill = 55, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 17, interval = 3000, minDamage = -150, maxDamage = -250, length = 8, spread = 3, effect = 2},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 3000, minDamage = 0, maxDamage = -500, range = 7, shootEffect = 32, effect = 18},
	{name = "speed",  chance = 10, interval = 1000, minDamage = 0, maxDamage = 0, duration = 20000, speedChange = -600, range = 7, shootEffect = 32, effect = 14},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 21, interval = 2000, minDamage = -200, maxDamage = -450, radius = 6, shootEffect = 32, effect = 3}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE, percent = -15},
	{type = COMBAT_ICEDAMAGE, percent = 50}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)