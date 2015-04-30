local mType = Game.createMonsterType("Clay Guardian")
local monster = {}
monster.description = "a clay guardian"
monster.experience = 400
monster.outfit = {
	lookType = 333
}

monster.health = 625
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 13972
monster.speed = 210
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 60
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 63, chance = 50000},
	{id = "lump of earth", chance = 25000},
	{id = "blank rune", chance = 25000},
	{id = "small stone", maxCount = 10, chance = 10000},
	{id = "earth arrow", maxCount = 8, chance = 5555},
	{id = "clay lump", chance = 1100},
	{id = "small topaz", chance = 320}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 40, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -30, maxDamage = -150, range = 7, shootEffect = 39, effect = 9},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 50, interval = 2000, minDamage = -20, maxDamage = -30, radius = 3, shootEffect = 39, effect = 3}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 40, maxDamage = 130, radius = 3, shootEffect = 39, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 40},
	{type = COMBAT_ICEDAMAGE, percent = 50},
	{type = COMBAT_DEATHDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = -5}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)