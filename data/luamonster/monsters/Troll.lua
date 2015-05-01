local mType = Game.createMonsterType("Troll")
local monster = {}
monster.description = "a troll"
monster.experience = 20
monster.outfit = {
	lookType = 15
}

monster.health = 50
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5960
monster.speed = 170
monsters.runHealth = 15
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
	{text = "Grrrr", yell = false},
	{text = "Groar", yell = false},
	{text = "Gruntz!", yell = false},
	{text = "Hmmm, bugs", yell = false},
	{text = "Hmmm, dogs", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 12, chance = 65300},
	{id = "hand axe", chance = 18000},
	{id = "meat", chance = 15000},
	{id = "spear", chance = 13000},
	{id = "leather helmet", chance = 12000},
	{id = "leather boots", chance = 10000},
	{id = "rope", chance = 7950},
	{id = "studded club", chance = 5000},
	{id = "wooden shield", chance = 4730},
	{id = "bunch of troll hair", chance = 1000},
	{id = "silver amulet", subType = 200, chance = 80}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -15, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 25},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
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