local mType = Game.createMonsterType("Goblin Scavenger")
local monster = {}
monster.description = "a goblin scavenger"
monster.experience = 37
monster.outfit = {
	lookType = 297
}

monster.health = 60
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6002
monster.speed = 180
monster.runHealth = 10
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
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
	{text = "Shiny, Shiny!", yell = false},
	{text = "You mean!", yell = false},
	{text = "All mine!", yell = false},
	{text = "Uhh!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 9, chance = 50810},
	{id = "small stone", maxCount = 2, chance = 25560},
	{id = "dagger", chance = 18280},
	{id = "bone", chance = 12450},
	{id = "small axe", chance = 9790},
	{id = "fish", chance = 13640},
	{id = "leather helmet", chance = 10180},
	{id = "mouldy cheese", chance = 7000},
	{id = "short sword", chance = 8900},
	{id = "leather armor", chance = 7700},
	{id = "bone club", chance = 5000}
}

monster.attacks = {
	{name = "melee",  attack = 15, skill = 10, minDamage = 0, maxDamage = -15, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -30, range = 7, shootEffect = 1},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 5, interval = 2000, minDamage = -22, maxDamage = -30, range = 7, shootEffect = 1, effect = 14},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 5, interval = 2000, minDamage = -1, maxDamage = -30, range = 7, shootEffect = 32, effect = 39}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 10, maxDamage = 16, shootEffect = 32, effect = 12}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
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