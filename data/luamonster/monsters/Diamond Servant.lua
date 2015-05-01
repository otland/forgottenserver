local mType = Game.createMonsterType("Diamond Servant")
local monster = {}
monster.description = "a diamond servant"
monster.experience = 700
monster.outfit = {
	lookType = 397
}

monster.health = 1000
monster.maxHealth = monster.health
monster.race = "energy"
monster.corpse = 13485
monster.speed = 210
monster.runHealth = 100
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
	{text = "Error. LOAD 'PROGRAM',8,1", yell = false},
	{text = "Remain. Obedient.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 44000},
	{id = "gold coin", maxCount = 79, chance = 50000},
	{id = "soul orb", chance = 45000},
	{id = "life crystal", chance = 9150},
	{id = "strong mana potion", chance = 5980},
	{id = "strong health potion", chance = 5790},
	{id = "crystal pedestal", chance = 5320},
	{id = "gear wheel", chance = 5000},
	{id = "gear crystal", chance = 5000},
	{id = "might ring", subType = 20, chance = 940},
	{id = "lightning pendant", subType = 200, chance = 710},
	{id = "yellow gem", chance = 550},
	{id = "wand of cosmic energy", chance = 530},
	{id = "slime mould", chance = 480},
	{id = "mastermind potion", chance = 400},
	{id = "shockwave amulet", subType = 5, chance = 110},
	{id = "crystalline armor", chance = 20},
	{id = "bonebreaker", chance = 10}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 30, minDamage = 0, maxDamage = -100, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 20, interval = 2000, minDamage = -80, maxDamage = -120, radius = 3, effect = 49},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 15, interval = 2000, minDamage = -125, maxDamage = -170, length = 5, spread = 2, shootEffect = 5, effect = 12},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 11, effect = 32}
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
	{type = "energy", condition = true, combat = true},
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