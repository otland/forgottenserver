local mType = Game.createMonsterType("Dwarf")
local monster = {}
monster.description = "a dwarf"
monster.experience = 45
monster.outfit = {
	lookType = 69
}

monster.health = 90
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6007
monster.speed = 180
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
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Hail Durin!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 8, chance = 35000},
	{id = "white mushroom", chance = 50000},
	{id = "hatchet", chance = 25000},
	{id = "axe", chance = 15000},
	{id = "leather legs", chance = 10000},
	{id = "copper shield", chance = 10000},
	{id = "letter", chance = 8000},
	{id = "pick", chance = 10000},
	{id = "studded armor", chance = 8000},
	{id = "iron ore", chance = 700},
	{id = "dwarven ring", chance = 100}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 20, interval = 2000, effect = 0}
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