local mType = Game.createMonsterType("Deepling Guard")
local monster = {}
monster.description = "a deepling guard"
monster.experience = 2100
monster.outfit = {
	lookType = 442
}

monster.health = 1900
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 15187
monster.speed = 220
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "QJELL NETA NA!!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 100000},
	{id = "gold coin", maxCount = 90, chance = 100000},
	{id = "platinum coin", maxCount = 2, chance = 70000},
	{id = "deepling breaktime snack", chance = 16666},
	{id = "great health potion", maxCount = 3, chance = 14285},
	{id = "great mana potion", maxCount = 3, chance = 14285},
	{id = "deepling guard belt buckle", chance = 12500},
	{id = "eye of a deepling", chance = 10000},
	{id = "deepling claw", chance = 9090},
	{id = "small sapphire", maxCount = 3, chance = 2890},
	{id = "heavy trident", chance = 1694},
	{id = "guardian axe", chance = 925},
	{id = "deepling squelcher", chance = 751},
	{id = "ornate crossbow", chance = 362},
	{id = "deepling backpack", chance = 333},
	{id = "foxtail", chance = 10}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -200, range = 7, shootEffect = 25},
	{name = "combat", type = COMBAT_DROWNDAMAGE,  chance = 15, interval = 2000, minDamage = -100, maxDamage = -150, range = 7, shootEffect = 1, effect = 2}
}

monster.defenses = {
	defense = 35,
    armor = 35,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 100, maxDamage = 200, shootEffect = 1, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -20},
	{type = COMBAT_EARTHDAMAGE, percent = -20},
	{type = COMBAT_DEATHDAMAGE, percent = 10}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)