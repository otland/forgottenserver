local mType = Game.createMonsterType("Dark Apprentice")
local monster = {}
monster.description = "a dark apprentice"
monster.experience = 100
monster.outfit = {
	lookFeet = 115,
	lookLegs = 95,
	lookHead = 78,
	lookType = 133,
	lookAddons = 1,
	lookBody = 57
}

monster.health = 225
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20363
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Outch!", yell = false},
	{text = "Oops, I did it again.", yell = false},
	{text = "From the spirits that I called Sir, deliver me!", yell = false},
	{text = "I must dispose of my masters enemies!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 45, chance = 7500},
	{id = "blank rune", maxCount = 3, chance = 8125},
	{id = "mana potion", chance = 2980},
	{id = "dead frog", chance = 12100},
	{id = "wand of dragonbreath", chance = 1980},
	{id = "health potion", chance = 2900},
	{id = "wand of decay", chance = 110},
	{id = "reins", chance = 10}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 30, minDamage = 0, maxDamage = -60, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -2, maxDamage = -26, range = 7, shootEffect = 4},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 10, interval = 2000, minDamage = -10, maxDamage = -20, range = 7, shootEffect = 5},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -24, range = 7, shootEffect = 10},
	{name = "outfit",  chance = 1, interval = 2000, minDamage = 0, maxDamage = 0, range = 3, shootEffect = 41},
	{name = "outfit",  chance = 1, interval = 2000, minDamage = 0, maxDamage = 0, radius = 4, shootEffect = 41, effect = 13}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 30, maxDamage = 40, shootEffect = 41, effect = 15},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 41, effect = 15}
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