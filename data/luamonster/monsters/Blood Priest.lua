local mType = Game.createMonsterType("Blood Priest")
local monster = {}
monster.description = "a blood priest"
monster.experience = 900
monster.outfit = {
	lookType = 553
}

monster.health = 820
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 21262
monster.speed = 210
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
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Blood for the dark god!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 71, chance = 50000},
	{id = "blood tincture in a vial", chance = 14460},
	{id = "lancet", chance = 14410},
	{id = "incantation notes", chance = 14660},
	{id = "book of necromantic rituals", chance = 13550},
	{id = "horoscope", chance = 9270},
	{id = "pieces of magic chalk", chance = 7770},
	{id = "strong mana potion", chance = 6000},
	{id = "small ruby", maxCount = 2, chance = 3510},
	{id = "underworld rod", chance = 470},
	{id = "spellbook of mind control", chance = 180},
	{id = "rough red gem", chance = 880},
	{id = "white piece of cloth", chance = 3040},
	{id = "mystic turban", chance = 2170},
	{id = "boots of haste", chance = 120},
	{id = "red piece of cloth", chance = 640},
	{id = "skull staff", chance = 180},
	{id = "spellbook of warding", chance = 290}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 20, interval = 2000, minDamage = -60, maxDamage = -100, range = 7, shootEffect = 11, effect = 39},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -40, maxDamage = -60, radius = 4, shootEffect = 11, effect = 14},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 3000, minDamage = -80, maxDamage = -130, range = 1, length = 7, radius = 4, shootEffect = 11, effect = 10},
	{name = "condition", type = CONDITION_BLEEDING,  chance = 5, interval = 2000, minDamage = -160, maxDamage = -290, target = true, range = 1, radius = 1, shootEffect = 11, effect = 1}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 2000, minDamage = 80, maxDamage = 120, radius = 1, shootEffect = 11, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = 5},
	{type = COMBAT_ICEDAMAGE, percent = 5}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)