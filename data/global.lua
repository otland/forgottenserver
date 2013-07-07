TRUE = true
FALSE = false

LUA_ERROR = false
LUA_NO_ERROR = true

NORTH = 0
EAST = 1
SOUTH = 2
WEST = 3
SOUTHWEST = 4
SOUTHEAST = 5
NORTHWEST = 6
NORTHEAST = 7

COMBAT_FORMULA_UNDEFINED = 0
COMBAT_FORMULA_LEVELMAGIC = 1
COMBAT_FORMULA_SKILL = 2
COMBAT_FORMULA_DAMAGE = 3

CONDITION_PARAM_OWNER = 1
CONDITION_PARAM_TICKS = 2
CONDITION_PARAM_OUTFIT = 3
CONDITION_PARAM_HEALTHGAIN = 4
CONDITION_PARAM_HEALTHTICKS = 5
CONDITION_PARAM_MANAGAIN = 6
CONDITION_PARAM_MANATICKS = 7
CONDITION_PARAM_DELAYED = 8
CONDITION_PARAM_SPEED = 9
CONDITION_PARAM_LIGHT_LEVEL = 10
CONDITION_PARAM_LIGHT_COLOR = 11
CONDITION_PARAM_SOULGAIN = 12
CONDITION_PARAM_SOULTICKS = 13
CONDITION_PARAM_MINVALUE = 14
CONDITION_PARAM_MAXVALUE = 15
CONDITION_PARAM_STARTVALUE = 16
CONDITION_PARAM_TICKINTERVAL = 17
CONDITION_PARAM_FORCEUPDATE = 18
CONDITION_PARAM_SKILL_MELEE = 19
CONDITION_PARAM_SKILL_FIST = 20
CONDITION_PARAM_SKILL_CLUB = 21
CONDITION_PARAM_SKILL_SWORD = 22
CONDITION_PARAM_SKILL_AXE = 23
CONDITION_PARAM_SKILL_DISTANCE = 24
CONDITION_PARAM_SKILL_SHIELD = 25
CONDITION_PARAM_SKILL_FISHING = 26
CONDITION_PARAM_STAT_MAXHITPOINTS = 27
CONDITION_PARAM_STAT_MAXMANAPOINTS = 28
CONDITION_PARAM_STAT_SOULPOINTS = 29
CONDITION_PARAM_STAT_MAGICPOINTS = 30
CONDITION_PARAM_STAT_MAXHITPOINTSPERCENT = 31
CONDITION_PARAM_STAT_MAXMANAPOINTSPERCENT = 32
CONDITION_PARAM_STAT_SOULPOINTSPERCENT = 33
CONDITION_PARAM_STAT_MAGICPOINTSPERCENT = 34
CONDITION_PARAM_PERIODICDAMAGE = 35
CONDITION_PARAM_SKILL_MELEEPERCENT = 36
CONDITION_PARAM_SKILL_FISTPERCENT = 37
CONDITION_PARAM_SKILL_CLUBPERCENT = 38
CONDITION_PARAM_SKILL_SWORDPERCENT = 39
CONDITION_PARAM_SKILL_AXEPERCENT = 40
CONDITION_PARAM_SKILL_DISTANCEPERCENT = 41
CONDITION_PARAM_SKILL_SHIELDPERCENT = 42
CONDITION_PARAM_SKILL_FISHINGPERCENT = 43
CONDITION_PARAM_BUFF_SPELL = 44
CONDITION_PARAM_SUBID = 45

COMBAT_PARAM_TYPE = 1
COMBAT_PARAM_EFFECT = 2
COMBAT_PARAM_DISTANCEEFFECT = 3
COMBAT_PARAM_BLOCKSHIELD = 4
COMBAT_PARAM_BLOCKARMOR = 5
COMBAT_PARAM_TARGETCASTERORTOPMOST = 6
COMBAT_PARAM_CREATEITEM = 7
COMBAT_PARAM_AGGRESSIVE = 8
COMBAT_PARAM_DISPEL = 9

CALLBACK_PARAM_LEVELMAGICVALUE = 1
CALLBACK_PARAM_SKILLVALUE = 2
CALLBACK_PARAM_TARGETTILE = 3
CALLBACK_PARAM_TARGETCREATURE = 4

COMBAT_NONE = 0
COMBAT_PHYSICALDAMAGE = 1
COMBAT_ENERGYDAMAGE = 2
COMBAT_EARTHDAMAGE = 4
COMBAT_POISONDAMAGE = 4
COMBAT_FIREDAMAGE = 8
COMBAT_UNDEFINEDDAMAGE = 16
COMBAT_LIFEDRAIN = 32
COMBAT_MANADRAIN = 64
COMBAT_HEALING = 128
COMBAT_DROWNDAMAGE = 256
COMBAT_ICEDAMAGE = 512
COMBAT_HOLYDAMAGE = 1024
COMBAT_DEATHDAMAGE = 2048

CONDITION_NONE = 0
CONDITION_POISON = 1
CONDITION_FIRE = 2
CONDITION_ENERGY = 4
CONDITION_BLEEDING = 8
CONDITION_HASTE = 16
CONDITION_PARALYZE = 32
CONDITION_OUTFIT = 64
CONDITION_INVISIBLE = 128
CONDITION_LIGHT = 256
CONDITION_MANASHIELD = 512
CONDITION_INFIGHT = 1024
CONDITION_DRUNK = 2048
CONDITION_EXHAUST_WEAPON = 4096
CONDITION_FOOD = 8192
CONDITION_REGENERATION = 8192
CONDITION_SOUL = 16384
CONDITION_DROWN = 32768
CONDITION_MUTED = 65536
CONDITION_ADVERTISINGTICKS = 131072
CONDITION_TRADETICKS = CONDITION_ADVERTISINGTICKS
CONDITION_YELLTICKS = 262144
CONDITION_ATTRIBUTES = 524288
CONDITION_FREEZING = 1048576
CONDITION_DAZZLED = 2097152
CONDITION_CURSED = 4194304
CONDITION_EXHAUST_COMBAT = 8388608
CONDITION_EXHAUST_HEAL = 16777216
CONDITION_PACIFIED = 33554432

CONST_SLOT_HEAD = 1
CONST_SLOT_NECKLACE = 2
CONST_SLOT_BACKPACK = 3
CONST_SLOT_ARMOR = 4
CONST_SLOT_RIGHT = 5
CONST_SLOT_LEFT = 6
CONST_SLOT_LEGS = 7
CONST_SLOT_FEET = 8
CONST_SLOT_RING = 9
CONST_SLOT_AMMO = 10

CONST_ME_DRAWBLOOD = 0
CONST_ME_LOSEENERGY = 1
CONST_ME_POFF = 2
CONST_ME_BLOCKHIT = 3
CONST_ME_EXPLOSIONAREA = 4
CONST_ME_EXPLOSIONHIT = 5
CONST_ME_FIREAREA = 6
CONST_ME_YELLOW_RINGS = 7
CONST_ME_GREEN_RINGS = 8
CONST_ME_HITAREA = 9
CONST_ME_TELEPORT = 10
CONST_ME_ENERGYHIT = 11
CONST_ME_MAGIC_BLUE = 12
CONST_ME_MAGIC_RED = 13
CONST_ME_MAGIC_GREEN = 14
CONST_ME_HITBYFIRE = 15
CONST_ME_HITBYPOISON = 16
CONST_ME_MORTAREA = 17
CONST_ME_SOUND_GREEN = 18
CONST_ME_SOUND_RED = 19
CONST_ME_POISONAREA = 20
CONST_ME_SOUND_YELLOW = 21
CONST_ME_SOUND_PURPLE = 22
CONST_ME_SOUND_BLUE = 23
CONST_ME_SOUND_WHITE = 24
CONST_ME_BUBBLES = 25
CONST_ME_CRAPS = 26
CONST_ME_GIFT_WRAPS = 27
CONST_ME_FIREWORK_YELLOW = 28
CONST_ME_FIREWORK_RED = 29
CONST_ME_FIREWORK_BLUE = 30
CONST_ME_STUN = 31
CONST_ME_SLEEP = 32
CONST_ME_WATERCREATURE = 33
CONST_ME_GROUNDSHAKER = 34
CONST_ME_HEARTS = 35
CONST_ME_FIREATTACK = 36
CONST_ME_ENERGYAREA = 37
CONST_ME_SMALLCLOUDS = 38
CONST_ME_HOLYDAMAGE = 39
CONST_ME_BIGCLOUDS = 40
CONST_ME_ICEAREA = 41
CONST_ME_ICETORNADO = 42
CONST_ME_ICEATTACK = 43
CONST_ME_STONES = 44
CONST_ME_SMALLPLANTS = 45
CONST_ME_CARNIPHILA = 46
CONST_ME_PURPLEENERGY = 47
CONST_ME_YELLOWENERGY = 48
CONST_ME_HOLYAREA = 49
CONST_ME_BIGPLANTS = 50
CONST_ME_CAKE = 51
CONST_ME_GIANTICE = 52
CONST_ME_WATERSPLASH = 53
CONST_ME_PLANTATTACK = 54
CONST_ME_TUTORIALARROW = 55
CONST_ME_TUTORIALSQUARE = 56
CONST_ME_MIRRORHORIZONTAL = 57
CONST_ME_MIRRORVERTICAL = 58
CONST_ME_SKULLHORIZONTAL = 59
CONST_ME_SKULLVERTICAL = 60
CONST_ME_ASSASSIN = 61
CONST_ME_STEPSHORIZONTAL = 62
CONST_ME_BLOODYSTEPS = 63
CONST_ME_STEPSVERTICAL = 64
CONST_ME_YALAHARIGHOST = 65
CONST_ME_BATS = 66
CONST_ME_SMOKE = 67
CONST_ME_INSECTS = 68
CONST_ME_DRAGONHEAD = 69
CONST_ME_ORCSHAMAN = 70
CONST_ME_ORCSHAMAN_FIRE = 71
CONST_ME_THUNDER = 72
CONST_ME_FERUMBRAS = 73
CONST_ME_CONFETTI_HORIZONTAL = 74
CONST_ME_CONFETTI_VERTICAL = 75
CONST_ME_NONE = 255

CONST_ANI_SPEAR = 0
CONST_ANI_BOLT = 1
CONST_ANI_ARROW = 2
CONST_ANI_FIRE = 3
CONST_ANI_ENERGY = 4
CONST_ANI_POISONARROW = 5
CONST_ANI_BURSTARROW = 6
CONST_ANI_THROWINGSTAR = 7
CONST_ANI_THROWINGKNIFE = 8
CONST_ANI_SMALLSTONE = 9
CONST_ANI_DEATH = 10
CONST_ANI_LARGEROCK = 11
CONST_ANI_SNOWBALL = 12
CONST_ANI_POWERBOLT = 13
CONST_ANI_POISON = 14
CONST_ANI_INFERNALBOLT = 15
CONST_ANI_HUNTINGSPEAR = 16
CONST_ANI_ENCHANTEDSPEAR = 17
CONST_ANI_REDSTAR = 18
CONST_ANI_GREENSTAR = 19
CONST_ANI_ROYALSPEAR = 20
CONST_ANI_SNIPERARROW = 21
CONST_ANI_ONYXARROW = 22
CONST_ANI_PIERCINGBOLT = 23
CONST_ANI_WHIRLWINDSWORD = 24
CONST_ANI_WHIRLWINDAXE = 25
CONST_ANI_WHIRLWINDCLUB = 26
CONST_ANI_ETHEREALSPEAR = 27
CONST_ANI_ICE = 28
CONST_ANI_EARTH = 29
CONST_ANI_HOLY = 30
CONST_ANI_SUDDENDEATH = 31
CONST_ANI_FLASHARROW = 32
CONST_ANI_FLAMMINGARROW = 33
CONST_ANI_SHIVERARROW = 34
CONST_ANI_ENERGYBALL = 35
CONST_ANI_SMALLICE = 36
CONST_ANI_SMALLHOLY = 37
CONST_ANI_SMALLEARTH = 38
CONST_ANI_EARTHARROW = 39
CONST_ANI_EXPLOSION = 40
CONST_ANI_CAKE = 41
CONST_ANI_TARSALARROW = 43
CONST_ANI_VORTEXBOLT = 44
CONST_ANI_PRISMATICBOLT = 47
CONST_ANI_CRYSTALLINEARROW = 48
CONST_ANI_DRILLBOLT = 49
CONST_ANI_ENVENOMEDARROW = 50
CONST_ANI_WEAPONTYPE = 254
CONST_ANI_NONE = 255
CONST_ANI_LAST = CONST_ANI_ENVENOMEDARROW

TALKTYPE_SAY = 1
TALKTYPE_WHISPER = 2
TALKTYPE_YELL = 3
TALKTYPE_PRIVATE_FROM = 4
TALKTYPE_PRIVATE_TO = 5
TALKTYPE_CHANNEL_Y = 7
TALKTYPE_CHANNEL_O = 8
TALKTYPE_PRIVATE_NP = 10
TALKTYPE_PRIVATE_PN = 11
TALKTYPE_BROADCAST = 12
TALKTYPE_CHANNEL_R1 = 13
TALKTYPE_PRIVATE_RED_FROM = 14
TALKTYPE_PRIVATE_RED_TO = 15
TALKTYPE_ORANGE_1 = 34
TALKTYPE_ORANGE_2 = 35

TALKTYPE_PRIVATE = TALKTYPE_PRIVATE_TO
TALKTYPE_PRIVATE_RED = TALKTYPE_PRIVATE_RED_TO

-- for internal use
TALKTYPE_RVR_CHANNEL = 256
TALKTYPE_RVR_ANSWER = 257
TALKTYPE_RVR_CONTINUE = 258
TALKTYPE_CHANNEL_R2 = 259
TALKTYPE_CHANNEL_W = 260
--

MESSAGE_STATUS_CONSOLE_BLUE = 4
MESSAGE_STATUS_CONSOLE_RED = 12
MESSAGE_STATUS_DEFAULT = 16
MESSAGE_STATUS_WARNING = 17
MESSAGE_EVENT_ADVANCE = 18
MESSAGE_STATUS_SMALL = 19
MESSAGE_INFO_DESCR = 20
MESSAGE_DAMAGE_DEALT = 21
MESSAGE_DAMAGE_RECEIVED = 22
MESSAGE_HEALED = 23
MESSAGE_EXPERIENCE = 24
MESSAGE_DAMAGE_OTHERS = 25
MESSAGE_HEALED_OTHERS = 26
MESSAGE_EXPERIENCE_OTHERS = 27
MESSAGE_EVENT_DEFAULT = 28
MESSAGE_EVENT_ORANGE = 34
MESSAGE_STATUS_CONSOLE_ORANGE = 35

TEXTCOLOR_BLUE = 5
TEXTCOLOR_LIGHTGREEN = 30
TEXTCOLOR_LIGHTBLUE = 35
TEXTCOLOR_GREEN = 54
TEXTCOLOR_TEAL = 65
TEXTCOLOR_PLATINUMBLUE = 89
TEXTCOLOR_MAYABLUE = 95
TEXTCOLOR_LIGHTGREY = 129
TEXTCOLOR_SKYBLUE = 143
TEXTCOLOR_DARKRED = 144
TEXTCOLOR_PURPLE = 154
TEXTCOLOR_RED = 180
TEXTCOLOR_ORANGE = 198
TEXTCOLOR_YELLOW = 210
TEXTCOLOR_WHITE_EXP = 215
TEXTCOLOR_NONE = 255

MAPMARK_TICK = 0
MAPMARK_QUESTION = 1
MAPMARK_EXCLAMATION = 2
MAPMARK_STAR = 3
MAPMARK_CROSS = 4
MAPMARK_TEMPLE = 5
MAPMARK_KISS = 6
MAPMARK_SHOVEL = 7
MAPMARK_SWORD = 8
MAPMARK_FLAG = 9
MAPMARK_LOCK = 10
MAPMARK_BAG = 11
MAPMARK_SKULL = 12
MAPMARK_DOLLAR = 13
MAPMARK_REDNORTH = 14
MAPMARK_REDSOUTH = 15
MAPMARK_REDEAST = 16
MAPMARK_REDWEST = 17
MAPMARK_GREENNORTH = 18
MAPMARK_GREENSOUTH = 19

ITEM_TYPE_DEPOT = 1
ITEM_TYPE_MAILBOX = 2
ITEM_TYPE_TRASHHOLDER = 3
ITEM_TYPE_CONTAINER = 4
ITEM_TYPE_DOOR = 5
ITEM_TYPE_MAGICFIELD = 6
ITEM_TYPE_TELEPORT = 7
ITEM_TYPE_BED = 8
ITEM_TYPE_KEY = 9
ITEM_TYPE_RUNE = 10

CONST_PROP_BLOCKSOLID = 0
CONST_PROP_HASHEIGHT = 1
CONST_PROP_BLOCKPROJECTILE = 2
CONST_PROP_BLOCKPATHFIND = 3
CONST_PROP_ISVERTICAL = 4
CONST_PROP_ISHORIZONTAL = 5
CONST_PROP_MOVEABLE = 6
CONST_PROP_BLOCKINGANDNOTMOVEABLE = 7
CONST_PROP_SUPPORTHANGABLE = 8

SKILL_FIST = 0
SKILL_CLUB = 1
SKILL_SWORD = 2
SKILL_AXE = 3
SKILL_DISTANCE = 4
SKILL_SHIELD = 5
SKILL_FISHING = 6
SKILL_MAGLEVEL = 7
SKILL_LEVEL = 8

GUILDLEVEL_MEMBER = 1
GUILDLEVEL_VICE = 2
GUILDLEVEL_LEADER = 3

SKULL_NONE = 0
SKULL_YELLOW = 1
SKULL_GREEN = 2
SKULL_WHITE = 3
SKULL_RED = 4
SKULL_BLACK = 5
SKULL_ORANGE = 5

WORLD_TYPE_NO_PVP = 1
WORLD_TYPE_PVP = 2
WORLD_TYPE_PVP_ENFORCED = 3

PLAYERSEX_FEMALE = 0
PLAYERSEX_MALE = 1

STACKPOS_GROUND = 0
STACKPOS_FIRST_ITEM_ABOVE_GROUNDTILE = 1
STACKPOS_SECOND_ITEM_ABOVE_GROUNDTILE = 2
STACKPOS_THIRD_ITEM_ABOVE_GROUNDTILE = 3
STACKPOS_FOURTH_ITEM_ABOVE_GROUNDTILE = 4
STACKPOS_FIFTH_ITEM_ABOVE_GROUNDTILE = 5
STACKPOS_TOP_CREATURE = 253
STACKPOS_TOP_FIELD = 254
STACKPOS_TOP_MOVEABLE_ITEM_OR_CREATURE = 255

RETURNVALUE_NOERROR = 1
RETURNVALUE_NOTPOSSIBLE = 2
RETURNVALUE_NOTENOUGHROOM = 3
RETURNVALUE_PLAYERISPZLOCKED = 4
RETURNVALUE_PLAYERISNOTINVITED = 5
RETURNVALUE_CANNOTTHROW = 6
RETURNVALUE_THEREISNOWAY = 7
RETURNVALUE_DESTINATIONOUTOFREACH = 8
RETURNVALUE_CREATUREBLOCK = 9
RETURNVALUE_NOTMOVEABLE = 10
RETURNVALUE_DROPTWOHANDEDITEM = 11
RETURNVALUE_BOTHHANDSNEEDTOBEFREE = 12
RETURNVALUE_CANONLYUSEONEWEAPON = 13
RETURNVALUE_NEEDEXCHANGE = 14
RETURNVALUE_CANNOTBEDRESSED = 15
RETURNVALUE_PUTTHISOBJECTINYOURHAND = 16
RETURNVALUE_PUTTHISOBJECTINBOTHHANDS = 17
RETURNVALUE_TOOFARAWAY = 18
RETURNVALUE_FIRSTGODOWNSTAIRS = 19
RETURNVALUE_FIRSTGOUPSTAIRS = 20
RETURNVALUE_CONTAINERNOTENOUGHROOM = 21
RETURNVALUE_NOTENOUGHCAPACITY = 22
RETURNVALUE_CANNOTPICKUP = 23
RETURNVALUE_THISISIMPOSSIBLE = 24
RETURNVALUE_DEPOTISFULL = 25
RETURNVALUE_CREATUREDOESNOTEXIST = 26
RETURNVALUE_CANNOTUSETHISOBJECT = 27
RETURNVALUE_PLAYERWITHTHISNAMEISNOTONLINE = 28
RETURNVALUE_NOTREQUIREDLEVELTOUSERUNE = 29
RETURNVALUE_YOUAREALREADYTRADING = 30
RETURNVALUE_THISPLAYERISALREADYTRADING = 31
RETURNVALUE_YOUMAYNOTLOGOUTDURINGAFIGHT = 32
RETURNVALUE_DIRECTPLAYERSHOOT = 33
RETURNVALUE_NOTENOUGHLEVEL = 34
RETURNVALUE_NOTENOUGHMAGICLEVEL = 35
RETURNVALUE_NOTENOUGHMANA = 36
RETURNVALUE_NOTENOUGHSOUL = 37
RETURNVALUE_YOUAREEXHAUSTED = 38
RETURNVALUE_PLAYERISNOTREACHABLE = 39
RETURNVALUE_CANONLYUSETHISRUNEONCREATURES = 40
RETURNVALUE_ACTIONNOTPERMITTEDINPROTECTIONZONE = 41
RETURNVALUE_YOUMAYNOTATTACKTHISPLAYER = 42
RETURNVALUE_YOUMAYNOTATTACKAPERSONINPROTECTIONZONE = 43
RETURNVALUE_YOUMAYNOTATTACKAPERSONWHILEINPROTECTIONZONE = 44
RETURNVALUE_YOUMAYNOTATTACKTHISCREATURE = 45
RETURNVALUE_YOUCANONLYUSEITONCREATURES = 46
RETURNVALUE_CREATUREISNOTREACHABLE = 47
RETURNVALUE_TURNSECUREMODETOATTACKUNMARKEDPLAYERS = 48
RETURNVALUE_YOUNEEDPREMIUMACCOUNT = 49
RETURNVALUE_YOUNEEDTOLEARNTHISSPELL = 50
RETURNVALUE_YOURVOCATIONCANNOTUSETHISSPELL = 51
RETURNVALUE_YOUNEEDAWEAPONTOUSETHISSPELL = 52
RETURNVALUE_PLAYERISPZLOCKEDLEAVEPVPZONE = 53
RETURNVALUE_PLAYERISPZLOCKEDENTERPVPZONE = 54
RETURNVALUE_ACTIONNOTPERMITTEDINANOPVPZONE = 55
RETURNVALUE_YOUCANNOTLOGOUTHERE = 56
RETURNVALUE_YOUNEEDAMAGICITEMTOCASTSPELL = 57
RETURNVALUE_CANNOTCONJUREITEMHERE = 58
RETURNVALUE_YOUNEEDTOSPLITYOURSPEARS = 59
RETURNVALUE_NAMEISTOOAMBIGIOUS = 60
RETURNVALUE_CANUSEONLYONESHIELD = 61
RETURNVALUE_NOPARTYMEMBERSINRANGE = 62
RETURNVALUE_YOUARENOTTHEOWNER = 63

PlayerFlag_CannotUseCombat = 0
PlayerFlag_CannotAttackPlayer = 1
PlayerFlag_CannotAttackMonster = 2
PlayerFlag_CannotBeAttacked = 3
PlayerFlag_CanConvinceAll = 4
PlayerFlag_CanSummonAll = 5
PlayerFlag_CanIllusionAll = 6
PlayerFlag_CanSenseInvisibility = 7
PlayerFlag_IgnoredByMonsters = 8
PlayerFlag_NotGainInFight = 9
PlayerFlag_HasInfiniteMana = 10
PlayerFlag_HasInfiniteSoul = 11
PlayerFlag_HasNoExhaustion = 12
PlayerFlag_CannotUseSpells = 13
PlayerFlag_CannotPickupItem = 14
PlayerFlag_CanAlwaysLogin = 15
PlayerFlag_CanBroadcast = 16
PlayerFlag_CanEditHouses = 17
PlayerFlag_CannotBeBanned = 18
PlayerFlag_CannotBePushed = 19
PlayerFlag_HasInfiniteCapacity = 20
PlayerFlag_CanPushAllCreatures = 21
PlayerFlag_CanTalkRedPrivate = 22
PlayerFlag_CanTalkRedChannel = 23
PlayerFlag_TalkOrangeHelpChannel = 24
PlayerFlag_NotGainExperience = 25
PlayerFlag_NotGainMana = 26
PlayerFlag_NotGainHealth = 27
PlayerFlag_NotGainSkill = 28
PlayerFlag_SetMaxSpeed = 29
PlayerFlag_SpecialVIP = 30
PlayerFlag_NotGenerateLoot = 31
PlayerFlag_CanTalkRedChannelAnonymous = 32
PlayerFlag_IgnoreProtectionZone = 33
PlayerFlag_IgnoreSpellCheck = 34
PlayerFlag_IgnoreWeaponCheck = 35
PlayerFlag_CannotBeMuted = 36
PlayerFlag_IsAlwaysPremium = 37
PlayerFlag_CanAnswerRuleViolations = 38

maleOutfits = {128, 129, 130, 131, 132, 133, 134, 143, 144, 145, 146, 151, 152, 153, 154, 251, 268, 273, 278, 289, 325, 328, 335, 367, 430, 432, 463, 465, 472, 512, 516}
femaleOutfits = {136, 137, 138, 139, 140, 141, 142, 147, 148, 149, 150, 155, 156, 157, 158, 252, 269, 270, 279, 288, 324, 329, 336, 366, 431, 433, 464, 466, 471, 513, 514}

doors = {[1209] = 1211, [1210] = 1211, [1212] = 1214, [1213] = 1214, [1219] = 1220, [1221] = 1222, [1231] = 1233, [1232] = 1233, [1234] = 1236, [1235] = 1236, [1237] = 1238, [1239] = 1240, [1249] = 1251, [1250] = 1251, [1252] = 1254, [1253] = 1254, [1539] = 1540, [1541] = 1542, [3535] = 3537, [3536] = 3537, [3538] = 3539, [3544] = 3546, [3545] = 3546, [3547] = 3548, [4913] = 4915, [4914] = 4915, [4916] = 4918, [4917] = 4918, [5082] = 5083, [5084] = 5085, [5098] = 5100, [5099] = 5100, [5101] = 5102, [5107] = 5109, [5108] = 5109, [5110] = 5111, [5116] = 5118, [5117] = 5118, [5119] = 5120, [5125] = 5127, [5126] = 5127, [5128] = 5129, [5134] = 5136, [5135] = 5136, [5137] = 5139, [5138] = 5139, [5140] = 5142, [5141] = 5142, [5143] = 5145, [5144] = 5145, [5278] = 5280, [5279] = 5280, [5281] = 5283, [5282] = 5283, [5284] = 5285, [5286] = 5287, [5515] = 5516, [5517] = 5518, [5732] = 5734, [5733] = 5734, [5735] = 5737, [5736] = 5737, [6192] = 6194, [6193] = 6194, [6195] = 6197, [6196] = 6197, [6198] = 6199, [6200] = 6201, [6249] = 6251, [6250] = 6251, [6252] = 6254, [6253] = 6254, [6255] = 6256, [6257] = 6258, [6795] = 6796, [6797] = 6798, [6799] = 6800, [6801] = 6802, [6891] = 6893, [6892] = 6893, [6894] = 6895, [6900] = 6902, [6901] = 6902, [6903] = 6904, [7033] = 7035, [7034] = 7035, [7036] = 7037, [7042] = 7044, [7043] = 7044, [7045] = 7046, [7054] = 7055, [7056] = 7057, [8541] = 8543, [8542] = 8543, [8544] = 8546, [8545] = 8546, [8547] = 8548, [8549] = 8550, [9165] = 9167, [9166] = 9167, [9168] = 9170, [9169] = 9170, [9171] = 9172, [9173] = 9174, [9267] = 9269, [9268] = 9269, [9270] = 9272, [9271] = 9272, [9273] = 9274, [9275] = 9276, [10276] = 10277, [10274] = 10275, [10268] = 10270, [10269] = 10270, [10271] = 10273, [10272] = 10273, [10471] = 10472, [10480] = 10481, [10477] = 10479, [10478] = 10479, [10468] = 10470, [10469] = 10470}
verticalOpenDoors = {1211, 1220, 1224, 1228, 1233, 1238, 1242, 1246, 1251, 1256, 1260, 1540, 3546, 3548, 3550, 3552, 4915, 5083, 5109, 5111, 5113, 5115, 5127, 5129, 5131, 5133, 5142, 5145, 5283, 5285, 5289, 5293, 5516, 5737, 5749, 6194, 6199, 6203, 6207, 6251, 6256, 6260, 6264, 6798, 6802, 6902, 6904, 6906, 6908, 7044, 7046, 7048, 7050, 7055, 8543, 8548, 8552, 8556, 9167, 9172, 9269, 9274, 9274, 9269, 9278, 9282, 10270, 10275, 10279, 10283, 10479, 10481, 10485, 10483}
horizontalOpenDoors = {1214, 1222, 1226, 1230, 1236, 1240, 1244, 1248, 1254, 1258, 1262, 1542, 3537, 3539, 3541, 3543, 4918, 5085, 5100, 5102, 5104, 5106, 5118, 5120, 5122, 5124, 5136, 5139, 5280, 5287, 5291, 5295, 5518, 5734, 5746, 6197, 6201, 6205, 6209, 6254, 6258, 6262, 6266, 6796, 6800, 6893, 6895, 6897, 6899, 7035, 7037, 7039, 7041, 7057, 8546, 8550, 8554, 8558, 9170, 9174, 9272, 9276, 9280, 9284, 10273, 10277, 10281, 10285, 10470, 10472, 10476, 10474}
openSpecialDoors = {1224, 1226, 1228, 1230, 1242, 1244, 1246, 1248, 1256, 1258, 1260, 1262, 3541, 3543, 3550, 3552, 5104, 5106, 5113, 5115, 5122, 5124, 5131, 5133, 5289, 5291, 5293, 5295, 6203, 6205, 6207, 6209, 6260, 6262, 6264, 6266, 6897, 6899, 6906, 6908, 7039, 7041, 7048, 7050, 8552, 8554, 8556, 8558, 9176, 9178, 9180, 9182, 9278, 9280, 9282, 9284, 10279, 10281, 10283, 10285, 10474, 10476, 10483, 10485}
questDoors = {1223, 1225, 1241, 1243, 1255, 1257, 3542, 3551, 5105, 5114, 5123, 5132, 5288, 5290, 5745, 5748, 6202, 6204, 6259, 6261, 6898, 6907, 7040, 7049, 8551, 8553, 9175, 9177, 9277, 9279, 10278, 10280, 10475, 10484}
levelDoors = {1227, 1229, 1245, 1247, 1259, 1261, 3540, 3549, 5103, 5112, 5121, 5130, 5292, 5294, 6206, 6208, 6263, 6265, 6896, 6905, 7038, 7047, 8555, 8557, 9179, 9181, 9281, 9283, 10282, 10284, 10473, 10482, 10780, 10789}
keys = {2086, 2087, 2088, 2089, 2090, 2091, 2092, 10032}

BLUEBERRYBUSH_DECAY_INTERVAL = 300000
CONTAINER_POSITION = 0xFFFF
ITEMCOUNT_MAX = 100

ITEM_GOLD_COIN = 2148
ITEM_PLATINUM_COIN = 2152
ITEM_CRYSTAL_COIN = 2160
ITEM_FISH = 2667
ITEM_WORM = 3976
ITEM_BLUEBERRY = 2677
ITEM_BLUEBERRYBUSH = 2785
ITEM_BUSH = 2786
ITEM_PARCEL = 2595
ITEM_LABEL = 2599

ITEM_FIREFIELD_PVP_FULL = 1487
ITEM_FIREFIELD_PVP_MEDIUM = 1488
ITEM_FIREFIELD_PVP_SMALL = 1489
ITEM_FIREFIELD_PERSISTENT_FULL = 1492
ITEM_FIREFIELD_PERSISTENT_MEDIUM = 1493
ITEM_FIREFIELD_PERSISTENT_SMALL = 1494
ITEM_FIREFIELD_NOPVP = 1500

ITEM_POISONFIELD_PVP = 1490
ITEM_POISONFIELD_PERSISTENT = 1496
ITEM_POISONFIELD_NOPVP = 1503

ITEM_ENERGYFIELD_PVP = 1491
ITEM_ENERGYFIELD_PERSISTENT = 1495
ITEM_ENERGYFIELD_NOPVP = 1504

ITEM_MAGICWALL = 1497
ITEM_MAGICWALL_PERSISTENT = 1498
ITEM_MAGICWALL_SAFE = 11098

ITEM_WILDGROWTH = 1499
ITEM_WILDGROWTH_PERSISTENT = 2721
ITEM_WILDGROWTH_SAFE = 11099

function doPlayerGiveItem(cid, itemid, count, charges)
	local isFluidContainer = isItemFluidContainer(itemid) == TRUE
	if isFluidContainer and charges == nil then
		charges = 1
	end
	while count > 0 do
		local tempcount = 1
		if(isItemStackable(itemid) == TRUE) then
			tempcount = math.min(100, count)
		end
		local ret = doPlayerAddItem(cid, itemid, tempcount, TRUE, charges)
		if ret == false then
			ret = doCreateItem(itemid, tempcount, getPlayerPosition(cid))
		end

		if(ret) then
			if(isFluidContainer) then
				count = count - 1
			elseif isItemRune(itemid) then
				return LUA_NO_ERROR
			else
				count = count - tempcount
			end
		else
			return LUA_ERROR
		end
	end
	return LUA_NO_ERROR
end

function doCreatureSayWithRadius(cid, text, type, radiusx, radiusy, position)
	for i, tid in ipairs(getSpectators(position or getCreaturePosition(cid), radiusx, radiusy, false)) do
		if(isPlayer(tid) == TRUE) then
			doCreatureSay(cid, text, type, false, tid, position or getCreaturePosition(cid))
		end
	end
	return TRUE
end

function doSummonCreatures(monsters, positions)
	for _, positions in pairs(positions) do
		doSummonCreature(monsters, positions)
	end
end

function setPlayerMultipleStorageValues(cid, storage, value)
	for _, storage in pairs(storage) do
		setPlayerStorageValue(cid, storage, value)
	end
end

function doPlayerTakeItem(cid, itemid, count)
	if(getPlayerItemCount(cid,itemid) >= count) then
		while count > 0 do
			local tempcount = 0
			if(isItemStackable(itemid) == TRUE) then
				tempcount = math.min (100, count)
			else
				tempcount = 1
			end
			local ret = doPlayerRemoveItem(cid, itemid, tempcount)
			if(ret ~= LUA_ERROR) then
				count = count-tempcount
			else
				return LUA_ERROR
			end
		end
		if(count == 0) then
			return LUA_NO_ERROR
		end
	else
		return LUA_ERROR
	end
end

function doPlayerBuyItem(cid, itemid, count, cost, charges)
	if(doPlayerRemoveMoney(cid, cost) == TRUE) then
		return doPlayerGiveItem(cid, itemid, count, charges)
	end
	return LUA_ERROR
end

function doPlayerSellItem(cid, itemid, count, cost)
	if(doPlayerTakeItem(cid, itemid, count) == LUA_NO_ERROR) then
		if doPlayerAddMoney(cid, cost) ~= TRUE then
			error('Could not add money to ' .. getPlayerName(cid) .. '(' .. cost .. 'gp)')
		end
		return LUA_NO_ERROR
	end
	return LUA_ERROR
end

function isInRange(pos, fromPos, toPos)
	if pos.x >= fromPos.x and pos.y >= fromPos.y and pos.z >= fromPos.z and pos.x <= toPos.x and pos.y <= toPos.y and pos.z <= toPos.z then
		return TRUE
	end
	return FALSE
end

function isPremium(cid)
	return (isPlayer(cid) == TRUE and (getPlayerPremiumDays(cid) > 0 or getConfigInfo('freePremium') == "yes")) and true or false
end

function rows(result)
	return function()
		print("Deprecated function")
		return nil
	end
end

function getMonthDayEnding(day)
	if day == "01" or day == "21" or day == "31" then
		return "st"
	elseif day == "02" or day == "22" then
		return "nd"
	elseif day == "03" or day == "23" then
		return "rd"
	else
		return "th"
	end
end

function getMonthString(m)
	return os.date("%B", os.time{year = 1970, month = m, day = 1})
end

function getArticle(str)
	return str:find("[AaEeIiOoUuYy]") == 1 and "an" or "a"
end

function isNumber(str)
	return tonumber(str) ~= nil and TRUE or FALSE
end

function getDistanceBetween(firstPosition, secondPosition)
	local xDif = math.abs(firstPosition.x - secondPosition.x)
	local yDif = math.abs(firstPosition.y - secondPosition.y)

	local posDif = math.max(xDif, yDif)
	if(firstPosition.z ~= secondPosition.z) then
		posDif = posDif + 9 + 6
	end
	return posDif
end

function doPlayerAddAddons(cid, addon)
	for i = 0, #maleOutfits do
		doPlayerAddOutfit(cid, maleOutfits[i], addon)
	end

	for i = 0, #femaleOutfits do
		doPlayerAddOutfit(cid, femaleOutfits[i], addon)
	end
end

function numRows(result)
	local rows = 0
	while result:fetch() do
		rows = rows + 1
	end
	result:close()
	return rows
end

function isSorcerer(cid)
	if(isPlayer(cid) == FALSE) then
		debugPrint("isSorcerer: Player not found.")
		return false
	end

	return (isInArray({1,5}, getPlayerVocation(cid)) == TRUE)
end

function isDruid(cid)
	if(isPlayer(cid) == FALSE) then
		debugPrint("isDruid: Player not found.")
		return false
	end

	return (isInArray({2,6}, getPlayerVocation(cid)) == TRUE)
end

function isPaladin(cid)
	if(isPlayer(cid) == FALSE) then
		debugPrint("isPaladin: Player not found.")
		return false
	end

	return (isInArray({3,7}, getPlayerVocation(cid)) == TRUE)
end

function isKnight(cid)
	if(isPlayer(cid) == FALSE) then
		debugPrint("isKnight: Player not found.")
		return false
	end

	return (isInArray({4,8}, getPlayerVocation(cid)) == TRUE)
end

function getConfigInfo(info)
	if (type(info) ~= 'string') then return nil end

	dofile('config.lua')
	return _G[info]
end

function doPlayerBuyItemContainer(cid, containerid, itemid, count, cost, charges)
	if(doPlayerRemoveMoney(cid, cost) ~= TRUE) then
		return LUA_ERROR
	end

	for i = 1, count do
		local container = doCreateItemEx(containerid, 1)
		for x = 1, getContainerCapById(containerid) do
			doAddContainerItem(container, itemid, charges)
		end

		if(doPlayerAddItemEx(cid, container, TRUE) ~= RETURNVALUE_NOERROR) then
			return LUA_ERROR
		end
	end

	return LUA_NO_ERROR
end

function getDirectionTo(pos1, pos2)
	local dir = NORTH
	if(pos1.x > pos2.x) then
		dir = WEST
		if(pos1.y > pos2.y) then
			dir = NORTHWEST
		elseif(pos1.y < pos2.y) then
			dir = SOUTHWEST
		end
	elseif(pos1.x < pos2.x) then
		dir = EAST
		if(pos1.y > pos2.y) then
			dir = NORTHEAST
		elseif(pos1.y < pos2.y) then
			dir = SOUTHEAST
		end
	else
		if(pos1.y > pos2.y) then
			dir = NORTH
		elseif(pos1.y < pos2.y) then
			dir = SOUTH
		end
	end
	return dir
end

function getPlayerLookPos(cid)
	return getPosByDir(getThingPos(cid), getPlayerLookDir(cid))
end

function getPosByDir(fromPosition, direction, size)
	local n = size or 1

	local pos = fromPosition
	if(direction == NORTH) then
		pos.y = pos.y - n
	elseif(direction == SOUTH) then
		pos.y = pos.y + n
	elseif(direction == WEST) then
		pos.x = pos.x - n
	elseif(direction == EAST) then
		pos.x = pos.x + n
	elseif(direction == NORTHWEST) then
		pos.y = pos.y - n
		pos.x = pos.x - n
	elseif(direction == NORTHEAST) then
		pos.y = pos.y - n
		pos.x = pos.x + n
	elseif(direction == SOUTHWEST) then
		pos.y = pos.y + n
		pos.x = pos.x - n
	elseif(direction == SOUTHEAST) then
		pos.y = pos.y + n
		pos.x = pos.x + n
	end

	return pos
end

function getCreaturesInRange(position, radiusx, radiusy, showMonsters, showPlayers, showSummons)
	local creaturesList = {}
	for x = -radiusx, radiusx do
		for y = -radiusy, radiusy do
			if not (x == 0 and y == 0) then
				creature = getTopCreature({x = position.x+x, y = position.y+y, z = position.z})
				if (creature.type == 1 and showPlayers == 1) or (creature.type == 2 and showMonsters == 1 and (showSummons ~= 1 or (showSummons == 1 and getCreatureMaster(creature.uid) == (creature.uid)))) then
					table.insert(creaturesList, creature.uid)
				end
			end
		end
	end

	local creature = getTopCreature(position)
	if (creature.type == 1 and showPlayers == 1) or (creature.type == 2 and showMonsters == 1 and (showSummons ~= 1 or (showSummons == 1 and getCreatureMaster(creature.uid) == (creature.uid)))) then
		if not(table.find(creaturesList, creature.uid)) then
			table.insert(creaturesList, creature.uid)
		end
	end
	return creaturesList
end

function addContainerWithItems(cid, container, item, item_count, count)
	local Container = doPlayerAddItem(cid, container, 1)
	for i = 1, count do
		doAddContainerItem(Container, item, item_count)
	end
end

function tableToPos(t)
	if type(t) == "table" and #t == 3 and tonumber(table.concat(t)) then
		return {x = tonumber(t[1]), y = tonumber(t[2]), z = tonumber(t[3])}
	end
	return FALSE
end

function positionExists(pos)
 	pos.stackpos = -1
	return type(getTileThingByPos(pos)) == "table"
end

function warnPlayer(cid, msg)
	doSendMagicEffect(getPlayerPosition(cid), CONST_ME_POFF)
	return doPlayerSendCancel(cid, msg)
end

string.split = function (str)
	local t = {}
	local function helper(word)
		table.insert(t, word)
		return ""
	end

	if(not str:gsub("%w+", helper):find("%S")) then
		return t
	end
end

string.trim = function(str)
	return (string.gsub(str, "^%s*(.-)%s*$", "%1"))
end

string.explode = function(str, sep)
	local pos, t = 1, {}
	if #sep == 0 or #str == 0 then
		return
	end

	for s, e in function() return str:find(sep, pos) end do
		table.insert(t, str:sub(pos, s - 1):trim())
		pos = e + 1
	end

	table.insert(t, str:sub(pos):trim())
	return t
end

function getCount(string)
	local b, e = string:find("%d+")
	return b and e and tonumber(string:sub(b, e)) or -1
end

 -- Returns player name if player exists in database or 0
function playerExists(name)
	dofile("./config.lua")
	if sqlType == "mysql" then
		env = assert(luasql.mysql())
		con = assert(env:connect(mysqlDatabase, mysqlUser, mysqlPass, mysqlHost, mysqlPort))
	else
		env = assert(luasql.sqlite3())
		con = assert(env:connect(sqliteDatabase))
	end

	local cur = assert(con:execute("SELECT `name` FROM `players` WHERE `name` = '" .. escapeString(name) .. "';"))
	local row = cur:fetch({}, "a")

	local name_ = ""
	if row ~= nil then
		name_ = row.name
	end

	cur:close()
	con:close()
	env:close()
	return name_
end

function isSummon(cid)
	return (isCreature(cid) == TRUE and (getCreatureMaster(cid) ~= cid)) and TRUE or FALSE
end

function isPlayerSummon(cid)
	return (isSummon(cid) == TRUE and isPlayer(getCreatureMaster(cid)) == TRUE) and TRUE or FALSE
end

function doCopyItem(item, attributes)
	local attributes = attributes or false

	local ret = doCreateItemEx(item.itemid, item.type)
	if(attributes) then
		if(item.actionid > 0) then
			doSetItemActionId(ret, item.actionid)
		end
	end

	if(isContainer(item.uid) == TRUE) then
		for i = (getContainerSize(item.uid) - 1), 0, -1 do
			local tmp = getContainerItem(item.uid, i)
			if(tmp.itemid > 0) then
				doAddContainerItemEx(ret, doCopyItem(tmp, true).uid)
			end
		end
	end

	return getThing(ret)
end

function getTibianTime()
	local worldTime = getWorldTime()
	local minutes = worldTime % 60

	local suffix = 'pm'
	if worldTime >= 720 then
		suffix = 'am'
		if worldTime >= 780 then
			worldTime = worldTime - 720
		end
	end

	local hours = math.floor(worldTime / 60)
	if minutes < 10 then
		minutes = '0' .. minutes
	end
	return hours .. ':' .. minutes .. ' ' .. suffix
end

looktypes = {
	["citizen"] = {
		looktypeMale = 128,
		looktypeFemale = 136
	},
	["hunter"] = {
		looktypeMale = 129,
		looktypeFemale = 137
	},
	["mage"] = {
		looktypeMale = 130,
		looktypeFemale = 138
	},
	["knight"] = {
		looktypeMale = 131,
		looktypeFemale = 139
	},
	["nobleman"] = {
		looktypeMale = 132,
		looktypeFemale = 140
	},
	["summoner"] = {
		looktypeMale = 133,
		looktypeFemale = 141
	},
	["warrior"] = {
		looktypeMale = 134,
		looktypeFemale = 142
	},
	["barbarian"] = {
		looktypeMale = 143,
		looktypeFemale = 147
	},
	["druid"] = {
		looktypeMale = 144,
		looktypeFemale = 148
	},
	["wizard"] = {
		looktypeMale = 145,
		looktypeFemale = 149
	},
	["oriental"] = {
		looktypeMale = 146,
		looktypeFemale = 150
	},
	["pirate"] = {
		looktypeMale = 151,
		looktypeFemale = 155
	},
	["assassin"] = {
		looktypeMale = 152,
		looktypeFemale = 156
	},
	["beggar"] = {
		looktypeMale = 153,
		looktypeFemale = 157
	},
	["shaman"] = {
		looktypeMale = 154,
		looktypeFemale = 158
	},
	["norseman"] = {
		looktypeMale = 251,
		looktypeFemale = 252
	},
	["nightmare"] = {
		looktypeMale = 268,
		looktypeFemale = 269
	},
	["jester"] = {
		looktypeMale = 273,
		looktypeFemale = 270
	},
	["brotherhood"] = {
		looktypeMale = 278,
		looktypeFemale = 279
	},
	["demonhunter"] = {
		looktypeMale = 289,
		looktypeFemale = 288
	},
	["yalaharian"] = {
		looktypeMale = 325,
		looktypeFemale = 324
	},
	["warmaster"] = {
		looktypeMale = 335,
		looktypeFemale = 336
	},

	["potionbelt"] = {
		looktypeMale = 133,
		looktypeFemale = 138
	},
	["tiara"] = {
		looktypeMale = 133,
		looktypeFemale = 138
	},
	["ferumbrashat"] = {
		looktypeMale = 130,
		looktypeFemale = 141
	},
	["wand"] = {
		looktypeMale = 130,
		looktypeFemale = 141
	}
}

function hasAddon(cid, looktype, addon)
	if looktypes[looktype] ~= nil then
		if looktype == "beggar" or looktype == "hunter" then
			if addon == 1 then
				return canPlayerWearOutfit(cid, looktypes[looktype].looktypeMale, 1) == TRUE or canPlayerWearOutfit(cid, looktypes[looktype].looktypeFemale, 2) == TRUE
			else
				return canPlayerWearOutfit(cid, looktypes[looktype].looktypeMale, 2) == TRUE or canPlayerWearOutfit(cid, looktypes[looktype].looktypeFemale, 1) == TRUE
			end
		end
		return canPlayerWearOutfit(cid, looktypes[looktype].looktypeMale, addon) == TRUE or canPlayerWearOutfit(cid, looktypes[looktype].looktypeFemale, addon) == TRUE
	end
	return false
end

function addAddon(cid, looktype, addon)
	if looktypes[looktype] ~= nil then
		if looktype == "beggar" or looktype == "hunter" then
			if addon == 1 then
				doPlayerAddOutfit(cid, looktypes[looktype].looktypeMale, 1)
				doPlayerAddOutfit(cid, looktypes[looktype].looktypeFemale, 2)
			else
				doPlayerAddOutfit(cid, looktypes[looktype].looktypeMale, 2)
				doPlayerAddOutfit(cid, looktypes[looktype].looktypeFemale, 1)
			end
		else
			doPlayerAddOutfit(cid, looktypes[looktype].looktypeMale, addon)
			doPlayerAddOutfit(cid, looktypes[looktype].looktypeFemale, addon)
		end
		doSendMagicEffect(getCreaturePosition(cid), CONST_ME_MAGIC_RED)
	end
end

table.find = function (table, value)
	for i, v in pairs(table) do
		if(v == value) then
			return i
		end
	end
	return nil
end

table.isStrIn = function (txt, str)
	for i, v in pairs(str) do
		if(txt:find(v) and not txt:find('(%w+)' .. v) and not txt:find(v .. '(%w+)')) then
			return true
		end
	end
	return false
end

function isMonsterInRange(monsterName, fromPos, toPos)
	for _x = fromPos.x, toPos.x do
		for _y = fromPos.y, toPos.y do
			for _z = fromPos.z, toPos.z do
				creature = getTopCreature({x = _x, y = _y, z = _z})
				if creature.type == 2 and getCreatureName(creature.uid):lower() == monsterName:lower() then
					return true
				end
			end
		end
	end
	return false
end

-- LuaSQL wrapper
luasql_environment = {
	connections = {}
}
function luasql_environment:new() return self end
function luasql_environment:connect()
	local connection = luasql_connection:new()
	table.insert(self.connections, connection)
	return connection
end
function luasql_environment:close()
	for _, v in pairs(self.connections) do
		v:close()
	end
	self.connections = {}
	return true
end

luasql_connection = {
	resultIds = {}
}
function luasql_connection:new() return self end
function luasql_connection:close()
	for _, v in ipairs(self.resultIds) do
		result.free(v)
	end
	self.resultIds = {}
	return true
end
function luasql_connection:execute(statement)
	if statement:sub(1, 6):upper() == "SELECT" then
		local cursor = luasql_cursor:new(self, statement)
		if cursor.resultId ~= false then
			table.insert(self.resultIds, cursor.resultId)
		end
		return cursor
	end
	return db.query(statement)
end
function luasql_connection:closedCursor(resultId)
	for k, v in ipairs(self.resultIds) do
		if v == resultId then
			table.remove(self.resultIds, k)
			break
		end
	end
end

luasql_cursor = {
	connection,
	resultId
}
function luasql_cursor:new(connection, statement)
	self.connection = connection
	self.resultId = db.storeQuery(statement)
	return self
end
function luasql_cursor:close()
	if self.resultId == false then
		return true
	end

	self.connection:closedCursor(self.resultId)
	return result.free(self.resultId)
end
function luasql_cursor:fetch()
	if self.resultId == false then
		return nil
	end

	local ret = result.getAllData(self.resultId)
	if ret == false then
		self:close()
		self.resultId = false
		return nil
	end

	if result.next(self.resultId) == false then
		self:close()
		self.resultId = false
	end
	return ret
end

luasql = {
	mysql = function() return luasql_environment:new() end,
	sqlite3 = function() return luasql_environment:new() end,
	odbc = function() return luasql_environment:new() end,
	postgres = function() return luasql_environment:new() end
}
--

function escapeString(str)
	str = db.escapeString(str)
	if str:len() <= 2 then
		return ""
	end
	return str:sub(2, str:len() - 1)
end

function createClass(parent)
	local newClass = {}
	function newClass:new(instance)
		local instance = instance or {}
		setmetatable(instance, {__index = newClass})
		return instance
	end

	if(parent ~= nil) then
		setmetatable(newClass, {__index = parent})
	end

	function newClass:getSelf()
		return newClass
	end

	function newClass:getParent()
		return baseClass
	end

	function newClass:isa(class)
		local tmp = newClass
		while(tmp ~= nil) do
			if(tmp == class) then
				return true
			end

			tmp = tmp:getParent()
		end

		return false
	end

	function newClass:setAttributes(attributes)
		for k, v in pairs(attributes) do
			newClass[k] = v
		end
	end

	return newClass
end

Result = createClass(nil)
Result:setAttributes({
	id = -1,
	query = ""
})

function Result:getID()
	return self.id
end

function Result:setID(_id)
	self.id = _id
end

function Result:getQuery()
	return self.query
end

function Result:setQuery(_query)
	self.query = _query
end

function Result:create(_query)
	self:setQuery(_query)
	local _id = db.storeQuery(self:getQuery())
	if(_id) then
		self:setID(_id)
	end

	return self:getID()
end

function Result:getRows(free)
	local free = free or false
	if(self:getID() == -1) then
		error("[Result:getRows] Result not set!")
	end

	local c = 0
	repeat
		c = c + 1
	until not self:next()

	local _query = self:getQuery()
	self:free()
	if(not free) then
		self:create(_query)
	end

	return c
end

function Result:getDataInt(s)
	if(self:getID() == -1) then
		error("[Result:getDataInt] Result not set!")
	end

	return result.getDataInt(self:getID(), s)
end

function Result:getDataLong(s)
	if(self:getID() == -1) then
		error("[Result:getDataLong] Result not set!")
	end

	return result.getDataLong(self:getID(), s)
end

function Result:getDataString(s)
	if(self:getID() == -1) then
		error("[Result:getDataString] Result not set!")
	end

	return result.getDataString(self:getID(), s)
end

function Result:getDataStream(s)
	if(self:getID() == -1) then
		error("[Result:getDataStream] Result not set!")
	end

	return result.getDataStream(self:getID(), s)
end

function Result:next()
	if(self:getID() == -1) then
		error("[Result:next] Result not set!")
	end

	return result.next(self:getID())
end

function Result:free()
	if(self:getID() == -1) then
		error("[Result:free] Result not set!")
	end

	self:setQuery("")
	local ret = result.free(self:getID())
	self:setID(-1)
	return ret
end

Result.numRows = Result.getRows
function db.getResult(query)
	if(type(query) ~= 'string') then
		return nil
	end

	local ret = Result:new()
	ret:create(query)
	return ret
end
