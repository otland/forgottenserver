local mType = Game.createMonsterType("Death Priest")
local monster = {}
monster.description = "a death priest"
monster.experience = 750
monster.outfit = {
	lookFeet = 115,
	lookLegs = 119,
	lookHead = 95,
	lookType = 99,
	lookBody = 116
}

monster.health = 800
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 13975
monster.speed = 220
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

monster.loot = {
    {id = "gold coin", maxCount = 144, chance = 70000},
	{id = "hieroglyph banner", chance = 27000},
	{id = "health potion", chance = 15000},
	{id = "mana potion", chance = 15000},
	{id = "scarab coin", maxCount = 3, chance = 10000},
	{id = "spellbook", chance = 6800},
	{id = "orichalcum pearl", maxCount = 4, chance = 6000},
	{id = "white pearl", chance = 3000},
	{id = "ring of healing", chance = 1000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 20, interval = 2000, minDamage = -120, maxDamage = -250, range = 7, shootEffect = 11, effect = 18}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 2000, minDamage = 80, maxDamage = 200, shootEffect = 11, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 75}
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