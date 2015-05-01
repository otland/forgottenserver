local mType = Game.createMonsterType("Blue Djinn")
local monster = {}
monster.description = "a blue djinn"
monster.experience = 215
monster.outfit = {
	lookType = 80
}

monster.health = 330
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6020
monster.speed = 180
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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Simsalabim", yell = false},
	{text = "Feel the power of my magic, tiny mortal!", yell = false},
	{text = "Be careful what you wish for.", yell = false},
	{text = "Wishes can come true.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 70, chance = 60000},
	{id = "gold coin", maxCount = 45, chance = 70000},
	{id = "carrot", chance = 23480},
	{id = "royal spear", maxCount = 2, chance = 4500},
	{id = "small sapphire", maxCount = 4, chance = 2560},
	{id = "small oil lamp", chance = 690},
	{id = "book", chance = 2350},
	{id = "blue piece of cloth", chance = 1920},
	{id = "mana potion", chance = 860},
	{id = "blue rose", chance = 440},
	{id = "mystic turban", chance = 70},
	{id = "dirty turban", chance = 1890}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 45, minDamage = 0, maxDamage = -110, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -45, maxDamage = -80, range = 7, shootEffect = 4},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -60, maxDamage = -105, target = true, range = 7, radius = 1, shootEffect = 32, effect = 39},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 5, effect = 39},
	{name = "outfit",  chance = 1, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 5, effect = 13},
	{name = "djinn electrify",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, range = 5, shootEffect = 5, effect = 13},
	{name = "djinn cancel invisibility",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 5, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 50},
	{type = COMBAT_DEATHDAMAGE, percent = -12},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 1},
	{type = COMBAT_FIREDAMAGE, percent = 80}
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