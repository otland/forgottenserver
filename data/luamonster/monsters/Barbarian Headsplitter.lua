local mType = Game.createMonsterType("Barbarian Headsplitter")
local monster = {}
monster.description = "a barbarian headsplitter"
monster.experience = 85
monster.outfit = {
	lookFeet = 132,
	lookLegs = 119,
	lookHead = 115,
	lookType = 253,
	lookBody = 105
}

monster.health = 100
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20343
monster.speed = 200
monster.maxSummons = 0

monster.changeTarget = {
    interval = 60000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "I will regain my honor with your blood!", yell = false},
	{text = "Surrender is not option!", yell = false},
	{text = "Its you or me!", yell = false},
	{text = "Die! Die! Die!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 30, chance = 60000},
	{id = "torch", chance = 61500},
	{id = "knife", chance = 15500},
	{id = "brass helmet", chance = 20360},
	{id = "scale armor", chance = 4090},
	{id = "viking helmet", chance = 5080},
	{id = "skull of Ratha", chance = 3270},
	{id = "brown piece of cloth", chance = 950},
	{id = "health potion", chance = 560},
	{id = "life ring", chance = 230},
	{id = "krimhorn helmet", chance = 110},
	{id = "fur boots", chance = 100}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 40, minDamage = 0, maxDamage = -50, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -60, target = true, range = 7, radius = 1, shootEffect = 26}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = -1},
	{type = COMBAT_ICEDAMAGE, percent = 50}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
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