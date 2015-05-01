local mType = Game.createMonsterType("Diabolic Imp")
local monster = {}
monster.description = "a diabolic imp"
monster.experience = 1950
monster.outfit = {
	lookType = 237
}

monster.health = 2900
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 6364
monster.speed = 220
monsters.runHealth = 400
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
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Muahaha!", yell = false},
	{text = "He he he.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 97, chance = 40000},
	{id = "pitchfork", chance = 50000},
	{id = "concentrated demonic blood", maxCount = 2, chance = 25000},
	{id = "blank rune", maxCount = 2, chance = 16666},
	{id = "demonic essence", chance = 8000},
	{id = "guardian shield", chance = 8130},
	{id = "cleaver", chance = 8830},
	{id = "scimitar", chance = 5660},
	{id = "soul orb", chance = 7230},
	{id = "gold coin", maxCount = 7, chance = 3390},
	{id = "small amethyst", maxCount = 3, chance = 2250},
	{id = "stealth ring", chance = 2702},
	{id = "double axe", chance = 1994},
	{id = "death ring", chance = 120},
	{id = "magma coat", chance = 250},
	{id = "magma monocle", chance = 430},
	{id = "necrotic rod", chance = 830}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 70, minDamage = 0, maxDamage = -240, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 20, interval = 2000, minDamage = -100, maxDamage = -240, target = true, range = 7, radius = 4, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -300, maxDamage = -430, target = true, range = 7, radius = 2, shootEffect = 4, effect = 37},
	{name = "diabolic imp skill reducer",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, range = 5, shootEffect = 4, effect = 37}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 650, maxDamage = 800, shootEffect = 4, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 2000, speedChange = 800, shootEffect = 4, effect = 14},
	{name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 4, effect = 11}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)