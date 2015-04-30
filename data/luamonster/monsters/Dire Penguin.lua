local mType = Game.createMonsterType("Dire Penguin")
local monster = {}
monster.description = "dire penguin"
monster.experience = 119
monster.outfit = {
	lookType = 250
}

monster.health = 173
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7334
monster.speed = 200
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 95
}

monster.loot = {
    {id = "gold coin", maxCount = 10, chance = 18000},
	{id = "fish", maxCount = 4, chance = 8000},
	{id = "green perch", chance = 7000},
	{id = "rainbow trout", chance = 8000},
	{id = "dragon hammer", chance = 200}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -35, target = true, range = 7, radius = 1, shootEffect = 10, effect = 5},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 9000, speedChange = -600, range = 7, radius = 4, shootEffect = 10, effect = 3}
}

monster.defenses = {
	defense = 10,
    armor = 10,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 3000, speedChange = 310, range = 7, radius = 4, shootEffect = 10, effect = 14}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
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