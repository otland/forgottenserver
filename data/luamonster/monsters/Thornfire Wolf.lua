local mType = Game.createMonsterType("Thornfire Wolf")
local monster = {}
monster.description = "a thornfire wolf"
monster.experience = 200
monster.outfit = {
	lookType = 414
}

monster.health = 600
monster.maxHealth = monster.health
monster.race = "energy"
monster.corpse = 13859
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 53, chance = 64000},
	{id = "meat", chance = 34000},
	{id = "flaming arrow", maxCount = 8, chance = 15000},
	{id = "wolf paw", chance = 7900},
	{id = "fiery heart", chance = 7900}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 35, minDamage = 0, maxDamage = -68, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = -35, maxDamage = -70, range = 1, shootEffect = 25},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -30, maxDamage = -70, target = true, range = 7, radius = 2, shootEffect = 4, effect = 6},
	{name = "firefield",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 2, shootEffect = 4, effect = 6}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 1000, minDamage = 100, maxDamage = 220, shootEffect = 4, effect = 13}
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
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)