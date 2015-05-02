local mType = Game.createMonsterType("Draken Abomination")
local monster = {}
monster.description = "a draken abomination"
monster.experience = 3800
monster.outfit = {
	lookType = 357
}

monster.health = 6250
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 12623
monster.speed = 230
monster.maxSummons = 2

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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 70
}

monster.summons = {
    {name = "Death Blob", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Ugggh!", yell = false},
	{text = "Gll", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 98, chance = 47000},
	{id = "platinum coin", maxCount = 8, chance = 50590},
	{id = "meat", maxCount = 4, chance = 50450},
	{id = "ultimate health potion", maxCount = 3, chance = 9400},
	{id = "great mana potion", maxCount = 3, chance = 9950},
	{id = "eye of corruption", chance = 12110},
	{id = "tail of corruption", chance = 6240},
	{id = "small topaz", maxCount = 4, chance = 2900},
	{id = "great spirit potion", maxCount = 3, chance = 4905},
	{id = "terra hood", chance = 8730},
	{id = "scale of corruption", chance = 10940},
	{id = "wand of voodoo", chance = 1020},
	{id = "draken boots", chance = 540},
	{id = "shield of corruption", chance = 10},
	{id = "snake god's wristguard", chance = 10},
	{id = "bamboo leaves", chance = 360},
	{id = "Zaoan legs", chance = 780},
	{id = "Zaoan armor", chance = 470},
	{id = "Zaoan helmet", chance = 560}
}

monster.attacks = {
	{name = "melee",  attack = 70, skill = 110, minDamage = 0, maxDamage = -420, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -310, maxDamage = -630, length = 4, spread = 3, effect = 6},
	{name = "draken abomination curse",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 5, effect = 6},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 15, interval = 2000, minDamage = -170, maxDamage = -370, length = 4, effect = 18},
	{name = "drunk",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, radius = 4, shootEffect = 15, effect = 21},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, radius = 3, shootEffect = 15, effect = 17}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 650, maxDamage = 700, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = 5}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)