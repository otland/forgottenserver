local mType = Game.createMonsterType("War Golem")
local monster = {}
monster.description = "a war golem"
monster.experience = 2550
monster.outfit = {
	lookType = 326
}

monster.health = 4300
monster.maxHealth = monster.health
monster.race = "energy"
monster.corpse = 10005
monster.speed = 240
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
	interval = 2500,
	chance = 10,
	{text = "Azerus barada nikto!", yell = false},
	{text = "Klonk klonk klonk", yell = false},
	{text = "Engaging Enemy!", yell = false},
	{text = "Threat level processed.", yell = false},
	{text = "Charging weapon systems!", yell = false},
	{text = "Auto repair in progress.", yell = false},
	{text = "The battle is joined!", yell = false},
	{text = "Termination initialized!", yell = false},
	{text = "Rrrtttarrrttarrrtta", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 37500},
	{id = "gold coin", maxCount = 80, chance = 100000},
	{id = "nail", maxCount = 5, chance = 5260},
	{id = "two handed sword", chance = 5500},
	{id = "plate shield", chance = 9000},
	{id = "morning star", chance = 7000},
	{id = "battle shield", chance = 6500},
	{id = "ultimate health potion", chance = 10080},
	{id = "great mana potion", chance = 8860},
	{id = "iron ore", chance = 1920},
	{id = "berserk potion", chance = 900},
	{id = "epee", chance = 6400},
	{id = "rusty armor", chance = 260},
	{id = "club ring", chance = 810},
	{id = "dwarven ring", chance = 1210},
	{id = "crystal of power", chance = 130},
	{id = "crystal pedestal", chance = 1080},
	{id = "lightning boots", chance = 90},
	{id = "steel boots", chance = 620},
	{id = "berserker", chance = 50},
	{id = "jade hammer", chance = 120},
	{id = "bonebreaker", chance = 770},
	{id = "life crystal", chance = 1000}
}

monster.attacks = {
	{name = "melee",  attack = 110, skill = 90, minDamage = 0, maxDamage = -550, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 15, interval = 2000, minDamage = -165, maxDamage = -220, range = 7, shootEffect = 5},
	{name = "outfit",  chance = 1, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 5},
	{name = "war golem electrify",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, range = 1, shootEffect = 5},
	{name = "war golem skill reducer",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 5}
}

monster.defenses = {
	defense = 40,
    armor = 40,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 300, shootEffect = 5, effect = 14},
	{name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 200, maxDamage = 250, shootEffect = 5, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 25},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_EARTHDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = 15},
	{type = COMBAT_HOLYDAMAGE, percent = 50},
	{type = COMBAT_DEATHDAMAGE, percent = 25},
	{type = COMBAT_ICEDAMAGE, percent = 30}
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