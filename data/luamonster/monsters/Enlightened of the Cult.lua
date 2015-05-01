local mType = Game.createMonsterType("Enlightened of the Cult")
local monster = {}
monster.description = "an enlightened of the cult"
monster.experience = 500
monster.outfit = {
	lookType = 193
}

monster.health = 700
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20391
monster.speed = 210
monster.maxSummons = 2

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
	targetDistance = 4,
	staticAttack = 50
}

monster.summons = {
    {name = "Crypt Shambler", chance = 10, interval = 2000},
	{name = "Ghost", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Praise to my master Urgith!", yell = false},
	{text = "You will rise as my servant!", yell = false},
	{text = "Praise to my masters! Long live the triangle!", yell = false},
	{text = "You will die in the name of the triangle!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 70, chance = 64550},
	{id = "cultish mask", chance = 10250},
	{id = "book", chance = 910},
	{id = "cultish symbol", chance = 890},
	{id = "protection amulet", subType = 250, chance = 790},
	{id = "strong mana potion", chance = 740},
	{id = "small sapphire", chance = 550},
	{id = "music sheet", chance = 490},
	{id = "energy ring", chance = 450},
	{id = "pirate voodoo doll", chance = 430},
	{id = "skull staff", chance = 350},
	{id = "platinum amulet", chance = 200},
	{id = "wand of inferno", chance = 180},
	{id = "enigmatic voodoo skull", chance = 130},
	{id = "piggy bank", chance = 130},
	{id = "amber staff", chance = 100},
	{id = "broken key ring", chance = 100},
	{id = "jewelled backpack", chance = 100},
	{id = "blue robe", chance = 40}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -100, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 25, interval = 2000, minDamage = -70, maxDamage = -185, target = true, range = 1, radius = 1, shootEffect = 31, effect = 50},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 31, effect = 40},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 6000, speedChange = -360, range = 7, shootEffect = 31, effect = 14}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 2000, minDamage = 60, maxDamage = 90, shootEffect = 31, effect = 13},
	{name = "invisible",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 31, effect = 8}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = 5}
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)