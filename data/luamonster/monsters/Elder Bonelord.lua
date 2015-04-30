local mType = Game.createMonsterType("Elder Bonelord")
local monster = {}
monster.description = "an elder bonelord"
monster.experience = 280
monster.outfit = {
	lookType = 108
}

monster.health = 500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6037
monster.speed = 180
monster.maxSummons = 6

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
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.summons = {
    {name = "Gazer", chance = 10, interval = 2000},
	{name = "Crypt Shambler", chance = 15, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Let me take a look at you!", yell = false},
	{text = "Inferior creatures, bow before my power!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 86, chance = 30000},
	{id = "spellbook", chance = 7500},
	{id = "steel shield", chance = 6000},
	{id = "two handed sword", chance = 6000},
	{id = "sniper arrow", maxCount = 5, chance = 10000},
	{id = "strong mana potion", chance = 1000},
	{id = "bonelord shield", chance = 150},
	{id = "bonelord helmet", chance = 150},
	{id = "small flask of eyedrops", chance = 10025},
	{id = "elder bonelord tentacle", chance = 21725},
	{id = "giant eye", chance = 850}
}

monster.attacks = {
	{name = "melee",  attack = 55, skill = 10, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 5, interval = 2000, minDamage = -45, maxDamage = -60, range = 7, shootEffect = 5},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 5, interval = 2000, minDamage = -40, maxDamage = -80, range = 7, shootEffect = 4},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -45, maxDamage = -90, range = 7, shootEffect = 32, effect = 39},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -20, maxDamage = -40, range = 7, shootEffect = 15, effect = 39},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 5, interval = 2000, minDamage = -45, maxDamage = -85, range = 7, shootEffect = 15, effect = 14},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 5, interval = 2000, minDamage = 0, maxDamage = -40, range = 7, shootEffect = 15, effect = 14},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 20000, speedChange = -600, range = 7, shootEffect = 15, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = 30},
	{type = COMBAT_DEATHDAMAGE, percent = 30},
	{type = COMBAT_FIREDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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