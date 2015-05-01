local mType = Game.createMonsterType("Hideous Fungus")
local monster = {}
monster.description = "a hideous fungus"
monster.experience = 2900
monster.outfit = {
	lookType = 499
}

monster.health = 4600
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 17428
monster.speed = 260
monsters.runHealth = 275
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
	targetDistance = 4,
	staticAttack = 90
}

monster.summons = {
    {name = "humorless fungus", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Munch munch munch!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 70000},
	{id = "gold coin", maxCount = 97, chance = 70000},
	{id = "platinum coin", maxCount = 7, chance = 100000},
	{id = "hideous chunk", chance = 14285},
	{id = "envenomed arrow", maxCount = 15, chance = 14500},
	{id = "mushroom pie", maxCount = 3, chance = 14500},
	{id = "mana potion", chance = 9090},
	{id = "great mana potion", chance = 7692},
	{id = "great health potion", chance = 7220},
	{id = "white piece of cloth", chance = 6430},
	{id = "war hammer", chance = 4010},
	{id = "blue piece of cloth", chance = 3700},
	{id = "green piece of cloth", chance = 3003},
	{id = "terra boots", chance = 2000},
	{id = "terra amulet", subType = 200, chance = 2000},
	{id = "red piece of cloth", chance = 1820},
	{id = "terra legs", chance = 920},
	{id = "terra mantle", chance = 800},
	{id = "muck rod", chance = 590},
	{id = "mycological bow", chance = 60},
	{id = "mushroom backpack", chance = 20}
}

monster.attacks = {
	{name = "melee",  attack = 90, skill = 90, minDamage = 0, maxDamage = -450, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -250, maxDamage = -430, range = 7, shootEffect = 39, effect = 46},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 15, interval = 2000, minDamage = -250, maxDamage = -550, length = 8, spread = 3, shootEffect = 13, effect = 46},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, duration = 60000, speedChange = -600, radius = 1, shootEffect = 13, effect = 14},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 5, shootEffect = 10, effect = 32},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -400, maxDamage = -640, range = 7, radius = 3, shootEffect = 10, effect = 17}
}

monster.defenses = {
	defense = 35,
    armor = 35,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 275, maxDamage = 350, shootEffect = 10, effect = 13},
	{name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 10, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_DEATHDAMAGE, percent = 1},
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