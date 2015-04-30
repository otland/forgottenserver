local mType = Game.createMonsterType("Dharalion")
local monster = {}
monster.description = "dharalion"
monster.experience = 380
monster.outfit = {
	lookType = 203
}

monster.health = 380
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6011
monster.speed = 240
monster.maxSummons = 2

monster.changeTarget = {
    interval = 5000,
	chance = 8
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

monster.summons = {
    {name = "demon skeleton", chance = 6, interval = 1000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Feel my wrath!", yell = false},
	{text = "Noone will stop my ascension!", yell = false},
	{text = "My powers are divine!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 20, chance = 100000},
	{id = "scroll", chance = 10000},
	{id = "closed trap", chance = 5000},
	{id = "blank rune", chance = 4000},
	{id = "staff", chance = 3000},
	{id = "sandals", chance = 20000},
	{id = "melon", chance = 6666},
	{id = "sling herb", chance = 10000},
	{id = "inkwell", chance = 33333},
	{id = "life crystal", chance = 2857},
	{id = "bread", maxCount = 3, chance = 20000},
	{id = "worm", maxCount = 10, chance = 50000},
	{id = "green tunic", chance = 5000},
	{id = "arrow", maxCount = 10, chance = 20000},
	{id = "bow", chance = 10000},
	{id = "bowl", chance = 4000},
	{id = "oil lamp", chance = 2857},
	{id = "crystal necklace", chance = 1818},
	{id = "yellow gem", chance = 1333}
}

monster.attacks = {
	{name = "melee",  attack = 28, skill = 30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 15, interval = 1000, minDamage = -30, maxDamage = -60, range = 7},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 13, interval = 1000, minDamage = -70, maxDamage = -90, range = 7, shootEffect = 5, effect = 12},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 1000, minDamage = -80, maxDamage = -151, range = 7, shootEffect = 32, effect = 12},
	{name = "effect",  chance = 13, interval = 1000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 25,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 1000, minDamage = 90, maxDamage = 120, shootEffect = 15, effect = 13},
	{name = "speed",  chance = 7, interval = 1000, minDamage = 0, maxDamage = 0, duration = 10000, speedChange = 300, shootEffect = 15, effect = 14}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)