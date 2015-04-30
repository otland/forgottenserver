local mType = Game.createMonsterType("Horestis")
local monster = {}
monster.description = "a horestis"
monster.experience = 3500
monster.outfit = {
	lookFeet = 50,
	lookLegs = 40,
	lookHead = 20,
	lookType = 88,
	lookBody = 30
}

monster.health = 6000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6031
monster.speed = 276
monster.maxSummons = 0

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

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Feel my eternal wrath!", yell = false},
	{text = "I might be dead but I'm not gone!", yell = false},
	{text = "Receive a lesson of the past!", yell = false}
}

monster.loot = {
    {id = "scorpion sceptre", chance = 100000},
	{id = "gold coin", maxCount = 243, chance = 50000},
	{id = "platinum coin", maxCount = 5, chance = 33333},
	{id = "pharaoh banner", chance = 16666},
	{id = "scarab coin", maxCount = 5, chance = 14285},
	{id = "hieroglyph banner", chance = 12500},
	{id = "silver brooch", chance = 12500},
	{id = "great mana potion", maxCount = 3, chance = 10000},
	{id = "great health potion", maxCount = 4, chance = 5000},
	{id = "pharaoh sword", chance = 5000},
	{id = "mini mummy", chance = 2500},
	{id = "rusty legs", chance = 2500},
	{id = "twin axe", chance = 2500}
}

monster.attacks = {
	{name = "melee",  attack = 74, skill = 150, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 8, interval = 1000, minDamage = -300, maxDamage = -500, range = 1},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 12, interval = 1000, minDamage = -300, maxDamage = -500, range = 7, shootEffect = 15, effect = 21},
	{name = "speed",  chance = 25, interval = 1000, minDamage = 0, maxDamage = 0, duration = 50000, speedChange = -850, range = 7, shootEffect = 15, effect = 14}
}

monster.defenses = {
	defense = 30,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 1000, minDamage = 200, maxDamage = 400, shootEffect = 15, effect = 13},
	{name = "invisible",  chance = 7, interval = 1000, minDamage = 0, maxDamage = 0, shootEffect = 15, effect = 13},
	{name = "outfit",  chance = 3, interval = 1000, minDamage = 0, maxDamage = 0, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_ICEDAMAGE, percent = 20}
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