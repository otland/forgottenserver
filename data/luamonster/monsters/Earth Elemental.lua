local mType = Game.createMonsterType("Earth Elemental")
local monster = {}
monster.description = "an earth elemental"
monster.experience = 450
monster.outfit = {
	lookType = 301
}

monster.health = 650
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 8933
monster.speed = 220
monsters.runHealth = 80
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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 80
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 43000},
	{id = "gold coin", maxCount = 30, chance = 50000},
	{id = "lump of earth", chance = 20460},
	{id = "earth arrow", maxCount = 30, chance = 20160},
	{id = "blank rune", chance = 10000},
	{id = "strong mana potion", chance = 1910},
	{id = "clay lump", chance = 570},
	{id = "coal", chance = 470},
	{id = "rusty armor", chance = 350},
	{id = "small stone", maxCount = 10, chance = 10000},
	{id = "small topaz", chance = 620}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 45, minDamage = 0, maxDamage = -110, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -72, maxDamage = -105, range = 7, shootEffect = 39, effect = 9},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -100, target = true, range = 7, radius = 2, shootEffect = 12, effect = 3},
	{name = "condition", type = CONDITION_POISON,  chance = 20, interval = 2000, minDamage = -200, maxDamage = -260, length = 6, shootEffect = 12, effect = 51},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -100, maxDamage = -140, radius = 5, shootEffect = 15, effect = 21},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = -330, range = 5, shootEffect = 15, effect = 46}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 5, interval = 2000, minDamage = 60, maxDamage = 80, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 50},
	{type = COMBAT_DEATHDAMAGE, percent = 50},
	{type = COMBAT_ICEDAMAGE, percent = 85},
	{type = COMBAT_HOLYDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = -25}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)