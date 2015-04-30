local mType = Game.createMonsterType("Fluffy")
local monster = {}
monster.description = "fluffy"
monster.experience = 3550
monster.outfit = {
	lookType = 240
}

monster.health = 4500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6332
monster.speed = 310
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
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

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Wooof!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "gold coin", maxCount = 20, chance = 100000},
	{id = "soul orb", chance = 5555},
	{id = "surprise bag", maxCount = 4, chance = 5538},
	{id = "surprise bag", chance = 1538},
	{id = "ham", maxCount = 8, chance = 50000},
	{id = "bone", chance = 25000},
	{id = "demonic essence", chance = 7200},
	{id = "knight axe", chance = 2857},
	{id = "spike sword", chance = 2500},
	{id = "concentrated demonic blood", chance = 8888}
}

monster.attacks = {
	{name = "melee",  attack = 120, skill = 98, interval = 1000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 100, interval = 1500, minDamage = -100, maxDamage = -200, length = 8, spread = 3, effect = 2},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 100, interval = 1000, minDamage = -120, maxDamage = -300, length = 8, spread = 3, effect = 21},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 25, interval = 1000, minDamage = -105, maxDamage = -235, range = 7, shootEffect = 32, effect = 21},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 1000, minDamage = -135, maxDamage = -255, range = 7, radius = 6, shootEffect = 32, effect = 2}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
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