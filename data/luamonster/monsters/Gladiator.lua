local mType = Game.createMonsterType("Gladiator")
local monster = {}
monster.description = "a gladiator"
monster.experience = 90
monster.outfit = {
	lookFeet = 114,
	lookLegs = 79,
	lookHead = 78,
	lookType = 131,
	lookBody = 3
}

monster.health = 185
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20407
monster.speed = 200
monsters.runHealth = 10
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = false,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "You are no match for me!", yell = false},
	{text = "Feel my prowess", yell = false},
	{text = "Fight!", yell = false},
	{text = "Take this!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 30, chance = 49500},
	{id = "meat", chance = 19000},
	{id = "plate shield", chance = 9950},
	{id = "sword", chance = 12620},
	{id = "mace", chance = 11160},
	{id = "brass armor", chance = 2750},
	{id = "chain helmet", chance = 5200},
	{id = "steel shield", chance = 840},
	{id = "belted cape", chance = 340},
	{id = "iron helmet", chance = 590}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 50, minDamage = 0, maxDamage = -90, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 215, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 15},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = 10}
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