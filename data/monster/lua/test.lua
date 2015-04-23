-- a testing layout
local test = NewMonsterType("Test")
local monster = {}
monster.description = "a Test"
monster.experience = 5000
monster.outfit = {
    lookType = 698,
    lookMount = 368,
    lookAddons = 3
}

monster.health = 1000
monster.maxHealth = monster.health
monster.race = "fire"
monster.skull = "black"
monster.corpse = 1987

monster.flags = {
    attackable = true,
    hiddenHealth = false,
    convinceable = true
}

monster.voices = {
	{text = "Works", interval = 5000, chance = 10000, yell = false},
	{text = "I can talk", interval = 5000, chance = 10000, yell = false},
	{text = "Test", interval = 5000, chance = 10000, yell = false}
}

monster.loot = {
	{id = "backpack", chance = 100000, child = {{id = "magic sword", chance = 100000}, {id = "gold coin", chance = 100000, maxCount = 100},{id = "club", chance = 100000}}},
	{id = "crystal coin", chance = 100000, maxCount = 100}
}

monster.attacks = {
	{name = "melee", attack = 300, skill = 130, effect = CONST_ME_DRAWBLOOD, interval = 2*1000, condition = {type = CONDITION_FREEZING, startDamage = -100, minDamage = -100, maxDamage = -120, duration = 30*1000, interval = 1000}},
	{script = "attack/divine caldera.lua", chance = 100, interval = 5*1000, minDamage = -1000, maxDamage = -5000},
	{script = "attack/energy strike.lua", chance = 40, interval = 3*1000, minDamage = -300, maxDamage = -1200, target = true},
	{name = "condition", type = CONDITION_FIRE, chance = 100, interval = 10*1000, duration = 5*1000, minDamage = -500, maxDamage = -600, target = true, effect = CONST_ME_MAGIC_BLUE, shootEffect = CONST_ANI_FIRE},
	{name = "combat", type = COMBAT_EARTHDAMAGE, chance = 100, interval = 3*1000, minDamage = -700, maxDamage = -2100, radius = 3, effect = CONST_ME_HITBYPOISON}
}

monster.defenses = {
	{name = "combat", type = COMBAT_HEALING, chance = 55, interval = 3*1000, minDamage = 1000, maxDamage = 2000, effect = CONST_ME_MAGIC_BLUE},
	{name = "speed", chance = 90, interval = 10*1000, speed = 1000, effect = CONST_ME_MAGIC_GREEN}
}

test:register(monster)





