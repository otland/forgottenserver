local mType = Game.createMonsterType("Stalker")
local monster = {}
monster.description = "a stalker"
monster.experience = 90
monster.outfit = {
	lookFeet = 114,
	lookLegs = 95,
	lookHead = 78,
	lookType = 128,
	lookBody = 116
}

monster.health = 120
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20511
monster.speed = 200
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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 8, chance = 13000},
	{id = "throwing knife", maxCount = 2, chance = 11170},
	{id = "brass legs", chance = 3500},
	{id = "brass legs", chance = 5510},
	{id = "blank rune", chance = 8670},
	{id = "katana", chance = 530},
	{id = "obsidian lance", chance = 1210},
	{id = "miraculum", chance = 1550}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 60, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 1000, minDamage = -20, maxDamage = -30, range = 1, effect = 14}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "invisible",  chance = 40, interval = 2000, minDamage = 0, maxDamage = 0, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -20},
	{type = COMBAT_DEATHDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE, percent = -5}
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)