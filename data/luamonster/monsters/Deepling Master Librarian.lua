local mType = Game.createMonsterType("Deepling Master Librarian")
local monster = {}
monster.description = "a deepling master librarian"
monster.experience = 1900
monster.outfit = {
	lookType = 443
}

monster.health = 1700
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 15211
monster.speed = 190
monsters.runHealth = 250
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
	staticAttack = 60
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Mmmmmoooaaaaaahaaa!!!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "platinum coin", maxCount = 3, chance = 90000},
	{id = "deepling filet", chance = 20000},
	{id = "spellsinger's seal", chance = 25000},
	{id = "key to the Drowned Library", chance = 20000},
	{id = "fish", maxCount = 2, chance = 9090},
	{id = "small sapphire", maxCount = 3, chance = 8440},
	{id = "eye of a deepling", chance = 9380},
	{id = "life ring", chance = 3200},
	{id = "deepling staff", chance = 3130},
	{id = "fish fin", chance = 1950},
	{id = "necklace of the deep", subType = 50, chance = 1330},
	{id = "ornate crossbow", chance = 39}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 60, minDamage = 0, maxDamage = -210, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 10, interval = 2000, minDamage = -260, maxDamage = -450, length = 10, spread = 3, effect = 43},
	{name = "combat", type = COMBAT_DROWNDAMAGE,  chance = 20, interval = 2000, minDamage = -150, maxDamage = -280, target = true, radius = 4, effect = 26},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -80, maxDamage = -140, range = 7, effect = 46},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = -60, maxDamage = -140, range = 7, effect = 46},
	{name = "deepling spellsinger skill reducer",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 5, effect = 46}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 40, maxDamage = 80, effect = 13}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)