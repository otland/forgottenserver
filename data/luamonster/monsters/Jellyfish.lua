local mType = Game.createMonsterType("Jellyfish")
local monster = {}
monster.description = "a jellyfish"
monster.experience = 0
monster.outfit = {
	lookType = 452
}

monster.health = 55
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 15284
monster.speed = 170
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
	{text = "Luuurrrp", yell = false}
}

monster.loot = {
    {id = "shrimp", chance = 8333}
}

monster.attacks = {
	{name = "melee",  attack = 10, skill = 10, minDamage = 0, maxDamage = -10, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 10,
    armor = 10,
    {name = "invisible",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)