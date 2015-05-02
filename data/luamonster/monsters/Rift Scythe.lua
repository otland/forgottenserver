local mType = Game.createMonsterType("Rift Scythe")
local monster = {}
monster.description = "a rift scythe"
monster.experience = 2000
monster.outfit = {
	lookType = 300
}

monster.health = 3600
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 0
monster.speed = 370
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 85
}

monster.attacks = {
	{name = "melee",  attack = 135, skill = 135, minDamage = 0, maxDamage = -979, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 60, interval = 2000, minDamage = 0, maxDamage = -200, range = 7, shootEffect = 32, effect = 18},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 50, interval = 2000, minDamage = 0, maxDamage = -600, length = 7, shootEffect = 32, effect = 1},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 40, interval = 2000, minDamage = 0, maxDamage = -395, radius = 4, shootEffect = 32, effect = 1},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 60, interval = 3000, minDamage = 0, maxDamage = -300, length = 7, spread = 3, shootEffect = 32, effect = 5}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 1000, minDamage = 100, maxDamage = 195, shootEffect = 32, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -20},
	{type = COMBAT_HOLYDAMAGE, percent = -20},
	{type = COMBAT_DEATHDAMAGE, percent = 60},
	{type = COMBAT_ICEDAMAGE, percent = 10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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

mType:register(monster)