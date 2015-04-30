local mType = Game.createMonsterType("Bandit")
local monster = {}
monster.description = "a bandit"
monster.experience = 65
monster.outfit = {
	lookFeet = 114,
	lookLegs = 45,
	lookHead = 58,
	lookType = 129,
	lookBody = 59
}

monster.health = 245
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20331
monster.speed = 180
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Your money or your life!", yell = false},
	{text = "Hand me your purse!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 30, chance = 49000},
	{id = "axe", chance = 29900},
	{id = "brass shield", chance = 16800},
	{id = "leather legs", chance = 15500},
	{id = "mace", chance = 10100},
	{id = "chain helmet", chance = 5000},
	{id = "brass armor", chance = 2500},
	{id = "iron helmet", chance = 520},
	{id = "tomato", maxCount = 2, chance = 7630}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 20, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = -5}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
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