local mType = Game.createMonsterType("Lost Soul")
local monster = {}
monster.description = "a lost soul"
monster.experience = 4000
monster.outfit = {
	lookType = 232
}

monster.health = 5800
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6310
monster.speed = 220
monsters.runHealth = 450
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 15
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
	{text = "Forgive Meeeee!", yell = false},
	{text = "Mouuuurn meeee!", yell = false},
	{text = "Help meeee!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 98, chance = 49000},
	{id = "platinum coin", maxCount = 7, chance = 100000},
	{id = "blank rune", maxCount = 3, chance = 35250},
	{id = "demonic essence", chance = 7500},
	{id = "great mana potion", maxCount = 2, chance = 14200},
	{id = "black pearl", maxCount = 3, chance = 12000},
	{id = "white pearl", maxCount = 3, chance = 10000},
	{id = "great health potion", maxCount = 2, chance = 8800},
	{id = "rusty armor", chance = 3500},
	{id = "rusty armor", chance = 3500},
	{id = "silver goblet", chance = 4950},
	{id = "ruby necklace", chance = 1500},
	{id = "stone skin amulet", subType = 5, chance = 2780},
	{id = "titan axe", chance = 1000},
	{id = "soul orb", chance = 15000},
	{id = "red gem", chance = 15000},
	{id = "death ring", chance = 2170},
	{id = "skeleton decoration", chance = 1250},
	{id = "skull staff", chance = 850},
	{id = "skull helmet", chance = 170},
	{id = "haunted blade", chance = 740},
	{id = "unholy bone", chance = 33010},
	{id = "tower shield", chance = 740}
}

monster.attacks = {
	{name = "melee",  attack = 120, skill = 60, minDamage = 0, maxDamage = -420, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -40, maxDamage = -210, length = 3, effect = 14},
	{name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, duration = 4000, speedChange = -800, radius = 6, effect = 39}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE, percent = -20},
	{type = COMBAT_ICEDAMAGE, percent = 50}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)