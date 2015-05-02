local mType = Game.createMonsterType("Dworc Voodoomaster")
local monster = {}
monster.description = "a dworc voodoomaster"
monster.experience = 55
monster.outfit = {
	lookType = 214
}

monster.health = 80
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6055
monster.speed = 190
monster.runHealth = 15
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Brak brrretz!", yell = false},
	{text = "Grow truk grrrrr.", yell = false},
	{text = "Prek tars, dekklep zurk.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 17, chance = 75000},
	{id = "skull", maxCount = 3, chance = 1950},
	{id = "torch", chance = 6000},
	{id = "bone", chance = 5800},
	{id = "leather armor", chance = 10000},
	{id = "poison dagger", chance = 1000},
	{id = "tribal mask", chance = 500},
	{id = "strange symbol", chance = 500},
	{id = "health potion", chance = 600},
	{id = "voodoo doll", chance = 130},
	{id = "big bone", chance = 3000}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 10, minDamage = 0, maxDamage = -20, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -40, range = 1, effect = 14},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = -800, range = 7, effect = 14},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 5, effect = 11},
	{name = "outfit",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 5, effect = 13},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -6, maxDamage = -18, radius = 6, shootEffect = 5, effect = 9},
	{name = "poisonfield",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 1, shootEffect = 15, effect = 9}
}

monster.defenses = {
	defense = 10,
    armor = 10,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 2000, minDamage = 3, maxDamage = 9, shootEffect = 15, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 4000, speedChange = 200, shootEffect = 15, effect = 14},
	{name = "invisible",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 15, effect = 13}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)