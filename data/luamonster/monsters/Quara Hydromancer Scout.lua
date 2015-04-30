local mType = Game.createMonsterType("Quara Hydromancer Scout")
local monster = {}
monster.description = "a quara hydromancer scout"
monster.experience = 800
monster.outfit = {
	lookType = 47
}

monster.health = 1100
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6066
monster.speed = 240
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
    {id = "gold coin", maxCount = 90, chance = 40000},
	{id = "gold coin", maxCount = 49, chance = 49000},
	{id = "quara eye", chance = 12720},
	{id = "fish", maxCount = 2, chance = 18930},
	{id = "white pearl", chance = 3850},
	{id = "black pearl", chance = 2660},
	{id = "small emerald", maxCount = 2, chance = 6000},
	{id = "wand of cosmic energy", chance = 890},
	{id = "fish fin", chance = 1630},
	{id = "ring of healing", chance = 590},
	{id = "knight armor", chance = 890},
	{id = "obsidian lance", chance = 1180},
	{id = "shrimp", maxCount = 4, chance = 8400}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 10, interval = 2000, minDamage = -100, maxDamage = -180, length = 8, effect = 17},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 10, interval = 2000, minDamage = -90, maxDamage = -150, radius = 3, effect = 26},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -170, maxDamage = -240, length = 8, radius = 3, effect = 26},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -170, range = 7, radius = 3, effect = 14},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 15000, speedChange = -600, range = 7, radius = 3, effect = 14}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 100, maxDamage = 120, radius = 3, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)