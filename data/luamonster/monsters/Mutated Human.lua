local mType = Game.createMonsterType("Mutated Human")
local monster = {}
monster.description = "a mutated human"
monster.experience = 150
monster.outfit = {
	lookType = 323
}

monster.health = 240
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 9107
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Take that creature off my back!! I can fell it!", yell = false},
	{text = "HEEEEEEEELP!", yell = false},
	{text = "You will be the next infected one... GRAAAAAAAAARRR!", yell = false},
	{text = "Science... is a curse.", yell = false},
	{text = "Run as fast as you can.", yell = false},
	{text = "Oh by the gods! What is this... aaaaaargh!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 80, chance = 44000},
	{id = "gold coin", maxCount = 50, chance = 44000},
	{id = "worm", maxCount = 2, chance = 7110},
	{id = "fishbone", chance = 10050},
	{id = "cheese", chance = 8000},
	{id = "strange talisman", subType = 200, chance = 4980},
	{id = "mutated flesh", chance = 19940},
	{id = "scale armor", chance = 8080},
	{id = "sword", chance = 5030},
	{id = "rusty armor", chance = 190},
	{id = "peanut", chance = 580},
	{id = "fern", chance = 420},
	{id = "silver amulet", subType = 200, chance = 70}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 50, minDamage = 0, maxDamage = -90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 15, interval = 2000, minDamage = -50, maxDamage = -60, length = 3, spread = 1, effect = 21},
	{name = "condition", type = CONDITION_POISON,  chance = 20, interval = 2000, minDamage = -190, maxDamage = -280, effect = 17},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = -600, range = 7, effect = 32}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 220, effect = 9}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
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