local mType = Game.createMonsterType("Countess Sorrow")
local monster = {}
monster.description = "a countess sorrow"
monster.experience = 5150
monster.outfit = {
	lookHead = 20,
	lookType = 241
}

monster.health = 7200
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6344
monster.speed = 250
monster.maxSummons = 0

monster.changeTarget = {
    interval = 60000,
	chance = 0
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

monster.loot = {
    {id = "Countess Sorrow's frozen tear", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 90, skill = 70, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 1000, minDamage = -150, maxDamage = -350, target = true, range = 7, radius = 4, shootEffect = 15, effect = 21},
	{name = "poisonfield",  chance = 17, interval = 1000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 3, shootEffect = 15, effect = 21},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 25, interval = 1000, minDamage = -100, maxDamage = -400, range = 7, radius = 3, shootEffect = 15, effect = 14}
}

monster.defenses = {
	defense = 20,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 50, interval = 1000, minDamage = 409, maxDamage = 812, radius = 3, shootEffect = 15, effect = 13}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = true},
	{type = "bleed", condition = true, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)