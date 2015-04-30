local mType = Game.createMonsterType("The Bloodweb")
local monster = {}
monster.description = "the bloodweb"
monster.experience = 900
monster.outfit = {
	lookType = 263
}

monster.health = 1250
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 7344
monster.speed = 340
monster.maxSummons = 0

monster.changeTarget = {
    interval = 20000,
	chance = 8
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 60
}

monster.voices = {
	interval = 2000,
	chance = 5,
	{text = "Screeech!", yell = false}
}

monster.loot = {
    {id = "plate armor", chance = 100000},
	{id = "sniper arrow", maxCount = 9, chance = 100000},
	{id = "strong mana potion", chance = 100000},
	{id = "spider silk", chance = 50000},
	{id = "glacier mask", chance = 33333},
	{id = "glacier kilt", chance = 33333},
	{id = "sai", chance = 20000},
	{id = "sapphire hammer", chance = 20000},
	{id = "jewelled backpack", chance = 7692},
	{id = "knight armor", chance = 7692},
	{id = "knight legs", chance = 5555},
	{id = "shard", chance = 3703},
	{id = "time ring", chance = 3703},
	{id = "party hat", chance = 1886}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 40, interval = 2000, effect = 0},
	{name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, duration = 8000, speedChange = -850, range = 7, radius = 7, effect = 3},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 25, interval = 1000, minDamage = -60, maxDamage = -150, range = 7, radius = 7, shootEffect = 5, effect = 12}
}

monster.defenses = {
	defense = 20,
    armor = 25,
    {name = "speed",  chance = 40, interval = 3000, minDamage = 0, maxDamage = 0, duration = 80000, speedChange = 380, radius = 7, shootEffect = 5, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -20},
	{type = COMBAT_EARTHDAMAGE, percent = 20}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)