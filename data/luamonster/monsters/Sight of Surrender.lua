local mType = Game.createMonsterType("Sight of Surrender")
local monster = {}
monster.description = "sight of surrender"
monster.experience = 17000
monster.outfit = {
	lookType = 583
}

monster.health = 28000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 22478
monster.speed = 300
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 20
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
	{text = "BOW LOW!", yell = false},
	{text = "FEEL THE TRUE MEANING OF VANQUISH!", yell = false},
	{text = "HAHAHAHA DO YOU WANT TO AMUSE YOUR MASTER?", yell = false},
	{text = "NOW YOU WILL SURRENDER!", yell = false}
}

monster.loot = {
    {id = "platinum coin", maxCount = 20, chance = 100000},
	{id = "might ring", subType = 20, chance = 8000},
	{id = "stone skin amulet", subType = 5, chance = 18000},
	{id = "hammer of wrath", chance = 1380},
	{id = "crystal mace", chance = 5500},
	{id = "magic plate armor", chance = 1380},
	{id = "crusader helmet", chance = 920},
	{id = "crown legs", chance = 920},
	{id = "tower shield", chance = 1380},
	{id = "steel boots", chance = 920},
	{id = "onyx flail", chance = 920},
	{id = "jade hammer", chance = 920},
	{id = "great mana potion", maxCount = 5, chance = 78000},
	{id = "great spirit potion", maxCount = 5, chance = 72000},
	{id = "ultimate health potion", maxCount = 5, chance = 30000},
	{id = "blue crystal shard", maxCount = 3, chance = 23000},
	{id = "violet crystal shard", maxCount = 3, chance = 32000},
	{id = "green crystal shard", maxCount = 3, chance = 21600},
	{id = "green crystal splinter", maxCount = 5, chance = 30000},
	{id = "brown crystal splinter", maxCount = 5, chance = 30410},
	{id = "blue crystal splinter", maxCount = 5, chance = 40000},
	{id = "cluster of solace", chance = 1800},
	{id = "sight of surrender's eye", chance = 100000},
	{id = "broken visor", chance = 100000},
	{id = "", chance = 460}
}

monster.attacks = {
	{name = "melee",  attack = 200, skill = 100, minDamage = 0, maxDamage = -1100, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 15, interval = 2000, minDamage = -500, maxDamage = -800, length = 8, effect = 49},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -500, target = true, radius = 1, shootEffect = 12, effect = 49}
}

monster.defenses = {
	defense = 70,
    armor = 70,
    {name = "combat", type = COMBAT_HEALING,  chance = 30, interval = 2000, minDamage = 550, maxDamage = 1100, shootEffect = 12, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 520, shootEffect = 12, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 40},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = 35},
	{type = COMBAT_HOLYDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = 10},
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