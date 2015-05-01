local mType = Game.createMonsterType("Pirate Marauder")
local monster = {}
monster.description = "a pirate marauder"
monster.experience = 125
monster.outfit = {
	lookType = 93
}

monster.health = 210
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20483
monster.speed = 200
monsters.runHealth = 20
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 15
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Plundeeeeer!", yell = false},
	{text = "Give up!", yell = false},
	{text = "Hiyaa!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 40, chance = 77670},
	{id = "torch", chance = 9880},
	{id = "spear", maxCount = 2, chance = 5140},
	{id = "plate shield", chance = 5000},
	{id = "chain armor", chance = 3000},
	{id = "treasure map", chance = 910},
	{id = "bandana", chance = 880},
	{id = "peg leg", chance = 520},
	{id = "hook", chance = 520},
	{id = "eye patch", chance = 530},
	{id = "rum flask", chance = 110},
	{id = "empty goldfish bowl", chance = 80},
	{id = "dice", chance = 90},
	{id = "pirate bag", chance = 430},
	{id = "compass", chance = 9720}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 60, minDamage = 0, maxDamage = -140, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -40, range = 7, shootEffect = 1}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = -10},
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)