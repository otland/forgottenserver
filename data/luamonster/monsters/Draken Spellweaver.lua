local mType = Game.createMonsterType("Draken Spellweaver")
local monster = {}
monster.description = "a draken spellweaver"
monster.experience = 3100
monster.outfit = {
	lookType = 340
}

monster.health = 5000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 11316
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
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Kazzzzuuuum!!", yell = false},
	{text = "Fissziss!", yell = false},
	{text = "Zzzzzooom!!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 41000},
	{id = "gold coin", maxCount = 100, chance = 58000},
	{id = "platinum coin", maxCount = 5, chance = 25510},
	{id = "small ruby", maxCount = 5, chance = 6910},
	{id = "focus cape", chance = 1450},
	{id = "meat", chance = 30400},
	{id = "harness", chance = 30},
	{id = "Zaoan shoes", chance = 1980},
	{id = "wand of inferno", chance = 1660},
	{id = "weaver's wandtip", chance = 19790},
	{id = "great mana potion", chance = 4970},
	{id = "ring of the sky", chance = 370},
	{id = "bamboo leaves", chance = 180},
	{id = "luminous orb", chance = 1980},
	{id = "spellweaver's robe", chance = 620},
	{id = "draken trophy", chance = 10},
	{id = "draken sulphur", chance = 3930},
	{id = "Zaoan robe", chance = 770},
	{id = "green gem", chance = 970}
}

monster.attacks = {
	{name = "melee",  attack = 63, skill = 70, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -240, maxDamage = -480, length = 4, spread = 3, effect = 6},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -100, maxDamage = -250, range = 7, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 10, interval = 2000, minDamage = -150, maxDamage = -300, range = 7, shootEffect = 5, effect = 12},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -200, maxDamage = -380, target = true, radius = 4, shootEffect = 5, effect = 3},
	{name = "soulfire",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 4, shootEffect = 5, effect = 3},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -280, maxDamage = -360, radius = 4, shootEffect = 15, effect = 3}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 4, shootEffect = 15, effect = 14},
	{name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 270, maxDamage = 530, radius = 4, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = 75},
	{type = COMBAT_ICEDAMAGE, percent = -10}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)