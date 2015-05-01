local mType = Game.createMonsterType("White Shade")
local monster = {}
monster.description = "a white shade"
monster.experience = 120
monster.outfit = {
	lookType = 560
}

monster.health = 260
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 21376
monster.speed = 190
monsters.runHealth = 30
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
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Leave this place... save yourself...", yell = false},
	{text = "The dark ones.. must be stopped... unholy, twisted, EVIL!", yell = false},
	{text = "No... don't go further stranger... danger lies in every step...", yell = false}
}

monster.loot = {
    {id = "white piece of cloth", chance = 3370}
}

monster.attacks = {
	{name = "melee",  attack = 5, skill = 20, minDamage = 0, maxDamage = -8, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 20, interval = 2000, minDamage = -4, maxDamage = -6, range = 7, shootEffect = 32, effect = 66},
	{name = "speed",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, duration = 3000, speedChange = -400, radius = 4, shootEffect = 32, effect = 10}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "invisible",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 32, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 90},
	{type = COMBAT_HOLYDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)