local mType = Game.createMonsterType("The Pit Lord")
local monster = {}
monster.description = "the pit lord"
monster.experience = 2500
monster.outfit = {
	lookType = 55
}

monster.health = 4500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7349
monster.speed = 270
monster.maxSummons = 0

monster.changeTarget = {
    interval = 0,
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
	staticAttack = 95
}

monster.voices = {
	interval = 2000,
	chance = 5,
	{text = "I'LL GET YOU ALL!", yell = true},
	{text = "I won't let you escape!", yell = false},
	{text = "I'll crush you beneath my feet!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 85, minDamage = 0, maxDamage = -380, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 100, interval = 7500, minDamage = -100, maxDamage = -250, shootEffect = 12}
}

monster.defenses = {
	defense = 44,
    armor = 46,
    {name = "speed",  chance = 100, interval = 5000, minDamage = 0, maxDamage = 0, duration = 2500, speedChange = 500, shootEffect = 12, effect = 14},
	{name = "combat", type = COMBAT_HEALING,  chance = 65, interval = 6000, minDamage = 20, maxDamage = 50, shootEffect = 12, effect = 13}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
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