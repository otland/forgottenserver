local mType = Game.createMonsterType("Roaring Water Elemental")
local monster = {}
monster.description = "a roaring water elemental"
monster.experience = 1300
monster.outfit = {
	lookType = 11
}

monster.health = 1750
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 8965
monster.speed = 390
monster.maxSummons = 0

monster.changeTarget = {
    interval = 20000,
	chance = 15
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
	staticAttack = 85
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 27000},
	{id = "gold coin", maxCount = 87, chance = 27000},
	{id = "iced soil", chance = 9000},
	{id = "small sapphire", maxCount = 2, chance = 4125},
	{id = "shiver arrow", maxCount = 2, chance = 1000},
	{id = "northwind rod", chance = 750}
}

monster.attacks = {
	{name = "melee",  attack = 90, skill = 40, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 15, interval = 1000, minDamage = -240, maxDamage = -320, target = true, radius = 2, shootEffect = 29, effect = 2}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 90, maxDamage = 150, radius = 2, shootEffect = 29, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE, percent = 30}
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
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)