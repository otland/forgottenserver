local mType = Game.createMonsterType("Massive Earth Elemental")
local monster = {}
monster.description = "a massive earth elemental"
monster.experience = 950
monster.outfit = {
	lookType = 285
}

monster.health = 1330
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 8933
monster.speed = 240
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
    {id = "gold coin", maxCount = 100, chance = 32000},
	{id = "gold coin", maxCount = 100, chance = 32000},
	{id = "gold coin", maxCount = 41, chance = 32000},
	{id = "small stone", maxCount = 10, chance = 25280},
	{id = "protection amulet", subType = 250, chance = 1580},
	{id = "rusty armor", chance = 3300},
	{id = "dwarven ring", chance = 2790},
	{id = "small topaz", maxCount = 2, chance = 5280},
	{id = "stone skin amulet", subType = 5, chance = 980},
	{id = "terra amulet", subType = 200, chance = 500},
	{id = "diamond sceptre", chance = 150},
	{id = "lump of earth", chance = 40680},
	{id = "clay lump", chance = 480},
	{id = "coal", chance = 430}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 45, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -99, maxDamage = -145, range = 7, shootEffect = 39, effect = 9},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = -95, maxDamage = -169, target = true, range = 7, radius = 2, shootEffect = 12, effect = 3},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 2000, minDamage = -300, maxDamage = -320, length = 6, radius = 2, shootEffect = 12, effect = 51},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -200, maxDamage = -220, radius = 5, shootEffect = 15, effect = 21},
	{name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = -330, range = 5, radius = 5, shootEffect = 15, effect = 46}
}

monster.defenses = {
	defense = 35,
    armor = 35,
    {name = "combat", type = COMBAT_HEALING,  chance = 5, interval = 2000, minDamage = 150, maxDamage = 180, radius = 5, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 30},
	{type = COMBAT_ENERGYDAMAGE, percent = 90},
	{type = COMBAT_FIREDAMAGE, percent = -15},
	{type = COMBAT_DEATHDAMAGE, percent = 45},
	{type = COMBAT_HOLYDAMAGE, percent = 50}
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