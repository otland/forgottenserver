local mType = Game.createMonsterType("Valkyrie")
local monster = {}
monster.description = "a valkyrie"
monster.experience = 85
monster.outfit = {
	lookFeet = 113,
	lookLegs = 95,
	lookHead = 113,
	lookType = 139,
	lookBody = 57
}

monster.health = 190
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20523
monster.speed = 190
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
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
	{text = "Another head for me!", yell = false},
	{text = "Head off!", yell = false},
	{text = "Your head will be mine!", yell = false},
	{text = "Stand still!", yell = false},
	{text = "One more head for me!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 12, chance = 30000},
	{id = "meat", chance = 30100},
	{id = "red apple", maxCount = 2, chance = 6800},
	{id = "skull", chance = 760},
	{id = "chain armor", chance = 10000},
	{id = "spear", maxCount = 3, chance = 55000},
	{id = "hunting spear", chance = 5155},
	{id = "plate armor", chance = 719},
	{id = "double axe", chance = 492},
	{id = "protection amulet", subType = 250, chance = 1086},
	{id = "health potion", chance = 591},
	{id = "small diamond", chance = 200},
	{id = "girlish hair decoration", chance = 5800},
	{id = "protective charm", chance = 3400}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -50, range = 7, shootEffect = 1}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE, percent = 5},
	{type = COMBAT_FIREDAMAGE, percent = 10}
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