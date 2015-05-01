local mType = Game.createMonsterType("Scarab")
local monster = {}
monster.description = "a scarab"
monster.experience = 120
monster.outfit = {
	lookType = 83
}

monster.health = 320
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 6024
monster.speed = 170
monster.runHealth = 80
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 52, chance = 86800},
	{id = "meat", maxCount = 2, chance = 40000},
	{id = "piece of scarab shell", chance = 4950},
	{id = "scarab coin", chance = 1098},
	{id = "small amethyst", chance = 540},
	{id = "small emerald", chance = 413},
	{id = "daramian mace", chance = 245}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 65, minDamage = 0, maxDamage = -75, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -35, range = 1, shootEffect = 15},
	{name = "poisonfield",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 1, shootEffect = 15}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 200, shootEffect = 15, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 15},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = -18},
	{type = COMBAT_ICEDAMAGE, percent = 20}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
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