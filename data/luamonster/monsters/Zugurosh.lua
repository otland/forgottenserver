local mType = Game.createMonsterType("Zugurosh")
local monster = {}
monster.description = "zugurosh"
monster.experience = 10000
monster.outfit = {
	lookFeet = 91,
	lookLegs = 57,
	lookHead = 2,
	lookType = 12,
	lookBody = 35
}

monster.health = 95000
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 6068
monster.speed = 340
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 15
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
	staticAttack = 85
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "You will run out of resources soon enough!!", yell = true},
	{text = "One little mistake and your all are mine!", yell = false},
	{text = "I sense your strength fading!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "gold coin", maxCount = 50, chance = 100000},
	{id = "death ring", chance = 7777},
	{id = "boots of haste", chance = 4444},
	{id = "giant sword", chance = 3000},
	{id = "fire axe", chance = 5555},
	{id = "silver dagger", chance = 10000},
	{id = "knight legs", chance = 6666},
	{id = "talon", maxCount = 11, chance = 6000},
	{id = "devil helmet", chance = 9700},
	{id = "small emerald", maxCount = 3, chance = 1428},
	{id = "platinum amulet", chance = 2333},
	{id = "blue gem", chance = 4444},
	{id = "golden legs", chance = 6500},
	{id = "demon shield", chance = 9999}
}

monster.attacks = {
	{name = "melee",  attack = 110, skill = 120, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 20, interval = 3000, minDamage = -250, maxDamage = -850, target = true, radius = 4, effect = 6},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 15, interval = 2000, minDamage = -100, maxDamage = -350, radius = 4, effect = 39},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 13, interval = 1000, minDamage = -60, maxDamage = -230, radius = 5, effect = 54},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -500, length = 7, radius = 5, effect = 18}
}

monster.defenses = {
	defense = 55,
    armor = 45,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 400, maxDamage = 900, radius = 5, effect = 15},
	{name = "invisible",  chance = 5, interval = 1000, minDamage = 0, maxDamage = 0, radius = 5, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = -8},
	{type = COMBAT_FIREDAMAGE, percent = 25},
	{type = COMBAT_ICEDAMAGE, percent = -15}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)