local mType = Game.createMonsterType("Crazed Beggar")
local monster = {}
monster.description = "a crazed beggar"
monster.experience = 35
monster.outfit = {
	lookFeet = 97,
	lookLegs = 38,
	lookHead = 59,
	lookType = 153,
	lookAddons = 3,
	lookBody = 38
}

monster.health = 100
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20351
monster.speed = 200
monster.runHealth = 10
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
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Hehehe!", yell = false},
	{text = "Raahhh!", yell = false},
	{text = "You are one of THEM! Die!", yell = false},
	{text = "Wanna buy roses??", yell = false},
	{text = "They're coming!", yell = false},
	{text = "Make it stop!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 9, chance = 99000},
	{id = "dirty cape", chance = 55000},
	{id = "roll", chance = 22500},
	{id = "meat", chance = 9500},
	{id = "wooden spoon", chance = 9750},
	{id = "wooden hammer", chance = 6500},
	{id = "rolling pin", chance = 5650},
	{id = "red rose", chance = 4700},
	{id = "small blue pillow", chance = 420},
	{id = "rum flask", chance = 420},
	{id = "sling herb", chance = 420},
	{id = "rusty armor", chance = 80},
	{id = "dwarven ring", chance = 120},
	{id = "lute", chance = 360},
	{id = "very noble-looking watch", chance = 300}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 15, minDamage = 0, maxDamage = -25, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
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