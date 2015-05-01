local mType = Game.createMonsterType("starving wolf")
local monster = {}
monster.description = "a starving wolf"
monster.experience = 65
monster.outfit = {
	lookFeet = 50,
	lookLegs = 40,
	lookHead = 20,
	lookType = 27,
	lookBody = 30
}

monster.health = 85
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5968
monster.speed = 170
monster.runHealth = 8
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Gaarrrrrr", yell = false},
	{text = "Gnoorrr", yell = false},
	{text = "Yoooohhuuuu!", yell = false}
}

monster.loot = {
    {id = "meat", maxCount = 2, chance = 5300},
	{id = "wolf paw", chance = 1430},
	{id = "", chance = 64730}
}

monster.attacks = {
	{name = "melee",  attack = 25, skill = 10, minDamage = 0, maxDamage = -25, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 10,
    armor = 10,
    {name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 1, effect = 8}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -30}
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)