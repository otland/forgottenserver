local mType = Game.createMonsterType("Wisp")
local monster = {}
monster.description = "a wisp"
monster.experience = 0
monster.outfit = {
	lookType = 294
}

monster.health = 115
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6324
monster.speed = 180
monsters.runHealth = 115
monster.maxSummons = 0

monster.changeTarget = {
    interval = 60000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 7,
	staticAttack = 15
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Crackle!", yell = false},
	{text = "Tsshh", yell = false}
}

monster.loot = {
    {id = "moon backpack", chance = 220}
}

monster.attacks = {
	{name = "melee",  attack = 10, skill = 10, minDamage = 0, maxDamage = -10, interval = 2000, effect = 13},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -3, maxDamage = -7, range = 1, shootEffect = 32, effect = 14}
}

monster.defenses = {
	defense = 10,
    armor = 10,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 200, shootEffect = 32, effect = 14},
	{name = "combat", type = COMBAT_HEALING,  chance = 5, interval = 2000, minDamage = 15, maxDamage = 25, shootEffect = 32, effect = 15},
	{name = "Invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 32, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 65},
	{type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = 90}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
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