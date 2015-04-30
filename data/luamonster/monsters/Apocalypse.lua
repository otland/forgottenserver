local mType = Game.createMonsterType("Apocalypse")
local monster = {}
monster.description = "apocalypse"
monster.experience = 35000
monster.outfit = {
	lookFeet = 94,
	lookHead = 38,
	lookType = 12,
	lookBody = 114
}

monster.health = 80000
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 6068
monster.speed = 380
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
	{text = "BOW TO THE POWER OF THE RUTHLESS SEVEN!", yell = true},
	{text = "DESTRUCTION!", yell = true},
	{text = "CHAOS!", yell = true},
	{text = "DEATH TO ALL!", yell = true}
}

monster.loot = {
    {id = "ancient amulet", chance = 3500},
	{id = "big bone", chance = 9000},
	{id = "black pearl", maxCount = 15, chance = 15000},
	{id = "blue gem", chance = 1500},
	{id = "boots of haste", chance = 4000},
	{id = "crystal ball", chance = 2500},
	{id = "crystal necklace", chance = 1500},
	{id = "crystal ring", chance = 5500},
	{id = "demon shield", chance = 15500},
	{id = "devil helmet", chance = 11000},
	{id = "double axe", chance = 20000},
	{id = "dragon hammer", chance = 4500},
	{id = "energy ring", chance = 13500},
	{id = "fire axe", chance = 17000},
	{id = "giant sword", chance = 12500},
	{id = "gold coin", maxCount = 100, chance = 99900},
	{id = "gold coin", maxCount = 100, chance = 88800},
	{id = "gold coin", maxCount = 100, chance = 77700},
	{id = "gold coin", maxCount = 100, chance = 66600},
	{id = "gold ring", chance = 8000},
	{id = "golden legs", chance = 5000},
	{id = "golden mug", chance = 7500},
	{id = "golden sickle", chance = 4500},
	{id = "green gem", chance = 1500},
	{id = "ice rapier", subType = 1, chance = 7500},
	{id = "life crystal", chance = 1000},
	{id = "magic light wand", chance = 11500},
	{id = "magic plate armor", chance = 3000},
	{id = "mastermind shield", chance = 7500},
	{id = "might ring", subType = 20, chance = 5000},
	{id = "mind stone", chance = 4000},
	{id = "moonlight rod", chance = 3500},
	{id = "orb", chance = 12000},
	{id = "platinum amulet", chance = 4500},
	{id = "protection amulet", subType = 250, chance = 4500},
	{id = "purple tome", chance = 2600},
	{id = "ring of healing", chance = 13000},
	{id = "ring of the sky", chance = 3500},
	{id = "silver amulet", subType = 200, chance = 13000},
	{id = "silver dagger", chance = 15500},
	{id = "skull staff", chance = 5000},
	{id = "small amethyst", maxCount = 20, chance = 13500},
	{id = "small diamond", maxCount = 5, chance = 9500},
	{id = "small emerald", maxCount = 10, chance = 15500},
	{id = "small sapphire", maxCount = 10, chance = 13500},
	{id = "snakebite rod", chance = 3500},
	{id = "stealth ring", chance = 9500},
	{id = "stone skin amulet", subType = 5, chance = 4000},
	{id = "strange symbol", chance = 2500},
	{id = "talon", maxCount = 7, chance = 14000},
	{id = "teddy bear", chance = 14500},
	{id = "thunder hammer", chance = 13500},
	{id = "two handed sword", chance = 20000},
	{id = "necrotic rod", chance = 3500},
	{id = "voodoo doll", chance = 100},
	{id = "wand of decay", chance = 2500},
	{id = "white pearl", maxCount = 15, chance = 12500}
}

monster.attacks = {
	{name = "melee",  attack = 210, skill = 230, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 8, interval = 1000, minDamage = -800, maxDamage = -1900, radius = 9, effect = 18},
	{name = "speed",  chance = 12, interval = 1000, minDamage = 0, maxDamage = 0, duration = 60000, speedChange = -850, radius = 6, effect = 21},
	{name = "strength",  chance = 10, interval = 1000, minDamage = -600, maxDamage = -1450, radius = 5, effect = 10},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 13, interval = 3000, minDamage = -300, maxDamage = -800, target = true, range = 7, radius = 7, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 8, interval = 3000, minDamage = -600, maxDamage = -700, radius = 10, shootEffect = 4, effect = 38},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 9, interval = 2000, minDamage = -400, maxDamage = -800, length = 8, radius = 10, shootEffect = 4, effect = 14},
	{name = "condition", type = CONDITION_POISON,  chance = 18, interval = 5000, minDamage = -800, maxDamage = -1000, radius = 10, shootEffect = 4, effect = 17},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 6, interval = 2000, minDamage = -600, maxDamage = -1200, radius = 14, shootEffect = 4, effect = 15}
}

monster.defenses = {
	defense = 145,
    armor = 188,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 1000, minDamage = 1000, maxDamage = 3000, radius = 14, shootEffect = 4, effect = 13},
	{name = "speed",  chance = 8, interval = 2000, minDamage = 0, maxDamage = 0, duration = 6000, speedChange = 480, radius = 14, shootEffect = 4, effect = 14}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = true, combat = true},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)