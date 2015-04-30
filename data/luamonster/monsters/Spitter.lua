local mType = Game.createMonsterType("Spitter")
local monster = {}
monster.description = "a spitter"
monster.experience = 1100
monster.outfit = {
	lookType = 461
}

monster.health = 1500
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 15392
monster.speed = 190
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
	staticAttack = 95
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 90, chance = 50000},
	{id = "platinum coin", chance = 75250},
	{id = "spitter nose", chance = 18000},
	{id = "compound eye", chance = 15000},
	{id = "great mana potion", chance = 8000},
	{id = "small amethyst", maxCount = 2, chance = 8000},
	{id = "brown mushroom", maxCount = 3, chance = 7500},
	{id = "great health potion", chance = 5000},
	{id = "time ring", chance = 2400},
	{id = "crystal sword", chance = 2000},
	{id = "calopteryx cape", chance = 240},
	{id = "green gem", chance = 210},
	{id = "mastermind potion", chance = 310},
	{id = "crusader helmet", chance = 230},
	{id = "platinum amulet", chance = 260},
	{id = "grasshopper legs", chance = 130}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 50, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -100, maxDamage = -160, target = true, range = 7, radius = 3, shootEffect = 15, effect = 21},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 15000, speedChange = -600, range = 7, radius = 3, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 400, radius = 3, shootEffect = 15, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = 5},
	{type = COMBAT_FIREDAMAGE, percent = 5}
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)