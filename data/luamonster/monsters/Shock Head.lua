local mType = Game.createMonsterType("Shock Head")
local monster = {}
monster.description = "shock head"
monster.experience = 4700
monster.outfit = {
	lookType = 579
}

monster.health = 5800
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 22392
monster.speed = 250
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
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "<brrzz>", yell = false},
	{text = "Thun... Thun... THUNDER!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 43000},
	{id = "gold coin", maxCount = 87, chance = 50000},
	{id = "small sapphire", chance = 5000},
	{id = "royal helmet", chance = 270},
	{id = "goosebump leather", chance = 10000}
}

monster.attacks = {
	{name = "melee",  attack = 145, skill = 100, minDamage = 0, maxDamage = -798, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 15, interval = 2000, minDamage = -200, maxDamage = -300, length = 5, spread = 2, effect = 158},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 7500, speedChange = -800, length = 8, spread = 3, effect = 48},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -350, target = true, radius = 4, shootEffect = 30, effect = 45},
	{name = "shock head skill reducer 1",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, range = 5, shootEffect = 30, effect = 45},
	{name = "shock head skill reducer 2",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 30, effect = 45}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 250, maxDamage = 350, shootEffect = 30, effect = 69}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_DEATHDAMAGE, percent = 30},
	{type = COMBAT_ICEDAMAGE, percent = 5}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)