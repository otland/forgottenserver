local mType = Game.createMonsterType("Orc Shaman")
local monster = {}
monster.description = "a orc shaman"
monster.experience = 110
monster.outfit = {
	lookType = 6
}

monster.health = 115
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5978
monster.speed = 180
monster.maxSummons = 4

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
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.summons = {
    {name = "Snake", chance = 20, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Huumans stinkk!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 5, chance = 90000},
	{id = "corncob", maxCount = 2, chance = 10600},
	{id = "chain armor", chance = 8750},
	{id = "spear", chance = 4850},
	{id = "wand of decay", chance = 1000},
	{id = "orc leather", chance = 4300},
	{id = "orc tooth", chance = 2100},
	{id = "shamanic hood", chance = 6860},
	{id = "broken shamanic staff", chance = 10300},
	{id = "book", chance = 520}
}

monster.attacks = {
	{name = "melee",  attack = 15, skill = 10, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 15, interval = 2000, minDamage = -20, maxDamage = -31, range = 7, shootEffect = 36},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 5, interval = 2000, minDamage = -5, maxDamage = -43, target = true, range = 7, radius = 1, shootEffect = 4}
}

monster.defenses = {
	defense = 10,
    armor = 10,
    {name = "combat", type = COMBAT_HEALING,  chance = 60, interval = 2000, minDamage = 27, maxDamage = 43, radius = 1, shootEffect = 4, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 50},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = 25}
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