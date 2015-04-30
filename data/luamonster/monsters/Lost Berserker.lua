local mType = Game.createMonsterType("Lost Berserker")
local monster = {}
monster.description = "a lost berserker"
monster.experience = 4400
monster.outfit = {
	lookType = 496
}

monster.health = 5900
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 17416
monster.speed = 250
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
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Kill! Kill! Kill!", yell = false},
	{text = "Death! Death! Death!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 100, chance = 50000},
	{id = "platinum coin", maxCount = 9, chance = 100000},
	{id = "brown mushroom", maxCount = 2, chance = 15190},
	{id = "brown crystal splinter", maxCount = 2, chance = 7540},
	{id = "small topaz", maxCount = 2, chance = 7970},
	{id = "drill bolt", maxCount = 10, chance = 8210},
	{id = "great health potion", chance = 13650},
	{id = "great mana potion", chance = 13650},
	{id = "green crystal fragment", chance = 6870},
	{id = "iron ore", chance = 8490},
	{id = "piggy bank", chance = 3920},
	{id = "blue crystal splinter", chance = 4610},
	{id = "violet crystal shard", chance = 3500},
	{id = "coal", chance = 2050},
	{id = "black shield", chance = 640},
	{id = "magic sulphur", chance = 720},
	{id = "spiked squelcher", chance = 810},
	{id = "guardian shield", chance = 1400},
	{id = "dwarven ring", chance = 2280},
	{id = "knight axe", chance = 2070},
	{id = "clay lump", chance = 930},
	{id = "chaos mace", chance = 550},
	{id = "tower shield", chance = 1002},
	{id = "terra boots", chance = 640},
	{id = "fire axe", chance = 380},
	{id = "royal helmet", chance = 150}
}

monster.attacks = {
	{name = "melee",  attack = 91, skill = 100, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -300, range = 7, shootEffect = 26},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -250, target = true, range = 7, radius = 3, shootEffect = 41, effect = 5},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = -150, maxDamage = -250, radius = 5, shootEffect = 41, effect = 14},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 20000, speedChange = -800, radius = 2, shootEffect = 41, effect = 14},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, radius = 4, shootEffect = 41, effect = 32}
}

monster.defenses = {
	defense = 40,
    armor = 40,
    {name = "invisible",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, radius = 4, shootEffect = 41, effect = 11}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 40},
	{type = COMBAT_ICEDAMAGE, percent = 40},
	{type = COMBAT_DEATHDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = 10}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)