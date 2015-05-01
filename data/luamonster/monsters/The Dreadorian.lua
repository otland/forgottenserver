local mType = Game.createMonsterType("The Dreadorian")
local monster = {}
monster.description = "the dreadorian"
monster.experience = 4000
monster.outfit = {
	lookType = 234
}

monster.health = 9000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6328
monster.speed = 250
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
	staticAttack = 90
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 64, minDamage = 0, maxDamage = -370, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 35,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 50, interval = 2000, minDamage = 100, maxDamage = 150, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = 90},
	{type = COMBAT_HOLYDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = 10},
	{type = COMBAT_ICEDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
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