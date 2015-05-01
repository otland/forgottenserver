local mType = Game.createMonsterType("The Noxious Spawn")
local monster = {}
monster.description = "the noxious spawn"
monster.experience = 6000
monster.outfit = {
	lookType = 220
}

monster.health = 9500
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 4323
monster.speed = 360
monsters.runHealth = 275
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
	{text = "Sssssouls for the one", yell = false},
	{text = "HISSSS", yell = true},
	{text = "Tsssse one will risssse again", yell = false},
	{text = "I bring your deathhh, mortalssss", yell = false}
}

monster.loot = {
    {id = "platinum coin", maxCount = 5, chance = 51850},
	{id = "noble axe", chance = 37000},
	{id = "platinum coin", maxCount = 95, chance = 10750},
	{id = "onyx arrow", chance = 525},
	{id = "claw of 'The Noxious Spawn'", chance = 31500},
	{id = "life ring", chance = 35000},
	{id = "power bolt", maxCount = 29, chance = 36250},
	{id = "mercenary sword", chance = 27750},
	{id = "green mushroom", chance = 11111}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 40, minDamage = 0, maxDamage = -250, interval = 2000, effect = 0},
	{name = "speed",  chance = 6, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = -660, range = 7},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 7, interval = 2000, minDamage = 0, maxDamage = -550, length = 8, spread = 3, effect = 46},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 9, interval = 2000, minDamage = 0, maxDamage = -550, length = 8, effect = 20},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 12, interval = 2000, minDamage = 0, maxDamage = -300, range = 1, shootEffect = 15, effect = 21},
	{name = "outfit",  chance = 11, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 15, effect = 13}
}

monster.defenses = {
	defense = 25,
    armor = 18,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 900, maxDamage = 1000, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -10}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)