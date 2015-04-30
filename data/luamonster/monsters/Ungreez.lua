local mType = Game.createMonsterType("Ungreez")
local monster = {}
monster.description = "ungreez"
monster.experience = 5000
monster.outfit = {
	lookType = 35
}

monster.health = 8200
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5995
monster.speed = 330
monster.maxSummons = 0

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
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "I'll teach you that even heros can die", yell = true},
	{text = "You wil die Begging like the others did", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 21000},
	{id = "fire mushroom", maxCount = 6, chance = 10000},
	{id = "great mana potion", chance = 20000},
	{id = "great health potion", chance = 20000}
}

monster.attacks = {
	{name = "melee",  attack = 120, skill = 70, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 13, interval = 2000, minDamage = 0, maxDamage = -110, range = 7, shootEffect = 32},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 14, interval = 1000, minDamage = -150, maxDamage = -250, target = true, range = 7, radius = 7, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 18, interval = 2000, minDamage = -200, maxDamage = -400, radius = 7, shootEffect = 5, effect = 48},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 12, interval = 1000, minDamage = -300, maxDamage = -380, length = 8, radius = 7, shootEffect = 5, effect = 48}
}

monster.defenses = {
	defense = 55,
    armor = 55,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 90, maxDamage = 150, radius = 7, shootEffect = 5, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 25},
	{type = COMBAT_ICEDAMAGE, percent = -15}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = true, combat = true},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)