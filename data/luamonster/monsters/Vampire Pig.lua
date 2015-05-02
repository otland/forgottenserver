local mType = Game.createMonsterType("Vampire Pig")
local monster = {}
monster.description = "a vampire pig"
monster.experience = 165
monster.outfit = {
	lookType = 60
}

monster.health = 305
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6000
monster.speed = 300
monster.runHealth = 30
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 0
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
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Oink", yell = false},
	{text = "Oink oink", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 40, chance = 90000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -100, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 30, interval = 2000, minDamage = -15, maxDamage = -25, target = true, radius = 4, effect = 67},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -35, maxDamage = -55, range = 3, effect = 67}
}

monster.defenses = {
	defense = 35,
    armor = 30,
    {name = "outfit",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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