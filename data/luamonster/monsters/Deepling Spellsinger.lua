local mType = Game.createMonsterType("Deepling Spellsinger")
local monster = {}
monster.description = "a deepling spellsinger"
monster.experience = 1000
monster.outfit = {
	lookType = 443
}

monster.health = 850
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 15208
monster.speed = 190
monsters.runHealth = 60
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
	targetDistance = 1,
	staticAttack = 60
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Jey Obu giotja!!", yell = false},
	{text = "Mmmmmoooaaaaaahaaa!!", yell = false}
}

monster.loot = {
    {id = "platinum coin", chance = 80000},
	{id = "gold coin", maxCount = 60, chance = 70000},
	{id = "spellsinger's seal", chance = 14285},
	{id = "deepling filet", chance = 14285},
	{id = "key to the Drowned Library", chance = 10000},
	{id = "fish", chance = 3448},
	{id = "small sapphire", chance = 2854},
	{id = "eye of a deepling", chance = 2500},
	{id = "life ring", chance = 2439},
	{id = "deepling staff", chance = 2000},
	{id = "necklace of the deep", subType = 50, chance = 813},
	{id = "fish fin", chance = 498},
	{id = "ornate crossbow", chance = 220}
}

monster.attacks = {
	{name = "melee",  attack = 55, skill = 45, minDamage = 0, maxDamage = -152, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 10, interval = 2000, minDamage = -180, maxDamage = -350, length = 10, spread = 3, effect = 43},
	{name = "combat", type = COMBAT_DROWNDAMAGE,  chance = 20, interval = 2000, minDamage = -90, maxDamage = -130, target = true, radius = 4, effect = 26},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -60, maxDamage = -140, range = 7, effect = 46},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = -60, maxDamage = -140, range = 7, effect = 46},
	{name = "deepling spellsinger skill reducer",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, range = 5, effect = 46}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -20},
	{type = COMBAT_EARTHDAMAGE, percent = -20},
	{type = COMBAT_DEATHDAMAGE, percent = 50}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)