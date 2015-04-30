local mType = Game.createMonsterType("Hive Pore")
local monster = {}
monster.description = "a hive pore"
monster.experience = 0
monster.outfit = {
	lookTypeEx = 15467
}

monster.health = 550
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 0
monster.speed = 0
monster.maxSummons = 3

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = false,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.summons = {
    {name = "Lesser Swarmer", chance = 100, interval = 30000},
	{name = "Lesser Swarmer", chance = 100, interval = 30000},
	{name = "Lesser Swarmer", chance = 100, interval = 30000}
}

monster.defenses = {
	defense = 0,
    armor = 0,
    {name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 100, interval = 30000, minDamage = 0, maxDamage = 0, radius = 3, effect = 17}
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
	{type = "physical", condition = false, combat = true},
	{type = "bleed", condition = true, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = true, combat = true},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)