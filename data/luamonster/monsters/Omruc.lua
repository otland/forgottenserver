local mType = Game.createMonsterType("Omruc")
local monster = {}
monster.description = "a omruc"
monster.experience = 2950
monster.outfit = {
	lookType = 90
}

monster.health = 4300
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6025
monster.speed = 370
monster.maxSummons = 4

monster.changeTarget = {
    interval = 5000,
	chance = 8
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 4,
	staticAttack = 90
}

monster.summons = {
    {name = "Stalker", chance = 1000, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Now chhhou shhhee me ... Now chhhou don't!!", yell = false},
	{text = "Chhhhou are marked ashhh my prey.", yell = false},
	{text = "Psssst, I am over chhhere.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 50000},
	{id = "gold coin", maxCount = 70, chance = 50000},
	{id = "ravager's axe", maxCount = 2, chance = 10000},
	{id = "arrow", maxCount = 21, chance = 10000},
	{id = "poison arrow", maxCount = 20, chance = 10000},
	{id = "burst arrow", maxCount = 15, chance = 10000},
	{id = "onyx arrow", maxCount = 2, chance = 10000},
	{id = "power bolt", maxCount = 3, chance = 10000},
	{id = "small diamond", maxCount = 3, chance = 7000},
	{id = "great health potion", chance = 7000},
	{id = "stealth ring", chance = 5000},
	{id = "yellow gem", chance = 5000},
	{id = "boots of haste", chance = 1500},
	{id = "hammer of wrath", chance = 300},
	{id = "crystal arrow", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 20, interval = 5000, minDamage = -100, maxDamage = -250, range = 1},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -200, maxDamage = -500, shootEffect = 6},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 20, interval = 1000, minDamage = -120, maxDamage = -450, range = 3, shootEffect = 7, effect = 5},
	{name = "melee",  attack = 60, skill = 30, interval = 3000, effect = 5},
	{name = "speed",  chance = 25, interval = 1000, minDamage = 0, maxDamage = 0, duration = 50000, speedChange = -900, range = 7, shootEffect = 3, effect = 14}
}

monster.defenses = {
	defense = 35,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 17, interval = 1000, minDamage = 100, maxDamage = 200, shootEffect = 3, effect = 13},
	{name = "invisible",  chance = 14, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 3, effect = 13}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)