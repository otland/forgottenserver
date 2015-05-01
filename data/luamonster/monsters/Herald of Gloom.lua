local mType = Game.createMonsterType("Herald of Gloom")
local monster = {}
monster.description = "a herald of gloom"
monster.experience = 450
monster.outfit = {
	lookType = 320
}

monster.health = 450
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 9915
monster.speed = 260
monster.maxSummons = 0

monster.changeTarget = {
    interval = 0,
	chance = 8
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
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "The powers of light are waning.", yell = true},
	{text = "You will join us in eternal night!", yell = true},
	{text = "The shadows will engulf the world.", yell = true}
}

monster.loot = {
    {id = "midnight shard", chance = 1886}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 50, minDamage = 0, maxDamage = -90, interval = 2000, effect = 0},
	{name = "speed",  chance = 10, interval = 3000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = -600, range = 7, effect = 14},
	{name = "divine missile",  chance = 24, interval = 2000, minDamage = -90, maxDamage = -170, effect = 14}
}

monster.defenses = {
	defense = 55,
    armor = 50,
    {name = "speed",  chance = 15, interval = 1000, minDamage = 0, maxDamage = 0, duration = 20000, speedChange = 200, effect = 14},
	{name = "invisible",  chance = 20, interval = 5000, minDamage = 0, maxDamage = 0, effect = 14},
	{name = "outfit",  chance = 20, interval = 1500, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "outfit",  chance = 10, interval = 1500, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "outfit",  chance = 10, interval = 1500, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "outfit",  chance = 10, interval = 1500, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "outfit",  chance = 10, interval = 1500, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "outfit",  chance = 10, interval = 1500, minDamage = 0, maxDamage = 0, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = 20}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)