local mType = Game.createMonsterType("The Count")
local monster = {}
monster.description = "the count"
monster.experience = 1750
monster.outfit = {
	lookType = 287
}

monster.health = 4600
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 8937
monster.speed = 370
monster.maxSummons = 1

monster.changeTarget = {
    interval = 5000,
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
	staticAttack = 90
}

monster.summons = {
    {name = "Banshee", chance = 50, interval = 4000}
}

monster.loot = {
    {id = "gold coin", maxCount = 98, chance = 40000},
	{id = "the ring of the count", chance = 100000},
	{id = "war hammer", chance = 2300}
}

monster.attacks = {
	{name = "melee",  attack = 135, skill = 130, minDamage = 0, maxDamage = -945, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 9, interval = 1000, minDamage = 0, maxDamage = -300, radius = 4, effect = 39}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 1000, minDamage = 100, maxDamage = 195, effect = 13},
	{name = "invisible",  chance = 30, interval = 3000, minDamage = 0, maxDamage = 0, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE, percent = -20}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)