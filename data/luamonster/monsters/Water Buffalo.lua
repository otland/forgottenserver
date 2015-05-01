local mType = Game.createMonsterType("Water Buffalo")
local monster = {}
monster.description = "water buffalo"
monster.experience = 20
monster.outfit = {
	lookType = 523
}

monster.health = 390
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 19701
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
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Gnorrr!", yell = false},
	{text = "Gnarrr!", yell = false},
	{text = "<snort>", yell = false},
	{text = "Mrrr!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 20, chance = 100000},
	{id = "meat", maxCount = 4, chance = 33600},
	{id = "ham", maxCount = 3, chance = 39200}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 10, minDamage = 0, maxDamage = -30, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 5}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)