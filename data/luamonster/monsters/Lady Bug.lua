local mType = Game.createMonsterType("Lady Bug")
local monster = {}
monster.description = "a lady bug"
monster.experience = 70
monster.outfit = {
	lookType = 448
}

monster.health = 255
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 15272
monster.speed = 200
monsters.runHealth = 60
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
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
	staticAttack = 95
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Nee pah!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 40, chance = 65000}
}

monster.attacks = {
	{name = "melee",  attack = 5, skill = 5, minDamage = 0, maxDamage = -4, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -20, range = 1, shootEffect = 15},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -20, range = 7, shootEffect = 15}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 5},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = -5}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)