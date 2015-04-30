local mType = Game.createMonsterType("Deepling Tyrant")
local monster = {}
monster.description = "a deepling tyrant"
monster.experience = 4200
monster.outfit = {
	lookType = 442
}

monster.health = 4900
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 15188
monster.speed = 260
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
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "QJELL NETA NA!!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 97, chance = 100000},
	{id = "gold coin", maxCount = 97, chance = 100000},
	{id = "platinum coin", maxCount = 4, chance = 70000},
	{id = "deepling claw", chance = 29960},
	{id = "deepling guard belt buckle", chance = 23100},
	{id = "deepling breaktime snack", chance = 34770},
	{id = "great mana potion", maxCount = 3, chance = 32640},
	{id = "eye of a deepling", chance = 28850},
	{id = "great health potion", maxCount = 3, chance = 33430},
	{id = "guardian axe", chance = 1420},
	{id = "deepling squelcher", chance = 1540},
	{id = "deepling backpack", chance = 510},
	{id = "foxtail", chance = 80}
}

monster.attacks = {
	{name = "melee",  attack = 91, skill = 100, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -375, range = 7, shootEffect = 25},
	{name = "combat", type = COMBAT_DROWNDAMAGE,  chance = 15, interval = 2000, minDamage = -180, maxDamage = -215, range = 7, shootEffect = 1, effect = 2}
}

monster.defenses = {
	defense = 45,
    armor = 45,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 200, maxDamage = 400, shootEffect = 1, effect = 13}
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