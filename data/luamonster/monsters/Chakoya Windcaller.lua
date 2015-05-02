local mType = Game.createMonsterType("Chakoya Windcaller")
local monster = {}
monster.description = "a chakoya windcaller"
monster.experience = 48
monster.outfit = {
	lookType = 260
}

monster.health = 84
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7320
monster.speed = 200
monster.maxSummons = 0

monster.changeTarget = {
    interval = 60000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Mupi! Si siyoqua jinuma!", yell = false},
	{text = "Siqsiq ji jusipa!", yell = false},
	{text = "Jagura taluka taqua!", yell = false},
	{text = "Quatu nguraka!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 26, chance = 50000},
	{id = "fish", maxCount = 3, chance = 29000},
	{id = "bone shield", chance = 950},
	{id = "green perch", chance = 60},
	{id = "rainbow trout", chance = 60},
	{id = "northern pike", chance = 60},
	{id = "brass helmet", chance = 3250}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 12, minDamage = 0, maxDamage = -22, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 15, interval = 2000, minDamage = -16, maxDamage = -32, range = 7, shootEffect = 29},
	{name = "condition", type = CONDITION_FREEZING,  chance = 10, interval = 2000, minDamage = -130, maxDamage = -160, radius = 3, shootEffect = 29, effect = 42},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 10, interval = 2000, minDamage = -9, maxDamage = -30, length = 5, spread = 2, shootEffect = 29, effect = 42}
}

monster.defenses = {
	defense = 10,
    armor = 10,
    {name = "invisible",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 29, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -15},
	{type = COMBAT_HOLYDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 50}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
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
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)