local mType = Game.createMonsterType("Bazir")
local monster = {}
monster.description = "bazir"
monster.experience = 30000
monster.outfit = {
	lookType = 35
}

monster.health = 110000
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 2916
monster.speed = 530
monster.runHealth = 3000
monster.maxSummons = 1

monster.changeTarget = {
    interval = 2000,
	chance = 10
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
	staticAttack = 95
}

monster.summons = {
    {name = "bazir", chance = 15, interval = 1000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "COME HERE! FREE ITEMS FOR EVERYONE!", yell = true},
	{text = "BOW TO THE POWER OF THE RUTHLESS SEVEN!", yell = false},
	{text = "Slay your friends and I will spare you!", yell = true},
	{text = "DON'T BE AFRAID! I AM COMING IN PEACE!", yell = true}
}

monster.loot = {
    {id = "ancient amulet", chance = 3500},
	{id = "big bone", chance = 9000},
	{id = "black pearl", maxCount = 15, chance = 15000},
	{id = "blue gem", chance = 1500},
	{id = "boots of haste", chance = 4000},
	{id = "moonlight rod", chance = 3500},
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
	{id = "snakebite rod", chance = 3500},
	{id = "green gem", chance = 1500},
	{id = "wand of decay", chance = 2500},
	{id = "ice rapier", subType = 1, chance = 7500},
	{id = "life crystal", chance = 1000},
	{id = "magic light wand", chance = 11500},
	{id = "magic plate armor", chance = 3000},
	{id = "mastermind shield", chance = 7500},
	{id = "might ring", subType = 20, chance = 5000},
	{id = "mind stone", chance = 4000},
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
	{id = "stealth ring", chance = 9500},
	{id = "stone skin amulet", subType = 5, chance = 4000},
	{id = "strange symbol", chance = 2500},
	{id = "talon", maxCount = 7, chance = 14000},
	{id = "teddy bear", chance = 14500},
	{id = "thunder hammer", chance = 13500},
	{id = "two handed sword", chance = 20000},
	{id = "voodoo doll", chance = 100},
	{id = "necrotic rod", chance = 3500},
	{id = "white pearl", maxCount = 15, chance = 12500}
}

monster.attacks = {
	{name = "melee",  attack = 250, skill = 200, minDamage = 0, maxDamage = -2625, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 7, interval = 1000, minDamage = -100, maxDamage = -1000, range = 7, shootEffect = 5, effect = 3},
	{name = "drunk",  chance = 7, interval = 1000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 5, effect = 38},
	{name = "strength",  chance = 9, interval = 1000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 12, effect = 38},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 13, interval = 1000, minDamage = -400, maxDamage = -700, radius = 8, shootEffect = 12, effect = 2},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 1000, minDamage = -400, maxDamage = -700, radius = 8, shootEffect = 12, effect = 15},
	{name = "speed",  chance = 12, interval = 1000, minDamage = 0, maxDamage = 0, duration = 60000, speedChange = -1000, radius = 6, shootEffect = 12, effect = 21},
	{name = "strength",  chance = 8, interval = 1000, minDamage = 0, maxDamage = 0, radius = 5, shootEffect = 12, effect = 10},
	{name = "outfit",  chance = 2, interval = 1000, minDamage = 0, maxDamage = 0, radius = 8, shootEffect = 12, effect = 2},
	{name = "outfit",  chance = 2, interval = 1000, minDamage = 0, maxDamage = 0, radius = 8, shootEffect = 12, effect = 2},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 34, interval = 1000, minDamage = -100, maxDamage = -900, target = true, range = 7, radius = 7, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 1000, minDamage = -500, maxDamage = -850, length = 8, shootEffect = 4, effect = 14}
}

monster.defenses = {
	defense = 160,
    armor = 160,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 1000, minDamage = 5000, maxDamage = 10000, shootEffect = 4, effect = 13},
	{name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 1000, minDamage = 2000, maxDamage = 3000, shootEffect = 4, effect = 13},
	{name = "speed",  chance = 8, interval = 1000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 1901, shootEffect = 4, effect = 14},
	{name = "invisible",  chance = 4, interval = 1000, minDamage = 0, maxDamage = 0, shootEffect = 4, effect = 13},
	{name = "invisible",  chance = 17, interval = 1000, minDamage = 0, maxDamage = 0, shootEffect = 4, effect = 13},
	{name = "outfit",  chance = 2, interval = 1000, minDamage = 0, maxDamage = 0, shootEffect = 4, effect = 13}
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