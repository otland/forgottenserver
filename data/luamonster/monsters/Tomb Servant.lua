local mType = Game.createMonsterType("Tomb Servant")
local monster = {}
monster.description = "a tomb servant"
monster.experience = 215
monster.outfit = {
	lookType = 100
}

monster.health = 475
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6029
monster.speed = 190
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

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Chaarr!!", yell = false},
	{text = "Ngl..Nglll...Gll", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 108, chance = 82350},
	{id = "bone", chance = 49000},
	{id = "worm", maxCount = 12, chance = 25325},
	{id = "scarab coin", chance = 8210},
	{id = "longsword", chance = 6560},
	{id = "bone shield", chance = 5300},
	{id = "rotten meat", chance = 2000},
	{id = "half-digested piece of meat", chance = 1000},
	{id = "fist on a stick", chance = 230}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 20, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 20, interval = 2000, minDamage = -40, maxDamage = -55, range = 7, effect = 14}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)