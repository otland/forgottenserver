local mType = Game.createMonsterType("Pirate Buccaneer")
local monster = {}
monster.description = "a pirate buccaneer"
monster.experience = 250
monster.outfit = {
	lookType = 97
}

monster.health = 425
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20471
monster.speed = 220
monsters.runHealth = 50
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 15
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Give up!", yell = false},
	{text = "Hiyaa", yell = false},
	{text = "Plundeeeeer!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 59, chance = 67740},
	{id = "throwing knife", maxCount = 5, chance = 9000},
	{id = "sabre", chance = 10100},
	{id = "worn leather boots", chance = 9900},
	{id = "torch", chance = 10190},
	{id = "battle shield", chance = 3850},
	{id = "treasure map", chance = 1000},
	{id = "pirate shirt", chance = 1200},
	{id = "plate armor", chance = 1130},
	{id = "peg leg", chance = 510},
	{id = "eye patch", chance = 420},
	{id = "strong health potion", chance = 670},
	{id = "hook", chance = 450},
	{id = "pirate backpack", chance = 430},
	{id = "dice", chance = 40},
	{id = "compass", chance = 9780},
	{id = "rum flask", chance = 120}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 30, minDamage = 0, maxDamage = -160, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -100, range = 4, shootEffect = 9}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = -5}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
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