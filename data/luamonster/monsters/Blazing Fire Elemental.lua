local mType = Game.createMonsterType("Blazing Fire Elemental")
local monster = {}
monster.description = "a blazing fire elemental"
monster.experience = 580
monster.outfit = {
	lookType = 49
}

monster.health = 580
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 8964
monster.speed = 220
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 40, chance = 10000},
	{id = "flaming arrow", maxCount = 4, chance = 5000},
	{id = "glimmering soil", chance = 2500},
	{id = "fiery heart", chance = 5475}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 30, minDamage = 0, maxDamage = -100, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 13, interval = 1000, minDamage = -65, maxDamage = -205, radius = 5, effect = 7},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 12, interval = 1000, minDamage = -110, maxDamage = -150, target = true, range = 7, radius = 5, shootEffect = 4, effect = 7},
	{name = "firefield",  chance = 15, interval = 1000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 1, shootEffect = 4, effect = 7}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
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