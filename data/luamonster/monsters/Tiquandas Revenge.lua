local mType = Game.createMonsterType("Tiquandas Revenge")
local monster = {}
monster.description = "a tiquandas revenge"
monster.experience = 1755
monster.outfit = {
	lookType = 120
}

monster.health = 2000
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 6047
monster.speed = 440
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

monster.loot = {
    {id = "gold coin", maxCount = 10, chance = 100000},
	{id = "gold coin", maxCount = 100, chance = 100000},
	{id = "meat", maxCount = 50, chance = 33333},
	{id = "ham", maxCount = 8, chance = 20000},
	{id = "small diamond", maxCount = 3, chance = 33333},
	{id = "seeds", chance = 4000},
	{id = "mandrake", chance = 100000},
	{id = "worm", maxCount = 10, chance = 50000}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 70, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 25, interval = 1000, minDamage = -60, maxDamage = -200, range = 7, shootEffect = 15, effect = 17},
	{name = "speed",  chance = 34, interval = 1000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = -850, range = 7, shootEffect = 15, effect = 17},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 12, interval = 1000, minDamage = -40, maxDamage = -130, radius = 3, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 31,
    armor = 30,
    {name = "ultimate healing",  chance = 35, interval = 1200, minDamage = 600, maxDamage = 800, radius = 3, shootEffect = 15, effect = 21}
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