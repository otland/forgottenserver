local mType = Game.createMonsterType("Zoralurk")
local monster = {}
monster.description = "zoralurk"
monster.experience = 30000
monster.outfit = {
	lookFeet = 94,
	lookLegs = 86,
	lookType = 12,
	lookBody = 98
}

monster.health = 55000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6068
monster.speed = 400
monster.maxSummons = 2

monster.changeTarget = {
    interval = 10000,
	chance = 20
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
	staticAttack = 98
}

monster.summons = {
    {name = "demon", chance = 50, interval = 4000}
}

monster.voices = {
	interval = 5000,
	chance = 30,
	{text = "I AM ZORALURK, THE DEMON WITH A THOUSAND FACES!", yell = true},
	{text = "BRING IT, COCKROACHES!", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "gold coin", maxCount = 90, chance = 50000},
	{id = "white pearl", maxCount = 5, chance = 10000},
	{id = "bright sword", chance = 20000},
	{id = "worn soft boots", chance = 16000},
	{id = "patched boots", chance = 7000},
	{id = "bright sword", chance = 20000},
	{id = "giant sword", chance = 60000},
	{id = "boots of haste", chance = 16033},
	{id = "warlord sword", chance = 6000}
}

monster.attacks = {
	{name = "melee",  attack = 135, skill = 140, minDamage = 0, maxDamage = -1013, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 12, interval = 1000, minDamage = -600, maxDamage = -900, radius = 7, effect = 12},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 12, interval = 1000, minDamage = -400, maxDamage = -800, radius = 7, effect = 46},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 25, interval = 2000, minDamage = -500, maxDamage = -800, range = 7, effect = 13},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 35, interval = 3000, minDamage = -200, maxDamage = -600, target = true, range = 7, radius = 7, shootEffect = 4, effect = 7}
}

monster.defenses = {
	defense = 65,
    armor = 55,
    {name = "combat", type = COMBAT_HEALING,  chance = 35, interval = 2000, minDamage = 300, maxDamage = 800, shootEffect = 4, effect = 13},
	{name = "speed",  chance = 80, interval = 4000, minDamage = 0, maxDamage = 0, duration = 6000, speedChange = 440, shootEffect = 4, effect = 14},
	{name = "outfit",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 4, effect = 27},
	{name = "outfit",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 4, effect = 27},
	{name = "outfit",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 4, effect = 27},
	{name = "outfit",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 4, effect = 27},
	{name = "outfit",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 4, effect = 27}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = true, combat = true},
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