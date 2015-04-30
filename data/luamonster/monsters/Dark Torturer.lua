local mType = Game.createMonsterType("Dark Torturer")
local monster = {}
monster.description = "a dark torturer"
monster.experience = 4650
monster.outfit = {
	lookType = 234
}

monster.health = 7350
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6328
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
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "You like it, don't you?", yell = false},
	{text = "IahaEhheAie!", yell = false},
	{text = "It's party time!", yell = false},
	{text = "Harrr, Harrr!", yell = false},
	{text = "The torturer is in!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 99, chance = 40000},
	{id = "ham", maxCount = 2, chance = 60000},
	{id = "platinum coin", maxCount = 8, chance = 55000},
	{id = "concentrated demonic blood", maxCount = 3, chance = 33333},
	{id = "soul orb", chance = 23000},
	{id = "great health potion", maxCount = 2, chance = 10000},
	{id = "great mana potion", maxCount = 2, chance = 14830},
	{id = "steel boots", chance = 5050},
	{id = "gold ingot", chance = 3140},
	{id = "demonic essence", chance = 8520},
	{id = "saw", chance = 5250},
	{id = "death ring", chance = 2008},
	{id = "assassin star", maxCount = 5, chance = 2222},
	{id = "cat's paw", chance = 2222},
	{id = "orichalcum pearl", maxCount = 2, chance = 2760},
	{id = "butcher's axe", chance = 850},
	{id = "jewelled backpack", chance = 1192},
	{id = "vile axe", chance = 480},
	{id = "golden legs", chance = 30}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -781, range = 7, shootEffect = 9},
	{name = "dark torturer skill reducer",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 9}
}

monster.defenses = {
	defense = 40,
    armor = 40,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 200, maxDamage = 250, shootEffect = 9, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = 90},
	{type = COMBAT_HOLYDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = 10},
	{type = COMBAT_ICEDAMAGE, percent = -10}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)