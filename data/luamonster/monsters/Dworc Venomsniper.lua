local mType = Game.createMonsterType("Dworc Venomsniper")
local monster = {}
monster.description = "a dworc venomsniper"
monster.experience = 35
monster.outfit = {
	lookType = 216
}

monster.health = 80
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6059
monster.speed = 180
monster.runHealth = 15
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
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Brak brrretz!", yell = false},
	{text = "Grow truk grrrrr.", yell = false},
	{text = "Prek tars, dekklep zurk.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 13, chance = 75000},
	{id = "skull", maxCount = 2, chance = 1000},
	{id = "leather armor", chance = 10000},
	{id = "poison arrow", maxCount = 3, chance = 5000},
	{id = "torch", chance = 6000},
	{id = "throwing star", maxCount = 2, chance = 8000},
	{id = "poison dagger", chance = 1500},
	{id = "tribal mask", chance = 510},
	{id = "bronze amulet", subType = 200, chance = 110},
	{id = "bast skirt", chance = 100},
	{id = "seeds", chance = 200}
}

monster.attacks = {
	{name = "melee",  attack = 15, skill = 10, minDamage = 0, maxDamage = -15, interval = 2000, effect = 0},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 2000, minDamage = -20, maxDamage = -40, range = 5, shootEffect = 15}
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