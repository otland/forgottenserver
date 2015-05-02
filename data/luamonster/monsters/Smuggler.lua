local mType = Game.createMonsterType("Smuggler")
local monster = {}
monster.description = "a smuggler"
monster.experience = 48
monster.outfit = {
	lookFeet = 115,
	lookLegs = 113,
	lookHead = 95,
	lookType = 134
}

monster.health = 130
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20507
monster.speed = 170
monster.runHealth = 15
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
	{text = "I will silence you forever!", yell = false},
	{text = "You saw something you shouldn't!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 10, chance = 80000},
	{id = "torch", maxCount = 2, chance = 30200},
	{id = "raspberry", maxCount = 5, chance = 5000},
	{id = "leather legs", chance = 14840},
	{id = "knife", chance = 9920},
	{id = "ham", chance = 10200},
	{id = "short sword", chance = 10000},
	{id = "leather helmet", chance = 10050},
	{id = "combat knife", chance = 4400},
	{id = "sword", chance = 5000},
	{id = "deer trophy", chance = 110}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 20, minDamage = 0, maxDamage = -60, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = -5}
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