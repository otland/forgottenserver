local mType = Game.createMonsterType("Kollos")
local monster = {}
monster.description = "a kollos"
monster.experience = 2400
monster.outfit = {
	lookType = 458
}

monster.health = 3800
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 15354
monster.speed = 200
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
	staticAttack = 95
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Zopp!", yell = false},
	{text = "Flzlzlzlzlzlz!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 49000},
	{id = "gold coin", maxCount = 97, chance = 50000},
	{id = "platinum coin", maxCount = 3, chance = 66000},
	{id = "kollos shell", chance = 15390},
	{id = "compound eye", chance = 15720},
	{id = "tarsal arrow", maxCount = 5, chance = 10210},
	{id = "small ruby", maxCount = 2, chance = 8180},
	{id = "great mana potion", maxCount = 4, chance = 8950},
	{id = "gold ingot", chance = 5160},
	{id = "ultimate health potion", maxCount = 3, chance = 4000},
	{id = "ring of healing", chance = 3270},
	{id = "giant shimmering pearl", chance = 2600},
	{id = "buggy backpack", chance = 460},
	{id = "calopteryx cape", chance = 360},
	{id = "hive scythe", chance = 700},
	{id = "steel boots", chance = 130},
	{id = "carapace shield", chance = 310}
}

monster.attacks = {
	{name = "melee",  attack = 70, skill = 80, minDamage = 0, maxDamage = -315, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -500, target = true, range = 7, radius = 3, shootEffect = 41, effect = 6}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_FIREDAMAGE, percent = 30}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
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