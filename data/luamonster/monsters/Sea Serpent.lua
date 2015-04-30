local mType = Game.createMonsterType("Sea Serpent")
local monster = {}
monster.description = "a sea serpent"
monster.experience = 2300
monster.outfit = {
	lookType = 275
}

monster.health = 1950
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 8307
monster.speed = 350
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
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "CHHHRRRR", yell = false},
	{text = "HISSSS", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 38000},
	{id = "gold coin", maxCount = 100, chance = 30000},
	{id = "gold coin", maxCount = 36, chance = 30000},
	{id = "platinum coin", maxCount = 3, chance = 25000},
	{id = "dragon ham", maxCount = 2, chance = 60000},
	{id = "sea serpent scale", chance = 10200},
	{id = "plate legs", chance = 7150},
	{id = "small sapphire", maxCount = 3, chance = 5600},
	{id = "strong health potion", chance = 5500},
	{id = "strong mana potion", chance = 3850},
	{id = "serpent sword", chance = 4200},
	{id = "great mana potion", chance = 920},
	{id = "spirit cloak", chance = 2900},
	{id = "ring of healing", chance = 1100},
	{id = "northwind rod", chance = 930},
	{id = "glacier amulet", subType = 200, chance = 880},
	{id = "stealth ring", chance = 400},
	{id = "glacier kilt", chance = 430},
	{id = "focus cape", chance = 370},
	{id = "oceanborn leviathan armor", chance = 110},
	{id = "crystalline armor", chance = 90}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 40, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -60, maxDamage = -300, length = 7, spread = 2, effect = 46},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 15, interval = 2000, minDamage = -101, maxDamage = -300, length = 7, spread = 2, effect = 44},
	{name = "sea serpent drown",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, range = 5, effect = 44}
}

monster.defenses = {
	defense = 45,
    armor = 45,
    {name = "combat", type = COMBAT_HEALING,  chance = 30, interval = 2000, minDamage = 70, maxDamage = 273, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 400, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -15},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_DEATHDAMAGE, percent = 10}
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
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)