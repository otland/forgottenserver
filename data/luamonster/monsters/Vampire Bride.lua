local mType = Game.createMonsterType("Vampire Bride")
local monster = {}
monster.description = "a vampire bride"
monster.experience = 1050
monster.outfit = {
	lookType = 312
}

monster.health = 1200
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 9658
monster.speed = 230
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
	{text = "Kneel before your Mistress!", yell = false},
	{text = "Dead is the new alive.", yell = false},
	{text = "Come, let me kiss you, darling. Oh wait, I meant kill.", yell = false},
	{text = "Enjoy the pain - I know you love it.", yell = false},
	{text = "Are you suffering nicely enough?", yell = false},
	{text = "You won't regret you came to me, sweetheart.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 149, chance = 90000},
	{id = "strong mana potion", chance = 10210},
	{id = "vampire teeth", chance = 10000},
	{id = "platinum coin", chance = 9910},
	{id = "moonlight rod", chance = 5500},
	{id = "strong health potion", chance = 5000},
	{id = "blood preservation", chance = 4950},
	{id = "emerald bangle", chance = 1100},
	{id = "hibiscus dress", chance = 1030},
	{id = "small diamond", maxCount = 2, chance = 1020},
	{id = "rusty armor", chance = 1010},
	{id = "velvet tapestry", chance = 970},
	{id = "boots of haste", chance = 220},
	{id = "flower bouquet", chance = 200},
	{id = "enigmatic voodoo skull", chance = 170},
	{id = "blood goblet", chance = 60},
	{id = "leather whip", chance = 20}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 3000, minDamage = -60, maxDamage = -130, range = 1, effect = 14},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -60, maxDamage = -150, range = 7, shootEffect = 32, effect = 18},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 5, interval = 4000, minDamage = -60, maxDamage = -150, range = 7, shootEffect = 32, effect = 36},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 15, interval = 2000, minDamage = -60, maxDamage = -150, range = 7, shootEffect = 5, effect = 36}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 40, maxDamage = 80, shootEffect = 5, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE, percent = -10},
	{type = COMBAT_DROWNDAMAGE, percent = 10}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)