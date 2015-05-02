local mType = Game.createMonsterType("Water Elemental")
local monster = {}
monster.description = "a water elemental"
monster.experience = 650
monster.outfit = {
	lookType = 286
}

monster.health = 550
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 10499
monster.speed = 220
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

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "platinum coin", chance = 10000},
	{id = "fish", chance = 20000},
	{id = "small emerald", maxCount = 2, chance = 1000},
	{id = "small diamond", chance = 1000},
	{id = "small sapphire", chance = 1000},
	{id = "giant shimmering pearl", chance = 800},
	{id = "giant shimmering pearl", chance = 800},
	{id = "strong health potion", chance = 10000},
	{id = "strong mana potion", chance = 10000},
	{id = "energy ring", chance = 950},
	{id = "life ring", chance = 930},
	{id = "rainbow trout", chance = 940},
	{id = "green perch", chance = 1050},
	{id = "leviathan's amulet", subType = 5, chance = 260}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 30, minDamage = 0, maxDamage = -160, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DROWNDAMAGE,  chance = 10, interval = 2000, minDamage = -125, maxDamage = -235, target = true, range = 7, radius = 2, effect = 2},
	{name = "icicle",  chance = 20, interval = 2000, minDamage = -88, maxDamage = -150, range = 7, effect = 2},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 2000, minDamage = -225, maxDamage = -260, radius = 5, effect = 21}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 5, interval = 2000, minDamage = 50, maxDamage = 80, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 300, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 35},
	{type = COMBAT_ENERGYDAMAGE, percent = -25},
	{type = COMBAT_DEATHDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE, percent = 50}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)