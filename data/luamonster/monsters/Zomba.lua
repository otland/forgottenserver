local mType = Game.createMonsterType("Zomba")
local monster = {}
monster.description = "a zomba"
monster.experience = 300
monster.outfit = {
	lookType = 41
}

monster.health = 300
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5986
monster.speed = 180
monsters.runHealth = 10
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Groarrr!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 25, chance = 100000},
	{id = "lion's mane", maxCount = 2, chance = 100000},
	{id = "platinum coin", chance = 25000},
	{id = "life ring", chance = 12500}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 30, minDamage = 0, maxDamage = -40, interval = 2000, effect = 0}
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