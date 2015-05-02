local mType = Game.createMonsterType("Orc Marauder")
local monster = {}
monster.description = "a orc marauder"
monster.experience = 205
monster.outfit = {
	lookType = 342
}

monster.health = 235
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 11251
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
	{text = "Grrrrrr", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 55000},
	{id = "meat", chance = 24600},
	{id = "bow", chance = 5210},
	{id = "obsidian lance", chance = 1110},
	{id = "shaggy tail", chance = 10090},
	{id = "orc tooth", chance = 3890},
	{id = "orcish axe", chance = 1320},
	{id = "orc leather", chance = 3800},
	{id = "silkweaver bow", chance = 70},
	{id = "broken crossbow", chance = 4830},
	{id = "crossbow", chance = 440}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 40, minDamage = 0, maxDamage = -100, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 50, interval = 2000, minDamage = 0, maxDamage = -100, range = 7, shootEffect = 23}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 350, shootEffect = 23, effect = 14}
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