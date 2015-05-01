local mType = Game.createMonsterType("Goblin Assassin")
local monster = {}
monster.description = "a goblin assassin"
monster.experience = 52
monster.outfit = {
	lookType = 296
}

monster.health = 75
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6002
monster.speed = 200
monsters.runHealth = 15
monster.maxSummons = 0

monster.changeTarget = {
    interval = 10000,
	chance = 0
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Goblin Powahhh!", yell = false},
	{text = "Me kill you!", yell = false},
	{text = "Me green menace!", yell = false},
	{text = "Gobabunga!", yell = false},
	{text = "Gooobliiiins!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 9, chance = 50000},
	{id = "fish", chance = 12400},
	{id = "bone club", chance = 4770},
	{id = "short sword", chance = 8820},
	{id = "bone", chance = 13000},
	{id = "dagger", chance = 17000},
	{id = "leather armor", chance = 7240},
	{id = "small axe", chance = 9800},
	{id = "small stone", maxCount = 3, chance = 9900},
	{id = "mouldy cheese", chance = 6610},
	{id = "leather helmet", chance = 13000}
}

monster.attacks = {
	{name = "melee",  attack = 15, skill = 10, minDamage = 0, maxDamage = -15, interval = 2000, effect = 0},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 15},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -35, range = 6, shootEffect = 9}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 9, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 3000, speedChange = 100, shootEffect = 9, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = -1},
	{type = COMBAT_HOLYDAMAGE, percent = 1}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
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