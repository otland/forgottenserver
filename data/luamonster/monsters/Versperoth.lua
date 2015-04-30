local mType = Game.createMonsterType("Versperoth")
local monster = {}
monster.description = "versperoth"
monster.experience = 20000
monster.outfit = {
	lookType = 295
}

monster.health = 100000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 0
monster.speed = 0
monster.maxSummons = 7

monster.changeTarget = {
    interval = 5000,
	chance = 8
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

monster.summons = {
    {name = "Minion of Vesperoth", chance = 16, interval = 2000}
}

monster.voices = {
	interval = 2500,
	chance = 10,
	{text = "GrrroaR!", yell = false},
	{text = "GROWL!", yell = false},
	{text = "Waaaah!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 120, interval = 1200, effect = 0},
	{name = "firefield",  chance = 30, interval = 1200, minDamage = 0, maxDamage = 0, range = 7, radius = 4, shootEffect = 4},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 40, interval = 1000, minDamage = -700, maxDamage = -1615, range = 5, length = 8, spread = 3, radius = 4, shootEffect = 4, effect = 7},
	{name = "melee",  attack = 100, skill = 120, interval = 1000, effect = 5}
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)