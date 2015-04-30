local mType = Game.createMonsterType("Mummy")
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
    {id = "gold coin", maxCount = 80, chance = 38000},
	{id = "worm", maxCount = 3, chance = 19000},
	{id = "flask of embalming fluid", chance = 11690},
	{id = "gauze bandage", chance = 10000},
	{id = "magic light wand", chance = 5800},
	{id = "strange talisman", subType = 200, chance = 5000},
	{id = "silver brooch", chance = 4000},
	{id = "crystal ring", chance = 1500},
	{id = "black pearl", chance = 1000},
	{id = "yellow piece of cloth", chance = 900},
	{id = "poison dagger", chance = 450},
	{id = "black shield", chance = 170},
	{id = "silver amulet", subType = 200, chance = 100},
	{id = "mini mummy", chance = 10}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 20, interval = 2000, minDamage = -30, maxDamage = -40, range = 1, effect = 39},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 10000, speedChange = -226, range = 7, effect = 14}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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