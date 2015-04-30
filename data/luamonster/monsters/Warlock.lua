local mType = Game.createMonsterType("Warlock")
local monster = {}
monster.description = "a warlock"
monster.experience = 4000
monster.outfit = {
	lookFeet = 128,
	lookLegs = 128,
	lookHead = 19,
	lookType = 130,
	lookAddons = 1,
	lookBody = 71
}

monster.health = 3500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20527
monster.speed = 220
monster.maxSummons = 1

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
	targetDistance = 4,
	staticAttack = 90
}

monster.summons = {
    {name = "stone golem", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Learn the secret of our magic! YOUR death!", yell = false},
	{text = "Even a rat is a better mage than you.", yell = false},
	{text = "We don't like intruders!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 80, chance = 29340},
	{id = "cherry", maxCount = 4, chance = 19000},
	{id = "bread", chance = 9000},
	{id = "poison dagger", chance = 7600},
	{id = "candlestick", chance = 1500},
	{id = "crystal ring", chance = 700},
	{id = "dark mushroom", chance = 3000},
	{id = "energy ring", chance = 2200},
	{id = "inkwell", chance = 1000},
	{id = "mind stone", chance = 2000},
	{id = "skull staff", chance = 6370},
	{id = "talon", chance = 1150},
	{id = "great health potion", chance = 5190},
	{id = "great mana potion", chance = 4760},
	{id = "blue robe", chance = 1410},
	{id = "assassin star", maxCount = 4, chance = 3500},
	{id = "small sapphire", chance = 1190},
	{id = "red tome", chance = 300},
	{id = "ring of the sky", chance = 430},
	{id = "stone skin amulet", subType = 5, chance = 330},
	{id = "lightning robe", chance = 1000},
	{id = "golden armor", chance = 240},
	{id = "piggy bank", chance = 60},
	{id = "luminous orb", chance = 510},
	{id = "assassin star", maxCount = 4, chance = 3470}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 42, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 25, interval = 2000, minDamage = -90, maxDamage = -180, range = 7, shootEffect = 5},
	{name = "warlock skill reducer",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, range = 5, shootEffect = 5},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -120, range = 7, shootEffect = 5},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 20, interval = 2000, minDamage = -50, maxDamage = -180, target = true, range = 7, radius = 3, shootEffect = 7, effect = 7},
	{name = "firefield",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 2, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 10, interval = 2000, minDamage = -150, maxDamage = -230, length = 8, radius = 2, shootEffect = 4, effect = 41},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 20000, speedChange = -600, range = 7, radius = 2, shootEffect = 4, effect = 14}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 2000, minDamage = 100, maxDamage = 225, radius = 2, shootEffect = 4, effect = 13},
	{name = "invisible",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, radius = 2, shootEffect = 4, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 95},
	{type = COMBAT_HOLYDAMAGE, percent = -5}
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
	{type = "energy", condition = true, combat = true},
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