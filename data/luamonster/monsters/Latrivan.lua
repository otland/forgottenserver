local mType = Game.createMonsterType("Latrivan")
local monster = {}
monster.description = "a latrivan"
monster.experience = 10000
monster.outfit = {
	lookFeet = 111,
	lookLegs = 121,
	lookHead = 120,
	lookType = 12,
	lookBody = 128
}

monster.health = 40000
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 6068
monster.speed = 340
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
	staticAttack = 85
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "I might reward you for killing my brother ~ with a swift dead!", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "gold coin", maxCount = 50, chance = 100000},
	{id = "platinum coin", maxCount = 6, chance = 80000},
	{id = "black pearl", maxCount = 28, chance = 7777},
	{id = "mastermind potion", chance = 17000},
	{id = "mastermind shield", chance = 4500},
	{id = "platinum amulet", chance = 12222},
	{id = "talon", maxCount = 13, chance = 6500},
	{id = "golden legs", chance = 6555},
	{id = "fire axe", chance = 8200}
}

monster.attacks = {
	{name = "melee",  attack = 135, skill = 120, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 1000, minDamage = -50, maxDamage = -850, length = 7, effect = 6}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 35},
	{type = COMBAT_DEATHDAMAGE, percent = -12},
	{type = COMBAT_ICEDAMAGE, percent = -10}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)