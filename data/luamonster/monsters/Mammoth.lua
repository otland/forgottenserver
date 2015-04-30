local mType = Game.createMonsterType("Mammoth")
local monster = {}
monster.description = "a mammoth"
monster.experience = 160
monster.outfit = {
	lookType = 199
}

monster.health = 320
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6074
monster.speed = 210
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Troooooot!", yell = false},
	{text = "Hooooot-Toooooot!", yell = false},
	{text = "Tooooot.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 40, chance = 90000},
	{id = "meat", chance = 39000},
	{id = "ham", maxCount = 3, chance = 30000},
	{id = "mammoth tusk", maxCount = 2, chance = 7500},
	{id = "thick fur", chance = 7280},
	{id = "mammoth whopper", chance = 2800},
	{id = "furry club", chance = 500},
	{id = "tusk shield", chance = 500}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 15},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = 20}
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