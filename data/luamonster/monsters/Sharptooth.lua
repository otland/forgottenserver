local mType = Game.createMonsterType("Sharptooth")
local monster = {}
monster.description = "a sharptooth"
monster.experience = 1600
monster.outfit = {
	lookType = 20
}

monster.health = 2500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6067
monster.speed = 290
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
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
    {id = "fishbone", chance = 50000}
}

monster.attacks = {
	{name = "melee",  attack = 500, skill = 30, minDamage = 0, maxDamage = -1000, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 29,
    armor = 20,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, speedChange = 210, effect = 15},
	{name = "combat", type = COMBAT_HEALING,  chance = 12, interval = 2000, minDamage = 200, maxDamage = 240, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 80}
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