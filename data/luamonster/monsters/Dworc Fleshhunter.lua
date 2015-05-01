local mType = Game.createMonsterType("Dworc Fleshhunter")
local monster = {}
monster.description = "a dworc fleshhunter"
monster.experience = 40
monster.outfit = {
	lookType = 215
}

monster.health = 85
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6058
monster.speed = 180
monster.runHealth = 8
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = false,
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
	{text = "Grak brrretz!", yell = false},
	{text = "Grow truk grrrrr.", yell = false},
	{text = "Prek tars, dekklep zurk.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 13, chance = 69000},
	{id = "skull", maxCount = 3, chance = 3300},
	{id = "cleaver", chance = 9750},
	{id = "torch", chance = 4750},
	{id = "wooden spoon", chance = 10000},
	{id = "tribal mask", chance = 500},
	{id = "hunting spear", chance = 2000},
	{id = "poison dagger", chance = 2250},
	{id = "bone shield", chance = 1000},
	{id = "ripper lance", chance = 90}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 15, minDamage = 0, maxDamage = -25, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -15, range = 7, shootEffect = 9}
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