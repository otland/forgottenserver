local mType = Game.createMonsterType("Undead Cavebear")
local monster = {}
monster.description = "an undead cavebear"
monster.experience = 600
monster.outfit = {
	lookType = 384
}

monster.health = 450
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 13323
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 80, chance = 9750},
	{id = "cavebear skull", chance = 3150},
	{id = "maxilla", chance = 2350},
	{id = "maxilla maximus", chance = 900}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = -15, maxDamage = -400, interval = 2000, effect = 0}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)