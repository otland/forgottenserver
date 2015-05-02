local mType = Game.createMonsterType("Wilting Leaf Golem")
local monster = {}
monster.description = "a wilting leaf golem"
monster.experience = 145
monster.outfit = {
	lookType = 573
}

monster.health = 380
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 21434
monster.speed = 145
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
    {id = "gold coin", maxCount = 45, chance = 97230},
	{id = "dowser", chance = 10690},
	{id = "fir cone", chance = 15630},
	{id = "white mushroom", maxCount = 3, chance = 4860},
	{id = "swampling club", chance = 4200},
	{id = "small emerald", chance = 830}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 50, minDamage = 0, maxDamage = -120, interval = 1500, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -50, target = true, range = 7, radius = 1, shootEffect = 12, effect = 6},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 2000, minDamage = -150, maxDamage = -200, radius = 4, shootEffect = 12, effect = 9},
	{name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, duration = 15000, speedChange = -600, radius = 3, shootEffect = 12, effect = 14}
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