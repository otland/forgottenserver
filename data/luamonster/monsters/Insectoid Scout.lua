local mType = Game.createMonsterType("Insectoid Scout")
local monster = {}
monster.description = "an insectoid scout"
monster.experience = 150
monster.outfit = {
	lookType = 403
}

monster.health = 230
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 13514
monster.speed = 190
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Chrrr! Chrr!", yell = false},
	{text = "Klk! Klk!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 80, chance = 80000},
	{id = "ripper lance", chance = 850},
	{id = "health potion", chance = 280},
	{id = "club ring", chance = 100}
}

monster.attacks = {
	{name = "melee",  attack = 37, skill = 30, minDamage = 0, maxDamage = -74, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = -5}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
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