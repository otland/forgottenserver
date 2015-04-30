local mType = Game.createMonsterType("The Imperor")
local monster = {}
monster.description = "the imperor"
monster.experience = 8000
monster.outfit = {
	lookType = 237
}

monster.health = 15000
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 8635
monster.speed = 330
monster.maxSummons = 2

monster.changeTarget = {
    interval = 5000,
	chance = 5
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 4,
	staticAttack = 90
}

monster.summons = {
    {name = "diabolic imp", chance = 30, interval = 5500}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Muahaha!", yell = false},
	{text = "He he he!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 79, chance = 100000},
	{id = "torch", chance = 15000},
	{id = "concentrated demonic blood", chance = 2222},
	{id = "blank rune", chance = 18000},
	{id = "the Imperor's trident", chance = 100000},
	{id = "death ring", chance = 999},
	{id = "brass armor", chance = 6577},
	{id = "pitchfork", chance = 20000},
	{id = "infernal bolt", maxCount = 5, chance = 5000},
	{id = "brass armor", chance = 6577},
	{id = "necrotic rod", chance = 833},
	{id = "chain armor", chance = 7887},
	{id = "soul orb", chance = 5666},
	{id = "magma coat", chance = 2000},
	{id = "magma monocle", chance = 2000},
	{id = "magma boots", chance = 2000},
	{id = "demonic essence", chance = 7777},
	{id = "small amethyst", chance = 8888},
	{id = "stealth ring", chance = 4555}
}

monster.attacks = {
	{name = "melee",  attack = 110, skill = 110, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 13, interval = 1000, minDamage = -50, maxDamage = -800, range = 1, shootEffect = 4, effect = 37},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 20, interval = 2000, minDamage = -50, maxDamage = -800, target = true, range = 7, radius = 6, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 30, interval = 2000, minDamage = -90, maxDamage = -350, length = 4, spread = 2, radius = 6, shootEffect = 4, effect = 7}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 50, interval = 2000, minDamage = 175, maxDamage = 505, radius = 6, shootEffect = 4, effect = 13},
	{name = "speed",  chance = 12, interval = 2000, minDamage = 0, maxDamage = 0, duration = 4000, speedChange = 380, radius = 6, shootEffect = 4, effect = 14},
	{name = "invisible",  chance = 20, interval = 3000, minDamage = 0, maxDamage = 0, radius = 6, shootEffect = 4, effect = 14}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)