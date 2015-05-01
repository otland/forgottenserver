local mType = Game.createMonsterType("Grand Mother Foulscale")
local monster = {}
monster.description = "a grand mother foulscale"
monster.experience = 1400
monster.outfit = {
	lookType = 34
}

monster.health = 1850
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5973
monster.speed = 180
monsters.runHealth = 400
monster.maxSummons = 4

monster.changeTarget = {
    interval = 5000,
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

monster.summons = {
    {name = "dragon hatchlings", chance = 40, interval = 4000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "GROOAAARRR", yell = true},
	{text = "FCHHHHH", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 70, chance = 37500},
	{id = "gold coin", maxCount = 50, chance = 37500},
	{id = "burst arrow", maxCount = 12, chance = 4000},
	{id = "dragon ham", maxCount = 3, chance = 15500},
	{id = "short sword", chance = 25000},
	{id = "mace", chance = 21500},
	{id = "steel shield", chance = 14000},
	{id = "crossbow", chance = 10000},
	{id = "longsword", chance = 5000},
	{id = "steel helmet", chance = 3000},
	{id = "plate legs", chance = 2000},
	{id = "broadsword", chance = 2000},
	{id = "strong health potion", chance = 1750},
	{id = "double axe", chance = 1333},
	{id = "wand of inferno", chance = 1800},
	{id = "green dragon scale", chance = 100000},
	{id = "dragon hammer", chance = 600},
	{id = "green dragon leather", chance = 100000},
	{id = "small diamond", chance = 500},
	{id = "dragon shield", chance = 500},
	{id = "serpent sword", chance = 500},
	{id = "dragonbone staff", chance = 650},
	{id = "life crystal", chance = 150}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = -20, maxDamage = -170, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -45, maxDamage = -85, target = true, range = 7, radius = 4, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 8, interval = 2000, minDamage = -90, maxDamage = -150, length = 8, spread = 3, shootEffect = 4, effect = 7}
}

monster.defenses = {
	defense = 20,
    armor = 27,
    {name = "combat", type = COMBAT_HEALING,  chance = 17, interval = 1000, minDamage = 34, maxDamage = 66, shootEffect = 4, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 80},
	{type = COMBAT_ICEDAMAGE, percent = -10}
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