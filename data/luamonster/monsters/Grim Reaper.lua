local mType = Game.createMonsterType("Grim Reaper")
local monster = {}
monster.description = "a grim reaper"
monster.experience = 5500
monster.outfit = {
	lookType = 300
}

monster.health = 3900
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 8955
monster.speed = 260
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 20
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
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Death!", yell = false},
	{text = "Come a little closer!", yell = false},
	{text = "The end is near!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 263, chance = 99000},
	{id = "concentrated demonic blood", chance = 35000},
	{id = "demonic essence", chance = 10600},
	{id = "great mana potion", chance = 10000},
	{id = "ultimate health potion", chance = 9600},
	{id = "scythe", chance = 9000},
	{id = "mystical hourglass", chance = 5300},
	{id = "platinum coin", maxCount = 4, chance = 5200},
	{id = "magic light wand", chance = 4850},
	{id = "dark shield", chance = 3000},
	{id = "rusty armor", chance = 2500},
	{id = "orichalcum pearl", maxCount = 4, chance = 1400},
	{id = "underworld rod", chance = 910},
	{id = "nightmare blade", chance = 880},
	{id = "death ring", chance = 330},
	{id = "glacier kilt", chance = 330},
	{id = "skullcracker armor", chance = 270}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -165, range = 7, shootEffect = 32, effect = 18},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -350, maxDamage = -720, length = 8, shootEffect = 32, effect = 1},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -300, length = 7, spread = 3, shootEffect = 32, effect = 5},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -225, maxDamage = -275, radius = 4, shootEffect = 32, effect = 1}
}

monster.defenses = {
	defense = 35,
    armor = 35,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 130, maxDamage = 205, radius = 4, shootEffect = 32, effect = 1},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 450, radius = 4, shootEffect = 32, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 20},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 40},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = 80},
	{type = COMBAT_ICEDAMAGE, percent = 65}
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