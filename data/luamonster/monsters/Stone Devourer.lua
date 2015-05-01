local mType = Game.createMonsterType("Stone Devourer")
local monster = {}
monster.description = "a stone devourer"
monster.experience = 2900
monster.outfit = {
	lookType = 486
}

monster.health = 4200
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 18375
monster.speed = 250
monsters.runHealth = 1
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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Rumble!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "platinum coin", maxCount = 7, chance = 92000},
	{id = "stone nose", chance = 18679},
	{id = "crystalline spikes", chance = 16320},
	{id = "great mana potion", maxCount = 2, chance = 15610},
	{id = "mana potion", maxCount = 2, chance = 15050},
	{id = "strong mana potion", maxCount = 2, chance = 14900},
	{id = "ultimate health potion", chance = 14410},
	{id = "strong health potion", maxCount = 2, chance = 13840},
	{id = "ancient stone", chance = 12850},
	{id = "coal", chance = 11360},
	{id = "crystalline arrow", maxCount = 10, chance = 9940},
	{id = "green crystal splinter", chance = 6960},
	{id = "cyan crystal fragment", chance = 6810},
	{id = "glorious axe", chance = 3340},
	{id = "dwarven ring", chance = 2840},
	{id = "stone skin amulet", subType = 5, chance = 2270},
	{id = "sapphire hammer", chance = 1490},
	{id = "spiked squelcher", chance = 1490},
	{id = "war axe", chance = 920},
	{id = "crystal mace", chance = 850},
	{id = "giant sword", chance = 570}
}

monster.attacks = {
	{name = "melee",  attack = 150, skill = 122, minDamage = 0, maxDamage = -990, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -230, maxDamage = -460, range = 7, shootEffect = 39, effect = 45},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -650, range = 7, shootEffect = 12, effect = 45},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -150, maxDamage = -260, length = 5, shootEffect = 12, effect = 45}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 1},
	{type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_HOLYDAMAGE, percent = 30},
	{type = COMBAT_DEATHDAMAGE, percent = 1},
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)