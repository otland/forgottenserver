local mType = Game.createMonsterType("Dwarf Soldier")
local monster = {}
monster.description = "a dwarf soldier"
monster.experience = 70
monster.outfit = {
	lookType = 71
}

monster.health = 135
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6014
monster.speed = 170
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Hail Durin!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 12, chance = 28000},
	{id = "bolt", maxCount = 7, chance = 40000},
	{id = "white mushroom", maxCount = 3, chance = 40000},
	{id = "shovel", chance = 10000},
	{id = "soldier helmet", chance = 12000},
	{id = "chain armor", chance = 8000},
	{id = "piercing bolt", maxCount = 3, chance = 4000},
	{id = "battle axe", chance = 2500},
	{id = "crossbow", chance = 3000},
	{id = "dwarven shield", chance = 3000},
	{id = "axe ring", chance = 120},
	{id = "iron ore", chance = 300}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 60, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -60, range = 7, shootEffect = 2}
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