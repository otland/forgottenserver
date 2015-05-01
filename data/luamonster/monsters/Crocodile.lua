local mType = Game.createMonsterType("Crocodile")
local monster = {}
monster.description = "a crocodile"
monster.experience = 40
monster.outfit = {
	lookType = 119
}

monster.health = 105
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6046
monster.speed = 190
monsters.runHealth = 10
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 10, chance = 50000},
	{id = "ham", chance = 40000},
	{id = "crocodile boots", chance = 100},
	{id = "piece of crocodile leather", chance = 20180}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 30, minDamage = 0, maxDamage = -40, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = 10}
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
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)