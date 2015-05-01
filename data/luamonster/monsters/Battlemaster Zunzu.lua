local mType = Game.createMonsterType("Battlemaster Zunzu")
local monster = {}
monster.description = "a battlemaster zunzu"
monster.experience = 6500
monster.outfit = {
	lookType = 343
}

monster.health = 5000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 11281
monster.speed = 420
monster.runHealth = 150
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "platinum coin", maxCount = 3, chance = 26800},
	{id = "red lantern", chance = 100000},
	{id = "zaogun shoulderplates", chance = 100000},
	{id = "Zaoan shoes", chance = 3150},
	{id = "zaogun flag", chance = 11250},
	{id = "strong health potion", maxCount = 2, chance = 2875},
	{id = "lizard scale", chance = 800},
	{id = "great health potion", maxCount = 2, chance = 2775},
	{id = "Zaoan legs", chance = 2625},
	{id = "lizard leather", chance = 1825},
	{id = "Zaoan armor", chance = 1050}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 65, minDamage = 0, maxDamage = -300, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 25, interval = 2000, minDamage = -115, maxDamage = -350, target = true, range = 1, radius = 1, effect = 21}
}

monster.defenses = {
	defense = 35,
    armor = 45,
    {name = "combat", type = COMBAT_HEALING,  chance = 18, interval = 1000, minDamage = 200, maxDamage = 400, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 15},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 25},
	{type = COMBAT_DEATHDAMAGE, percent = 10},
	{type = COMBAT_ICEDAMAGE, percent = 15}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)