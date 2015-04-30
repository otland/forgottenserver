local mType = Game.createMonsterType("Tarantula")
local monster = {}
monster.description = "a tarantula"
monster.experience = 120
monster.outfit = {
	lookType = 219
}

monster.health = 225
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 6060
monster.speed = 210
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 40, chance = 79000},
	{id = "tarantula egg", chance = 10000},
	{id = "brass legs", chance = 3050},
	{id = "plate shield", chance = 2000},
	{id = "steel helmet", chance = 990},
	{id = "time ring", chance = 120},
	{id = "spider fangs", chance = 4820}
}

monster.attacks = {
	{name = "melee",  attack = 45, skill = 30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 1, radius = 1, shootEffect = 15, effect = 47}
}

monster.defenses = {
	defense = 10,
    armor = 10,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 220, radius = 1, shootEffect = 15, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = -15}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = false, combat = false},
	{type = "outfit", condition = true, combat = false},
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