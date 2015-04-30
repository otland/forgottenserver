local mType = Game.createMonsterType("Efreet")
local monster = {}
monster.description = "an efreet"
monster.experience = 410
monster.outfit = {
	lookType = 103
}

monster.health = 550
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6032
monster.speed = 180
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.summons = {
    {name = "green djinn", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "I grant you a deathwish!", yell = false},
	{text = "Good wishes are for fairytales", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 75, chance = 50000},
	{id = "gold coin", maxCount = 50, chance = 60000},
	{id = "pear", maxCount = 5, chance = 9390},
	{id = "royal spear", maxCount = 3, chance = 15570},
	{id = "heavy machete", chance = 5000},
	{id = "small oil lamp", chance = 160},
	{id = "small emerald", chance = 7000},
	{id = "seeds", maxCount = 2, chance = 2000},
	{id = "green tapestry", chance = 2200},
	{id = "green piece of cloth", chance = 3000},
	{id = "wand of inferno", chance = 390},
	{id = "mystic turban", chance = 160},
	{id = "magma monocle", chance = 360},
	{id = "jewelled belt", chance = 8540},
	{id = "noble turban", chance = 1130},
	{id = "strong mana potion", chance = 3500},
	{id = "green gem", chance = 200}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 45, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -40, maxDamage = -110, range = 7, shootEffect = 4},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 10, interval = 2000, minDamage = -30, maxDamage = -90, radius = 3, shootEffect = 4, effect = 12},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 15, interval = 2000, minDamage = -65, maxDamage = -120, range = 7, radius = 3, shootEffect = 5, effect = 12},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 15000, speedChange = -650, range = 7, radius = 3, shootEffect = 5, effect = 14},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, radius = 3, shootEffect = 5, effect = 14},
	{name = "outfit",  chance = 1, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, radius = 3, shootEffect = 5, effect = 13},
	{name = "djinn electrify",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, range = 5, radius = 3, shootEffect = 5, effect = 13}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 50, maxDamage = 80, radius = 3, shootEffect = 5, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 60},
	{type = COMBAT_EARTHDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = 90},
	{type = COMBAT_HOLYDAMAGE, percent = -8},
	{type = COMBAT_DEATHDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = -5}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)