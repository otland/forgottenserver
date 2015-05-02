local mType = Game.createMonsterType("Grave Guard")
local monster = {}
monster.description = "a grave guard"
monster.experience = 485
monster.outfit = {
	lookType = 234
}

monster.health = 720
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 13975
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
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 30, chance = 50000},
	{id = "grave flower", chance = 70000},
	{id = "mana potion", chance = 20000},
	{id = "health potion", chance = 20000},
	{id = "scarab coin", chance = 4166},
	{id = "death ring", chance = 1754},
	{id = "daramian waraxe", chance = 1170}
}

monster.attacks = {
	{name = "melee",  attack = 45, skill = 55, minDamage = 0, maxDamage = -147, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, radius = 1, effect = 24}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = -10}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)