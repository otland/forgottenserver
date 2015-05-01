local mType = Game.createMonsterType("Munster")
local monster = {}
monster.description = "a munster"
monster.experience = 23
monster.outfit = {
	lookType = 56
}

monster.health = 58
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 2813
monster.speed = 200
monsters.runHealth = 10
monster.maxSummons = 2

monster.changeTarget = {
    interval = 60000,
	chance = 0
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 80
}

monster.summons = {
    {name = "Rat", chance = 20, interval = 2000}
}

monster.voices = {
	interval = 2000,
	chance = 5,
	{text = "Meeeeep!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 16, chance = 100000},
	{id = "cheese", chance = 50000},
	{id = "cookie", chance = 5000},
	{id = "jacket", chance = 50000},
	{id = "bone club", chance = 5000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = -1, maxDamage = -15, interval = 2000, effect = 0}
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