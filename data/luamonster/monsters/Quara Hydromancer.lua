local mType = Game.createMonsterType("Quara Hydromancer")
local monster = {}
monster.description = "a quara hydromancer"
monster.experience = 800
monster.outfit = {
	lookType = 47
}

monster.health = 1100
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6066
monster.speed = 250
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
	{text = "Qua hah tsh!", yell = false},
	{text = "Teech tsha tshul!", yell = false},
	{text = "Quara tsha Fach!", yell = false},
	{text = "Tssssha Quara!", yell = false},
	{text = "Blubber.", yell = false},
	{text = "Blup.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 50, chance = 50000},
	{id = "gold coin", maxCount = 60, chance = 50000},
	{id = "quara eye", chance = 15930},
	{id = "shrimp", maxCount = 5, chance = 4545},
	{id = "black pearl", chance = 3150},
	{id = "white pearl", chance = 5250},
	{id = "small emerald", maxCount = 2, chance = 5111},
	{id = "wand of cosmic energy", chance = 900},
	{id = "great mana potion", chance = 3100},
	{id = "fish fin", chance = 1280},
	{id = "ring of healing", chance = 1008},
	{id = "knight armor", chance = 200}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 30, minDamage = 0, maxDamage = -80, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 10, interval = 2000, minDamage = -100, maxDamage = -180, length = 8, effect = 26},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 10, interval = 2000, minDamage = -90, maxDamage = -150, radius = 3, effect = 26},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -170, maxDamage = -240, length = 8, effect = 17},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -170, range = 7, effect = 14},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 15000, speedChange = -600, range = 7, effect = 14}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 100, maxDamage = 120, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -25},
	{type = COMBAT_EARTHDAMAGE, percent = -10}
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
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)