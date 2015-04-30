local mType = Game.createMonsterType("Ferumbras")
local monster = {}
monster.description = "ferumbras"
monster.experience = 12000
monster.outfit = {
	lookType = 229
}

monster.health = 35000
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 6078
monster.speed = 320
monster.maxSummons = 3

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
	targetDistance = 2,
	staticAttack = 90
}

monster.summons = {
    {name = "Demon", chance = 12, interval = 3000}
}

monster.voices = {
	interval = 5000,
	chance = 20,
	{text = "NOONE WILL STOP ME THIS TIME!", yell = true},
	{text = "THE POWER IS MINE!", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "gold coin", maxCount = 25, chance = 100000},
	{id = "giant sword", chance = 16060},
	{id = "mastermind shield", chance = 12050},
	{id = "magic plate armor", chance = 7000},
	{id = "coconut", maxCount = 3, chance = 20000},
	{id = "gold coin", maxCount = 100, chance = 100000},
	{id = "talon", maxCount = 4, chance = 12000},
	{id = "Ferumbras' hat", chance = 100000},
	{id = "small emerald", maxCount = 3, chance = 15000},
	{id = "platinum amulet", chance = 30000},
	{id = "great axe", chance = 6000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 20, interval = 2000, minDamage = -300, maxDamage = -700, range = 7, effect = 14},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 3000, minDamage = -250, maxDamage = -550, radius = 6, effect = 21},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 18, interval = 2000, minDamage = -200, maxDamage = -400, radius = 6, effect = 12},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 16, interval = 2000, minDamage = -225, maxDamage = -375, radius = 6, effect = 14},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 21, interval = 2000, minDamage = -200, maxDamage = -450, radius = 6, effect = 3},
	{name = "effect",  chance = 20, interval = 3000, minDamage = -200, maxDamage = -600, radius = 5, effect = 10},
	{name = "condition", type = CONDITION_FIRE,  chance = 20, interval = 3000, minDamage = -200, maxDamage = -800, target = true, range = 7, radius = 7, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 25, interval = 2000, minDamage = -50, maxDamage = -250, length = 8, radius = 7, shootEffect = 4, effect = 17}
}

monster.defenses = {
	defense = 120,
    armor = 100,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 900, maxDamage = 1500, radius = 7, shootEffect = 4, effect = 15},
	{name = "speed",  chance = 30, interval = 3000, minDamage = 0, maxDamage = 0, duration = 7000, speedChange = 360, radius = 7, shootEffect = 4, effect = 13},
	{name = "invisible",  chance = 20, interval = 4000, minDamage = 0, maxDamage = 0, radius = 7, shootEffect = 4, effect = 13}
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
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)