local mType = Game.createMonsterType("Undead Prospector")
local monster = {}
monster.description = "an undead prospector"
monster.experience = 85
monster.outfit = {
	lookType = 18
}

monster.health = 100
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5976
monster.speed = 170
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Our mine... leave us alone.", yell = false},
	{text = "Turn back...", yell = false},
	{text = "These mine is ours... you shall not pass.", yell = false}
}

monster.loot = {
    {id = "worm", maxCount = 6, chance = 92310},
	{id = "gold coin", maxCount = 30, chance = 53850},
	{id = "torch", chance = 46150},
	{id = "brass helmet", chance = 23000},
	{id = "knife", chance = 15380},
	{id = "scale armor", chance = 1000},
	{id = "skull", chance = 240},
	{id = "brown piece of cloth", chance = 1000},
	{id = "viking helmet", chance = 1000},
	{id = "life ring", chance = 200}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -50, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 0,
    armor = 0,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 5, maxDamage = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE, percent = -25},
	{type = COMBAT_ICEDAMAGE, percent = 10}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)