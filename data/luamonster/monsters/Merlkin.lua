local mType = Game.createMonsterType("Merlkin")
local monster = {}
monster.description = "a merlkin"
monster.experience = 145
monster.outfit = {
	lookType = 117
}

monster.health = 235
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6044
monster.speed = 200
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
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Ugh! Ugh! Ugh!", yell = false},
	{text = "Holy banana!", yell = false},
	{text = "Chakka! Chakka!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 45, chance = 69500},
	{id = "banana", maxCount = 12, chance = 30350},
	{id = "orange", maxCount = 5, chance = 1000},
	{id = "wand of decay", chance = 1050},
	{id = "magic light wand", chance = 3000},
	{id = "mana potion", chance = 660},
	{id = "small amethyst", chance = 260},
	{id = "ape fur", chance = 1000},
	{id = "banana staff", chance = 100},
	{id = "banana sash", chance = 1800}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -60, maxDamage = -90, range = 7, shootEffect = 4, effect = 16},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 20, interval = 2000, minDamage = -15, maxDamage = -45, range = 7, shootEffect = 5, effect = 12},
	{name = "poisonfield",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 1, shootEffect = 15, effect = 12}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 2000, minDamage = 30, maxDamage = 40, radius = 1, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = -15},
	{type = COMBAT_HOLYDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = 20}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)