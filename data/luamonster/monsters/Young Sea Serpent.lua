local mType = Game.createMonsterType("Young Sea Serpent")
local monster = {}
monster.description = "a young sea serpent"
monster.experience = 1000
monster.outfit = {
	lookType = 317
}

monster.health = 1050
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 9879
monster.speed = 320
monster.runHealth = 400
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
	staticAttack = 85
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "CHHHRRRR", yell = false},
	{text = "HISSSS", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 74, chance = 48000},
	{id = "small sapphire", maxCount = 2, chance = 1900},
	{id = "stealth ring", chance = 1000},
	{id = "strong mana potion", chance = 4000},
	{id = "strong health potion", chance = 5000},
	{id = "rusty armor", chance = 7940},
	{id = "rusty armor", chance = 7940},
	{id = "morning star", chance = 40000},
	{id = "life crystal", chance = 300},
	{id = "battle hammer", chance = 5000},
	{id = "battle axe", chance = 8000},
	{id = "sea serpent scale", chance = 5000}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 40, minDamage = 0, maxDamage = -200, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -10, maxDamage = -250, length = 7, spread = 2, effect = 46},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 10, interval = 2000, minDamage = -80, maxDamage = -250, length = 7, spread = 2, effect = 44},
	{name = "young sea serpent drown",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, range = 5, effect = 44}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 30, interval = 2000, minDamage = 25, maxDamage = 175, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 400, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -15},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_DEATHDAMAGE, percent = -15}
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