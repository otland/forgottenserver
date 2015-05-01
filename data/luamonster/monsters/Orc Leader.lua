local mType = Game.createMonsterType("Orc Leader")
local monster = {}
monster.description = "a orc leader"
monster.experience = 270
monster.outfit = {
	lookType = 59
}

monster.health = 450
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6001
monster.speed = 220
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
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

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Ulderek futgyr human!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 35, chance = 28000},
	{id = "fish", chance = 29400},
	{id = "throwing knife", maxCount = 4, chance = 9950},
	{id = "scimitar", chance = 1860},
	{id = "brass legs", chance = 3100},
	{id = "brown mushroom", chance = 9650},
	{id = "plate armor", chance = 1650},
	{id = "plate shield", chance = 1650},
	{id = "royal spear", chance = 2400},
	{id = "sword ring", chance = 3920},
	{id = "plate legs", chance = 440},
	{id = "broadsword", chance = 610},
	{id = "longsword", chance = 2800},
	{id = "health potion", chance = 550},
	{id = "warrior helmet", chance = 180},
	{id = "orc leather", chance = 19510},
	{id = "orc tooth", chance = 1030},
	{id = "skull belt", chance = 2008}
}

monster.attacks = {
	{name = "melee",  attack = 74, skill = 40, minDamage = 0, maxDamage = -185, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -70, range = 7, shootEffect = 9}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = 20}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
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