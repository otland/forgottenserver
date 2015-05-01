local mType = Game.createMonsterType("Hunter")
local monster = {}
monster.description = "a hunter"
monster.experience = 150
monster.outfit = {
	lookFeet = 115,
	lookLegs = 121,
	lookHead = 95,
	lookType = 129,
	lookBody = 116
}

monster.health = 150
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20419
monster.speed = 195
monster.runHealth = 10
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Guess who we're hunting, haha!", yell = false},
	{text = "Guess who we are hunting!", yell = false},
	{text = "Bullseye!", yell = false},
	{text = "You'll make a nice trophy!", yell = false}
}

monster.loot = {
    {id = "poison arrow", maxCount = 4, chance = 4500},
	{id = "arrow", maxCount = 22, chance = 82000},
	{id = "orange", maxCount = 2, chance = 20300},
	{id = "roll", maxCount = 2, chance = 11370},
	{id = "torch", chance = 3300},
	{id = "brass armor", chance = 5070},
	{id = "bow", chance = 5770},
	{id = "brass helmet", chance = 5050},
	{id = "dragon necklace", subType = 200, chance = 3000},
	{id = "burst arrow", maxCount = 3, chance = 5360},
	{id = "wolf trophy", chance = 190},
	{id = "lion trophy", chance = 70},
	{id = "deer trophy", chance = 520},
	{id = "sniper gloves", chance = 610},
	{id = "small ruby", chance = 150},
	{id = "slingshot", chance = 120},
	{id = "hunter's quiver", chance = 10240}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 10, minDamage = 0, maxDamage = -20, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 50, interval = 2000, minDamage = 0, maxDamage = -100, range = 7, shootEffect = 3}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 20}
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