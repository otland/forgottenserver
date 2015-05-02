local mType = Game.createMonsterType("Enslaved Dwarf")
local monster = {}
monster.description = "enslaved dwarf"
monster.experience = 2900
monster.outfit = {
	lookType = 494
}

monster.health = 4200
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 17408
monster.speed = 280
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
	{text = "Bark!", yell = false},
	{text = "Blood!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 99, chance = 50000},
	{id = "gold coin", maxCount = 50, chance = 50000},
	{id = "platinum coin", maxCount = 6, chance = 100000},
	{id = "great mana potion", maxCount = 2, chance = 6660},
	{id = "brown mushroom", maxCount = 2, chance = 15150},
	{id = "great health potion", chance = 7960},
	{id = "glorious axe", chance = 1940},
	{id = "green crystal splinter", chance = 3110},
	{id = "ancient shield", chance = 3690},
	{id = "spiked squelcher", chance = 80},
	{id = "sapphire hammer", chance = 80},
	{id = "iron ore", chance = 11070},
	{id = "brown crystal splinter", maxCount = 2, chance = 5630},
	{id = "drill bolt", maxCount = 5, chance = 3690},
	{id = "guardian shield", chance = 2300},
	{id = "tower shield", chance = 190},
	{id = "small amethyst", maxCount = 2, chance = 10680},
	{id = "small emerald", maxCount = 2, chance = 10290},
	{id = "axe ring", chance = 1550},
	{id = "red crystal fragment", chance = 3690},
	{id = "green crystal shard", chance = 1750},
	{id = "war hammer", chance = 5050},
	{id = "titan axe", chance = 1170},
	{id = "warrior helmet", chance = 580},
	{id = "shiny stone", chance = 4660},
	{id = "coal", chance = 780}
}

monster.attacks = {
	{name = "melee",  attack = 91, skill = 100, minDamage = 0, maxDamage = -501, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -340, range = 7, shootEffect = 12},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -250, target = true, range = 7, radius = 3, shootEffect = 41, effect = 6},
	{name = "drunk",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, radius = 5, shootEffect = 41, effect = 4},
	{name = "enslaved dwarf skill reducer 1",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 41, effect = 4},
	{name = "enslaved dwarf skill reducer 2",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 41, effect = 4}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 396, maxDamage = 478, shootEffect = 41, effect = 15}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = 1},
	{type = COMBAT_ICEDAMAGE, percent = 1}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)