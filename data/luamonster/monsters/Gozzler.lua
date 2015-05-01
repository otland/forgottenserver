local mType = Game.createMonsterType("Gozzler")
local monster = {}
monster.description = "a gozzler"
monster.experience = 180
monster.outfit = {
	lookType = 313
}

monster.health = 240
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 9938
monster.speed = 190
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Huhuhuhuuu!", yell = false},
	{text = "Let the fun begin!", yell = false},
	{text = "Yihahaha!", yell = false},
	{text = "I'll bite you! Nyehehehehe!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 70, chance = 52500},
	{id = "battle axe", chance = 3100},
	{id = "brown flask", chance = 8750},
	{id = "clerical mace", chance = 900},
	{id = "morning star", chance = 5000},
	{id = "plate shield", chance = 10000},
	{id = "sabre", chance = 8250},
	{id = "serpent sword", chance = 250},
	{id = "dwarven ring", chance = 190},
	{id = "small sapphire", chance = 360}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 45, minDamage = 0, maxDamage = -110, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -32, maxDamage = -135, range = 1, effect = 1}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 30, maxDamage = 50, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 210, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE, percent = 50}
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