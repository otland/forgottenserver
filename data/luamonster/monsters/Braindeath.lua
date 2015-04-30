local mType = Game.createMonsterType("Braindeath")
local monster = {}
monster.description = "a braindeath"
monster.experience = 895
monster.outfit = {
	lookType = 256
}

monster.health = 1225
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 7256
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
	canPushCreatures = true,
	targetDistance = 4,
	staticAttack = 80
}

monster.summons = {
    {name = "Vampire", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "You have disturbed my thoughts!", yell = false},
	{text = "Let me turn you into something more useful!", yell = false},
	{text = "Let me taste your brain!", yell = false},
	{text = "You will be punished!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 89, chance = 55000},
	{id = "sniper arrow", maxCount = 4, chance = 9000},
	{id = "spellbook", chance = 690},
	{id = "bone sword", chance = 8000},
	{id = "clerical mace", chance = 2000},
	{id = "steel shield", chance = 6000},
	{id = "bonelord eye", chance = 3380},
	{id = "haunted blade", chance = 1500},
	{id = "bonelord shield", chance = 750},
	{id = "spiked squelcher", chance = 950},
	{id = "bonelord helmet", chance = 180},
	{id = "piece of dead brain", chance = 4725}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 10, interval = 2000, minDamage = -93, maxDamage = -170, range = 7, shootEffect = 5},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -75, maxDamage = -125, range = 7, shootEffect = 4},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -85, maxDamage = -170, range = 7, shootEffect = 32, effect = 39},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -65, maxDamage = -125, range = 7, shootEffect = 15, effect = 39},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -75, maxDamage = -85, range = 7, shootEffect = 15, effect = 14},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -155, range = 7, shootEffect = 15, effect = 14}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 150, maxDamage = 200, shootEffect = 15, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 260, shootEffect = 15, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = -15},
	{type = COMBAT_HOLYDAMAGE, percent = -20},
	{type = COMBAT_ICEDAMAGE, percent = 20}
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)