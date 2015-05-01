local mType = Game.createMonsterType("Deepling Warrior")
local monster = {}
monster.description = "a deepling warrior"
monster.experience = 1500
monster.outfit = {
	lookType = 441
}

monster.health = 1600
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 15175
monster.speed = 210
monsters.runHealth = 30
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
	{text = "Jou wjil all djie!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 60000},
	{id = "gold coin", maxCount = 80, chance = 60000},
	{id = "deeptags", chance = 14285},
	{id = "deepling filet", chance = 14285},
	{id = "deepling ridge", chance = 11111},
	{id = "great health potion", chance = 11111},
	{id = "deepling warts", chance = 10000},
	{id = "great mana potion", chance = 9090},
	{id = "vortex bolt", maxCount = 5, chance = 3571},
	{id = "life ring", chance = 2941},
	{id = "small emerald", chance = 2854},
	{id = "heavy trident", chance = 1030},
	{id = "fish fin", chance = 961},
	{id = "warrior's shield", chance = 684},
	{id = "eye of a deepling", chance = 574},
	{id = "warrior's axe", chance = 534}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 50, minDamage = 0, maxDamage = -300, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -290, range = 7, shootEffect = 26}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 50, maxDamage = 150, shootEffect = 26, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = -20}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)