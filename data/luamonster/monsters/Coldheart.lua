local mType = Game.createMonsterType("Coldheart")
local monster = {}
monster.description = "coldheart"
monster.experience = 3500
monster.outfit = {
	lookType = 261
}

monster.health = 7000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 7282
monster.speed = 195
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 9
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 50
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 100, minDamage = 0, maxDamage = -220, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 25, interval = 2000, minDamage = 0, maxDamage = -710, length = 8, effect = 42}
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
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)