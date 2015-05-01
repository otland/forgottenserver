local mType = Game.createMonsterType("Worker Golem")
local monster = {}
monster.description = "a worker golem"
monster.experience = 1250
monster.outfit = {
	lookType = 304
}

monster.health = 1470
monster.maxHealth = monster.health
monster.race = "energy"
monster.corpse = 9801
monster.speed = 230
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
	{text = "INTRUDER ALARM!", yell = false},
	{text = "klonk klonk klonk", yell = false},
	{text = "Rrrtttarrrttarrrtta", yell = false},
	{text = "Awaiting orders.", yell = false},
	{text = "Secret objective complete.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 43000},
	{id = "gold coin", maxCount = 40, chance = 50000},
	{id = "small diamond", maxCount = 2, chance = 1000},
	{id = "nail", maxCount = 5, chance = 5000},
	{id = "iron ore", chance = 1001},
	{id = "gear wheel", chance = 1007},
	{id = "rusty armor", chance = 200},
	{id = "rusty legs", chance = 50},
	{id = "great spirit potion", chance = 830},
	{id = "great health potion", chance = 2100},
	{id = "berserk potion", chance = 820},
	{id = "spiked squelcher", chance = 1003},
	{id = "war hammer", chance = 920},
	{id = "life crystal", chance = 890},
	{id = "might ring", subType = 20, chance = 370},
	{id = "bonebreaker", chance = 130},
	{id = "great mana potion", chance = 1470},
	{id = "gear crystal", chance = 2270},
	{id = "crystal pedestal", chance = 2270}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 50, minDamage = 0, maxDamage = -240, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -125, range = 7, shootEffect = 10}
}

monster.defenses = {
	defense = 35,
    armor = 35,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 200, maxDamage = 250, shootEffect = 10, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE, percent = 50},
	{type = COMBAT_DEATHDAMAGE, percent = 10},
	{type = COMBAT_ICEDAMAGE, percent = 10}
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