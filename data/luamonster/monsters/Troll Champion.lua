local mType = Game.createMonsterType("Troll Champion")
local monster = {}
monster.description = "a troll champion"
monster.experience = 40
monster.outfit = {
	lookType = 281
}

monster.health = 75
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7926
monster.speed = 190
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
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
	staticAttack = 95
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Meee maity!", yell = false},
	{text = "Grrrr", yell = false},
	{text = "Whaaaz up!?", yell = false},
	{text = "Gruntz!", yell = false},
	{text = "Groar", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 12, chance = 64000},
	{id = "spear", chance = 25000},
	{id = "meat", chance = 9650},
	{id = "leather boots", chance = 9000},
	{id = "wooden shield", chance = 6000},
	{id = "arrow", maxCount = 5, chance = 5450},
	{id = "studded club", chance = 5450},
	{id = "bunch of troll hair", chance = 3000},
	{id = "trollroot", chance = 750},
	{id = "silver amulet", subType = 200, chance = 230}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 10}
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