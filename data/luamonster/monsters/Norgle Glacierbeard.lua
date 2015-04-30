local mType = Game.createMonsterType("Norgle Glacierbeard")
local monster = {}
monster.description = "norgle glacierbeard"
monster.experience = 2100
monster.outfit = {
	lookType = 257
}

monster.health = 4300
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7349
monster.speed = 195
monster.maxSummons = 0

monster.changeTarget = {
    interval = 0,
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
	staticAttack = 95
}

monster.voices = {
	interval = 2000,
	chance = 5,
	{text = "I'll extinguish you warmbloods.", yell = false},
	{text = "REVENGE!", yell = false},
	{text = "Far too hot.", yell = false},
	{text = "DISGUSTING WARMBLOODS!", yell = false},
	{text = "Revenge is sweetest when served cold.", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 80, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = -15}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)