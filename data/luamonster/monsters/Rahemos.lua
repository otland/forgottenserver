local mType = Game.createMonsterType("Rahemos")
local monster = {}
monster.description = "a rahemos"
monster.experience = 3100
monster.outfit = {
	lookType = 87
}

monster.health = 3700
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6031
monster.speed = 320
monster.maxSummons = 1

monster.changeTarget = {
    interval = 5000,
	chance = 8
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

monster.summons = {
    {name = "Demon", chance = 12, interval = 1000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "It's a kind of magic.", yell = false},
	{text = "Abrah Kadabrah!", yell = false},
	{text = "Nothing hidden in my wrappings.", yell = false},
	{text = "It's not a trick, it's Rahemos.", yell = false},
	{text = "Meet my friend from hell.", yell = false},
	{text = "I will make you believe in magic.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 50000},
	{id = "gold coin", maxCount = 80, chance = 50000},
	{id = "gold coin", maxCount = 60, chance = 50000},
	{id = "small amethyst", maxCount = 3, chance = 7000},
	{id = "ring of healing", chance = 7000},
	{id = "great mana potion", chance = 7000},
	{id = "magician hat", chance = 300},
	{id = "orb", chance = 500},
	{id = "necrotic rod", chance = 500},
	{id = "violet gem", chance = 500},
	{id = "crystal wand", chance = 500},
	{id = "twin axe", chance = 200},
	{id = "ancient rune", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 150, skill = 90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 7, interval = 3000, minDamage = -75, maxDamage = -750, range = 1},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 20, interval = 2000, minDamage = -60, maxDamage = -600, range = 7, shootEffect = 5, effect = 12},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 3000, minDamage = -60, maxDamage = -600, range = 7, shootEffect = 32, effect = 18},
	{name = "speed",  chance = 12, interval = 1000, minDamage = 0, maxDamage = 0, duration = 60000, speedChange = -650, radius = 6, shootEffect = 32, effect = 21},
	{name = "drunk",  chance = 8, interval = 1000, minDamage = 0, maxDamage = 0, range = 7, radius = 6, shootEffect = 5, effect = 38},
	{name = "outfit",  chance = 15, interval = 1000, minDamage = 0, maxDamage = 0, range = 7, radius = 6, shootEffect = 5, effect = 13}
}

monster.defenses = {
	defense = 35,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 1000, minDamage = 200, maxDamage = 500, radius = 6, shootEffect = 5, effect = 13},
	{name = "outfit",  chance = 5, interval = 1000, minDamage = 0, maxDamage = 0, radius = 6, shootEffect = 5, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 92},
	{type = COMBAT_HOLYDAMAGE, percent = -25},
	{type = COMBAT_FIREDAMAGE, percent = 94}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)