local mType = Game.createMonsterType("Kraknaknork Demon")
local monster = {}
monster.description = "a kraknaknork demon"
monster.experience = 0
monster.outfit = {
	lookLegs = 117,
	lookHead = 117,
	lookType = 12,
	lookBody = 58
}

monster.health = 120
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 6068
monster.speed = 400
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 15
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
	staticAttack = 85
}

monster.voices = {
	interval = 5000,
	chance = 100,
	{text = "STEP A BIT CLOSER, RIGHT THERE, HAHAHA!", yell = true},
	{text = "COME AND DIE!", yell = true},
	{text = "ROOKIE FOR BREAKFAST", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 48025},
	{id = "gold coin", maxCount = 86, chance = 48025},
	{id = "gold coin", chance = 24400}
}

monster.attacks = {
	{name = "melee",  attack = 95, skill = 95, minDamage = 0, maxDamage = -499, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 20, interval = 3000, minDamage = -10, maxDamage = -40, target = true, radius = 4, effect = 6}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = -8},
	{type = COMBAT_FIREDAMAGE, percent = 25},
	{type = COMBAT_ICEDAMAGE, percent = -15}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)