local mType = Game.createMonsterType("Infernatil")
local monster = {}
monster.description = "infernatil"
monster.experience = 85000
monster.outfit = {
	lookType = 35
}

monster.health = 270000
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 2916
monster.speed = 605
monsters.runHealth = 3000
monster.maxSummons = 4

monster.changeTarget = {
    interval = 2000,
	chance = 15
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
    {name = "hellfire fighter", chance = 8, interval = 1000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Worship Zathroth pathetic mortal!", yell = false},
	{text = "Your soul will be mine!", yell = false},
	{text = "ASHES TO ASHES!", yell = true},
	{text = "YOU WILL ALL BURN!", yell = true},
	{text = "THE DAY OF RECKONING IS AT HAND!", yell = true},
	{text = "BOW TO THE POWER OF THE RUTHLESS SEVEN!", yell = true}
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
	{name = "melee",  attack = 260, skill = 210, minDamage = 0, maxDamage = -2860, interval = 2000, effect = 0},
	{name = "condition", type = CONDITION_FIRE,  chance = 7, interval = 1000, minDamage = -800, maxDamage = -1200, range = 2, shootEffect = 4, effect = 4},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 7, interval = 1000, minDamage = -250, maxDamage = -750, radius = 6, shootEffect = 4, effect = 6},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 50, interval = 1000, minDamage = -200, maxDamage = -500, radius = 5, shootEffect = 4, effect = 4},
	{name = "firefield",  chance = 4, interval = 1000, minDamage = 0, maxDamage = 0, radius = 8, shootEffect = 4, effect = 6},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 34, interval = 1000, minDamage = -350, maxDamage = -850, target = true, range = 7, radius = 7, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 13, interval = 1000, minDamage = -500, maxDamage = -1000, length = 8, shootEffect = 4, effect = 6},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 1000, minDamage = -300, maxDamage = -1500, length = 8, spread = 3, shootEffect = 4, effect = 7}
}

monster.defenses = {
	defense = 150,
    armor = 165,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 1000, minDamage = 5000, maxDamage = 10000, shootEffect = 4, effect = 13},
	{name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 1000, minDamage = 2000, maxDamage = 3000, shootEffect = 4, effect = 13},
	{name = "speed",  chance = 10, interval = 1000, minDamage = 0, maxDamage = 0, duration = 4000, speedChange = 1800, shootEffect = 4, effect = 14}
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