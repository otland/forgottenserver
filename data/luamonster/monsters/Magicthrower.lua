local mType = Game.createMonsterType("Magicthrower")
local monster = {}
monster.description = "a magicthrower"
monster.experience = 0
monster.outfit = {
	lookTypeEx = 1551
}

monster.health = 9950
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 0
monster.speed = 0
monsters.runHealth = 100
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = false,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = false,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.attacks = {
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 100, interval = 2000, minDamage = 0, maxDamage = -100, range = 7, shootEffect = 15}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
	{type = "manadrain", condition = false, combat = true},
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