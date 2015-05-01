local mType = Game.createMonsterType("Green Djinn")
local monster = {}
monster.description = "a green djinn"
monster.experience = 215
monster.outfit = {
	lookType = 51
}

monster.health = 330
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6016
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
	{text = "I grant you a deathwish!", yell = false},
	{text = "Muhahahaha!", yell = false},
	{text = "I wish you a merry trip to hell!", yell = false},
	{text = "Good wishes are for fairytales", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 70, chance = 41000},
	{id = "gold coin", maxCount = 45, chance = 51000},
	{id = "cheese", chance = 23500},
	{id = "grave flower", chance = 1000},
	{id = "royal spear", maxCount = 2, chance = 4870},
	{id = "oil lamp", chance = 830},
	{id = "book", chance = 2280},
	{id = "small emerald", maxCount = 4, chance = 2960},
	{id = "mana potion", chance = 490},
	{id = "green piece of cloth", chance = 2000},
	{id = "mystic turban", chance = 140},
	{id = "dirty turban", chance = 2210}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 45, minDamage = 0, maxDamage = -110, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -45, maxDamage = -80, range = 7, shootEffect = 4},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -50, maxDamage = -105, target = true, range = 7, radius = 1, shootEffect = 32, effect = 39},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 5, effect = 39},
	{name = "outfit",  chance = 1, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 5, effect = 13},
	{name = "djinn electrify",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, range = 5, shootEffect = 5, effect = 13},
	{name = "djinn cancel invisibility",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 5, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 20},
	{type = COMBAT_ENERGYDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = 80},
	{type = COMBAT_HOLYDAMAGE, percent = -13},
	{type = COMBAT_DEATHDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = -10}
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