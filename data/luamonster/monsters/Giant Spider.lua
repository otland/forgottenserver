local mType = Game.createMonsterType("Giant Spider")
local monster = {}
monster.description = "a giant spider"
monster.experience = 900
monster.outfit = {
	lookType = 38
}

monster.health = 1300
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 5977
monster.speed = 230
monster.maxSummons = 2

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
	staticAttack = 70
}

monster.summons = {
    {name = "Poison Spider", chance = 10, interval = 2000}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 95, chance = 50000},
	{id = "poison arrow", maxCount = 12, chance = 12500},
	{id = "plate legs", chance = 8333},
	{id = "plate armor", chance = 8333},
	{id = "two handed sword", chance = 5000},
	{id = "steel helmet", chance = 4545},
	{id = "strong health potion", chance = 3571},
	{id = "spider silk", chance = 2140},
	{id = "knight legs", chance = 870},
	{id = "time ring", chance = 710},
	{id = "platinum amulet", chance = 280},
	{id = "knight armor", chance = 530},
	{id = "lightning headband", chance = 220}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -300, interval = 2000, effect = 0},
	{name = "poisonfield",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 1, shootEffect = 15},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -40, maxDamage = -70, target = true, range = 7, radius = 1, shootEffect = 15}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 390, shootEffect = 15, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -10}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)