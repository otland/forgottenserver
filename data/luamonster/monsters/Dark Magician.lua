local mType = Game.createMonsterType("Dark Magician")
local monster = {}
monster.description = "a dark magician"
monster.experience = 185
monster.outfit = {
	lookFeet = 132,
	lookLegs = 50,
	lookHead = 116,
	lookType = 133,
	lookAddons = 2,
	lookBody = 95
}

monster.health = 325
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20367
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Feel the power of my runes!", yell = false},
	{text = "Killing you is getting expensive!", yell = false},
	{text = "My secrets are mine alone!", yell = false},
	{text = "Stand Still!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 55, chance = 75100},
	{id = "mana potion", chance = 11900},
	{id = "health potion", chance = 12000},
	{id = "strong health potion", chance = 3000},
	{id = "blank rune", chance = 10000},
	{id = "strong mana potion", chance = 2860},
	{id = "small enchanted emerald", chance = 510},
	{id = "wand of decay", chance = 280},
	{id = "reins", chance = 20}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 30, minDamage = 0, maxDamage = -40, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 20, interval = 2000, minDamage = -5, maxDamage = -40, range = 7, shootEffect = 4},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 15, interval = 2000, minDamage = -20, maxDamage = -30, range = 7, shootEffect = 5}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 2000, minDamage = 60, maxDamage = 80, shootEffect = 5, effect = 15},
	{name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 5, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = 10}
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