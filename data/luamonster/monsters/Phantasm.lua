local mType = Game.createMonsterType("Phantasm")
local monster = {}
monster.description = "a phantasm"
monster.experience = 4400
monster.outfit = {
	lookHead = 20,
	lookType = 241
}

monster.health = 3950
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6344
monster.speed = 240
monster.runHealth = 350
monster.maxSummons = 4

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
	staticAttack = 90
}

monster.summons = {
    {name = "Phantasm Summon", chance = 20, interval = 2000},
	{name = "Phantasm Summon", chance = 20, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Oh my, you forgot to put your pants on!", yell = false},
	{text = "Weeheeheeheehee!", yell = false},
	{text = "Its nothing but a dream!", yell = false},
	{text = "Give in!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 30000},
	{id = "gold coin", maxCount = 100, chance = 30000},
	{id = "gold coin", maxCount = 33, chance = 28000},
	{id = "platinum coin", maxCount = 4, chance = 87730},
	{id = "blank rune", maxCount = 2, chance = 22500},
	{id = "demonic essence", chance = 16320},
	{id = "great mana potion", maxCount = 2, chance = 32750},
	{id = "ultimate health potion", chance = 14680},
	{id = "shadow herb", maxCount = 2, chance = 26930},
	{id = "small emerald", maxCount = 3, chance = 10190},
	{id = "abyss hammer", chance = 110},
	{id = "crown armor", chance = 660},
	{id = "stealth ring", chance = 550},
	{id = "death ring", chance = 330},
	{id = "shadow sceptre", chance = 550},
	{id = "small ruby", maxCount = 3, chance = 12160},
	{id = "small topaz", maxCount = 3, chance = 12810},
	{id = "small amethyst", maxCount = 3, chance = 14570}
}

monster.attacks = {
	{name = "melee",  attack = 95, skill = 90, minDamage = 0, maxDamage = -475, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -250, maxDamage = -610, range = 7, shootEffect = 32, effect = 39},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 20, interval = 2000, minDamage = -5, maxDamage = -80, radius = 3, shootEffect = 32, effect = 8},
	{name = "phantasm drown",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 32, effect = 8},
	{name = "drunk",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, radius = 5, shootEffect = 32, effect = 14}
}

monster.defenses = {
	defense = 0,
    armor = 0,
    {name = "combat", type = COMBAT_HEALING,  chance = 30, interval = 2000, minDamage = 228, maxDamage = 449, shootEffect = 32, effect = 13},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 6000, speedChange = 500, shootEffect = 32, effect = 14},
	{name = "invisible",  chance = 25, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 32, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = 20}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = true},
	{type = "bleed", condition = true, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)