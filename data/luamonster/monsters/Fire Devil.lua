local mType = Game.createMonsterType("Fire Devil")
local monster = {}
monster.description = "a fire devil"
monster.experience = 145
monster.outfit = {
	lookType = 40
}

monster.health = 200
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5985
monster.speed = 190
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
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Hot, eh?", yell = false},
	{text = "Hell, oh hell!", yell = false}
}

monster.loot = {
    {id = "small pitchfork", chance = 19770},
	{id = "torch", chance = 10000},
	{id = "blank rune", chance = 10950},
	{id = "cleaver", chance = 1100},
	{id = "scimitar", chance = 3000},
	{id = "torch", maxCount = 2, chance = 1420},
	{id = "double axe", chance = 1500},
	{id = "small amethyst", chance = 300},
	{id = "guardian shield", chance = 210},
	{id = "necrotic rod", chance = 460}
}

monster.attacks = {
	{name = "melee",  attack = 35, skill = 10, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 20, interval = 2000, minDamage = -60, maxDamage = -90, target = true, range = 7, radius = 4, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -20, maxDamage = -50, target = true, range = 7, radius = 2, shootEffect = 4, effect = 7}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = 5},
	{type = COMBAT_ICEDAMAGE, percent = -20}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)