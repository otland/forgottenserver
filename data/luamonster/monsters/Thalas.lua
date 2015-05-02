local mType = Game.createMonsterType("Thalas")
local monster = {}
monster.description = "a thalas"
monster.experience = 2950
monster.outfit = {
	lookType = 90
}

monster.health = 4100
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6025
monster.speed = 320
monster.maxSummons = 8

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
    {name = "Slime2", chance = 30000000, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "You will become a feast for my maggots.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 80, chance = 50000},
	{id = "gold coin", maxCount = 80, chance = 50000},
	{id = "small emerald", maxCount = 3, chance = 7000},
	{id = "poison dagger", chance = 7000},
	{id = "time ring", chance = 7000},
	{id = "stealth ring", chance = 1500},
	{id = "great health potion", chance = 1500},
	{id = "serpent sword", chance = 500},
	{id = "green gem", chance = 500},
	{id = "djinn blade", chance = 200},
	{id = "cobrafang dagger", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 120, skill = 140, minDamage = 0, maxDamage = -900, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 25, interval = 2000, minDamage = -150, maxDamage = -650, range = 7, shootEffect = 15, effect = 21},
	{name = "melee",  attack = 0, skill = 0, minDamage = -150, maxDamage = -650, interval = 3000, effect = 21},
	{name = "speed",  chance = 6, interval = 1000, minDamage = 0, maxDamage = 0, duration = 20000, speedChange = -800, range = 7, shootEffect = 15, effect = 14},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 1000, minDamage = -34, maxDamage = -35, radius = 5, shootEffect = 15, effect = 21},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 17, interval = 3000, minDamage = -55, maxDamage = -550, length = 8, spread = 3, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 30,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 1000, minDamage = 150, maxDamage = 450, shootEffect = 15, effect = 13}
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