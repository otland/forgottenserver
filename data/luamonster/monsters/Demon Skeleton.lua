local mType = Game.createMonsterType("Demon Skeleton")
local monster = {}
monster.description = "a demon skeleton"
monster.experience = 240
monster.outfit = {
	lookType = 37
}

monster.health = 400
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 5963
monster.speed = 170
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
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 75, chance = 97000},
	{id = "torch", chance = 5270},
	{id = "throwing star", maxCount = 3, chance = 10000},
	{id = "battle hammer", chance = 4000},
	{id = "iron helmet", chance = 3450},
	{id = "battle shield", chance = 5000},
	{id = "mysterious fetish", chance = 690},
	{id = "mana potion", chance = 5300},
	{id = "health potion", maxCount = 2, chance = 10120},
	{id = "mind stone", chance = 520},
	{id = "guardian shield", chance = 100},
	{id = "health potion", maxCount = 2, chance = 10000},
	{id = "demonic skeletal hand", chance = 12600},
	{id = "black pearl", chance = 2900},
	{id = "small ruby", chance = 1400}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 64, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -30, maxDamage = -50, target = true, range = 1, radius = 1, effect = 39}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = false, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)