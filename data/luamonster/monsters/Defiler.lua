local mType = Game.createMonsterType("Defiler")
local monster = {}
monster.description = "a defiler"
monster.experience = 3700
monster.outfit = {
	lookType = 238
}

monster.health = 3650
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 6532
monster.speed = 230
monsters.runHealth = 85
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
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Blubb", yell = false},
	{text = "Blubb Blubb", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "gold coin", maxCount = 100, chance = 100000},
	{id = "gold coin", maxCount = 72, chance = 100000},
	{id = "platinum coin", maxCount = 6, chance = 95000},
	{id = "soul orb", chance = 20000},
	{id = "demonic essence", chance = 20320},
	{id = "glob of acid slime", chance = 14210},
	{id = "glob of tar", chance = 12000},
	{id = "talon", chance = 5710},
	{id = "blue gem", chance = 300},
	{id = "small emerald", maxCount = 3, chance = 5366},
	{id = "small ruby", maxCount = 2, chance = 3000},
	{id = "death ring", chance = 3030},
	{id = "small diamond", maxCount = 2, chance = 2439},
	{id = "red gem", chance = 1538},
	{id = "yellow gem", chance = 1219},
	{id = "green gem", chance = 613}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 50, minDamage = 0, maxDamage = -240, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -160, maxDamage = -270, range = 7, shootEffect = 15},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 2000, minDamage = -400, maxDamage = -640, range = 7, radius = 7, shootEffect = 15, effect = 17},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -120, maxDamage = -170, radius = 3, shootEffect = 15, effect = 21},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -500, maxDamage = -1000, length = 8, spread = 3, shootEffect = 15, effect = 46},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 15000, speedChange = -700, length = 8, spread = 3, shootEffect = 15, effect = 39}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 280, maxDamage = 350, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -25}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)