local mType = Game.createMonsterType("Infernalist")
local monster = {}
monster.description = "an infernalist"
monster.experience = 4000
monster.outfit = {
	lookFeet = 115,
	lookLegs = 94,
	lookHead = 78,
	lookType = 130,
	lookAddons = 2,
	lookBody = 76
}

monster.health = 3650
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20427
monster.speed = 220
monsters.runHealth = 900
monster.maxSummons = 1

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
	staticAttack = 95
}

monster.summons = {
    {name = "fire elemental", chance = 20, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Nothing will remain but your scorched bones!", yell = false},
	{text = "Some like it hot!", yell = false},
	{text = "It's cooking time!", yell = false},
	{text = "Feel the heat of battle!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 56500},
	{id = "gold coin", maxCount = 47, chance = 40000},
	{id = "great mana potion", chance = 19700},
	{id = "great health potion", chance = 1900},
	{id = "raspberry", maxCount = 5, chance = 8500},
	{id = "skull staff", chance = 6500},
	{id = "small enchanted ruby", chance = 4250},
	{id = "red piece of cloth", chance = 1420},
	{id = "energy ring", chance = 1800},
	{id = "spellbook of mind control", chance = 370},
	{id = "magic sulphur", chance = 600},
	{id = "red tome", chance = 300},
	{id = "gold ingot", chance = 70},
	{id = "black skull", chance = 820},
	{id = "magma boots", chance = 300},
	{id = "crystal of power", chance = 220},
	{id = "piggy bank", chance = 220},
	{id = "royal tapestry", chance = 520}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 30, minDamage = 0, maxDamage = -100, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 40, interval = 2000, minDamage = -65, maxDamage = -180, range = 7, shootEffect = 4, effect = 16},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 20, interval = 2000, minDamage = -90, maxDamage = -180, target = true, range = 7, radius = 3, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 20, interval = 2000, minDamage = -53, maxDamage = -120, target = true, range = 7, radius = 3, shootEffect = 36, effect = 11},
	{name = "firefield",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 3, shootEffect = 4, effect = 11},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -150, maxDamage = -250, length = 8, shootEffect = 4, effect = 37},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 5, interval = 2000, minDamage = -100, maxDamage = -150, radius = 2, shootEffect = 4, effect = 5}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 60, maxDamage = 230, shootEffect = 4, effect = 13},
	{name = "invisible",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 4, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 95},
	{type = COMBAT_HOLYDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = 5},
	{type = COMBAT_ICEDAMAGE, percent = -5}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)