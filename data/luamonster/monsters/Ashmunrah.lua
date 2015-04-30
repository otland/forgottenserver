local mType = Game.createMonsterType("Ashmunrah")
local monster = {}
monster.description = "an ashmunrah"
monster.experience = 3100
monster.outfit = {
	lookType = 87
}

monster.health = 5000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6031
monster.speed = 320
monster.maxSummons = 4

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

monster.summons = {
    {name = "Ancient Scarab", chance = 2000, interval = 1000},
	{name = "Green Djinn", chance = 1000, interval = 1000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "No mortal or undead will steal my secrets!", yell = false},
	{text = "Ahhhh all those long years.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 80, chance = 50000},
	{id = "gold coin", maxCount = 60, chance = 50000},
	{id = "silver brooch", chance = 7000},
	{id = "great mana potion", chance = 1500},
	{id = "might ring", subType = 20, chance = 1000},
	{id = "crown armor", chance = 80000},
	{id = "holy scarab", chance = 400}
}

monster.attacks = {
	{name = "melee",  attack = 200, skill = 90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 7, interval = 3000, minDamage = -100, maxDamage = -700, range = 1},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 12, interval = 2000, minDamage = -100, maxDamage = -500, range = 7, shootEffect = 15, effect = 21},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 12, interval = 3000, minDamage = -120, maxDamage = -750, range = 7, shootEffect = 5, effect = 18},
	{name = "speed",  chance = 25, interval = 3000, minDamage = 0, maxDamage = 0, duration = 50000, speedChange = -650, range = 7, shootEffect = 5, effect = 14},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 18, interval = 2000, minDamage = -50, maxDamage = -550, length = 8, spread = 3, shootEffect = 5, effect = 8}
}

monster.defenses = {
	defense = 30,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 1000, minDamage = 200, maxDamage = 400, shootEffect = 5, effect = 13},
	{name = "invisible",  chance = 7, interval = 1000, minDamage = 0, maxDamage = 0, shootEffect = 5, effect = 13},
	{name = "outfit",  chance = 3, interval = 1000, minDamage = 0, maxDamage = 0, shootEffect = 5, effect = 13}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)