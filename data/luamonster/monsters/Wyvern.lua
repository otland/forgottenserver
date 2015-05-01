local mType = Game.createMonsterType("Wyvern")
local monster = {}
monster.description = "a wyvern"
monster.experience = 515
monster.outfit = {
	lookType = 239
}

monster.health = 795
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6302
monster.speed = 200
monsters.runHealth = 300
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
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Shriiiek", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 100000},
	{id = "dragon ham", maxCount = 3, chance = 60500},
	{id = "power bolt", maxCount = 2, chance = 3400},
	{id = "small sapphire", chance = 5000},
	{id = "strong health potion", chance = 2500},
	{id = "wand of inferno", chance = 810},
	{id = "wyvern fang", chance = 410},
	{id = "emerald bangle", chance = 540},
	{id = "wyvern talisman", chance = 12300}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 50, minDamage = 0, maxDamage = -120, interval = 2000, effect = 0},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 2000, minDamage = -240, maxDamage = -240, length = 8, spread = 3, effect = 21},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, length = 3, spread = 2, effect = 20}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 45, maxDamage = 65, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 300, effect = 15}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = 10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
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