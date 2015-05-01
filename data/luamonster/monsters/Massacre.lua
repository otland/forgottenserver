local mType = Game.createMonsterType("Massacre")
local monster = {}
monster.description = "the massacre"
monster.experience = 9800
monster.outfit = {
	lookType = 244
}

monster.health = 30000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6336
monster.speed = 390
monster.maxSummons = 0

monster.changeTarget = {
    interval = 60000,
	chance = 0
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
	staticAttack = 90
}

monster.loot = {
    {id = "piece of Massacre's shell", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 150, skill = 150, minDamage = 0, maxDamage = -1200, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 24, interval = 2000, minDamage = -280, maxDamage = -500, range = 7, shootEffect = 12}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "speed",  chance = 12, interval = 2000, minDamage = 0, maxDamage = 0, duration = 4000, speedChange = 380, shootEffect = 12, effect = 14}
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
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)