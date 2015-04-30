local mType = Game.createMonsterType("Vampire")
local monster = {}
monster.description = "a vampire"
monster.experience = 305
monster.outfit = {
	lookType = 68
}

monster.health = 475
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6006
monster.speed = 220
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
	{text = "BLOOD!", yell = true},
	{text = "Let me kiss your neck", yell = false},
	{text = "I smell warm blood!", yell = false},
	{text = "I call you, my bats! Come!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 60, chance = 90230},
	{id = "vampire teeth", chance = 7600},
	{id = "blood preservation", chance = 5100},
	{id = "grave flower", chance = 1910},
	{id = "black pearl", chance = 1800},
	{id = "katana", chance = 1560},
	{id = "strong health potion", chance = 1500},
	{id = "spike sword", chance = 1000},
	{id = "skull", chance = 1000},
	{id = "ice rapier", subType = 1, chance = 420},
	{id = "strange helmet", chance = 420},
	{id = "emerald bangle", chance = 230},
	{id = "vampire shield", chance = 230},
	{id = "bronze amulet", subType = 200, chance = 220}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 20, interval = 2000, minDamage = -50, maxDamage = -200, range = 1, effect = 39},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 60000, speedChange = -400, range = 1, effect = 14}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "outfit",  chance = 10, interval = 4000, minDamage = 0, maxDamage = 0, effect = 35},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 3000, speedChange = 300, effect = 14},
	{name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 15, maxDamage = 25, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 35},
	{type = COMBAT_HOLYDAMAGE, percent = -25},
	{type = COMBAT_FIREDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)