local mType = Game.createMonsterType("Island Troll")
local monster = {}
monster.description = "an island troll"
monster.experience = 20
monster.outfit = {
	lookType = 282
}

monster.health = 50
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7930
monster.speed = 190
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
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
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Hmmm, turtles", yell = false},
	{text = "Hmmm, dogs", yell = false},
	{text = "Hmmm, worms", yell = false},
	{text = "Groar", yell = false},
	{text = "Gruntz!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 10, chance = 60000},
	{id = "wood", chance = 30000},
	{id = "spear", chance = 20000},
	{id = "hand axe", chance = 18000},
	{id = "wooden shield", chance = 16000},
	{id = "leather boots", chance = 10500},
	{id = "leather helmet", chance = 10000},
	{id = "rope", chance = 8000},
	{id = "studded club", chance = 5000},
	{id = "mango", chance = 5000},
	{id = "silver amulet", subType = 200, chance = 70},
	{id = "marlin", chance = 40}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0}
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