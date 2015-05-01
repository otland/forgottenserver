local mType = Game.createMonsterType("Enraged Crystal Golem")
local monster = {}
monster.description = "enraged crystal golem"
monster.experience = 550
monster.outfit = {
	lookType = 508
}

monster.health = 700
monster.maxHealth = monster.health
monster.race = "energy"
monster.corpse = 18466
monster.speed = 200
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Crrrrk! Chhhhr!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "crystalline axe", maxCount = 5, chance = 5008},
	{id = "crystalline spikes", chance = 7980},
	{id = "strong health potion", chance = 7140},
	{id = "strong mana potion", chance = 8270},
	{id = "blue crystal splinter", chance = 4120},
	{id = "crystal sword", chance = 4710},
	{id = "cyan crystal fragment", chance = 1050},
	{id = "glorious axe", chance = 360}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 50, minDamage = 0, maxDamage = -150, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 15}
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