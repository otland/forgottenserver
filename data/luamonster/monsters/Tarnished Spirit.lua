local mType = Game.createMonsterType("Tarnished Spirit")
local monster = {}
monster.description = "a tarnished spirit"
monster.experience = 120
monster.outfit = {
	lookType = 566
}

monster.health = 150
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 21366
monster.speed = 180
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Huh!", yell = false}
}

monster.loot = {
    {id = "morning star", chance = 10610},
	{id = "combat knife", chance = 7002},
	{id = "shadow herb", chance = 14400},
	{id = "cape", chance = 8800},
	{id = "book", chance = 1310},
	{id = "white piece of cloth", chance = 1940},
	{id = "ancient shield", chance = 860},
	{id = "stealth ring", chance = 180},
	{id = "ghostly tissue", chance = 1870}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -20, maxDamage = -45, range = 1, effect = 14}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = true},
	{type = "bleed", condition = true, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)