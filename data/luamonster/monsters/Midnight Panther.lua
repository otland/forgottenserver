local mType = Game.createMonsterType("Midnight Panther")
local monster = {}
monster.description = "a midnight panther"
monster.experience = 900
monster.outfit = {
	lookType = 385
}

monster.health = 1200
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 13327
monster.speed = 250
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 80
}

monster.loot = {
    {id = "gold coin", maxCount = 57, chance = 100000},
	{id = "meat", maxCount = 4, chance = 25000},
	{id = "panther head", chance = 12500},
	{id = "life ring", chance = 12500},
	{id = "panther paw", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 36, skill = 40, minDamage = 0, maxDamage = -90, interval = 1500, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 15, interval = 2000, minDamage = -75, maxDamage = -215, range = 7, shootEffect = 5, effect = 38}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 370, shootEffect = 5, effect = 14},
	{name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 50, maxDamage = 125, shootEffect = 5, effect = 13},
	{name = "invisible",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 5, effect = 13}
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)