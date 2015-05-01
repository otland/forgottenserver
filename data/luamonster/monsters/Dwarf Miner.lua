local mType = Game.createMonsterType("Dwarf Miner")
local monster = {}
monster.description = "a dwarf miner"
monster.experience = 60
monster.outfit = {
	lookFeet = 115,
	lookLegs = 97,
	lookHead = 77,
	lookType = 160,
	lookBody = 101
}

monster.health = 120
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6007
monster.speed = 210
monster.maxSummons = 0

monster.changeTarget = {
    interval = 0,
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
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Work, work!", yell = false},
	{text = "Intruders in the mines!", yell = false},
	{text = "Mine, all mine!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 10, chance = 33333},
	{id = "axe", chance = 14285},
	{id = "pick", chance = 11111},
	{id = "leather legs", chance = 9090},
	{id = "studded armor", chance = 6666},
	{id = "meat", chance = 3846},
	{id = "dwarven ring", chance = 793},
	{id = "iron ore", chance = 793}
}

monster.attacks = {
	{name = "melee",  attack = 15, skill = 24, minDamage = 0, maxDamage = -26, interval = 2000, effect = 0}
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