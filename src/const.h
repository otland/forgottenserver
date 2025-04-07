// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_CONST_H
#define FS_CONST_H

static constexpr int32_t NETWORKMESSAGE_MAXSIZE = 24590;
static constexpr int32_t MIN_MARKET_FEE = 20;
static constexpr int32_t MAX_MARKET_FEE = 100000;

enum MagicEffectsType_t : uint8_t
{
	MAGIC_EFFECTS_END_LOOP = 0,              // ends the magic effect loop
	MAGIC_EFFECTS_DELTA = 1,                 // needs uint8_t delta after type to adjust position
	MAGIC_EFFECTS_DELAY = 2,                 // needs uint16_t delay after type to delay in miliseconds effect display
	MAGIC_EFFECTS_CREATE_EFFECT = 3,         // needs uint8_t effectid after type
	MAGIC_EFFECTS_CREATE_DISTANCEEFFECT = 4, // needs uint8_t and deltaX(int8_t), deltaY(int8_t) after type
	MAGIC_EFFECTS_CREATE_DISTANCEEFFECT_REVERSED = 5, // needs uint8_t and deltaX(int8_t), deltaY(int8_t) after type
};

enum MagicEffectClasses : uint8_t
{
	CONST_ME_NONE,

	CONST_ME_DRAWBLOOD = 1,
	CONST_ME_LOSEENERGY = 2,
	CONST_ME_POFF = 3,
	CONST_ME_BLOCKHIT = 4,
	CONST_ME_EXPLOSIONAREA = 5,
	CONST_ME_EXPLOSIONHIT = 6,
	CONST_ME_FIREAREA = 7,
	CONST_ME_YELLOW_RINGS = 8,
	CONST_ME_GREEN_RINGS = 9,
	CONST_ME_HITAREA = 10,
	CONST_ME_TELEPORT = 11,
	CONST_ME_ENERGYHIT = 12,
	CONST_ME_MAGIC_BLUE = 13,
	CONST_ME_MAGIC_RED = 14,
	CONST_ME_MAGIC_GREEN = 15,
	CONST_ME_HITBYFIRE = 16,
	CONST_ME_HITBYPOISON = 17,
	CONST_ME_MORTAREA = 18,
	CONST_ME_SOUND_GREEN = 19,
	CONST_ME_SOUND_RED = 20,
	CONST_ME_POISONAREA = 21,
	CONST_ME_SOUND_YELLOW = 22,
	CONST_ME_SOUND_PURPLE = 23,
	CONST_ME_SOUND_BLUE = 24,
	CONST_ME_SOUND_WHITE = 25,
	CONST_ME_BUBBLES = 26,
	CONST_ME_CRAPS = 27,
	CONST_ME_GIFT_WRAPS = 28,
	CONST_ME_FIREWORK_YELLOW = 29,
	CONST_ME_FIREWORK_RED = 30,
	CONST_ME_FIREWORK_BLUE = 31,
	CONST_ME_STUN = 32,
	CONST_ME_SLEEP = 33,
	CONST_ME_WATERCREATURE = 34,
	CONST_ME_GROUNDSHAKER = 35,
	CONST_ME_HEARTS = 36,
	CONST_ME_FIREATTACK = 37,
	CONST_ME_ENERGYAREA = 38,
	CONST_ME_SMALLCLOUDS = 39,
	CONST_ME_HOLYDAMAGE = 40,
	CONST_ME_BIGCLOUDS = 41,
	CONST_ME_ICEAREA = 42,
	CONST_ME_ICETORNADO = 43,
	CONST_ME_ICEATTACK = 44,
	CONST_ME_STONES = 45,
	CONST_ME_SMALLPLANTS = 46,
	CONST_ME_CARNIPHILA = 47,
	CONST_ME_PURPLEENERGY = 48,
	CONST_ME_YELLOWENERGY = 49,
	CONST_ME_HOLYAREA = 50,
	CONST_ME_BIGPLANTS = 51,
	CONST_ME_CAKE = 52,
	CONST_ME_GIANTICE = 53,
	CONST_ME_WATERSPLASH = 54,
	CONST_ME_PLANTATTACK = 55,
	CONST_ME_TUTORIALARROW = 56,
	CONST_ME_TUTORIALSQUARE = 57,
	CONST_ME_MIRRORHORIZONTAL = 58,
	CONST_ME_MIRRORVERTICAL = 59,
	CONST_ME_SKULLHORIZONTAL = 60,
	CONST_ME_SKULLVERTICAL = 61,
	CONST_ME_ASSASSIN = 62,
	CONST_ME_STEPSHORIZONTAL = 63,
	CONST_ME_BLOODYSTEPS = 64,
	CONST_ME_STEPSVERTICAL = 65,
	CONST_ME_YALAHARIGHOST = 66,
	CONST_ME_BATS = 67,
	CONST_ME_SMOKE = 68,
	CONST_ME_INSECTS = 69,
	CONST_ME_DRAGONHEAD = 70,
	CONST_ME_ORCSHAMAN = 71,
	CONST_ME_ORCSHAMAN_FIRE = 72,
	CONST_ME_THUNDER = 73,
	CONST_ME_FERUMBRAS = 74,
	CONST_ME_CONFETTI_HORIZONTAL = 75,
	CONST_ME_CONFETTI_VERTICAL = 76,
	// 77-157 are empty
	CONST_ME_BLACKSMOKE = 158,
	// 159-166 are empty
	CONST_ME_REDSMOKE = 167,
	CONST_ME_YELLOWSMOKE = 168,
	CONST_ME_GREENSMOKE = 169,
	CONST_ME_PURPLESMOKE = 170,
	CONST_ME_EARLY_THUNDER = 171,
	CONST_ME_RAGIAZ_BONECAPSULE = 172,
	CONST_ME_CRITICAL_DAMAGE = 173,
	// 174 is empty
	CONST_ME_PLUNGING_FISH = 175,
	CONST_ME_BLUECHAIN = 176,
	CONST_ME_ORANGECHAIN = 177,
	CONST_ME_GREENCHAIN = 178,
	CONST_ME_PURPLECHAIN = 179,
	CONST_ME_GREYCHAIN = 180,
	CONST_ME_YELLOWCHAIN = 181,
	CONST_ME_YELLOWSPARKLES = 182,
	// 183 is empty
	CONST_ME_FAEEXPLOSION = 184,
	CONST_ME_FAECOMING = 185,
	CONST_ME_FAEGOING = 186,
	// 187 is empty
	CONST_ME_BIGCLOUDSSINGLESPACE = 188,
	CONST_ME_STONESSINGLESPACE = 189,
	// 190 is empty
	CONST_ME_BLUEGHOST = 191,
	// 192 is empty
	CONST_ME_POINTOFINTEREST = 193,
	CONST_ME_MAPEFFECT = 194,
	CONST_ME_PINKSPARK = 195,
	CONST_ME_FIREWORK_GREEN = 196,
	CONST_ME_FIREWORK_ORANGE = 197,
	CONST_ME_FIREWORK_PURPLE = 198,
	CONST_ME_FIREWORK_TURQUOISE = 199,
	// 200 is empty
	CONST_ME_THECUBE = 201,
	CONST_ME_DRAWINK = 202,
	CONST_ME_PRISMATICSPARKLES = 203,
	CONST_ME_THAIAN = 204,
	CONST_ME_THAIANGHOST = 205,
	CONST_ME_GHOSTSMOKE = 206,
	// 207 is empty
	CONST_ME_FLOATINGBLOCK = 208,
	CONST_ME_BLOCK = 209,
	CONST_ME_ROOTING = 210,
	// 211-212 were removed from the client
	CONST_ME_GHOSTLYSCRATCH = 213,
	CONST_ME_GHOSTLYBITE = 214,
	CONST_ME_BIGSCRATCHING = 215,
	CONST_ME_SLASH = 216,
	CONST_ME_BITE = 217,
	// 218 is empty
	CONST_ME_CHIVALRIOUSCHALLENGE = 219,
	CONST_ME_DIVINEDAZZLE = 220,
	CONST_ME_ELECTRICALSPARK = 221,
	CONST_ME_PURPLETELEPORT = 222,
	CONST_ME_REDTELEPORT = 223,
	CONST_ME_ORANGETELEPORT = 224,
	CONST_ME_GREYTELEPORT = 225,
	CONST_ME_LIGHTBLUETELEPORT = 226,
	// 227-229 are empty
	CONST_ME_FATAL = 230,
	CONST_ME_DODGE = 231,
	CONST_ME_HOURGLASS = 232,
	CONST_ME_FIREWORKSSTAR = 233,
	CONST_ME_FIREWORKSCIRCLE = 234,
	CONST_ME_FERUMBRAS_1 = 235,
	CONST_ME_GAZHARAGOTH = 236,
	CONST_ME_MAD_MAGE = 237,
	CONST_ME_HORESTIS = 238,
	CONST_ME_DEVOVORGA = 239,
	CONST_ME_FERUMBRAS_2 = 240,
	CONST_ME_FOAM = 241,
};

enum ShootType_t : uint8_t
{
	CONST_ANI_NONE,

	CONST_ANI_SPEAR = 1,
	CONST_ANI_BOLT = 2,
	CONST_ANI_ARROW = 3,
	CONST_ANI_FIRE = 4,
	CONST_ANI_ENERGY = 5,
	CONST_ANI_POISONARROW = 6,
	CONST_ANI_BURSTARROW = 7,
	CONST_ANI_THROWINGSTAR = 8,
	CONST_ANI_THROWINGKNIFE = 9,
	CONST_ANI_SMALLSTONE = 10,
	CONST_ANI_DEATH = 11,
	CONST_ANI_LARGEROCK = 12,
	CONST_ANI_SNOWBALL = 13,
	CONST_ANI_POWERBOLT = 14,
	CONST_ANI_POISON = 15,
	CONST_ANI_INFERNALBOLT = 16,
	CONST_ANI_HUNTINGSPEAR = 17,
	CONST_ANI_ENCHANTEDSPEAR = 18,
	CONST_ANI_REDSTAR = 19,
	CONST_ANI_GREENSTAR = 20,
	CONST_ANI_ROYALSPEAR = 21,
	CONST_ANI_SNIPERARROW = 22,
	CONST_ANI_ONYXARROW = 23,
	CONST_ANI_PIERCINGBOLT = 24,
	CONST_ANI_WHIRLWINDSWORD = 25,
	CONST_ANI_WHIRLWINDAXE = 26,
	CONST_ANI_WHIRLWINDCLUB = 27,
	CONST_ANI_ETHEREALSPEAR = 28,
	CONST_ANI_ICE = 29,
	CONST_ANI_EARTH = 30,
	CONST_ANI_HOLY = 31,
	CONST_ANI_SUDDENDEATH = 32,
	CONST_ANI_FLASHARROW = 33,
	CONST_ANI_FLAMMINGARROW = 34,
	CONST_ANI_SHIVERARROW = 35,
	CONST_ANI_ENERGYBALL = 36,
	CONST_ANI_SMALLICE = 37,
	CONST_ANI_SMALLHOLY = 38,
	CONST_ANI_SMALLEARTH = 39,
	CONST_ANI_EARTHARROW = 40,
	CONST_ANI_EXPLOSION = 41,
	CONST_ANI_CAKE = 42,

	CONST_ANI_TARSALARROW = 44,
	CONST_ANI_VORTEXBOLT = 45,

	CONST_ANI_PRISMATICBOLT = 48,
	CONST_ANI_CRYSTALLINEARROW = 49,
	CONST_ANI_DRILLBOLT = 50,
	CONST_ANI_ENVENOMEDARROW = 51,

	CONST_ANI_GLOOTHSPEAR = 53,
	CONST_ANI_SIMPLEARROW = 54,

	CONST_ANI_LEAFSTAR = 56,
	CONST_ANI_DIAMONDARROW = 57,
	CONST_ANI_SPECTRALBOLT = 58,
	CONST_ANI_ROYALSTAR = 59,

	// for internal use, don't send to client
	CONST_ANI_WEAPONTYPE = 0xFE, // 254
};

enum SpeakClasses : uint8_t
{
	TALKTYPE_SAY = 1,
	TALKTYPE_WHISPER = 2,
	TALKTYPE_YELL = 3,
	TALKTYPE_PRIVATE_FROM = 4, // Received private message
	TALKTYPE_PRIVATE_TO = 5,   // Sent private message
	// TALKTYPE_CHANNEL_M = 6 // not working (?)
	TALKTYPE_CHANNEL_Y = 7,
	TALKTYPE_CHANNEL_O = 8,
	TALKTYPE_SPELL = 9,               // Like SAY but with "casts" instead of "says"
	TALKTYPE_PRIVATE_NP = 10,         // NPC speaking to player
	TALKTYPE_PRIVATE_NP_CONSOLE = 11, // NPC channel message, no text on game screen, for sendPrivateMessage use only
	TALKTYPE_PRIVATE_PN = 12,         // Player speaking to NPC
	TALKTYPE_BROADCAST = 13,
	TALKTYPE_CHANNEL_R1 = 14,       // red - #c text
	TALKTYPE_PRIVATE_RED_FROM = 15, // @name@text
	TALKTYPE_PRIVATE_RED_TO = 16,   // @name@text
	TALKTYPE_MONSTER_SAY = 36,
	TALKTYPE_MONSTER_YELL = 37,
	TALKTYPE_POTION = 52, // Like MONSTER_SAY but can be disabled in client settings
};

enum MessageClasses : uint8_t
{
	MESSAGE_STATUS_DEFAULT = 17,  // White, bottom + console
	MESSAGE_STATUS_WARNING = 18,  // Red, over player + console
	MESSAGE_EVENT_ADVANCE = 19,   // White, over player + console
	MESSAGE_STATUS_WARNING2 = 20, // Red, over player + console
	MESSAGE_STATUS_SMALL = 21,    // White, bottom of the screen
	MESSAGE_INFO_DESCR = 22,      // Green, over player + console

	// White, console
	MESSAGE_DAMAGE_DEALT = 23,
	MESSAGE_DAMAGE_RECEIVED = 24,
	MESSAGE_HEALED = 25,
	MESSAGE_EXPERIENCE = 26,
	MESSAGE_DAMAGE_OTHERS = 27,
	MESSAGE_HEALED_OTHERS = 28,
	MESSAGE_EXPERIENCE_OTHERS = 29,

	MESSAGE_EVENT_DEFAULT = 30, // White, bottom + console
	MESSAGE_LOOT = 31,          // White, over player + console, supports colors as {text|itemClientId}
	MESSAGE_TRADE = 32,         // Green, over player + console

	// White, in channel (needs channel Id)
	MESSAGE_GUILD = 33,
	MESSAGE_PARTY_MANAGEMENT = 34,
	MESSAGE_PARTY = 35,

	MESSAGE_REPORT = 38,         // White, over player + conosle
	MESSAGE_HOTKEY_PRESSED = 39, // Green, over player + console
	// MESSAGE_TUTORIAL_HINT = 40, // not working (?)
	// MESSAGE_THANK_YOU = 41, // not working (?)
	MESSAGE_MARKET = 42, // Window "Market Message" + "Ok" button
	// MESSAGE_MANA = 43, // not working (?)
	MESSAGE_BEYOND_LAST = 44,     // White, console only
	MESSAGE_TOURNAMENT_INFO = 45, // Window "Tournament" + "Ok" button
	// unused 46?
	// unused 47?
	MESSAGE_ATTENTION = 48,        // White, console only
	MESSAGE_BOOSTED_CREATURE = 49, // White, console only
	MESSAGE_OFFLINE_TRAINING = 50, // White, over player + console
	MESSAGE_TRANSACTION = 51,      // White, console only
};

enum FluidColors_t : uint8_t
{
	FLUID_EMPTY,
	FLUID_BLUE,
	FLUID_RED,
	FLUID_BROWN,
	FLUID_GREEN,
	FLUID_YELLOW,
	FLUID_WHITE,
	FLUID_PURPLE,
	FLUID_BLACK,
};

enum FluidTypes_t : uint8_t
{
	FLUID_NONE = FLUID_EMPTY,
	FLUID_WATER = FLUID_BLUE,
	FLUID_BLOOD = FLUID_RED,
	FLUID_BEER = FLUID_BROWN,
	FLUID_SLIME = FLUID_GREEN,
	FLUID_LEMONADE = FLUID_YELLOW,
	FLUID_MILK = FLUID_WHITE,
	FLUID_MANA = FLUID_PURPLE,
	FLUID_INK = FLUID_BLACK,

	FLUID_LIFE = FLUID_RED + 8,
	FLUID_OIL = FLUID_BROWN + 8,
	FLUID_URINE = FLUID_YELLOW + 8,
	FLUID_COCONUTMILK = FLUID_WHITE + 8,
	FLUID_WINE = FLUID_PURPLE + 8,

	FLUID_MUD = FLUID_BROWN + 16,
	FLUID_FRUITJUICE = FLUID_YELLOW + 16,

	FLUID_LAVA = FLUID_RED + 24,
	FLUID_RUM = FLUID_BROWN + 24,
	FLUID_SWAMP = FLUID_GREEN + 24,

	FLUID_TEA = FLUID_BROWN + 32,

	FLUID_MEAD = FLUID_BROWN + 40,
};

const uint8_t reverseFluidMap[] = {
    FLUID_EMPTY, FLUID_WATER, FLUID_MANA,     FLUID_BEER, FLUID_EMPTY, FLUID_BLOOD,
    FLUID_SLIME, FLUID_EMPTY, FLUID_LEMONADE, FLUID_MILK, FLUID_INK,
};

const uint8_t clientToServerFluidMap[] = {
    FLUID_EMPTY,      FLUID_WATER,       FLUID_MANA, FLUID_BEER, FLUID_MUD,  FLUID_BLOOD, FLUID_SLIME,
    FLUID_RUM,        FLUID_LEMONADE,    FLUID_MILK, FLUID_WINE, FLUID_LIFE, FLUID_URINE, FLUID_OIL,
    FLUID_FRUITJUICE, FLUID_COCONUTMILK, FLUID_TEA,  FLUID_MEAD, FLUID_INK,
};

enum ClientFluidTypes_t : uint8_t
{
	CLIENTFLUID_EMPTY = 0,
	CLIENTFLUID_BLUE = 1,
	CLIENTFLUID_PURPLE = 2,
	CLIENTFLUID_BROWN_1 = 3,
	CLIENTFLUID_BROWN_2 = 4,
	CLIENTFLUID_RED = 5,
	CLIENTFLUID_GREEN = 6,
	CLIENTFLUID_BROWN = 7,
	CLIENTFLUID_YELLOW = 8,
	CLIENTFLUID_WHITE = 9,
	CLIENTFLUID_BLACK = 18,
};

const uint8_t fluidMap[] = {
    CLIENTFLUID_EMPTY,  CLIENTFLUID_BLUE,  CLIENTFLUID_RED,    CLIENTFLUID_BROWN_1, CLIENTFLUID_GREEN,
    CLIENTFLUID_YELLOW, CLIENTFLUID_WHITE, CLIENTFLUID_PURPLE, CLIENTFLUID_BLACK,
};

enum SquareColor_t : uint8_t
{
	SQ_COLOR_BLACK = 0,
};

enum TextColor_t : uint8_t
{
	TEXTCOLOR_BLUE = 5,
	TEXTCOLOR_LIGHTGREEN = 30,
	TEXTCOLOR_LIGHTBLUE = 35,
	TEXTCOLOR_DARKGREY = 86,
	TEXTCOLOR_MAYABLUE = 95,
	TEXTCOLOR_DARKRED = 108,
	TEXTCOLOR_LIGHTGREY = 129,
	TEXTCOLOR_SKYBLUE = 143,
	TEXTCOLOR_PURPLE = 154,
	TEXTCOLOR_ELECTRICPURPLE = 155,
	TEXTCOLOR_RED = 180,
	TEXTCOLOR_PASTELRED = 194,
	TEXTCOLOR_ORANGE = 198,
	TEXTCOLOR_YELLOW = 210,
	TEXTCOLOR_WHITE_EXP = 215,
	TEXTCOLOR_NONE = 255,
};

enum Icons_t
{
	ICON_POISON = 1 << 0,
	ICON_BURN = 1 << 1,
	ICON_ENERGY = 1 << 2,
	ICON_DRUNK = 1 << 3,
	ICON_MANASHIELD = 1 << 4,
	ICON_PARALYZE = 1 << 5,
	ICON_HASTE = 1 << 6,
	ICON_SWORDS = 1 << 7,
	ICON_DROWNING = 1 << 8,
	ICON_FREEZING = 1 << 9,
	ICON_DAZZLED = 1 << 10,
	ICON_CURSED = 1 << 11,
	ICON_PARTY_BUFF = 1 << 12,
	ICON_REDSWORDS = 1 << 13,
	ICON_PIGEON = 1 << 14,
	ICON_BLEEDING = 1 << 15,
	ICON_LESSERHEX = 1 << 16,
	ICON_INTENSEHEX = 1 << 17,
	ICON_GREATERHEX = 1 << 18,
	ICON_ROOT = 1 << 19,
	ICON_FEAR = 1 << 20,
	ICON_GOSHNAR1 = 1 << 21,
	ICON_GOSHNAR2 = 1 << 22,
	ICON_GOSHNAR3 = 1 << 23,
	ICON_GOSHNAR4 = 1 << 24,
	ICON_GOSHNAR5 = 1 << 25,
	ICON_MANASHIELD_BREAKABLE = 1 << 26,
};

enum WeaponType_t : uint8_t
{
	WEAPON_NONE,
	WEAPON_SWORD,
	WEAPON_CLUB,
	WEAPON_AXE,
	WEAPON_SHIELD,
	WEAPON_DISTANCE,
	WEAPON_WAND,
	WEAPON_AMMO,
	WEAPON_QUIVER,
};

enum Ammo_t : uint8_t
{
	AMMO_NONE,
	AMMO_BOLT,
	AMMO_ARROW,
	AMMO_SPEAR,
	AMMO_THROWINGSTAR,
	AMMO_THROWINGKNIFE,
	AMMO_STONE,
	AMMO_SNOWBALL,
};

enum WeaponAction_t : uint8_t
{
	WEAPONACTION_NONE,
	WEAPONACTION_REMOVECOUNT,
	WEAPONACTION_REMOVECHARGE,
	WEAPONACTION_MOVE,
};

enum WieldInfo_t
{
	WIELDINFO_NONE = 0 << 0,
	WIELDINFO_LEVEL = 1 << 0,
	WIELDINFO_MAGLV = 1 << 1,
	WIELDINFO_VOCREQ = 1 << 2,
	WIELDINFO_PREMIUM = 1 << 3,
};

enum Skulls_t : uint8_t
{
	SKULL_NONE = 0,
	SKULL_YELLOW = 1,
	SKULL_GREEN = 2,
	SKULL_WHITE = 3,
	SKULL_RED = 4,
	SKULL_BLACK = 5,
	SKULL_ORANGE = 6,
};

enum PartyShields_t : uint8_t
{
	SHIELD_NONE = 0,
	SHIELD_WHITEYELLOW = 1,
	SHIELD_WHITEBLUE = 2,
	SHIELD_BLUE = 3,
	SHIELD_YELLOW = 4,
	SHIELD_BLUE_SHAREDEXP = 5,
	SHIELD_YELLOW_SHAREDEXP = 6,
	SHIELD_BLUE_NOSHAREDEXP_BLINK = 7,
	SHIELD_YELLOW_NOSHAREDEXP_BLINK = 8,
	SHIELD_BLUE_NOSHAREDEXP = 9,
	SHIELD_YELLOW_NOSHAREDEXP = 10,
	SHIELD_GRAY = 11,
};

enum GuildEmblems_t : uint8_t
{
	GUILDEMBLEM_NONE = 0,
	GUILDEMBLEM_ALLY = 1,
	GUILDEMBLEM_ENEMY = 2,
	GUILDEMBLEM_NEUTRAL = 3,
	GUILDEMBLEM_MEMBER = 4,
	GUILDEMBLEM_OTHER = 5,
};

enum item_t : uint16_t
{
	ITEM_BROWSEFIELD = 460, // for internal use
	ITEM_DECORATION_KIT = 26054,

	ITEM_FIREFIELD_PVP_FULL = 1487,
	ITEM_FIREFIELD_PVP_MEDIUM = 1488,
	ITEM_FIREFIELD_PVP_SMALL = 1489,
	ITEM_FIREFIELD_PERSISTENT_FULL = 1492,
	ITEM_FIREFIELD_PERSISTENT_MEDIUM = 1493,
	ITEM_FIREFIELD_PERSISTENT_SMALL = 1494,
	ITEM_FIREFIELD_NOPVP = 1500,
	ITEM_FIREFIELD_NOPVP_MEDIUM = 1501,

	ITEM_POISONFIELD_PVP = 1490,
	ITEM_POISONFIELD_PERSISTENT = 1496,
	ITEM_POISONFIELD_NOPVP = 1503,

	ITEM_ENERGYFIELD_PVP = 1491,
	ITEM_ENERGYFIELD_PERSISTENT = 1495,
	ITEM_ENERGYFIELD_NOPVP = 1504,

	ITEM_MAGICWALL = 1497,
	ITEM_MAGICWALL_PERSISTENT = 1498,
	ITEM_MAGICWALL_SAFE = 11098,
	ITEM_MAGICWALL_NOPVP = 20669,

	ITEM_WILDGROWTH = 1499,
	ITEM_WILDGROWTH_PERSISTENT = 2721,
	ITEM_WILDGROWTH_SAFE = 11099,
	ITEM_WILDGROWTH_NOPVP = 20670,

	ITEM_BAG = 1987,
	ITEM_SHOPPING_BAG = 23782,

	ITEM_GOLD_COIN = 2148,
	ITEM_PLATINUM_COIN = 2152,
	ITEM_CRYSTAL_COIN = 2160,
	ITEM_STORE_COIN = 24774, // in-game store currency

	ITEM_DEPOT = 2594,
	ITEM_LOCKER = 2589,
	ITEM_INBOX = 14404,
	ITEM_MARKET = 14405,
	ITEM_STORE_INBOX = 26052,

	// move to separate enum class?
	ITEM_DEPOT_BOX_I = 25453,
	ITEM_DEPOT_BOX_II = 25454,
	ITEM_DEPOT_BOX_III = 25455,
	ITEM_DEPOT_BOX_IV = 25456,
	ITEM_DEPOT_BOX_V = 25457,
	ITEM_DEPOT_BOX_VI = 25458,
	ITEM_DEPOT_BOX_VII = 25459,
	ITEM_DEPOT_BOX_VIII = 25460,
	ITEM_DEPOT_BOX_IX = 25461,
	ITEM_DEPOT_BOX_X = 25462,
	ITEM_DEPOT_BOX_XI = 25463,
	ITEM_DEPOT_BOX_XII = 25464,
	ITEM_DEPOT_BOX_XIII = 25465,
	ITEM_DEPOT_BOX_XIV = 25466,
	ITEM_DEPOT_BOX_XV = 25467,
	ITEM_DEPOT_BOX_XVI = 25468,
	ITEM_DEPOT_BOX_XVII = 25469,
	ITEM_DEPOT_BOX_XVIII = 34571,
	ITEM_DEPOT_BOX_XIX = 44714,
	ITEM_DEPOT_BOX_XX = 44715,

	ITEM_MALE_CORPSE = 3058,
	ITEM_FEMALE_CORPSE = 3065,

	ITEM_FULLSPLASH = 2016,
	ITEM_SMALLSPLASH = 2019,

	ITEM_PARCEL = 2595,
	ITEM_LETTER = 2597,
	ITEM_LETTER_STAMPED = 2598,
	ITEM_LABEL = 2599,

	ITEM_AMULETOFLOSS = 2173,

	ITEM_DOCUMENT_RO = 1968, // read-only
	ITEM_SUPPLY_STASH = 31406, //read-only
};

enum ResourceTypes_t : uint8_t
{
	RESOURCE_BANK_BALANCE = 0x00,
	RESOURCE_GOLD_EQUIPPED = 0x01,
	RESOURCE_PREY_WILDCARDS = 0x0A,
	RESOURCE_DAILYREWARD_STREAK = 0x14,
	RESOURCE_DAILYREWARD_JOKERS = 0x15,
};

enum PlayerFlags : uint64_t
{
	PlayerFlag_CannotUseCombat = 1 << 0,
	PlayerFlag_CannotAttackPlayer = 1 << 1,
	PlayerFlag_CannotAttackMonster = 1 << 2,
	PlayerFlag_CannotBeAttacked = 1 << 3,
	PlayerFlag_CanConvinceAll = 1 << 4,
	PlayerFlag_CanSummonAll = 1 << 5,
	PlayerFlag_CanIllusionAll = 1 << 6,
	PlayerFlag_CanSenseInvisibility = 1 << 7,
	PlayerFlag_IgnoredByMonsters = 1 << 8,
	PlayerFlag_NotGainInFight = 1 << 9,
	PlayerFlag_HasInfiniteMana = 1 << 10,
	PlayerFlag_HasInfiniteSoul = 1 << 11,
	PlayerFlag_HasNoExhaustion = 1 << 12,
	PlayerFlag_CannotUseSpells = 1 << 13,
	PlayerFlag_CannotPickupItem = 1 << 14,
	PlayerFlag_CanAlwaysLogin = 1 << 15,
	PlayerFlag_CanBroadcast = 1 << 16,
	PlayerFlag_CanEditHouses = 1 << 17,
	PlayerFlag_CannotBeBanned = 1 << 18,
	PlayerFlag_CannotBePushed = 1 << 19,
	PlayerFlag_HasInfiniteCapacity = 1 << 20,
	PlayerFlag_CanPushAllCreatures = 1 << 21,
	PlayerFlag_CanTalkRedPrivate = 1 << 22,
	PlayerFlag_CanTalkRedChannel = 1 << 23,
	PlayerFlag_TalkOrangeHelpChannel = 1 << 24,
	PlayerFlag_NotGainExperience = 1 << 25,
	PlayerFlag_NotGainMana = 1 << 26,
	PlayerFlag_NotGainHealth = 1 << 27,
	PlayerFlag_NotGainSkill = 1 << 28,
	PlayerFlag_SetMaxSpeed = 1 << 29,
	PlayerFlag_SpecialVIP = 1 << 30,
	PlayerFlag_NotGenerateLoot = static_cast<uint64_t>(1) << 31,
	// PlayerFlag with exponent 32 existed but was deprecated (feature dropped from client).
	PlayerFlag_IgnoreProtectionZone = static_cast<uint64_t>(1) << 33,
	PlayerFlag_IgnoreSpellCheck = static_cast<uint64_t>(1) << 34,
	PlayerFlag_IgnoreWeaponCheck = static_cast<uint64_t>(1) << 35,
	PlayerFlag_CannotBeMuted = static_cast<uint64_t>(1) << 36,
	PlayerFlag_IsAlwaysPremium = static_cast<uint64_t>(1) << 37,
	PlayerFlag_IgnoreYellCheck = static_cast<uint64_t>(1) << 38,
	PlayerFlag_IgnoreSendPrivateCheck = static_cast<uint64_t>(1) << 39,
};

enum PodiumFlags : uint8_t
{
	PODIUM_SHOW_PLATFORM = 0, // show the platform below the outfit
	PODIUM_SHOW_OUTFIT = 1,   // show outfit
	PODIUM_SHOW_MOUNT = 2     // show mount
};

enum ReloadTypes_t : uint8_t
{
	RELOAD_TYPE_ALL,
	RELOAD_TYPE_ACTIONS,
	RELOAD_TYPE_CHAT,
	RELOAD_TYPE_CONFIG,
	RELOAD_TYPE_CREATURESCRIPTS,
	RELOAD_TYPE_EVENTS,
	RELOAD_TYPE_GLOBAL,
	RELOAD_TYPE_GLOBALEVENTS,
	RELOAD_TYPE_ITEMS,
	RELOAD_TYPE_MONSTERS,
	RELOAD_TYPE_MOUNTS,
	RELOAD_TYPE_MOVEMENTS,
	RELOAD_TYPE_NPCS,
	RELOAD_TYPE_QUESTS,
	RELOAD_TYPE_SCRIPTS,
	RELOAD_TYPE_SPELLS,
	RELOAD_TYPE_TALKACTIONS,
	RELOAD_TYPE_WEAPONS,
};

enum MonsterIcon_t : uint8_t
{
	MONSTER_ICON_VULNERABLE = 1,
	MONSTER_ICON_WEAKENED,
	MONSTER_ICON_MELEE,
	MONSTER_ICON_INFLUENCED,
	MONSTER_ICON_FIENDISH,

	MONSTER_ICON_FIRST = MONSTER_ICON_VULNERABLE,
	MONSTER_ICON_LAST = MONSTER_ICON_FIENDISH
};

enum CreatureIcon_t : uint8_t
{
	CREATURE_ICON_CROSS_WHITE = 1,
	CREATURE_ICON_CROSS_WHITE_RED,
	CREATURE_ICON_ORB_RED,
	CREATURE_ICON_ORB_GREEN,
	CREATURE_ICON_ORB_RED_GREEN,
	CREATURE_ICON_GEM_GREEN,
	CREATURE_ICON_GEM_YELLOW,
	CREATURE_ICON_GEM_BLUE,
	CREATURE_ICON_GEM_PURPLE,
	CREATURE_ICON_GEM_RED,
	CREATURE_ICON_PIGEON,
	CREATURE_ICON_ENERGY,
	CREATURE_ICON_POISON,
	CREATURE_ICON_WATER,
	CREATURE_ICON_FIRE,
	CREATURE_ICON_ICE,
	CREATURE_ICON_ARROW_UP,
	CREATURE_ICON_ARROW_DOWN,
	CREATURE_ICON_WARNING,
	CREATURE_ICON_QUESTION,
	CREATURE_ICON_CROSS_RED,

	CREATURE_ICON_FIRST = CREATURE_ICON_CROSS_WHITE,
	CREATURE_ICON_LAST = CREATURE_ICON_CROSS_RED
};

enum Supply_Stash_Actions_t : uint8_t {
	SUPPLY_STASH_ACTION_STOW_ITEM = 0,
	SUPPLY_STASH_ACTION_STOW_CONTAINER = 1,
	SUPPLY_STASH_ACTION_STOW_STACK = 2,
	SUPPLY_STASH_ACTION_WITHDRAW = 3
};

static constexpr int32_t CHANNEL_GUILD = 0x00;
static constexpr int32_t CHANNEL_PARTY = 0x01;
static constexpr int32_t CHANNEL_PRIVATE = 0xFFFF;

// Reserved player storage key ranges;
//[10000000 - 20000000];
static constexpr int32_t PSTRG_RESERVED_RANGE_START = 10000000;
static constexpr int32_t PSTRG_RESERVED_RANGE_SIZE = 10000000;

#define IS_IN_KEYRANGE(key, range) \
	(key >= PSTRG_##range##_START && ((key - PSTRG_##range##_START) <= PSTRG_##range##_SIZE))

#endif // FS_CONST_H
