local mType = Game.createMonsterType("Ghazbaran")
local monster = {}
monster.description = "a ghazbaran"
monster.experience = 15000
monster.outfit = {
	lookFeet = 94,
	lookLegs = 97,
	lookType = 12,
	lookBody = 123
}

monster.health = 60000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6068
monster.speed = 400
monster.maxSummons = 4

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
    {name = "Deathslicer", chance = 20, interval = 4000}
}

monster.voices = {
	interval = 5000,
	chance = 30,
	{text = "COME AND GIVE ME SOME AMUSEMENT", yell = false},
	{text = "IS THAT THE BEST YOU HAVE TO OFFER?", yell = true},
	{text = "I AM GHAZBARAN OF THE TRIANGLE... AND I AM HERE TO CHALLENGE YOU ALL.", yell = true},
	{text = "FLAWLESS VICTORY!", yell = true}
}

monster.loot = {
    {id = "demonic essence", chance = 100000},
	{id = "platinum coin", maxCount = 69, chance = 100000},
	{id = "demon horn", maxCount = 2, chance = 33333},
	{id = "small diamond", maxCount = 5, chance = 25000},
	{id = "death ring", chance = 25000},
	{id = "great spirit potion", chance = 25000},
	{id = "spellscroll of prophecies", chance = 25000},
	{id = "ultimate health potion", chance = 25000},
	{id = "white pearl", maxCount = 15, chance = 25000},
	{id = "great health potion", chance = 20000},
	{id = "ring of healing", chance = 20000},
	{id = "small emerald", maxCount = 10, chance = 25000},
	{id = "blue tome", chance = 20000},
	{id = "great mana potion", chance = 20000},
	{id = "green gem", chance = 20000},
	{id = "mind stone", chance = 20000},
	{id = "scrying crystal ball", chance = 20000},
	{id = "small amethyst", maxCount = 17, chance = 25000},
	{id = "small sapphire", maxCount = 10, chance = 25000},
	{id = "spellbook of warding", chance = 20000},
	{id = "gold ring", chance = 20000},
	{id = "spellbook of dark mysteries", chance = 20000},
	{id = "havoc blade", chance = 16666},
	{id = "oceanborn leviathan armor", chance = 16666},
	{id = "spellbook of lost souls", chance = 16666},
	{id = "blue gem", chance = 14285},
	{id = "ravenwing", chance = 14285},
	{id = "ruthless axe", chance = 14285},
	{id = "assassin star", maxCount = 44, chance = 12500},
	{id = "demon shield", chance = 12500},
	{id = "life crystal", chance = 12500},
	{id = "might ring", subType = 20, chance = 12500},
	{id = "stealth ring", chance = 12500},
	{id = "talon", maxCount = 7, chance = 12500},
	{id = "teddy bear", chance = 12500},
	{id = "black pearl", maxCount = 14, chance = 11111},
	{id = "spellbook of mind control", chance = 11111},
	{id = "strange symbol", chance = 11111},
	{id = "twin axe", chance = 11111},
	{id = "crystal ring", chance = 8333},
	{id = "frozen plate", chance = 8333},
	{id = "glacier kilt", chance = 8333},
	{id = "golden armor", chance = 8333},
	{id = "golden boots", chance = 8333},
	{id = "magic plate armor", chance = 8333}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 10},
	{name = "melee",  attack = 0, skill = 0, interval = 3000, effect = 1},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 30, interval = 4000, minDamage = -100, maxDamage = -800, length = 8, radius = 1, shootEffect = 25, effect = 18},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 3000, minDamage = -200, maxDamage = -480, range = 14, radius = 5, shootEffect = 25, effect = 3},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 4000, minDamage = -100, maxDamage = -650, range = 7, radius = 13, shootEffect = 25, effect = 4},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 18, interval = 4000, minDamage = -200, maxDamage = -600, radius = 14, shootEffect = 25, effect = 2},
	{name = "melee",  attack = 0, skill = 0, interval = 3000, effect = 38}
}

monster.defenses = {
	defense = 65,
    armor = 55,
    {name = "combat", type = COMBAT_HEALING,  chance = 35, interval = 3000, minDamage = 300, maxDamage = 800, radius = 4, shootEffect = 25, effect = 13},
	{name = "speed",  chance = 80, interval = 4000, minDamage = 0, maxDamage = 0, duration = 6000, speedChange = 440, radius = 4, shootEffect = 25, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 1},
	{type = COMBAT_DEATHDAMAGE, percent = 1},
	{type = COMBAT_HOLYDAMAGE, percent = -1}
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
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)