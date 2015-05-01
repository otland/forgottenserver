local mType = Game.createMonsterType("Undead Dragon")
local monster = {}
monster.description = "a undead dragon"
monster.experience = 7200
monster.outfit = {
	lookType = 231
}

monster.health = 8350
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6306
monster.speed = 250
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
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "FEEEED MY ETERNAL HUNGER!", yell = true},
	{text = "I SENSE LIFE", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 35500},
	{id = "gold coin", maxCount = 98, chance = 55500},
	{id = "demonic essence", chance = 12460},
	{id = "platinum coin", maxCount = 5, chance = 52000},
	{id = "death ring", chance = 1150},
	{id = "great health potion", chance = 21200},
	{id = "golden mug", chance = 6002},
	{id = "power bolt", maxCount = 15, chance = 15190},
	{id = "dragonbone staff", chance = 4000},
	{id = "knight armor", chance = 5500},
	{id = "life crystal", chance = 2500},
	{id = "hardened bone", chance = 14180},
	{id = "gold ingot", chance = 570},
	{id = "royal helmet", chance = 1720},
	{id = "war axe", chance = 1290},
	{id = "divine plate", chance = 430},
	{id = "unholy bone", chance = 33380},
	{id = "small sapphire", maxCount = 2, chance = 28370},
	{id = "assassin star", maxCount = 5, chance = 26650},
	{id = "dragon slayer", chance = 860},
	{id = "black pearl", maxCount = 2, chance = 22780},
	{id = "great mana potion", chance = 21490},
	{id = "spellweaver's robe", chance = 860},
	{id = "golden armor", chance = 860},
	{id = "skullcracker armor", chance = 290}
}

monster.attacks = {
	{name = "melee",  attack = 96, skill = 90, minDamage = 0, maxDamage = -481, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 5, interval = 2000, minDamage = -300, maxDamage = -400, target = true, range = 7, radius = 4, effect = 1},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -125, maxDamage = -600, range = 7, shootEffect = 32, effect = 39},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 5, interval = 2000, minDamage = -100, maxDamage = -390, target = true, range = 7, radius = 4, shootEffect = 15, effect = 21},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -180, range = 7, shootEffect = 15, effect = 21},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -150, maxDamage = -690, length = 8, spread = 3, shootEffect = 15, effect = 21},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -300, maxDamage = -700, length = 8, spread = 3, shootEffect = 15, effect = 14},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -100, maxDamage = -200, radius = 3, shootEffect = 15, effect = 14},
	{name = "undead dragon curse",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 15, effect = 14}
}

monster.defenses = {
	defense = 40,
    armor = 40,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 200, maxDamage = 250, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE, percent = -25},
	{type = COMBAT_ICEDAMAGE, percent = 50}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)