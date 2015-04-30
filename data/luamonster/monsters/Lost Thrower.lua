local mType = Game.createMonsterType("Lost Thrower")
local monster = {}
monster.description = "a lost thrower"
monster.experience = 1200
monster.outfit = {
	lookType = 539
}

monster.health = 1700
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 19998
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 4,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 37, chance = 50000},
	{id = "platinum coin", maxCount = 2, chance = 70000},
	{id = "brown mushroom", maxCount = 2, chance = 14500},
	{id = "lost bracers", chance = 14860},
	{id = "helmet of the lost", chance = 13560},
	{id = "basalt figurine", chance = 7900},
	{id = "basalt fetish", chance = 6150},
	{id = "broken throwing axe", chance = 1390},
	{id = "holy ash", chance = 15670},
	{id = "lost basher's spike", chance = 11410},
	{id = "mad froth", chance = 11810},
	{id = "iron ore", chance = 9470},
	{id = "buckle", chance = 850},
	{id = "great health potion", chance = 14730},
	{id = "great mana potion", chance = 13470},
	{id = "coal", chance = 12180}
}

monster.attacks = {
	{name = "melee",  attack = 70, skill = 76, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -250, target = true, range = 7, radius = 1, shootEffect = 8, effect = 5},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, radius = 2, shootEffect = 8, effect = 14},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = -150, maxDamage = -300, target = true, range = 7, radius = 2, shootEffect = 27, effect = 32},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, radius = 3, shootEffect = 26, effect = 5}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 5, interval = 2000, minDamage = 100, maxDamage = 500, radius = 3, shootEffect = 26, effect = 13},
	{name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, shootEffect = 26, effect = 11}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_ICEDAMAGE, percent = 5},
	{type = COMBAT_DEATHDAMAGE, percent = 5},
	{type = COMBAT_FIREDAMAGE, percent = 40}
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
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)