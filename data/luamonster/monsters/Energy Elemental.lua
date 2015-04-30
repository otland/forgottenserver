local mType = Game.createMonsterType("Energy Elemental")
local monster = {}
monster.description = "an energy elemental"
monster.experience = 550
monster.outfit = {
	lookType = 293
}

monster.health = 500
monster.maxHealth = monster.health
monster.race = "energy"
monster.corpse = 8966
monster.speed = 200
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
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 85
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 70, chance = 50000},
	{id = "mana potion", chance = 11711},
	{id = "dead cat", maxCount = 10, chance = 10000},
	{id = "throwing star", maxCount = 5, chance = 9900},
	{id = "strong mana potion", chance = 7692},
	{id = "crystal sword", chance = 5882},
	{id = "small amethyst", maxCount = 2, chance = 5000},
	{id = "obsidian lance", chance = 3571},
	{id = "crystal ring", chance = 2000},
	{id = "silver amulet", subType = 200, chance = 1020},
	{id = "energy ring", chance = 892},
	{id = "wand of cosmic energy", chance = 636},
	{id = "guardian shield", chance = 243}
}

monster.attacks = {
	{name = "melee",  attack = 70, skill = 40, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 10, interval = 2000, minDamage = -125, maxDamage = -252, target = true, range = 7, radius = 2, shootEffect = 5, effect = 12},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 15, interval = 2000, minDamage = -100, maxDamage = -130, range = 7, radius = 2, shootEffect = 36, effect = 12},
	{name = "energy elemental electrify",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, radius = 2, shootEffect = 36, effect = 12}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 90, maxDamage = 150, radius = 2, shootEffect = 36, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = -15},
	{type = COMBAT_DEATHDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE, percent = 10}
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
	{type = "energy", condition = true, combat = true},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)