-- const to string maps
elementToStringMap = {
	[COMBAT_PHYSICALDAMAGE] = "physical",
	[COMBAT_ENERGYDAMAGE] = "energy",
	[COMBAT_EARTHDAMAGE] = "earth",
	[COMBAT_FIREDAMAGE] = "fire",
	[COMBAT_UNDEFINEDDAMAGE] = "undefined",
	[COMBAT_LIFEDRAIN] = "lifedrain",
	[COMBAT_MANADRAIN] = "manadrain",
	[COMBAT_HEALING] = "healing",
	[COMBAT_DROWNDAMAGE] = "drowning",
	[COMBAT_ICEDAMAGE] = "ice",
	[COMBAT_HOLYDAMAGE] = "holy",
	[COMBAT_DEATHDAMAGE] = "death"
}

skillToStringMap = {
	[SKILL_LEVEL] = "level",
	[SKILL_MAGLEVEL] = "magic level",
	[SKILL_FIST] = "fist fighting",
	[SKILL_CLUB] = "club fighting",
	[SKILL_SWORD] = "sword fighting",
	[SKILL_AXE] = "axe fighting",
	[SKILL_DISTANCE] = "distance fighting",
	[SKILL_SHIELD] = "shielding",
	[SKILL_FISHING] = "fishing"
}

specialSkillToStringMap = {
	SPECIALSKILL_CRITICALHITCHANCE = "critical hit chance",
	SPECIALSKILL_CRITICALHITAMOUNT = "critical hit amount",
	SPECIALSKILL_LIFELEECHCHANCE = "life leech chance",
	SPECIALSKILL_LIFELEECHAMOUNT = "life leech amount",
	SPECIALSKILL_MANALEECHCHANCE = "mana leech chance",
	SPECIALSKILL_MANALEECHAMOUNT = "mana leech amount"
}

statToStringMap = {
	[STAT_MAXHITPOINTS] = "max health",
	[STAT_MAXMANAPOINTS] = "max mana",
	[STAT_SOULPOINTS] = "soul points",
	[STAT_MAGICPOINTS] = "magic level"
}

-- container
CONTAINER_POSITION = 0xFFFF

-- damage list
DAMAGELIST_EXPONENTIAL_DAMAGE = 0
DAMAGELIST_LOGARITHMIC_DAMAGE = 1
DAMAGELIST_VARYING_PERIOD = 2
DAMAGELIST_CONSTANT_PERIOD = 3

-- screenshot
-- player:takeScreenshot(screenshotType)
SCREENSHOT_TYPE_ACHIEVEMENT = 1
SCREENSHOT_TYPE_BESTIARYENTRYCOMPLETED = 2
SCREENSHOT_TYPE_BESTIARYENTRYUNLOCKED = 3
SCREENSHOT_TYPE_BOSSDEFEATED = 4
SCREENSHOT_TYPE_DEATHPVE = 5
SCREENSHOT_TYPE_DEATHPVP = 6
SCREENSHOT_TYPE_LEVELUP = 7
SCREENSHOT_TYPE_PLAYERKILLASSIST = 8
SCREENSHOT_TYPE_PLAYERKILL = 9
SCREENSHOT_TYPE_PLAYERATTACKING = 10
SCREENSHOT_TYPE_TREASUREFOUND = 11
SCREENSHOT_TYPE_SKILLUP = 12

SCREENSHOT_TYPE_FIRST = SCREENSHOT_TYPE_ACHIEVEMENT
SCREENSHOT_TYPE_LAST = SCREENSHOT_TYPE_SKILLUP