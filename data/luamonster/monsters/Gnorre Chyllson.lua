local mType = Game.createMonsterType("Gnorre Chyllson")
local monster = {}
monster.description = "gnorre chyllson"
monster.experience = 4000
monster.outfit = {
	lookFeet = 85,
	lookLegs = 11,
	lookHead = 11,
	lookType = 251,
	lookAddons = 1,
	lookBody = 9
}

monster.health = 7100
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7349
monster.speed = 370
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
	{text = "I am like the merciless northwind.", yell = false},
	{text = "Snow will be your death shroud.", yell = false},
	{text = "Feel the wrath of father chyll!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 130, skill = 60, minDamage = 0, maxDamage = -455, interval = 2000, effect = 0},
	{name = "melee",  attack = 0, skill = 0, minDamage = -500, maxDamage = -800, interval = 3000, effect = 10},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 15, interval = 1000, minDamage = -170, maxDamage = -200, range = 7, shootEffect = 13, effect = 10}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -15},
	{type = COMBAT_DEATHDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)