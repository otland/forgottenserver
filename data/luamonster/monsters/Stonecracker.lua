local mType = Game.createMonsterType("Stonecracker")
local monster = {}
monster.description = "stonecracker"
monster.experience = 3500
monster.outfit = {
	lookType = 55
}

monster.health = 6500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5999
monster.speed = 280
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

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "HUAHAHA!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 76225},
	{id = "perfect behemoth fang", chance = 64800},
	{id = "meat", chance = 36200},
	{id = "behemoth claw", chance = 50500},
	{id = "assassin star", maxCount = 2, chance = 11225},
	{id = "dark armor", chance = 7650},
	{id = "pick", chance = 14800},
	{id = "crowbar", chance = 14800},
	{id = "small amethyst", maxCount = 2, chance = 7650},
	{id = "double axe", chance = 7650},
	{id = "plate armor", chance = 7650}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 90, minDamage = 0, maxDamage = -500, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = -200, maxDamage = -280, range = 7, shootEffect = 12}
}

monster.defenses = {
	defense = 45,
    armor = 40,
    {name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 4000, speedChange = 360, shootEffect = 12, effect = 14},
	{name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 500, maxDamage = 600, shootEffect = 12, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 15},
	{type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_EARTHDAMAGE, percent = 85},
	{type = COMBAT_FIREDAMAGE, percent = 40},
	{type = COMBAT_HOLYDAMAGE, percent = 35},
	{type = COMBAT_ICEDAMAGE, percent = -5}
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