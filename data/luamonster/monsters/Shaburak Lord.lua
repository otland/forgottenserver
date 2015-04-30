local mType = Game.createMonsterType("Shaburak Lord")
local monster = {}
monster.description = "a shaburak lord"
monster.experience = 1200
monster.outfit = {
	lookType = 409
}

monster.health = 2100
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 13958
monster.speed = 230
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
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "GREEN IS MEAN!", yell = true},
	{text = "WE RULE!", yell = true},
	{text = "POWER TO THE SHABURAK!", yell = true},
	{text = "DEATH TO THE ASKARAK!", yell = false},
	{text = "ONLY WE ARE TRUE DEMONS!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 78, chance = 50000},
	{id = "platinum coin", maxCount = 2, chance = 33333},
	{id = "strong mana potion", chance = 7000},
	{id = "strong health potion", chance = 7000},
	{id = "small ruby", maxCount = 5, chance = 6120},
	{id = "brown mushroom", chance = 5346},
	{id = "energy ring", chance = 1652},
	{id = "bullseye potion", chance = 400},
	{id = "wand of inferno", chance = 800},
	{id = "magic sulphur", chance = 620},
	{id = "magma coat", chance = 100},
	{id = "steel boots", chance = 160}
}

monster.attacks = {
	{name = "melee",  attack = 65, skill = 67, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 20, interval = 2000, minDamage = -30, maxDamage = -75, range = 7, radius = 6, shootEffect = 4, effect = 37},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -80, maxDamage = -170, length = 4, spread = 3, radius = 6, shootEffect = 4, effect = 16},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -140, maxDamage = -200, length = 4, radius = 6, shootEffect = 4, effect = 37},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, duration = 15000, speedChange = -650, radius = 1, shootEffect = 4, effect = 14}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 5, interval = 2000, minDamage = 100, maxDamage = 150, radius = 1, shootEffect = 4, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 50},
	{type = COMBAT_EARTHDAMAGE, percent = -30},
	{type = COMBAT_ICEDAMAGE, percent = 50}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)