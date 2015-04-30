local mType = Game.createMonsterType("Lord of the Elements")
local monster = {}
monster.description = "lord of the elements"
monster.experience = 8000
monster.outfit = {
	lookType = 290
}

monster.health = 8000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 9009
monster.speed = 370
monster.maxSummons = 2

monster.changeTarget = {
    interval = 5000,
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

monster.summons = {
    {name = "Blistering Fire Elemental", chance = 50, interval = 4000},
	{name = "Jagged Earth Elemental", chance = 50, interval = 4000},
	{name = "Roaring Water Elemental", chance = 50, interval = 4000},
	{name = "Overcharged Energy Elemental", chance = 50, interval = 4000}
}

monster.voices = {
	interval = 5000,
	chance = 30,
	{text = "WHO DARES CALLING ME?", yell = true},
	{text = "I'LL FREEZE YOU THEN I CRUSH YOU!", yell = true}
}

monster.loot = {
    {id = "platinum coin", maxCount = 9, chance = 50000},
	{id = "gold ingot", chance = 25000},
	{id = "small emerald", maxCount = 4, chance = 11111},
	{id = "small amethyst", maxCount = 3, chance = 11111},
	{id = "small ruby", maxCount = 4, chance = 11111},
	{id = "small sapphire", maxCount = 4, chance = 7142},
	{id = "earthborn titan armor", chance = 2063}
}

monster.attacks = {
	{name = "melee",  attack = 115, skill = 110, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 1000, minDamage = 100, maxDamage = 195, effect = 13},
	{name = "outfit",  chance = 40, interval = 1500, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "outfit",  chance = 40, interval = 1500, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "outfit",  chance = 40, interval = 1500, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "outfit",  chance = 40, interval = 1500, minDamage = 0, maxDamage = 0, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 1},
	{type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = 45},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_ICEDAMAGE, percent = 30}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = true, combat = true},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)