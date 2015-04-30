local mType = Game.createMonsterType("Dwarf Geomancer")
local monster = {}
monster.description = "a dwarf geomancer"
monster.experience = 265
monster.outfit = {
	lookType = 66
}

monster.health = 380
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6015
monster.speed = 190
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
	targetDistance = 4,
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Hail Durin!", yell = false},
	{text = "Earth is the strongest element.", yell = false},
	{text = "Dust to dust.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 35, chance = 50000},
	{id = "blank rune", chance = 33000},
	{id = "white mushroom", maxCount = 2, chance = 60000},
	{id = "leather boots", chance = 47500},
	{id = "pear", chance = 25000},
	{id = "magic light wand", chance = 14000},
	{id = "spellbook", chance = 360},
	{id = "clerical mace", chance = 1120},
	{id = "dwarven ring", chance = 530},
	{id = "small sapphire", chance = 710},
	{id = "iron ore", chance = 120},
	{id = "terra boots", chance = 470},
	{id = "geomancer's staff", chance = 7000},
	{id = "geomancer's robe", chance = 8000}
}

monster.attacks = {
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -50, maxDamage = -110, range = 7, shootEffect = 39, effect = 9},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = -50, maxDamage = -80, range = 7, shootEffect = 39, effect = 9}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 40, interval = 2000, minDamage = 75, maxDamage = 125, shootEffect = 39, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 60},
	{type = COMBAT_HOLYDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = -1},
	{type = COMBAT_ICEDAMAGE, percent = -5}
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)