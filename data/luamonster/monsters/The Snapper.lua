local mType = Game.createMonsterType("The Snapper")
local monster = {}
monster.description = "the snapper"
monster.experience = 150
monster.outfit = {
	lookType = 119
}

monster.health = 300
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6046
monster.speed = 240
monster.runHealth = 30
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "health potion", maxCount = 5, chance = 51100},
	{id = "plate legs", chance = 44025},
	{id = "gold coin", maxCount = 100, chance = 50675},
	{id = "gold coin", maxCount = 94, chance = 50675},
	{id = "plate armor", chance = 35800},
	{id = "life ring (faster regeneration)", chance = 100000},
	{id = "crocodile boots", chance = 9900},
	{id = "knight armor", chance = 4025}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 20, minDamage = 0, maxDamage = -60, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 15},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = 10}
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