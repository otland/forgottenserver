local mType = Game.createMonsterType("Fernfang")
local monster = {}
monster.description = "fernfang"
monster.experience = 400
monster.outfit = {
	lookType = 206
}

monster.health = 400
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20566
monster.speed = 240
monster.maxSummons = 3

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
    {name = "War Wolf", chance = 13, interval = 1000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "You desecrated this place!", yell = false},
	{text = "I will cleanse this isle!", yell = false},
	{text = "Grrrrrrr", yell = false},
	{text = "Yoooohhuuuu!", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 20, chance = 100000},
	{id = "scroll", chance = 10000},
	{id = "leather armor", chance = 10000},
	{id = "sandals", chance = 6666},
	{id = "worm", maxCount = 10, chance = 50000},
	{id = "lamp", chance = 6666},
	{id = "bread", maxCount = 3, chance = 20000},
	{id = "staff", chance = 3333},
	{id = "power ring", chance = 1428}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 50, minDamage = 0, maxDamage = -120, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 25, interval = 1000, minDamage = -25, maxDamage = -45, range = 7, effect = 14},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 13, interval = 1000, minDamage = -70, maxDamage = -90, range = 7, shootEffect = 5, effect = 12},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 1000, minDamage = -140, maxDamage = -180, range = 7, shootEffect = 32, effect = 12}
}

monster.defenses = {
	defense = 10,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 1000, minDamage = 90, maxDamage = 120, shootEffect = 32, effect = 13},
	{name = "speed",  chance = 7, interval = 1000, minDamage = 0, maxDamage = 0, duration = 10000, speedChange = 310, shootEffect = 32, effect = 14},
	{name = "effect",  chance = 5, interval = 1000, minDamage = 0, maxDamage = 0, shootEffect = 32, effect = 14},
	{name = "outfit",  chance = 5, interval = 1000, minDamage = 0, maxDamage = 0, shootEffect = 32, effect = 13}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)