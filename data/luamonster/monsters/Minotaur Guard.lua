local mType = Game.createMonsterType("Minotaur Guard")
local monster = {}
monster.description = "a minotaur guard"
monster.experience = 160
monster.outfit = {
	lookType = 29
}

monster.health = 185
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5983
monster.speed = 250
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
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Kirll Karrrl!", yell = false},
	{text = "Kaplar!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 20, chance = 59640},
	{id = "fishing rod", chance = 480},
	{id = "brass armor", chance = 4390},
	{id = "chain armor", chance = 2980},
	{id = "battle shield", chance = 2150},
	{id = "minotaur leather", chance = 1000},
	{id = "double axe", chance = 400},
	{id = "health potion", chance = 400},
	{id = "minotaur trophy", chance = 80},
	{id = "minotaur horn", maxCount = 2, chance = 8330},
	{id = "piece of warrior armor", chance = 5040}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 30, minDamage = 0, maxDamage = -100, interval = 2000, effect = 0}
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