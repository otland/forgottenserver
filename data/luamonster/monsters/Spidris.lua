local mType = Game.createMonsterType("Spidris")
local monster = {}
monster.description = "a spidris"
monster.experience = 2600
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
	{text = "Eeeeeeyyyyh!", yell = false},
	{text = "Iiiiieeeeeh!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 100, chance = 50000},
	{id = "platinum coin", maxCount = 4, chance = 45000},
	{id = "spidris mandible", chance = 14960},
	{id = "compound eye", chance = 12500},
	{id = "small ruby", maxCount = 5, chance = 11900},
	{id = "great mana potion", maxCount = 2, chance = 11500},
	{id = "ultimate health potion", maxCount = 2, chance = 6250},
	{id = "death ring", chance = 2700},
	{id = "giant shimmering pearl", chance = 1700},
	{id = "titan axe", chance = 920},
	{id = "violet gem", chance = 770},
	{id = "calopteryx cape", chance = 370},
	{id = "hive scythe", chance = 690},
	{id = "carapace shield", chance = 720}
}

monster.attacks = {
	{name = "melee",  attack = 70, skill = 75, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -150, maxDamage = -310, target = true, range = 7, radius = 3, shootEffect = 15, effect = 9}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 450, radius = 3, shootEffect = 15, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = 5},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 5}
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