local mType = Game.createMonsterType("Lost Husher")
local monster = {}
monster.description = "a lost husher"
monster.experience = 1800
monster.outfit = {
	lookType = 537
}

monster.health = 1600
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 19964
monster.speed = 220
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Arr far zwar!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "platinum coin", maxCount = 2, chance = 58670},
	{id = "brown mushroom", maxCount = 2, chance = 14920},
	{id = "red hair dye", chance = 8510},
	{id = "strong health potion", maxCount = 3, chance = 11050},
	{id = "great mana potion", maxCount = 2, chance = 10770},
	{id = "small topaz", chance = 9780},
	{id = "basalt figurine", chance = 8560},
	{id = "buckle", chance = 8180},
	{id = "bonecarving knife", chance = 8510},
	{id = "lost husher's staff", chance = 9280},
	{id = "coal", chance = 10930},
	{id = "skull shatterer", chance = 12320},
	{id = "wimp tooth chain", chance = 8950},
	{id = "mad froth", chance = 15580},
	{id = "leather harness", chance = 11330},
	{id = "clay lump", chance = 500},
	{id = "dwarven ring (hard drinking)", chance = 2870},
	{id = "guardian shield", chance = 830},
	{id = "fiery knight axe", subType = 1000, chance = 770},
	{id = "spiked squelcher", chance = 50},
	{id = "tower shield", chance = 440},
	{id = "buckle", chance = 770},
	{id = "skull staff", chance = 280},
	{id = "terra boots", chance = 880},
	{id = "fire axe", chance = 330},
	{id = "terra legs", chance = 50}
}

monster.attacks = {
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -150, maxDamage = -300, length = 6, effect = 158},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = -150, maxDamage = -250, radius = 5, effect = 158},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -150, maxDamage = -200, range = 7, shootEffect = 32, effect = 18},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -150, maxDamage = -250, target = true, range = 7, radius = 2, shootEffect = 39, effect = 15},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 4, shootEffect = 39, effect = 20}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 75, maxDamage = 92, shootEffect = 39, effect = 49},
	{name = "invisible",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 39, effect = 11}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_FIREDAMAGE, percent = 40},
	{type = COMBAT_HOLYDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = 5},
	{type = COMBAT_ICEDAMAGE, percent = 5}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)