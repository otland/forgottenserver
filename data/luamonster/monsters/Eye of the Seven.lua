local mType = Game.createMonsterType("Eye of the Seven")
local monster = {}
monster.description = "an eye of the seven"
monster.experience = 0
monster.outfit = {
	lookTypeEx = 1560
}

monster.health = 1
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 0
monster.speed = 0
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 20
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
	{name = "melee",  attack = 50, skill = 190, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 9, interval = 1000, minDamage = 0, maxDamage = -500, range = 6}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = false, combat = false},
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