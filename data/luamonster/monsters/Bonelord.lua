local mType = Game.createMonsterType("Bonelord")
local monster = {}
monster.description = "a bonelord"
monster.experience = 170
monster.outfit = {
	lookType = 17
}

monster.health = 260
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 5992
monster.speed = 170
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
    {name = "Skeleton", chance = 20, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Eye for eye!", yell = false},
	{text = "Here's looking at you!", yell = false},
	{text = "Let me take a look at you!", yell = false},
	{text = "You've got the look!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 48, chance = 65000},
	{id = "longsword", chance = 8980},
	{id = "morning star", chance = 6950},
	{id = "spellbook", chance = 4650},
	{id = "steel shield", chance = 4001},
	{id = "two handed sword", chance = 3840},
	{id = "bonelord eye", chance = 940},
	{id = "mana potion", chance = 280},
	{id = "terra rod", chance = 570},
	{id = "small flask of eyedrops", chance = 4940},
	{id = "bonelord shield", chance = 80}
}

monster.attacks = {
	{name = "melee",  attack = 5, skill = 10, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 5, interval = 2000, minDamage = -15, maxDamage = -45, range = 7, shootEffect = 5},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 5, interval = 2000, minDamage = -25, maxDamage = -45, range = 7, shootEffect = 4},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 5, interval = 2000, minDamage = -30, maxDamage = -50, range = 7, shootEffect = 32, effect = 39},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 5, interval = 2000, minDamage = -5, maxDamage = -45, range = 7, shootEffect = 15, effect = 39},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 5, interval = 2000, minDamage = -5, maxDamage = -50, range = 7, shootEffect = 11, effect = 39},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 5, interval = 2000, minDamage = 0, maxDamage = -45, range = 7, shootEffect = 11, effect = 14},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 5, interval = 2000, minDamage = -5, maxDamage = -35, range = 7, shootEffect = 11, effect = 14}
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