local mType = Game.createMonsterType("Nightstalker")
local monster = {}
monster.description = "a nightstalker"
monster.experience = 500
monster.outfit = {
	lookType = 320
}

monster.health = 700
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 9915
monster.speed = 260
monster.maxSummons = 0

monster.changeTarget = {
    interval = 0,
	chance = 8
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
	{text = "The sunlight is so depressing.", yell = false},
	{text = "Come with me, my child.", yell = false},
	{text = "I've been in the shadow under your bed last night.", yell = false},
	{text = "You never know what hides in the night.", yell = false},
	{text = "I remember your face - and I know where you sleep.", yell = false},
	{text = "Only the sweetest and cruelest dreams for you, my love.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 10, chance = 50000},
	{id = "shadow herb", chance = 4761},
	{id = "strong mana potion", chance = 1612},
	{id = "crystal ring", chance = 1030},
	{id = "protection amulet", subType = 250, chance = 847},
	{id = "haunted blade", chance = 318},
	{id = "crystal of balance", chance = 127},
	{id = "boots of haste", chance = 121},
	{id = "chaos mace", chance = 121},
	{id = "platinum amulet", chance = 121},
	{id = "spirit cloak", chance = 520}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 50, minDamage = 0, maxDamage = -90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 20, interval = 2000, minDamage = -60, maxDamage = -170, range = 7, effect = 50},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 15000, speedChange = -600, effect = 33}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 240, effect = 14},
	{name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, effect = 8},
	{name = "outfit",  chance = 10, interval = 5000, minDamage = 0, maxDamage = 0, effect = 8},
	{name = "outfit",  chance = 10, interval = 5000, minDamage = 0, maxDamage = 0, effect = 8},
	{name = "outfit",  chance = 10, interval = 5000, minDamage = 0, maxDamage = 0, effect = 8},
	{name = "outfit",  chance = 10, interval = 5000, minDamage = 0, maxDamage = 0, effect = 8},
	{name = "outfit",  chance = 10, interval = 5000, minDamage = 0, maxDamage = 0, effect = 8},
	{name = "outfit",  chance = 1, interval = 5000, minDamage = 0, maxDamage = 0, effect = 8}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = 20}
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