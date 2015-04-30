local mType = Game.createMonsterType("Humongous Fungus")
local monster = {}
monster.description = "a humongous fungus"
monster.experience = 2600
monster.outfit = {
	lookType = 488
}

monster.health = 3400
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 18382
monster.speed = 270
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 15
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
	{text = "Munch munch munch!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 60000},
	{id = "gold coin", maxCount = 100, chance = 60000},
	{id = "platinum coin", maxCount = 6, chance = 100000},
	{id = "mushroom pie", maxCount = 3, chance = 16666},
	{id = "brown piece of cloth", chance = 15285},
	{id = "drill bolt", maxCount = 15, chance = 14285},
	{id = "white piece of cloth", chance = 10000},
	{id = "humongous chunk", chance = 10000},
	{id = "strong health potion", maxCount = 2, chance = 5000},
	{id = "great mana potion", maxCount = 2, chance = 5000},
	{id = "mana potion", maxCount = 3, chance = 5000},
	{id = "great health potion", maxCount = 2, chance = 5000},
	{id = "strong mana potion", maxCount = 2, chance = 5000},
	{id = "blue piece of cloth", chance = 4545},
	{id = "red piece of cloth", chance = 2941},
	{id = "terra amulet", subType = 200, chance = 2127},
	{id = "terra boots", chance = 1851},
	{id = "angelic axe", chance = 1265},
	{id = "terra legs", chance = 1123},
	{id = "terra mantle", chance = 869},
	{id = "muck rod", chance = 680},
	{id = "mycological bow", chance = 150},
	{id = "mushroom backpack", chance = 20}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -180, maxDamage = -350, range = 7, shootEffect = 39, effect = 46},
	{name = "poisonfield",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, radius = 4, shootEffect = 39, effect = 46},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -500, maxDamage = -1000, length = 8, radius = 4, shootEffect = 39, effect = 9},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -130, maxDamage = -260, length = 5, radius = 4, shootEffect = 39, effect = 14},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -400, maxDamage = -640, range = 7, radius = 3, shootEffect = 39, effect = 17}
}

monster.defenses = {
	defense = 45,
    armor = 45,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 225, maxDamage = 380, radius = 3, shootEffect = 39, effect = 13},
	{name = "invisible",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, shootEffect = 39, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_DEATHDAMAGE, percent = 15},
	{type = COMBAT_ICEDAMAGE, percent = 15},
	{type = COMBAT_HOLYDAMAGE, percent = 5},
	{type = COMBAT_FIREDAMAGE, percent = 5}
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