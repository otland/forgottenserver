local mType = Game.createMonsterType("Assassin")
local monster = {}
monster.description = "an assassin"
monster.experience = 105
monster.outfit = {
	lookFeet = 95,
	lookLegs = 95,
	lookHead = 114,
	lookType = 152,
	lookAddons = 3,
	lookBody = 95
}

monster.health = 175
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20327
monster.speed = 215
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Die!", yell = false},
	{text = "Feel the hand of death!", yell = false},
	{text = "You are on my deathlist!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 50, chance = 83210},
	{id = "torch", maxCount = 2, chance = 29980},
	{id = "gold coin", maxCount = 14, chance = 7250},
	{id = "viper star", maxCount = 7, chance = 4200},
	{id = "knife", chance = 9500},
	{id = "combat knife", chance = 4000},
	{id = "steel helmet", chance = 3230},
	{id = "steel shield", chance = 970},
	{id = "plate shield", chance = 1900},
	{id = "leopard armor", chance = 480},
	{id = "battle shield", chance = 1600},
	{id = "small diamond", chance = 220},
	{id = "horseman helmet", chance = 230}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -40, range = 7, shootEffect = 8},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -120, maxDamage = -160, range = 7, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = -5}
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
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)