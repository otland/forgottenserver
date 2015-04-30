local mType = Game.createMonsterType("Dark Monk")
local monster = {}
monster.description = "a dark monk"
monster.experience = 145
monster.outfit = {
	lookType = 225
}

monster.health = 190
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20371
monster.speed = 210
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "You are no match to us!", yell = false},
	{text = "This is where your path will end!", yell = false},
	{text = "Your end has come!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 18, chance = 14600},
	{id = "ankh", chance = 900},
	{id = "bread", chance = 20550},
	{id = "brown flask", chance = 380},
	{id = "book of prayers", chance = 1900},
	{id = "lamp", chance = 550},
	{id = "rope belt", chance = 6666},
	{id = "safety pin", chance = 990},
	{id = "life crystal", chance = 990},
	{id = "mana potion", chance = 790},
	{id = "power ring", chance = 120},
	{id = "dark rosary", chance = 10500},
	{id = "scroll", chance = 1790},
	{id = "sandals", chance = 890}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -20, maxDamage = -50, range = 1, effect = 14}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 25, maxDamage = 49, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 6000, speedChange = 300, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -20},
	{type = COMBAT_DEATHDAMAGE, percent = 40},
	{type = COMBAT_HOLYDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
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