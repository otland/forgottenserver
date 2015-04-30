local mType = Game.createMonsterType("Draken Warmaster")
local monster = {}
monster.description = "a draken warmaster"
monster.experience = 2400
monster.outfit = {
	lookType = 334
}

monster.health = 4150
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 11107
monster.speed = 220
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Attack aggrezzively! Dezztroy zze intruderzz!", yell = false},
	{text = "Hizzzzz!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 47000},
	{id = "gold coin", maxCount = 100, chance = 50000},
	{id = "platinum coin", maxCount = 5, chance = 50625},
	{id = "great health potion", maxCount = 3, chance = 4850},
	{id = "tower shield", chance = 2035},
	{id = "Zaoan halberd", chance = 7925},
	{id = "ultimate health potion", chance = 4020},
	{id = "meat", chance = 30300},
	{id = "Zaoan shoes", chance = 1900},
	{id = "ring of the sky", chance = 180},
	{id = "small ruby", maxCount = 5, chance = 1525},
	{id = "drakinata", chance = 860},
	{id = "Zaoan legs", chance = 960},
	{id = "Zaoan armor", chance = 790},
	{id = "bone shoulderplate", chance = 12010},
	{id = "warmaster's wristguards", chance = 7000}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -240, maxDamage = -520, length = 4, spread = 3, effect = 6}
}

monster.defenses = {
	defense = 40,
    armor = 40,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 510, maxDamage = 600, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE, percent = 5},
	{type = COMBAT_DEATHDAMAGE, percent = 50},
	{type = COMBAT_ICEDAMAGE, percent = -5}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)