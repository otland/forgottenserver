local mType = Game.createMonsterType("Leaf Golem")
local monster = {}
monster.description = "a leaf golem"
monster.experience = 45
monster.outfit = {
	lookType = 567
}

monster.health = 90
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 21358
monster.speed = 140
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "*crackle*", yell = false},
	{text = "*swwwwishhhh*", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 27, chance = 86830},
	{id = "dowser", chance = 15230},
	{id = "fir cone", chance = 14350},
	{id = "white mushroom", maxCount = 3, chance = 4940},
	{id = "swampling club", chance = 4940},
	{id = "small emerald", chance = 1230}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 35, minDamage = 0, maxDamage = -90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -15, range = 7, shootEffect = 10},
	{name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, duration = 9000, speedChange = -600, radius = 3, shootEffect = 10, effect = 46}
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)