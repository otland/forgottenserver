local mType = Game.createMonsterType("Banshee")
local monster = {}
monster.description = "a banshee"
monster.experience = 900
monster.outfit = {
	lookType = 78
}

monster.health = 1000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6019
monster.speed = 200
monster.runHealth = 500
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Dance for me your dance of death!", yell = false},
	{text = "Let the music play!", yell = false},
	{text = "I will mourn your death!", yell = false},
	{text = "Are you ready to rock?", yell = false},
	{text = "Feel my gentle kiss of death.", yell = false},
	{text = "That's what I call easy listening!", yell = false},
	{text = "IIIIEEEeeeeeehhhHHHH!", yell = true}
}

monster.loot = {
    {id = "candlestick", chance = 70000},
	{id = "gold coin", maxCount = 80, chance = 30000},
	{id = "silver amulet", subType = 200, chance = 8700},
	{id = "simple dress", chance = 6050},
	{id = "hair of a banshee", chance = 4810},
	{id = "petrified scream", chance = 4150},
	{id = "black pearl", chance = 2030},
	{id = "poison dagger", chance = 1350},
	{id = "silver brooch", chance = 1250},
	{id = "white pearl", chance = 1010},
	{id = "lyre", chance = 910},
	{id = "stone skin amulet", subType = 5, chance = 820},
	{id = "ring of healing", chance = 730},
	{id = "blue robe", chance = 700},
	{id = "strong mana potion", chance = 680},
	{id = "spellbook", chance = 520},
	{id = "wedding ring", chance = 460},
	{id = "terra mantle", chance = 340},
	{id = "red robe", chance = 150},
	{id = "life crystal", chance = 70},
	{id = "crystal ring", chance = 60},
	{id = "sweet smelling bait", chance = 40}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -100, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -100, maxDamage = -200, radius = 4, effect = 20},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 5, interval = 2000, minDamage = -55, maxDamage = -350, target = true, range = 1, radius = 1, effect = 39},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 15000, speedChange = -300, range = 7, effect = 14}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 120, maxDamage = 190, effect = 13}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)