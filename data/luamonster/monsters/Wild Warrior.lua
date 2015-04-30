local mType = Game.createMonsterType("Wild Warrior")
local monster = {}
monster.description = "a wild warrior"
monster.experience = 55
monster.outfit = {
	lookFeet = 57,
	lookLegs = 57,
	lookHead = 57,
	lookType = 131,
	lookBody = 57
}

monster.health = 135
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20531
monster.speed = 190
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "An enemy!", yell = false},
	{text = "Gimme your money!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 30, chance = 49070},
	{id = "egg", maxCount = 2, chance = 9730},
	{id = "leather legs", chance = 27500},
	{id = "doll", chance = 520},
	{id = "brass armor", chance = 2540},
	{id = "steel shield", chance = 910},
	{id = "axe", chance = 30710},
	{id = "mace", chance = 9800},
	{id = "chain helmet", chance = 5250},
	{id = "brass shield", chance = 17000},
	{id = "iron helmet", chance = 580},
	{id = "war hammer", chance = 100}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 60, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 200, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = 10}
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