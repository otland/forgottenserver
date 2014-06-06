-- Combat settings
-- NOTE: valid values for worldType are: "pvp", "no-pvp" and "pvp-enforced"
worldType = "pvp"
hotkeyAimbotEnabled = "yes"
protectionLevel = 1
killsToRedSkull = 3
killsToBlackSkull = 6
pzLocked = 60000
removeAmmoWhenUsingDistanceWeapon = "yes"
removeChargesFromRunes = "yes"
timeToDecreaseFrags = 24 * 60 * 60 * 1000
whiteSkullTime = 15 * 60 * 1000
stairJumpExhaustion = 2000
experienceByKillingPlayers = "no"
expFromPlayersLevelRange = 75
noDamageToSameLookfeet = "no"

-- Connection Config
-- NOTE: maxPlayers set to 0 means no limit
ip = "127.0.0.1"
bindOnlyGlobalAddress = "no"
loginProtocolPort = 7171
gameProtocolPort = 7172
statusProtocolPort = 7171
maxPlayers = 0
motd = "Welcome to The Forgotten Server!"
onePlayerOnlinePerAccount = "yes"
allowClones = "no"
serverName = "Forgotten"
statusTimeout = 5000
replaceKickOnLogin = "yes"
maxPacketsPerSecond = 25

-- Deaths
-- NOTE: Leave deathLosePercent as -1 if you want to use the default
-- death penalty formula. For the old formula, set it to 10. For
-- no skill/experience loss, set it to 0.
deathLosePercent = -1

-- Houses
-- NOTE: set housePriceEachSQM to -1 to disable the ingame buy house functionality
housePriceEachSQM = 1000
houseRentPeriod = "never"

-- Item Usage
timeBetweenActions = 200
timeBetweenExActions = 1000

-- Map
-- NOTE: set mapName WITHOUT .otbm at the end
mapName = "forgotten"
mapAuthor = "Komic"

-- Market
marketOfferDuration = 30 * 24 * 60 * 60
premiumToCreateMarketOffer = "yes"
checkExpiredMarketOffersEachMinutes = 60
maxMarketOffersAtATimePerPlayer = 100

-- MySQL
mysqlHost = "127.0.0.1"
mysqlUser = "forgottenserver"
mysqlPass = ""
mysqlDatabase = "forgottenserver"
mysqlPort = 3306
mysqlSock = ""

-- Misc.
allowChangeOutfit = "yes"
freePremium = "no"
kickIdlePlayerAfterMinutes = 15
maxMessageBuffer = 4
emoteSpells = "no"

-- Rates
-- NOTE: rateExp is not used if you have enabled stages in data/XML/stages.xml
rateExp = 5
rateSkill = 3
rateLoot = 2
rateMagic = 3
rateSpawn = 1

-- Monsters
deSpawnRange = 2
deSpawnRadius = 50

-- Stamina
staminaSystem = "yes"

-- Startup
-- NOTE: defaultPriority only works on Windows and sets process priority.
defaultPriority = "high"
startupDatabaseOptimization = "no"

-- Status server information
ownerName = ""
ownerEmail = ""
url = "http://otland.net/"
location = "Sweden"
