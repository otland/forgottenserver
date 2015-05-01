local mType = Game.createMonsterType("Golden Servant")
local monster = {}
monster.description = "a golden servant"
monster.experience = 450
monster.outfit = {
	lookType = 396
}

monster.health = 550
monster.maxHealth = monster.health
monster.race = "energy"
monster.corpse = 13489
monster.speed = 205
monsters.runHealth = 50
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
    {id = "gold coin", maxCount = 100, chance = 40000},
	{id = "gold coin", maxCount = 40, chance = 45000},
	{id = "gear wheel", chance = 940},
	{id = "golden can of oil", chance = 40},
	{id = "health potion", chance = 4930},
	{id = "golden armor", chance = 10},
	{id = "mana potion", chance = 4950},
	{id = "gold ring", chance = 10},
	{id = "slime mould", chance = 340},
	{id = "spellbook of enlightenment", chance = 520},
	{id = "stealth ring", chance = 450},
	{id = "halberd", chance = 3003},
	{id = "green mushroom", chance = 1450}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 30, minDamage = 0, maxDamage = -100, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 15, interval = 2000, minDamage = -60, maxDamage = -100, target = true, radius = 4, shootEffect = 5, effect = 49},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 15, interval = 2000, minDamage = -80, maxDamage = -110, target = true, radius = 4, shootEffect = 5, effect = 12},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 10, interval = 2000, minDamage = -90, maxDamage = -150, length = 5, spread = 2, shootEffect = 5, effect = 49}
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
	{type = "holy", condition = true, combat = true},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)