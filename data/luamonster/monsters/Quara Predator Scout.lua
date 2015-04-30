local mType = Game.createMonsterType("Quara Predator Scout")
local monster = {}
monster.description = "a quara predator scout"
monster.experience = 400
monster.outfit = {
	lookType = 20
}

monster.health = 890
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6067
monster.speed = 250
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
	{text = "Gnarrr!", yell = false},
	{text = "Tcharrr!", yell = false},
	{text = "Rrrah!", yell = false},
	{text = "Rraaar!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 60, chance = 50000},
	{id = "gold coin", maxCount = 69, chance = 48000},
	{id = "quara bone", chance = 10150},
	{id = "double axe", chance = 5800},
	{id = "scale armor", chance = 8000},
	{id = "small diamond", maxCount = 2, chance = 5520},
	{id = "fish fin", chance = 980},
	{id = "two handed sword", chance = 2320},
	{id = "northwind rod", chance = 800},
	{id = "shrimp", maxCount = 5, chance = 4700}
}

monster.attacks = {
	{name = "melee",  attack = 55, skill = 60, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)