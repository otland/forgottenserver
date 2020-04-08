-- Items ordered by ClientID

-- battle axe
local battleAxe = Weapon(WEAPON_AXE)
battleAxe:id(3266)
battleAxe:attack(25)
battleAxe:defense(10)
battleAxe:slotType("two-handed")
battleAxe:vocation("knight", true, true)
battleAxe:vocation("elite knight")
battleAxe:register()

-- hand axe
local handAxe = Weapon(WEAPON_AXE)
handAxe:id(3268)
handAxe:attack(10)
handAxe:defense(5)
handAxe:vocation("knight", true, true)
handAxe:vocation("elite knight")
handAxe:register()

-- halberd
local halberd = Weapon(WEAPON_AXE)
halberd:id(3269)
halberd:level(25)
halberd:attack(35)
halberd:defense(14)
halberd:wieldUnproperly(true)
halberd:slotType("two-handed")
halberd:vocation("knight", true, true)
halberd:vocation("elite knight")
halberd:register()

-- axe
local axe = Weapon(WEAPON_AXE)
axe:id(3274)
axe:attack(12)
axe:defense(6)
axe:register()

-- double axe
local doubleAxe = Weapon(WEAPON_AXE)
doubleAxe:id(3275)
doubleAxe:level(25)
doubleAxe:attack(12)
doubleAxe:defense(6)
doubleAxe:wieldUnproperly(true)
doubleAxe:slotType("two-handed")
doubleAxe:vocation("knight", true, true)
doubleAxe:vocation("elite knight")
doubleAxe:register()

-- hatchet
local hatchet = Weapon(WEAPON_AXE)
hatchet:id(3276)
hatchet:attack(15)
hatchet:defense(8)
hatchet:register()

-- sickle
local sickle = Weapon(WEAPON_AXE)
sickle:id(3293)
sickle:attack(5)
sickle:defense(4)
sickle:register()

-- dragon lance
local dragonLance = Weapon(WEAPON_AXE)
dragonLance:id(3302)
dragonLance:level(60)
dragonLance:attack(47)
dragonLance:defense(16)
dragonLance:wieldUnproperly(true)
dragonLance:slotType("two-handed")
dragonLance:register()

-- great axe
local greatAxe = Weapon(WEAPON_AXE)
greatAxe:id(3303)
greatAxe:level(95)
greatAxe:attack(52)
greatAxe:defense(22)
greatAxe:wieldUnproperly(true)
greatAxe:slotType("two-handed")
greatAxe:vocation("knight", true, true)
greatAxe:vocation("elite knight")
greatAxe:register()

-- golden sickle
local goldenSickle = Weapon(WEAPON_AXE)
goldenSickle:id(3306)
goldenSickle:attack(13)
goldenSickle:defense(6)
goldenSickle:register()

-- obsidian lance
local obsidianLance = Weapon(WEAPON_AXE)
obsidianLance:id(3313)
obsidianLance:level(20)
obsidianLance:attack(34)
obsidianLance:defense(10)
obsidianLance:wieldUnproperly(true)
obsidianLance:slotType("two-handed")
obsidianLance:register()

-- naginata
local naginata = Weapon(WEAPON_AXE)
naginata:id(3314)
naginata:level(25)
naginata:attack(39)
naginata:defense(25)
naginata:wieldUnproperly(true)
naginata:slotType("two-handed")
naginata:register()

-- guardian halberd
local guardianHalberd = Weapon(WEAPON_AXE)
guardianHalberd:id(3315)
guardianHalberd:level(55)
guardianHalberd:attack(46)
guardianHalberd:defense(15)
guardianHalberd:wieldUnproperly(true)
guardianHalberd:slotType("two-handed")
guardianHalberd:register()

-- orcish axe
local orcishAxe = Weapon(WEAPON_AXE)
orcishAxe:id(3316)
orcishAxe:attack(23)
orcishAxe:defense(12)
orcishAxe:register()

-- barbarian axe
local barbarianAxe = Weapon(WEAPON_AXE)
barbarianAxe:id(3317)
barbarianAxe:level(18)
barbarianAxe:attack(28)
barbarianAxe:defense(10,1)
barbarianAxe:wieldUnproperly(true)
barbarianAxe:register()

-- knight axe
local knightAxe = Weapon(WEAPON_AXE)
knightAxe:id(3318)
knightAxe:level(25)
knightAxe:attack(33)
knightAxe:defense(21,1)
knightAxe:wieldUnproperly(true)
knightAxe:register()

-- stonecutter axe
local stonecutterAxe = Weapon(WEAPON_AXE)
stonecutterAxe:id(3319)
stonecutterAxe:level(90)
stonecutterAxe:attack(50)
stonecutterAxe:defense(30,3)
stonecutterAxe:wieldUnproperly(true)
stonecutterAxe:register()

-- fire axe
local fireAxe = Weapon(WEAPON_AXE)
fireAxe:id(3320)
fireAxe:level(35)
fireAxe:attack(27)
fireAxe:defense(16,1)
fireAxe:wieldUnproperly(true)
fireAxe:extraElement(11, "fire")
fireAxe:register()

-- dwarven axe
local dwarvenAxe = Weapon(WEAPON_AXE)
dwarvenAxe:id(3323)
dwarvenAxe:level(18)
dwarvenAxe:attack(31)
dwarvenAxe:defense(19,2)
dwarvenAxe:wieldUnproperly(true)
dwarvenAxe:register()

-- twin axe
local twinAxe = Weapon(WEAPON_AXE)
twinAxe:id(3335)
twinAxe:level(50)
twinAxe:attack(45)
twinAxe:defense(24)
twinAxe:wieldUnproperly(true)
twinAxe:slotType("two-handed")
twinAxe:vocation("knight", true, true)
twinAxe:vocation("elite knight")
twinAxe:register()

-- beastslayer axe
local warAxe = Weapon(WEAPON_AXE)
warAxe:id(3344)
warAxe:level(30)
warAxe:attack(35)
warAxe:defense(12,1)
warAxe:wieldUnproperly(true)
warAxe:slotType("two-handed")
warAxe:register()

-- daramian waraxe
local daramianWaraxe = Weapon(WEAPON_AXE)
daramianWaraxe:id(3328)
daramianWaraxe:level(25)
daramianWaraxe:attack(39)
daramianWaraxe:defense(15)
daramianWaraxe:wieldUnproperly(true)
daramianWaraxe:slotType("two-handed")
daramianWaraxe:vocation("knight", true, true)
daramianWaraxe:vocation("elite knight")
daramianWaraxe:register()

-- daramian axe
local daramianAaxe = Weapon(WEAPON_AXE)
daramianAaxe:id(3329)
daramianAaxe:attack(17)
daramianAaxe:defense(8)
daramianAaxe:register()

-- ravager's axe
local ravagersAxe = Weapon(WEAPON_AXE)
ravagersAxe:id(3331)
ravagersAxe:level(70)
ravagersAxe:attack(49)
ravagersAxe:defense(14)
ravagersAxe:wieldUnproperly(true)
ravagersAxe:slotType("two-handed")
ravagersAxe:vocation("knight", true, true)
ravagersAxe:vocation("elite knight")
ravagersAxe:register()

-- war axe
local warAxe = Weapon(WEAPON_AXE)
warAxe:id(3342)
warAxe:level(65)
warAxe:attack(48)
warAxe:defense(10)
warAxe:wieldUnproperly(true)
warAxe:slotType("two-handed")
warAxe:vocation("knight", true, true)
warAxe:vocation("elite knight")
warAxe:register()

-- ripper lance
local ripperLance = Weapon(WEAPON_AXE)
ripperLance:id(3346)
ripperLance:attack(28)
ripperLance:defense(7)
ripperLance:register()

-- scythe
local scythe = Weapon(WEAPON_AXE)
scythe:id(3453)
scythe:attack(8)
scythe:defense(3)
scythe:register()

-- ruthless axe
local ruthlessAxe = Weapon(WEAPON_AXE)
ruthlessAxe:id(6553)
ruthlessAxe:level(75)
ruthlessAxe:attack(50)
ruthlessAxe:defense(15)
ruthlessAxe:wieldUnproperly(true)
ruthlessAxe:slotType("two-handed")
ruthlessAxe:register()

-- headchopper
local headchopper = Weapon(WEAPON_AXE)
headchopper:id(7380)
headchopper:level(35)
headchopper:attack(42)
headchopper:defense(20)
headchopper:wieldUnproperly(true)
headchopper:slotType("two-handed")
headchopper:vocation("knight", true, true)
headchopper:vocation("elite knight")
headchopper:register()

-- vile axe
local vileAxe = Weapon(WEAPON_AXE)
vileAxe:id(7388)
vileAxe:level(55)
vileAxe:attack(43)
vileAxe:defense(19,3)
vileAxe:wieldUnproperly(true)
vileAxe:register()

-- heroic axe
local heroicAxe = Weapon(WEAPON_AXE)
heroicAxe:id(7389)
heroicAxe:level(60)
heroicAxe:attack(44)
heroicAxe:defense(24,1)
heroicAxe:wieldUnproperly(true)
heroicAxe:register()

-- ornamented axe
local ornamentedAxe = Weapon(WEAPON_AXE)
ornamentedAxe:id(7411)
ornamentedAxe:level(50)
ornamentedAxe:attack(42)
ornamentedAxe:defense(22,2)
ornamentedAxe:wieldUnproperly(true)
ornamentedAxe:register()

-- butcher's axe
local ornamentedAxe = Weapon(WEAPON_AXE)
ornamentedAxe:id(7412)
ornamentedAxe:level(45)
ornamentedAxe:attack(41)
ornamentedAxe:defense(24,-2)
ornamentedAxe:wieldUnproperly(true)
ornamentedAxe:register()

-- titan axe
local titanAxe = Weapon(WEAPON_AXE)
titanAxe:id(7413)
titanAxe:level(40)
titanAxe:attack(43)
titanAxe:defense(30)
titanAxe:wieldUnproperly(true)
titanAxe:slotType("two-handed")
titanAxe:register()

-- dreaded cleaver
local dreadedCleaver = Weapon(WEAPON_AXE)
dreadedCleaver:id(7419)
dreadedCleaver:level(40)
dreadedCleaver:attack(40)
dreadedCleaver:defense(19,-3)
dreadedCleaver:wieldUnproperly(true)
dreadedCleaver:register()

-- reaper's axe
local reapersAxe = Weapon(WEAPON_AXE)
reapersAxe:id(7420)
reapersAxe:level(70)
reapersAxe:attack(46)
reapersAxe:defense(25,2)
reapersAxe:wieldUnproperly(true)
reapersAxe:register()

-- ravenwing
local ravenwing = Weapon(WEAPON_AXE)
ravenwing:id(7433)
ravenwing:level(65)
ravenwing:attack(45)
ravenwing:defense(22,1)
ravenwing:wieldUnproperly(true)
ravenwing:register()

-- royal axe
local royalAxe = Weapon(WEAPON_AXE)
royalAxe:id(7434)
royalAxe:level(75)
royalAxe:attack(47)
royalAxe:defense(25,3)
royalAxe:wieldUnproperly(true)
royalAxe:register()

-- impaler
local impaler = Weapon(WEAPON_AXE)
impaler:id(7435)
impaler:level(85)
impaler:attack(49)
impaler:defense(25,-2)
impaler:wieldUnproperly(true)
impaler:register()

-- angelic axe
local angelicAxe = Weapon(WEAPON_AXE)
angelicAxe:id(7436)
angelicAxe:level(45)
angelicAxe:attack(44)
angelicAxe:defense(24)
angelicAxe:wieldUnproperly(true)
angelicAxe:slotType("two-handed")
angelicAxe:register()

-- executioner
local executioner = Weapon(WEAPON_AXE)
executioner:id(7453)
executioner:level(85)
executioner:attack(51)
executioner:defense(20)
executioner:wieldUnproperly(true)
daramianWaraxe:slotType("two-handed")
daramianWaraxe:vocation("knight", true, true)
daramianWaraxe:vocation("elite knight")
executioner:register()

-- glorious axe
local gloriousAxe = Weapon(WEAPON_AXE)
gloriousAxe:id(7454)
gloriousAxe:level(30)
gloriousAxe:attack(40)
gloriousAxe:defense(23)
gloriousAxe:wieldUnproperly(true)
gloriousAxe:slotType("two-handed")
gloriousAxe:vocation("knight", true, true)
gloriousAxe:vocation("elite knight")
gloriousAxe:register()

-- mythril axe
local mythrilAxe = Weapon(WEAPON_AXE)
mythrilAxe:id(7455)
mythrilAxe:level(35)
mythrilAxe:attack(39)
mythrilAxe:defense(22)
mythrilAxe:wieldUnproperly(true)
mythrilAxe:register()

-- noble axe
local nobleAxe = Weapon(WEAPON_AXE)
nobleAxe:id(7456)
nobleAxe:level(35)
nobleAxe:attack(39)
nobleAxe:defense(22)
nobleAxe:wieldUnproperly(true)
nobleAxe:register()

-- steel axe
local steelAxe = Weapon(WEAPON_AXE)
steelAxe:id(7773)
steelAxe:attack(21)
steelAxe:defense(10)
steelAxe:register()

-- hellforged axe
local hellforgedAxe = Weapon(WEAPON_AXE)
hellforgedAxe:id(8096)
hellforgedAxe:level(110)
hellforgedAxe:attack(51)
hellforgedAxe:defense(28,2)
hellforgedAxe:wieldUnproperly(true)
hellforgedAxe:register()

-- solar axe
local solarAxe = Weapon(WEAPON_AXE)
solarAxe:id(8097)
solarAxe:level(130)
solarAxe:attack(52)
solarAxe:defense(29,3)
solarAxe:wieldUnproperly(true)
solarAxe:register()

-- demonwing axe
local demonwingAxe = Weapon(WEAPON_AXE)
demonwingAxe:id(8098)
demonwingAxe:level(120)
demonwingAxe:attack(53)
demonwingAxe:defense(20)
demonwingAxe:wieldUnproperly(true)
demonwingAxe:slotType("two-handed")
demonwingAxe:vocation("knight", true, true)
demonwingAxe:vocation("elite knight")
demonwingAxe:register()

-- drakinata
local drakinata = Weapon(WEAPON_AXE)
drakinata:id(10388)
drakinata:level(60)
drakinata:attack(47)
drakinata:defense(20)
drakinata:wieldUnproperly(true)
drakinata:slotType("two-handed")
drakinata:register()

-- twin hooks
local twinHooks = Weapon(WEAPON_AXE)
twinHooks:id(10392)
twinHooks:level(20)
twinHooks:attack(32)
twinHooks:defense(22)
twinHooks:wieldUnproperly(true)
twinHooks:slotType("two-handed")
twinHooks:vocation("knight", true, true)
twinHooks:vocation("elite knight")
twinHooks:register()

-- Zaoan halberd
local zaoanHalberd = Weapon(WEAPON_AXE)
zaoanHalberd:id(10406)
zaoanHalberd:level(25)
zaoanHalberd:attack(37)
zaoanHalberd:defense(15)
zaoanHalberd:wieldUnproperly(true)
zaoanHalberd:slotType("two-handed")
zaoanHalberd:register()

-- deepling axe
local deeplingAxe = Weapon(WEAPON_AXE)
deeplingAxe:id(13991)
deeplingAxe:level(80)
deeplingAxe:attack(49)
deeplingAxe:defense(29)
deeplingAxe:wieldUnproperly(true)
deeplingAxe:register()

-- warrior's axe
local warriorsAxe = Weapon(WEAPON_AXE)
warriorsAxe:id(14040)
warriorsAxe:level(40)
warriorsAxe:attack(42)
warriorsAxe:defense(21)
warriorsAxe:wieldUnproperly(true)
warriorsAxe:register()

-- guardian axe
local guardianAxe = Weapon(WEAPON_AXE)
guardianAxe:id(14043)
guardianAxe:level(50)
guardianAxe:attack(46)
guardianAxe:defense(11)
guardianAxe:wieldUnproperly(true)
guardianAxe:slotType("two-handed")
guardianAxe:register()

-- hive scythe
local hiveScythe = Weapon(WEAPON_AXE)
hiveScythe:id(14089)
hiveScythe:level(70)
hiveScythe:attack(46)
hiveScythe:defense(24)
hiveScythe:wieldUnproperly(true)
hiveScythe:register()

-- crystalline axe
local crystallineAxe = Weapon(WEAPON_AXE)
crystallineAxe:id(16161)
crystallineAxe:level(120)
crystallineAxe:attack(51)
crystallineAxe:defense(29,3)
crystallineAxe:wieldUnproperly(true)
crystallineAxe:register()

-- crude umbral axe
local crudeUmbralAxe = Weapon(WEAPON_AXE)
crudeUmbralAxe:id(20070)
crudeUmbralAxe:level(75)
crudeUmbralAxe:attack(49)
crudeUmbralAxe:defense(24,1)
crudeUmbralAxe:wieldUnproperly(true)
crudeUmbralAxe:vocation("knight", true, true)
crudeUmbralAxe:vocation("elite knight")
crudeUmbralAxe:register()

-- umbral axe
local umbralAxe = Weapon(WEAPON_AXE)
umbralAxe:id(20071)
umbralAxe:level(120)
umbralAxe:attack(51)
umbralAxe:defense(27)
umbralAxe:wieldUnproperly(true)
umbralAxe:vocation("knight", true, true)
umbralAxe:vocation("elite knight")
umbralAxe:register()

-- umbral master axe
local umbralMasterAxe = Weapon(WEAPON_AXE)
umbralMasterAxe:id(20072)
umbralMasterAxe:level(250)
umbralMasterAxe:attack(53)
umbralMasterAxe:defense(30,3)
umbralMasterAxe:wieldUnproperly(true)
umbralMasterAxe:vocation("knight", true, true)
umbralMasterAxe:vocation("elite knight")
umbralMasterAxe:register()

-- crude umbral chopper
local crudeUmbralChopper = Weapon(WEAPON_AXE)
crudeUmbralChopper:id(20073)
crudeUmbralChopper:level(75)
crudeUmbralChopper:attack(51)
crudeUmbralChopper:defense(27)
crudeUmbralChopper:wieldUnproperly(true)
crudeUmbralChopper:slotType("two-handed")
crudeUmbralChopper:vocation("knight", true, true)
crudeUmbralChopper:vocation("elite knight")
crudeUmbralChopper:register()

-- umbral chopper
local umbralChopper = Weapon(WEAPON_AXE)
umbralChopper:id(20074)
umbralChopper:level(120)
umbralChopper:attack(52)
umbralChopper:defense(30)
umbralChopper:wieldUnproperly(true)
umbralChopper:slotType("two-handed")
umbralChopper:vocation("knight", true, true)
umbralChopper:vocation("elite knight")
umbralChopper:register()

-- umbral master chopper
local umbralMasterChopper = Weapon(WEAPON_AXE)
umbralMasterChopper:id(20075)
umbralMasterChopper:level(250)
umbralMasterChopper:attack(54)
umbralMasterChopper:defense(34)
umbralMasterChopper:wieldUnproperly(true)
umbralMasterChopper:slotType("two-handed")
umbralMasterChopper:vocation("knight", true, true)
umbralMasterChopper:vocation("elite knight")
umbralMasterChopper:register()

-- mino lance
local minoLance = Weapon(WEAPON_AXE)
minoLance:id(21174)
minoLance:level(45)
minoLance:attack(40)
minoLance:defense(23)
minoLance:wieldUnproperly(true)
minoLance:register()

-- execowtioner axe
local execowtionerAxe = Weapon(WEAPON_AXE)
execowtionerAxe:id(21176)
execowtionerAxe:level(55)
execowtionerAxe:attack(46)
execowtionerAxe:defense(18)
execowtionerAxe:wieldUnproperly(true)
execowtionerAxe:slotType("two-handed")
execowtionerAxe:register()

-- glooth axe
local gloothAxe = Weapon(WEAPON_AXE)
gloothAxe:id(21180)
gloothAxe:level(75)
gloothAxe:attack(39)
gloothAxe:defense(1)
gloothAxe:wieldUnproperly(true)
gloothAxe:extraElement(26, "earth")
gloothAxe:charges(2, true) -- showCharges = true
gloothAxe:action("removecharge")
gloothAxe:slotType("two-handed")
gloothAxe:vocation("knight", true, true)
gloothAxe:vocation("elite knight")
gloothAxe:register()

-- ogre choppa
local ogreChoppa = Weapon(WEAPON_AXE)
ogreChoppa:id(22172)
ogreChoppa:level(25)
ogreChoppa:attack(39)
ogreChoppa:defense(23)
ogreChoppa:wieldUnproperly(true)
ogreChoppa:slotType("two-handed")
ogreChoppa:register()

-- rift lance
local riftLance = Weapon(WEAPON_AXE)
riftLance:id(22727)
riftLance:level(70)
riftLance:attack(48)
riftLance:defense(28)
riftLance:wieldUnproperly(true)
riftLance:slotType("two-handed")
riftLance:register()
