local mType = Game.createMonsterType("Skeleton")
local monster = {}
monster.description = "a skeleton"
monster.experience = 35
monster.outfit = {
	lookType = 33
}

monster.health = 50
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 5972
monster.speed = 150
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 10, chance = 43900},
	{id = "torch", chance = 10000},
	{id = "bone", chance = 49100},
	{id = "hatchet", chance = 4850},
	{id = "mace", chance = 4850},
	{id = "brass shield", chance = 2090},
	{id = "torch", chance = 9880},
	{id = "viking helmet", chance = 7520},
	{id = "sword", chance = 1940},
	{id = "pelvis bone", chance = 9940}
}

monster.attacks = {
	{name = "melee",  attack = 16, skill = 15, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -7, maxDamage = -13, range = 1}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)