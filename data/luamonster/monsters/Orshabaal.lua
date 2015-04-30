local mType = Game.createMonsterType("Orshabaal")
local monster = {}
monster.description = "orshabaal"
monster.experience = 10000
monster.outfit = {
	lookType = 201
}

monster.health = 20500
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 2916
monster.speed = 380
monster.maxSummons = 4

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
    {name = "demon", chance = 10, interval = 1000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "PRAISED BE MY MASTERS, THE RUTHLESS SEVEN!", yell = false},
	{text = "YOU ARE DOOMED!", yell = false},
	{text = "ORSHABAAL IS BACK!", yell = false},
	{text = "Be prepared for the day my masters will come for you!", yell = false},
	{text = "SOULS FOR ORSHABAAL!", yell = false}
}

monster.loot = {
    {id = "demonic essence", chance = 100000},
	{id = "platinum coin", maxCount = 69, chance = 100000},
	{id = "death ring", chance = 50000},
	{id = "demon horn", chance = 50000},
	{id = "white pearl", maxCount = 15, chance = 33333},
	{id = "devil helmet", chance = 33333},
	{id = "great mana potion", chance = 33333},
	{id = "ring of healing", chance = 33333},
	{id = "small sapphire", maxCount = 8, chance = 33333},
	{id = "ultimate health potion", chance = 33333},
	{id = "black pearl", maxCount = 8, chance = 25000},
	{id = "demon shield", chance = 25000},
	{id = "giant sword", chance = 25000},
	{id = "small emerald", maxCount = 7, chance = 25000},
	{id = "blue gem", chance = 20000},
	{id = "crystal necklace", chance = 20000},
	{id = "great health potion", chance = 20000},
	{id = "mind stone", chance = 20000},
	{id = "protection amulet", subType = 250, chance = 20000},
	{id = "purple tome", chance = 20000},
	{id = "silver amulet", subType = 200, chance = 20000},
	{id = "small amethyst", maxCount = 17, chance = 20000},
	{id = "small diamond", maxCount = 5, chance = 20000},
	{id = "strange symbol", chance = 20000},
	{id = "talon", maxCount = 3, chance = 20000},
	{id = "assassin star", maxCount = 42, chance = 12500},
	{id = "boots of haste", chance = 12500},
	{id = "fire axe", chance = 12500},
	{id = "golden legs", chance = 12500},
	{id = "golden mug", chance = 12500},
	{id = "great spirit potion", chance = 12500},
	{id = "life crystal", chance = 12500},
	{id = "platinum amulet", chance = 12500},
	{id = "two handed sword", chance = 12500},
	{id = "dragon hammer", chance = 6666},
	{id = "gold ingot", chance = 6666},
	{id = "golden sickle", chance = 6666},
	{id = "green gem", chance = 6666},
	{id = "magic light wand", chance = 6666},
	{id = "magic plate armor", chance = 6666},
	{id = "mastermind shield", chance = 6666},
	{id = "might ring", subType = 20, chance = 6666},
	{id = "orb", chance = 6666},
	{id = "Orshabaal's brain", chance = 6666},
	{id = "silver dagger", chance = 6666}
}

monster.attacks = {
	{name = "melee",  attack = 199, skill = 190, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 13, interval = 1000, minDamage = -300, maxDamage = -600, range = 7},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 6, interval = 1000, minDamage = -150, maxDamage = -350, radius = 5, effect = 21},
	{name = "effect",  chance = 6, interval = 1000, minDamage = 0, maxDamage = 0, radius = 5, effect = 10},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 34, interval = 1000, minDamage = -310, maxDamage = -600, target = true, range = 7, radius = 7, shootEffect = 4, effect = 7},
	{name = "firefield",  chance = 10, interval = 1000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 4, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 15, interval = 1000, minDamage = -500, maxDamage = -850, length = 8, radius = 4, shootEffect = 4, effect = 12}
}

monster.defenses = {
	defense = 111,
    armor = 90,
    {name = "combat", type = COMBAT_HEALING,  chance = 9, interval = 1000, minDamage = 1500, maxDamage = 2500, radius = 4, shootEffect = 4, effect = 13},
	{name = "combat", type = COMBAT_HEALING,  chance = 17, interval = 1000, minDamage = 600, maxDamage = 1000, radius = 4, shootEffect = 4, effect = 13},
	{name = "speed",  chance = 5, interval = 1000, minDamage = 0, maxDamage = 0, duration = 7000, speedChange = 1901, radius = 4, shootEffect = 4, effect = 14}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)