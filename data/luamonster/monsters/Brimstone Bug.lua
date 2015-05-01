local mType = Game.createMonsterType("Brimstone Bug")
local monster = {}
monster.description = "a brimstone bug"
monster.experience = 900
monster.outfit = {
	lookType = 352
}

monster.health = 1300
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 12527
monster.speed = 200
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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Chrrr!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 100, chance = 50000},
	{id = "poisonous slime", chance = 50000},
	{id = "lump of earth", chance = 20000},
	{id = "sulphurous stone", chance = 14970},
	{id = "brimstone shell", chance = 10000},
	{id = "strong mana potion", chance = 9025},
	{id = "strong health potion", chance = 9003},
	{id = "brimstone fangs", chance = 5710},
	{id = "small emerald", maxCount = 4, chance = 2702},
	{id = "magic sulphur", chance = 1639},
	{id = "stealth ring", chance = 892},
	{id = "platinum amulet", chance = 110}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 75, minDamage = 0, maxDamage = -213, interval = 2000, effect = 0},
	{name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, duration = 10000, speedChange = -600, range = 7, shootEffect = 11, effect = 18},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 5, interval = 2000, minDamage = -140, maxDamage = -310, radius = 6, shootEffect = 11, effect = 46},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = -130, maxDamage = -200, length = 6, shootEffect = 11, effect = 17},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -80, maxDamage = -120, length = 8, spread = 3, shootEffect = 11, effect = 8}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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