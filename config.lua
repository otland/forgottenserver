-- Banishments
broadcastBanishments = "no"
banDays = 7
finalBanDays = 30

-- Combat settings
worldType = "pvp"
hotkeyAimbotEnabled = "yes"
protectionLevel = 1
killsToRedSkull = 3
killsToBlackSkull = 6
killsToBan = 0
pzLocked = 60000
criticalHitChance = 0
removeAmmoWhenUsingDistanceWeapon = "yes"
removeChargesFromRunes = "yes"
removeChargesFromWeapons = "yes"
timeToDecreaseFrags = 24 * 60 * 60 * 1000
whiteSkullTime = 15 * 60 * 1000
oldConditionAccuracy = "no"
stairJumpExhaustion = 2000
experienceByKillingPlayers = "no"

-- Connection Config
ip = "127.0.0.1"
bindOnlyGlobalAddress = "no"
loginProtocolPort = 7171
gameProtocolPort = 7172
adminProtocolPort = 7171
statusProtocolPort = 7171
loginTries = 10
retryTimeout = 5 * 1000
loginTimeout = 60 * 1000
maxPlayers = "1000"
motd = "Welcome to The Forgotten Server!"
onePlayerOnlinePerAccount = "yes"
allowClones = "no"
serverName = "Forgotten"
loginMessage = "Welcome to The Forgotten Server!"
adminLogsEnabled = "no"
statusTimeout = 5 * 60 * 1000
replaceKickOnLogin = "yes"
maxPacketsPerSecond = 25

-- Deaths
-- NOTE: Leave deathLosePercent as -1 if you want to use Tibia's
-- death penalty formula. For the old formula, set it to 10. For
-- no skill/experience loss, set it to 0.
deathLosePercent = -1
deathListEnabled = "yes"
maxDeathRecords = 5

-- Houses
housePriceEachSQM = 1000
houseRentPeriod = "never"

-- Item Usage
timeBetweenActions = 200
timeBetweenExActions = 1000

-- Map
mapName = "forgotten"
mapAuthor = "Komic"

-- Market
marketEnabled = "yes"
marketOfferDuration = 30 * 24 * 60 * 60
premiumToCreateMarketOffer = "yes"
checkExpiredMarketOffersEachMinutes = 60
maxMarketOffersAtATimePerPlayer = 100

-- MySQL
mysqlHost = "localhost"
mysqlUser = "root"
mysqlPass = ""
mysqlDatabase = "theforgottenserver"
mysqlPort = 3306

-- Misc.
allowChangeOutfit = "yes"
displayGamemastersWithOnlineCommand = "no"
displayOnOrOffAtCharlist = "no"
freePremium = "no"
kickIdlePlayerAfterMinutes = 15
maxMessageBuffer = 4
noDamageToSameLookfeet = "no"

-- Rates
-- NOTE: rateExp is not used if you have enabled stages in data/XML/stages.xml
rateExp = 5
rateSkill = 3
rateLoot = 2
rateMagic = 3
rateSpawn = 1

-- Server save
-- NOTE: serverSaveHour is the hour of the day when the server save will occur,
-- if you would rather save the server with intervals, disable server save and
-- use autoSaveEachMinutes.
serverSaveEnabled = "no"
serverSaveHour = 10
shutdownAtServerSave = "yes"
cleanMapAtServerSave = "yes"
autoSaveEachMinutes = 0
saveGlobalStorage = "no"

-- Monsters
deSpawnRange = 2
deSpawnRadius = 50

-- Stamina
staminaSystem = "yes"

-- Startup
-- NOTE: defaultPriority only works on Windows and sets process priority.
defaultPriority = "high"
startupDatabaseOptimization = "yes"

-- Storage
passwordType = "plain"
mapStorageType = "binary-tilebased"

-- Status server information
ownerName = "Mark Samman"
ownerEmail = "mark.samman@gmail.com"
url = "http://otland.net/"
location = "Sweden"
