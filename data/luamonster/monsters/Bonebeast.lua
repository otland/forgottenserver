local mType = Game.createMonsterType("Bonebeast")
local monster = {}
monster.description = "a bonebeast"
monster.experience = 580
monster.outfit = {
	lookType = 101
}

monster.health = 515
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6030
monster.speed = 210
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
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Cccchhhhhhhhh!", yell = false},
	{text = "Knooorrrrr!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 50, chance = 14000},
	{id = "gold coin", maxCount = 40, chance = 16000},
	{id = "hardened bone", chance = 960},
	{id = "bone", chance = 47750},
	{id = "bone club", chance = 4950},
	{id = "bone shield", chance = 2000},
	{id = "green mushroom", chance = 1350},
	{id = "plate armor", chance = 8000},
	{id = "health potion", chance = 540},
	{id = "skull", chance = 20000},
	{id = "bony tail", chance = 9780},
	{id = "bonebeast trophy", chance = 120}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 40, minDamage = 0, maxDamage = -200, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -50, maxDamage = -90, range = 7, shootEffect = 15, effect = 21},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -25, maxDamage = -47, radius = 3, shootEffect = 15, effect = 14},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -50, maxDamage = -60, radius = 3, shootEffect = 15, effect = 21},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -70, maxDamage = -80, length = 6, shootEffect = 15, effect = 21},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 13000, speedChange = -600, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 50, maxDamage = 60, shootEffect = 15, effect = 17}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)