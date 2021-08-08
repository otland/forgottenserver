local mtype = Game.createMonsterType("mummy")
local monster = {}
monster.description = "a mummy"
monster.experience = 150
monster.outfit = {
	lookType = 65
}

monster.health = 240
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6004
monster.speed = 150
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
    chance = 10
}

monster.flags = {
    healthHidden = false,
    summonable = false,
    attackable = true,
    hostile = true,
    convinceable = false,
    illusionable = true,
    canPushItems = true,
    canPushCreatures = false,
    targetDistance = 1,
    staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "I will ssswallow your sssoul!", yell = false},
	{text = "Mort ulhegh dakh visss.", yell = false},
	{text = "Flesssh to dussst!", yell = false},
	{text = "I will tassste life again!", yell = false},
	{text = "Ahkahra exura belil mort!", yell = false},
	{text = "Yohag Sssetham!", yell = false}
}

monster.loot = {
    {id = 2124 --[[crystal ring]], chance = 1500},
	{id = 2134 --[[silver brooch]], chance = 4000},
	{id = 2144 --[[black pearl]], chance = 1000},
	{id = 2161 --[[strange talisman]], subType = 200, chance = 5000},
	{id = 2162 --[[magic light wand]], chance = 5800},
	{id = 2170 --[[silver amulet]], subType = 200, chance = 100},
	{id = 2411 --[[poison dagger]], chance = 450},
	{id = 2529 --[[black shield]], chance = 170},
	{id = 3976 --[[worm]], maxCount = 3, chance = 19000},
	{id = 5914 --[[yellow piece of cloth]], chance = 900},
	{id = 10566 --[[gauze bandage]], chance = 10000},
	{id = 11207 --[[mini mummy]], chance = 10},
	{id = 12422 --[[flask of embalming fluid]], chance = 11690}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -85, interval = 2000},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 20, interval = 2000, minDamage = -30, maxDamage = -40, range = 1},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, range = 7}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "poison", condition = true, combat = true},
	{type = "cursed", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "manashield", condition = false, combat = false},
	{type = "freezing", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "haste", condition = false, combat = false},
	{type = "healing", condition = false, combat = false},
	{type = "light", condition = false, combat = false},
	{type = "death", condition = false, combat = true},
	{type = "ice", condition = false, combat = false},
	{type = "infight", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "holy", condition = false, combat = false}
}

mtype:register(monster)