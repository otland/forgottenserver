local mType = Game.createMonsterType("Ghoul")
local monster = {}
monster.description = "a ghoul"
monster.experience = 85
monster.outfit = {
	lookType = 18
}

monster.health = 100
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5976
monster.speed = 170
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
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 30, chance = 68000},
	{id = "rotten piece of cloth", chance = 14470},
	{id = "worm", maxCount = 2, chance = 9600},
	{id = "ghoul snack", chance = 5130},
	{id = "torch", chance = 5000},
	{id = "scale armor", chance = 1000},
	{id = "brown piece of cloth", chance = 1000},
	{id = "viking helmet", chance = 990},
	{id = "pile of grave earth", chance = 950},
	{id = "skull", chance = 240},
	{id = "life ring", chance = 180}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -15, maxDamage = -27, target = true, range = 1, radius = 1, effect = 39}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 5, interval = 2000, minDamage = 9, maxDamage = 15, radius = 1, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE, percent = -25},
	{type = COMBAT_ICEDAMAGE, percent = 10}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)