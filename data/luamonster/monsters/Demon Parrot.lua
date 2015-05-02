local mType = Game.createMonsterType("Demon Parrot")
local monster = {}
monster.description = "a demon parrot"
monster.experience = 225
monster.outfit = {
	lookType = 217
}

monster.health = 360
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6056
monster.speed = 300
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 20
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
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "ISHH THAT THE BESHHT YOU HAVE TO OFFERRR?", yell = false},
	{text = "YOU ARRRRRE DOOMED!", yell = false},
	{text = "I SHHMELL FEEAARRR!", yell = false},
	{text = "MY SHHEED IS FEARRR AND MY HARRRVEST ISHH YOURRR SHHOUL!", yell = false},
	{text = "Your shhoooul will be mineee!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 99, chance = 50000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -100, interval = 1200, effect = 0},
	{name = "drunk",  chance = 30, interval = 1000, minDamage = 0, maxDamage = 0, length = 5, effect = 20},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 30, interval = 1000, minDamage = -25, maxDamage = -45, range = 5, shootEffect = 32, effect = 20},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 30, interval = 1000, minDamage = -15, maxDamage = -45, range = 1, shootEffect = 32, effect = 20}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
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