local mType = Game.createMonsterType("Necropharus")
local monster = {}
monster.description = "necropharus"
monster.experience = 700
monster.outfit = {
	lookType = 209
}

monster.health = 750
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20574
monster.speed = 240
monster.maxSummons = 4

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
	targetDistance = 4,
	staticAttack = 90
}

monster.summons = {
    {name = "ghoul", chance = 20, interval = 1000},
	{name = "ghost", chance = 17, interval = 1000},
	{name = "mummy", chance = 15, interval = 1000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "You will rise as my servant!", yell = false},
	{text = "Praise to my master Urgith!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 40, chance = 100000},
	{id = "bowl", chance = 10000},
	{id = "horseman helmet", chance = 150},
	{id = "fire sword", chance = 220},
	{id = "shadow herb", maxCount = 2, chance = 20000},
	{id = "worm", maxCount = 10, chance = 50000},
	{id = "soul stone", chance = 100000},
	{id = "moonlight rod", chance = 500},
	{id = "scale armor", chance = 8500},
	{id = "green mushroom", chance = 22500},
	{id = "big bone", chance = 6000},
	{id = "bone", chance = 30000},
	{id = "bone club", chance = 19900},
	{id = "bone shield", chance = 7500},
	{id = "katana", chance = 10000},
	{id = "skull", maxCount = 3, chance = 20000},
	{id = "grave flower", chance = 20000},
	{id = "skull staff", chance = 833},
	{id = "mystic turban", chance = 909},
	{id = "boots of haste", chance = 666},
	{id = "clerical mace", chance = 5000}
}

monster.attacks = {
	{name = "melee",  attack = 45, skill = 35, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 20, interval = 1000, minDamage = -80, maxDamage = -120, range = 1, effect = 1},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 17, interval = 1000, minDamage = -50, maxDamage = -140, range = 7, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 1000, minDamage = 60, maxDamage = 90, shootEffect = 15, effect = 13}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)