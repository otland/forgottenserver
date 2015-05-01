local mType = Game.createMonsterType("The Handmaiden")
local monster = {}
monster.description = "the handmaiden"
monster.experience = 3850
monster.outfit = {
	lookType = 230
}

monster.health = 14500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6312
monster.speed = 450
monster.runHealth = 3100
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
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 70
}

monster.loot = {
    {id = "the Handmaiden's protector", chance = 35000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -800, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 25, interval = 2000, minDamage = -150, maxDamage = -800, range = 7, effect = 13},
	{name = "drunk",  chance = 12, interval = 1000, minDamage = 0, maxDamage = 0, range = 1, effect = 13}
}

monster.defenses = {
	defense = 35,
    armor = 25,
    {name = "speed",  chance = 12, interval = 3000, minDamage = 0, maxDamage = 0, duration = 8000, speedChange = 380, effect = 14},
	{name = "invisible",  chance = 50, interval = 4000, minDamage = 0, maxDamage = 0, effect = 14},
	{name = "combat", type = COMBAT_HEALING,  chance = 50, interval = 2000, minDamage = 100, maxDamage = 250, effect = 13},
	{name = "speed",  chance = 35, interval = 1000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = 370, effect = 14}
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
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)