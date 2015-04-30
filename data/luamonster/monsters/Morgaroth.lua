local mType = Game.createMonsterType("Morgaroth")
local monster = {}
monster.description = "a morgaroth"
monster.experience = 15000
monster.outfit = {
	lookFeet = 79,
	lookLegs = 79,
	lookType = 12,
	lookBody = 94
}

monster.health = 55000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6068
monster.speed = 400
monster.maxSummons = 6

monster.changeTarget = {
    interval = 10000,
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
	staticAttack = 98
}

monster.summons = {
    {name = "Demon", chance = 33, interval = 4000}
}

monster.voices = {
	interval = 5000,
	chance = 30,
	{text = "I AM MORGAROTH, LORD OF THE TRIANGLE... AND YOU ARE LOST!", yell = true},
	{text = "MY SEED IS FEAR AND MY HARVEST ARE YOUR SOULS!", yell = true},
	{text = "ZATHROTH! LOOK AT THE DESTRUCTION I AM CAUSING IN YOUR NAME!", yell = true},
	{text = "THE TRIANGLE OF TERROR WILL RISE!", yell = true}
}

monster.loot = {
    {id = "demonic essence", chance = 50000},
	{id = "platinum coin", maxCount = 74, chance = 50000},
	{id = "great mana potion", chance = 50000},
	{id = "green gem", chance = 33333},
	{id = "small amethyst", maxCount = 18, chance = 33333},
	{id = "the devileye", chance = 33333},
	{id = "ring of healing", chance = 25000},
	{id = "small sapphire", maxCount = 8, chance = 25000},
	{id = "the ironworker", chance = 25000},
	{id = "chain bolter", chance = 20000},
	{id = "dark lord's cape", chance = 20000},
	{id = "demon horn", maxCount = 2, chance = 20000},
	{id = "magic plate armor", chance = 20000},
	{id = "might ring", subType = 20, chance = 20000},
	{id = "red tome", chance = 20000},
	{id = "small emerald", maxCount = 6, chance = 20000},
	{id = "ultimate health potion", chance = 20000},
	{id = "assassin star", maxCount = 28, chance = 16666},
	{id = "black pearl", maxCount = 15, chance = 16666},
	{id = "death ring", chance = 16666},
	{id = "demonbone", chance = 16666},
	{id = "double axe", chance = 16666},
	{id = "fireborn giant armor", chance = 16666},
	{id = "golden mug", chance = 16666},
	{id = "mind stone", chance = 16666},
	{id = "Morgaroth's heart", chance = 16666},
	{id = "obsidian truncheon", chance = 16666},
	{id = "talon", maxCount = 7, chance = 16666},
	{id = "blue gem", chance = 11111},
	{id = "giant sword", chance = 11111},
	{id = "golden legs", chance = 11111},
	{id = "life crystal", chance = 11111},
	{id = "life crystal", chance = 11111},
	{id = "scrying crystal ball", chance = 11111},
	{id = "royal crossbow", chance = 11111},
	{id = "stealth ring", chance = 11111},
	{id = "steel boots", chance = 11111},
	{id = "strange symbol", chance = 11111},
	{id = "teddy bear", chance = 11111},
	{id = "white pearl", maxCount = 13, chance = 11111},
	{id = "crystal ring", chance = 5882},
	{id = "dragon robe", chance = 5882},
	{id = "energy ring", chance = 5882},
	{id = "gold ring", chance = 5882},
	{id = "great health potion", chance = 5882},
	{id = "great spirit potion", chance = 5882}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 35, interval = 3000, minDamage = -700, maxDamage = -1200, target = true, range = 7, radius = 7, shootEffect = 4, effect = 7},
	{name = "melee",  attack = 0, skill = 0, interval = 1800, effect = 10},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 30, interval = 3000, minDamage = -300, maxDamage = -1000, length = 8, radius = 5, shootEffect = 4, effect = 12},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2500, minDamage = -200, maxDamage = -480, range = 7, radius = 5, shootEffect = 4, effect = 15},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = -250, maxDamage = -500, range = 7, radius = 13, shootEffect = 4, effect = 20},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = -200, maxDamage = -450, radius = 14, shootEffect = 4, effect = 2},
	{name = "melee",  attack = 0, skill = 0, interval = 3000, effect = 13}
}

monster.defenses = {
	defense = 65,
    armor = 55,
    {name = "combat", type = COMBAT_HEALING,  chance = 35, interval = 3000, minDamage = 300, maxDamage = 800, radius = 3, shootEffect = 4, effect = 13},
	{name = "speed",  chance = 80, interval = 4000, minDamage = 0, maxDamage = 0, duration = 6000, speedChange = 440, radius = 3, shootEffect = 4, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 1},
	{type = COMBAT_ENERGYDAMAGE, percent = 1},
	{type = COMBAT_DEATHDAMAGE, percent = 80},
	{type = COMBAT_ICEDAMAGE, percent = -5}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)