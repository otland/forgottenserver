local mType = Game.createMonsterType("Spidris Elite")
local monster = {}
monster.description = "a spidris elite"
monster.experience = 4000
monster.outfit = {
	lookType = 457
}

monster.health = 3700
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 15296
monster.speed = 260
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
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

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 100, chance = 50000},
	{id = "platinum coin", maxCount = 6, chance = 45000},
	{id = "spidris mandible", chance = 27440},
	{id = "compound eye", chance = 13210},
	{id = "small ruby", maxCount = 5, chance = 23280},
	{id = "great mana potion", maxCount = 2, chance = 20400},
	{id = "ultimate health potion", maxCount = 2, chance = 9250},
	{id = "death ring", chance = 4480},
	{id = "giant shimmering pearl", chance = 3040},
	{id = "titan axe", chance = 1440},
	{id = "violet gem", chance = 1120},
	{id = "calopteryx cape", chance = 1280},
	{id = "hive scythe", chance = 1390},
	{id = "carapace shield", chance = 1170}
}

monster.attacks = {
	{name = "melee",  attack = 82, skill = 75, interval = 2000, effect = 0}
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