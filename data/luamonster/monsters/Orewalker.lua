local mType = Game.createMonsterType("Orewalker")
local monster = {}
monster.description = "orewalker"
monster.experience = 4800
monster.outfit = {
	lookType = 490
}

monster.health = 7200
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 17256
monster.speed = 250
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
	{text = "Sizzle!", yell = false},
	{text = "CLONK!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 98, chance = 50000},
	{id = "platinum coin", maxCount = 10, chance = 100000},
	{id = "small topaz", maxCount = 3, chance = 16500},
	{id = "prismatic bolt", maxCount = 5, chance = 15500},
	{id = "blue crystal splinter", maxCount = 2, chance = 16000},
	{id = "strong mana potion", maxCount = 2, chance = 14000},
	{id = "great mana potion", maxCount = 2, chance = 14000},
	{id = "mana potion", maxCount = 4, chance = 14000},
	{id = "strong health potion", maxCount = 2, chance = 15600},
	{id = "ultimate health potion", maxCount = 2, chance = 9500},
	{id = "cyan crystal fragment", chance = 13000},
	{id = "sulphurous stone", chance = 20700},
	{id = "pulverized ore", chance = 20500},
	{id = "iron ore", chance = 15000},
	{id = "vein of ore", chance = 15000},
	{id = "shiny stone", chance = 13700},
	{id = "green crystal shard", chance = 8000},
	{id = "dwarven ring", chance = 4660},
	{id = "magic sulphur", chance = 3000},
	{id = "titan axe", chance = 2600},
	{id = "glorious axe", chance = 1870},
	{id = "knight legs", chance = 1910},
	{id = "yellow gem", chance = 1030},
	{id = "wand of defiance", chance = 1300},
	{id = "crown helmet", chance = 890},
	{id = "crystalline armor", chance = 560},
	{id = "crown armor", chance = 370},
	{id = "crystal crossbow", chance = 300}
}

monster.attacks = {
	{name = "melee",  attack = 75, skill = 70, minDamage = 0, maxDamage = -300, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 15, interval = 2000, minDamage = -296, maxDamage = -700, length = 4, spread = 3, effect = 18},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -1500, length = 6, effect = 35},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -800, maxDamage = -1080, target = true, radius = 3, shootEffect = 39, effect = 46},
	{name = "drunk",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, radius = 4, shootEffect = 39, effect = 23},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 20000, speedChange = -800, radius = 2, shootEffect = 39, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_FIREDAMAGE, percent = 65},
	{type = COMBAT_DEATHDAMAGE, percent = 15},
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