local mType = Game.createMonsterType("Feversleep")
local monster = {}
monster.description = "feversleep"
monster.experience = 4400
monster.outfit = {
	lookType = 593
}

monster.health = 5900
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 22497
monster.speed = 270
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
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "platinum coin", maxCount = 9, chance = 100000},
	{id = "small emerald", maxCount = 2, chance = 11000},
	{id = "small amethyst", maxCount = 3, chance = 12000},
	{id = "blue robe", chance = 1500},
	{id = "great mana potion", maxCount = 2, chance = 40000},
	{id = "ultimate health potion", chance = 18000},
	{id = "small topaz", maxCount = 2, chance = 16000},
	{id = "blue crystal splinter", chance = 13000},
	{id = "blue crystal shard", chance = 11000},
	{id = "cyan crystal fragment", chance = 18000},
	{id = "trapped bad dream monster", chance = 17000},
	{id = "bowl of terror sweat", chance = 14000}
}

monster.attacks = {
	{name = "melee",  attack = 90, skill = 90, minDamage = 0, maxDamage = -450, interval = 2000, effect = 0},
	{name = "condition", type = CONDITION_POISON,  chance = 20, interval = 2000, minDamage = -800, maxDamage = -1000, radius = 7, effect = 8},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = -70, maxDamage = -100, radius = 5, effect = 14},
	{name = "feversleep skill reducer",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, effect = 14},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -250, maxDamage = -300, length = 6, effect = 49},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 15, interval = 2000, minDamage = -150, maxDamage = -300, target = true, radius = 1, shootEffect = 32, effect = 18}
}

monster.defenses = {
	defense = 45,
    armor = 45,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 2000, minDamage = 250, maxDamage = 425, shootEffect = 32, effect = 13},
	{name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 32, effect = 10}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_FIREDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE, percent = -10},
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