local mType = Game.createMonsterType("Iron Servant")
local monster = {}
monster.description = "a iron servant"
monster.experience = 210
monster.outfit = {
	lookType = 395
}

monster.health = 350
monster.maxHealth = monster.health
monster.race = "energy"
monster.corpse = 13486
monster.speed = 190
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
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
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Error. LOAD 'PROGRAM", yell = false},
	{text = "8,1", yell = false},
	{text = "Remain. Obedient.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 55, chance = 82000},
	{id = "gear wheel", chance = 4840},
	{id = "rusty armor", chance = 200},
	{id = "small health potion", chance = 1980},
	{id = "slime mould", chance = 310},
	{id = "halberd", chance = 1000}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 20, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -60, range = 7, shootEffect = 10, effect = 5},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 41, effect = 32}
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

mType:register(monster)