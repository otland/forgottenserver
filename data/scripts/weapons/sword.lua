-- Items ordered by ClientID

-- sword
local sword = Weapon(WEAPON_SWORD)
sword:id(3264)
sword:attack(14)
sword:defense(12,1)
sword:register()

-- two handed sword
local twoHanded = Weapon(WEAPON_SWORD)
twoHanded:id(3265)
twoHanded:level(20)
twoHanded:attack(30)
twoHanded:defense(25)
twoHanded:wieldUnproperly(true)
twoHanded:vocation("knight", true, true)
twoHanded:vocation("elite knight")
twoHanded:register()

-- dagger
local dagger = Weapon(WEAPON_SWORD)
dagger:id(3267)
dagger:attack(8)
dagger:defense(6)
dagger:register()

-- spike sword
local spikeSword = Weapon(WEAPON_SWORD)
spikeSword:id(3271)
spikeSword:attack(24)
spikeSword:defense(21,2)
spikeSword:register()

-- rapier
local rapier = Weapon(WEAPON_SWORD)
rapier:id(3272)
rapier:attack(10)
rapier:defense(8,1)
rapier:register()

-- sabre
local sabre = Weapon(WEAPON_SWORD)
sabre:id(3273)
sabre:attack(12)
sabre:defense(10,1)
sabre:register()

-- magic longsword
local magicLongsword = Weapon(WEAPON_SWORD)
magicLongsword:id(3278)
magicLongsword:level(140)
magicLongsword:attack(55)
magicLongsword:defense(40)
magicLongsword:wieldUnproperly(true)
magicLongsword:slotType("two-handed")
magicLongsword:vocation("knight", true, true)
magicLongsword:vocation("elite knight")
magicLongsword:register()

-- fire sword
local fireSword = Weapon(WEAPON_SWORD)
fireSword:id(3280)
fireSword:level(30)
fireSword:attack(24)
fireSword:defense(20, 1)
fireSword:wieldUnproperly(true)
fireSword:extraElement(11, "fire")
fireSword:register()

-- giant sword
local giantSword = Weapon(WEAPON_SWORD)
giantSword:id(3281)
giantSword:level(55)
giantSword:attack(46)
giantSword:defense(22)
giantSword:wieldUnproperly(true)
giantSword:slotType("two-handed")
giantSword:vocation("knight", true, true)
giantSword:vocation("elite knight")
giantSword:register()

-- carlin sword
local carlinSword = Weapon(WEAPON_SWORD)
carlinSword:id(3283)
carlinSword:attack(15)
carlinSword:defense(13,1)
carlinSword:register()

-- ice rapier
local iceRapier = Weapon(WEAPON_SWORD)
iceRapier:id(3284)
iceRapier:attack(42)
iceRapier:defense(1)
iceRapier:extraElement(18, "ice")
iceRapier:charges(1, true) -- showCharges = true
iceRapier:action("removecharge")
iceRapier:register()

-- carlin sword
local longsword = Weapon(WEAPON_SWORD)
longsword:id(3285)
longsword:attack(17)
longsword:defense(14)
longsword:register()

-- magic sword
local magicSword = Weapon(WEAPON_SWORD)
magicSword:id(3288)
magicSword:level(80)
magicSword:attack(48)
magicSword:defense(35, 3)
magicSword:wieldUnproperly(true)
magicSword:register()

-- silver dagger
local silverDagger = Weapon(WEAPON_SWORD)
silverDagger:id(3290)
silverDagger:attack(9)
silverDagger:defense(7)
silverDagger:register()

-- knife
local knife = Weapon(WEAPON_SWORD)
knife:id(3291)
knife:attack(5)
knife:defense(5)
knife:register()

-- combat knife
local combatKnife = Weapon(WEAPON_SWORD)
combatKnife:id(3292)
combatKnife:attack(8)
combatKnife:defense(6)
combatKnife:register()

-- short sword
local shortSword = Weapon(WEAPON_SWORD)
shortSword:id(3294)
shortSword:attack(11)
shortSword:defense(11)
shortSword:register()

-- bright sword
local brightSword = Weapon(WEAPON_SWORD)
brightSword:id(3295)
brightSword:level(30)
brightSword:attack(36)
brightSword:defense(30, 1)
brightSword:wieldUnproperly(true)
brightSword:extraElement(11, "fire")
brightSword:register()

-- warlord sword
local warlordSword = Weapon(WEAPON_SWORD)
warlordSword:id(3296)
warlordSword:level(120)
warlordSword:attack(53)
warlordSword:defense(38)
warlordSword:wieldUnproperly(true)
warlordSword:slotType("two-handed")
warlordSword:vocation("knight", true, true)
warlordSword:vocation("elite knight")
warlordSword:register()

-- serpent sword
local serpentSword = Weapon(WEAPON_SWORD)
serpentSword:id(3297)
serpentSword:attack(18)
serpentSword:defense(15,1)
serpentSword:extraElement(8, "earth")
serpentSword:register()

-- poison dagger
local poisonDagger = Weapon(WEAPON_SWORD)
poisonDagger:id(3299)
poisonDagger:attack(18)
poisonDagger:defense(8)
poisonDagger:extraElement(2, "earth")
poisonDagger:register()

-- katana
local katana = Weapon(WEAPON_SWORD)
katana:id(3300)
katana:attack(16)
katana:defense(12,1)
katana:register()

-- broadsword
local broadsword = Weapon(WEAPON_SWORD)
broadsword:id(3301)
broadsword:level(5)
broadsword:attack(26)
broadsword:defense(23)
broadsword:wieldUnproperly(true)
broadsword:vocation("knight", true, true)
broadsword:vocation("elite knight")
broadsword:register()

-- scimitar
local scimitar = Weapon(WEAPON_SWORD)
scimitar:id(3307)
scimitar:attack(19)
scimitar:defense(13,1)
scimitar:register()

-- machete
local machete = Weapon(WEAPON_SWORD)
machete:id(3308)
machete:attack(12)
machete:defense(9)
machete:register()

-- epee
local epee = Weapon(WEAPON_SWORD)
epee:id(3326)
epee:level(30)
epee:attack(37)
epee:defense(23)
epee:wieldUnproperly(true)
epee:register()

-- heavy machete
local heavyMachete = Weapon(WEAPON_SWORD)
heavyMachete:id(3330)
heavyMachete:attack(16)
heavyMachete:defense(10)
heavyMachete:register()

-- pharaoh sword
local pharaohSword = Weapon(WEAPON_SWORD)
pharaohSword:id(3334)
pharaohSword:level(45)
pharaohSword:attack(41)
pharaohSword:defense(23, 2)
pharaohSword:wieldUnproperly(true)
pharaohSword:vocation("knight", true, true)
pharaohSword:vocation("elite knight")
pharaohSword:register()

-- bone sword
local boneSword = Weapon(WEAPON_SWORD)
boneSword:id(3338)
boneSword:attack(13)
boneSword:defense(10)
boneSword:register()

-- djin blade
local djinBlade = Weapon(WEAPON_SWORD)
djinBlade:id(3339)
djinBlade:level(35)
djinBlade:attack(38)
djinBlade:defense(22, 2)
djinBlade:wieldUnproperly(true)
djinBlade:register()

-- templar scytheblade
local templarCytheblade = Weapon(WEAPON_SWORD)
templarCytheblade:id(3345)
templarCytheblade:attack(23)
templarCytheblade:defense(15,1)
templarCytheblade:register()

-- Ron the Ripper's sabre
local ronRipperSabre = Weapon(WEAPON_SWORD)
ronRipperSabre:id(6101)
ronRipperSabre:attack(12)
ronRipperSabre:defense(10)
ronRipperSabre:register()

-- the avenger
local theAvenger = Weapon(WEAPON_SWORD)
theAvenger:id(6527)
theAvenger:level(75)
theAvenger:attack(50)
theAvenger:defense(38)
theAvenger:wieldUnproperly(true)
theAvenger:slotType("two-handed")
theAvenger:vocation("knight", true, true)
theAvenger:vocation("elite knight")
theAvenger:register()

-- demonrage sword
local demonrageSword = Weapon(WEAPON_SWORD)
demonrageSword:id(7382)
demonrageSword:level(60)
demonrageSword:attack(47)
demonrageSword:defense(22)
demonrageSword:wieldUnproperly(true)
demonrageSword:slotType("two-handed")
demonrageSword:vocation("knight", true, true)
demonrageSword:vocation("elite knight")
demonrageSword:register()

-- relic sword
local relicSword = Weapon(WEAPON_SWORD)
relicSword:id(7383)
relicSword:level(50)
relicSword:attack(42)
relicSword:defense(24, 1)
relicSword:wieldUnproperly(true)
relicSword:slotType("two-handed")
relicSword:register()

-- mystic blade
local mysticBlade = Weapon(WEAPON_SWORD)
mysticBlade:id(7384)
mysticBlade:level(60)
mysticBlade:attack(44)
mysticBlade:defense(25, 2)
mysticBlade:wieldUnproperly(true)
mysticBlade:register()

-- crimson sword
local crimsonSword = Weapon(WEAPON_SWORD)
crimsonSword:id(7385)
crimsonSword:level(20)
crimsonSword:attack(28)
crimsonSword:defense(20)
crimsonSword:wieldUnproperly(true)
crimsonSword:register()

-- mercenary sword
local mercenarySword = Weapon(WEAPON_SWORD)
mercenarySword:id(7386)
mercenarySword:level(40)
mercenarySword:attack(43)
mercenarySword:defense(27)
mercenarySword:wieldUnproperly(true)
mercenarySword:vocation("knight", true, true)
mercenarySword:vocation("elite knight")
mercenarySword:register()

-- the justice seeker
local justiceSeeker = Weapon(WEAPON_SWORD)
justiceSeeker:id(7390)
justiceSeeker:level(75)
justiceSeeker:attack(47)
justiceSeeker:defense(24, 3)
justiceSeeker:wieldUnproperly(true)
justiceSeeker:register()

-- thaian sword
local thaianSword = Weapon(WEAPON_SWORD)
thaianSword:id(7391)
thaianSword:level(50)
thaianSword:attack(45)
thaianSword:defense(29)
thaianSword:wieldUnproperly(true)
thaianSword:slotType("two-handed")
thaianSword:vocation("knight", true, true)
thaianSword:vocation("elite knight")
thaianSword:register()

-- dragon slayer
local dragonSlayer = Weapon(WEAPON_SWORD)
dragonSlayer:id(7402)
dragonSlayer:level(45)
dragonSlayer:attack(44)
dragonSlayer:defense(28)
dragonSlayer:wieldUnproperly(true)
dragonSlayer:slotType("two-handed")
dragonSlayer:vocation("knight", true, true)
dragonSlayer:vocation("elite knight")
dragonSlayer:register()

-- berserker
local berserker = Weapon(WEAPON_SWORD)
berserker:id(7403)
berserker:level(65)
berserker:attack(48)
berserker:defense(21)
berserker:wieldUnproperly(true)
berserker:slotType("two-handed")
berserker:vocation("knight", true, true)
berserker:vocation("elite knight")
berserker:register()

-- assassin dagger
local assassinDagger = Weapon(WEAPON_SWORD)
assassinDagger:id(7404)
assassinDagger:level(40)
assassinDagger:attack(40)
assassinDagger:defense(12,-2)
assassinDagger:wieldUnproperly(true)
assassinDagger:register()

-- havoc blade
local havocBlade = Weapon(WEAPON_SWORD)
havocBlade:id(7405)
havocBlade:level(70)
havocBlade:attack(49)
havocBlade:defense(34)
havocBlade:wieldUnproperly(true)
havocBlade:slotType("two-handed")
havocBlade:vocation("knight", true, true)
havocBlade:vocation("elite knight")
havocBlade:register()

-- blacksteel sword
local blacksteelSword = Weapon(WEAPON_SWORD)
blacksteelSword:id(7406)
blacksteelSword:level(35)
blacksteelSword:attack(42)
blacksteelSword:defense(22)
blacksteelSword:wieldUnproperly(true)
blacksteelSword:slotType("two-handed")
blacksteelSword:register()

-- haunted blade
local hauntedBlade = Weapon(WEAPON_SWORD)
hauntedBlade:id(7407)
hauntedBlade:level(30)
hauntedBlade:attack(40)
hauntedBlade:defense(12)
hauntedBlade:wieldUnproperly(true)
hauntedBlade:slotType("two-handed")
hauntedBlade:vocation("knight", true, true)
hauntedBlade:vocation("elite knight")
hauntedBlade:register()

-- wyvern fang
local wyvernFang = Weapon(WEAPON_SWORD)
wyvernFang:id(7408)
wyvernFang:level(25)
wyvernFang:attack(32)
wyvernFang:defense(19)
wyvernFang:wieldUnproperly(true)
wyvernFang:register()

-- bloody edge
local bloodyEdge = Weapon(WEAPON_SWORD)
bloodyEdge:id(7416)
bloodyEdge:level(55)
bloodyEdge:attack(43)
bloodyEdge:defense(21, -3)
bloodyEdge:wieldUnproperly(true)
bloodyEdge:slotType("two-handed")
bloodyEdge:register()

-- runed sword
local runedSword = Weapon(WEAPON_SWORD)
runedSword:id(7417)
runedSword:level(65)
runedSword:attack(45)
runedSword:defense(32, 2)
runedSword:wieldUnproperly(true)
runedSword:register()

-- nightmare blade
local nightmareBlade = Weapon(WEAPON_SWORD)
nightmareBlade:id(7418)
nightmareBlade:level(70)
nightmareBlade:attack(46)
nightmareBlade:defense(23, -3)
nightmareBlade:wieldUnproperly(true)
nightmareBlade:vocation("knight", true, true)
nightmareBlade:vocation("elite knight")
nightmareBlade:register()

-- crystal sword
local crystalSword = Weapon(WEAPON_SWORD)
crystalSword:id(7449)
crystalSword:level(25)
crystalSword:attack(35)
crystalSword:defense(26)
crystalSword:wieldUnproperly(true)
crystalSword:vocation("knight", true, true)
crystalSword:vocation("elite knight")
crystalSword:register()

-- jagged sword
local jaggedSword = Weapon(WEAPON_SWORD)
jaggedSword:id(7774)
jaggedSword:attack(21)
jaggedSword:defense(14,1)
jaggedSword:register()

-- emerald sword
local emeraldSword = Weapon(WEAPON_SWORD)
emeraldSword:id(8102)
emeraldSword:level(100)
emeraldSword:attack(49)
emeraldSword:defense(33, 3)
emeraldSword:wieldUnproperly(true)
emeraldSword:register()

-- the epiphany
local theEpiphany = Weapon(WEAPON_SWORD)
theEpiphany:id(8103)
theEpiphany:level(120)
theEpiphany:attack(50)
theEpiphany:defense(35,3)
theEpiphany:wieldUnproperly(true)
theEpiphany:register()

-- the calamity
local theCalamity = Weapon(WEAPON_SWORD)
theCalamity:id(8104)
theCalamity:level(100)
theCalamity:attack(51)
theCalamity:defense(35)
theCalamity:wieldUnproperly(true)
theCalamity:slotType("two-handed")
theCalamity:vocation("knight", true, true)
theCalamity:vocation("elite knight")
theCalamity:register()

-- pointed rabbitslayer
local pointedRabbitslayer = Weapon(WEAPON_SWORD)
pointedRabbitslayer:id(9375)
pointedRabbitslayer:attack(16)
pointedRabbitslayer:defense(8)
pointedRabbitslayer:register()

-- farmer's avenger
local farmersAvenger = Weapon(WEAPON_SWORD)
farmersAvenger:id(9386)
farmersAvenger:attack(17)
farmersAvenger:defense(7)
farmersAvenger:slotType("two-handed")
farmersAvenger:register()

-- poet's fencing quill
local poetsFencingQuill = Weapon(WEAPON_SWORD)
poetsFencingQuill:id(9387)
poetsFencingQuill:attack(10)
poetsFencingQuill:defense(8)
poetsFencingQuill:register()

-- incredible mumpiz slayer
local incredibleMumpizSlayer = Weapon(WEAPON_SWORD)
incredibleMumpizSlayer:id(9396)
incredibleMumpizSlayer:attack(17)
incredibleMumpizSlayer:defense(14)
incredibleMumpizSlayer:register()

-- sai
local sai = Weapon(WEAPON_SWORD)
sai:id(10389)
sai:level(50)
sai:attack(45)
sai:defense(27)
sai:wieldUnproperly(true)
sai:slotType("two-handed")
sai:vocation("knight", true, true)
sai:vocation("elite knight")
sai:register()

-- zaoan sword
local zaoanSword = Weapon(WEAPON_SWORD)
zaoanSword:id(10390)
zaoanSword:level(55)
zaoanSword:attack(43)
zaoanSword:defense(18, 2)
zaoanSword:wieldUnproperly(true)
zaoanSword:register()

-- twiceslicer
local twiceslicer = Weapon(WEAPON_SWORD)
twiceslicer:id(11657)
twiceslicer:level(58)
twiceslicer:attack(47)
twiceslicer:defense(30)
twiceslicer:wieldUnproperly(true)
twiceslicer:slotType("two-handed")
twiceslicer:vocation("knight", true, true)
twiceslicer:vocation("elite knight")
twiceslicer:register()

-- blade of corruption
local bladeCorruption = Weapon(WEAPON_SWORD)
bladeCorruption:id(11693)
bladeCorruption:level(82)
bladeCorruption:attack(48)
bladeCorruption:defense(29, 2)
bladeCorruption:wieldUnproperly(true)
bladeCorruption:register()

-- wooden sword
local woodenSword = Weapon(WEAPON_SWORD)
woodenSword:id(12673)
woodenSword:attack(7)
woodenSword:defense(7)
woodenSword:register()

-- heavy trident
local heavyTrident = Weapon(WEAPON_SWORD)
heavyTrident:id(12683)
heavyTrident:level(25)
heavyTrident:attack(35)
heavyTrident:defense(17)
heavyTrident:wieldUnproperly(true)
heavyTrident:slotType("two-handed")
heavyTrident:register()

-- shimmer sword
local shimmerSword = Weapon(WEAPON_SWORD)
shimmerSword:id(12731)
shimmerSword:level(40)
shimmerSword:attack(42)
shimmerSword:duration(604800)
shimmerSword:decayTo(0)
shimmerSword:defense(20)
shimmerSword:wieldUnproperly(true)
shimmerSword:vocation("knight", true, true)
shimmerSword:vocation("elite knight")
shimmerSword:register()

-- crystalline sword
local crystallineSword = Weapon(WEAPON_SWORD)
crystallineSword:id(16160)
crystallineSword:level(62)
crystallineSword:attack(47)
crystallineSword:defense(34, 1)
crystallineSword:wieldUnproperly(true)
crystallineSword:register()

-- shiny blade
local shinyBlade = Weapon(WEAPON_SWORD)
shinyBlade:id(16175)
shinyBlade:level(120)
shinyBlade:attack(50)
shinyBlade:defense(35,3)
shinyBlade:wieldUnproperly(true)
shinyBlade:register()

-- mean knight sword
local meanKnightSword = Weapon(WEAPON_DISTANCE)
meanKnightSword:id(17109)
meanKnightSword:attack(14)
meanKnightSword:defense(7)
meanKnightSword:vocation("None", false, false)
meanKnightSword:wieldUnproperly(true)
meanKnightSword:register()

-- ratana
local ratana = Weapon(WEAPON_SWORD)
ratana:id(17812)
broadsword:level(15)
ratana:attack(27)
ratana:defense(19,1)
ratana:wieldUnproperly(true)
ratana:register()

-- crude umbral blade
local crudeUmbralBlade = Weapon(WEAPON_SWORD)
crudeUmbralBlade:id(20064)
crudeUmbralBlade:level(75)
crudeUmbralBlade:attack(48)
crudeUmbralBlade:defense(26,1)
crudeUmbralBlade:wieldUnproperly(true)
crudeUmbralBlade:vocation("knight", true, true)
crudeUmbralBlade:vocation("elite knight")
crudeUmbralBlade:register()

-- umbral blade
local umbralBlade = Weapon(WEAPON_SWORD)
umbralBlade:id(20065)
umbralBlade:level(120)
umbralBlade:attack(50)
umbralBlade:defense(29,2)
umbralBlade:wieldUnproperly(true)
umbralBlade:vocation("knight", true, true)
umbralBlade:vocation("elite knight")
umbralBlade:register()

-- umbral masterblade
local umbralMasterBlade = Weapon(WEAPON_SWORD)
umbralMasterBlade:id(20066)
umbralMasterBlade:level(250)
umbralMasterBlade:attack(52)
umbralMasterBlade:defense(31,3)
umbralMasterBlade:wieldUnproperly(true)
umbralMasterBlade:vocation("knight", true, true)
umbralMasterBlade:vocation("elite knight")
umbralMasterBlade:register()

-- crude umbral slayer
local crudeUmbralSlayer = Weapon(WEAPON_SWORD)
crudeUmbralSlayer:id(20067)
crudeUmbralSlayer:level(75)
crudeUmbralSlayer:attack(51)
crudeUmbralSlayer:defense(29)
crudeUmbralSlayer:wieldUnproperly(true)
crudeUmbralSlayer:slotType("two-handed")
crudeUmbralSlayer:vocation("knight", true, true)
crudeUmbralSlayer:vocation("elite knight")
crudeUmbralSlayer:register()

-- umbral slayer
local umbralSlayer = Weapon(WEAPON_SWORD)
umbralSlayer:id(20068)
umbralSlayer:level(120)
umbralSlayer:attack(52)
umbralSlayer:defense(31)
umbralSlayer:wieldUnproperly(true)
umbralSlayer:slotType("two-handed")
umbralSlayer:vocation("knight", true, true)
umbralSlayer:vocation("elite knight")
umbralSlayer:register()

-- umbral masterslayer
local umbralMasterSlayer = Weapon(WEAPON_SWORD)
umbralMasterSlayer:id(20069)
umbralMasterSlayer:level(250)
umbralMasterSlayer:attack(54)
umbralMasterSlayer:defense(35)
umbralMasterSlayer:wieldUnproperly(true)
umbralSlayer:slotType("two-handed")
umbralMasterSlayer:vocation("knight", true, true)
umbralMasterSlayer:vocation("elite knight")
umbralMasterSlayer:register()

-- cowtana
local cowtana = Weapon(WEAPON_SWORD)
cowtana:id(21177)
cowtana:level(25)
cowtana:attack(34)
cowtana:defense(19,1)
cowtana:wieldUnproperly(true)
cowtana:register()

-- glooth blade
local gloothBlade = Weapon(WEAPON_SWORD)
gloothBlade:id(21179)
gloothBlade:level(75)
gloothBlade:attack(39)
gloothBlade:defense(1)
gloothBlade:wieldUnproperly(true)
iceRapier:extraElement(26, "earth")
iceRapier:charges(2, true) -- showCharges = true
iceRapier:action("removecharge")
gloothBlade:slotType("two-handed")
gloothBlade:vocation("knight", true, true)
gloothBlade:vocation("elite knight")
gloothBlade:register()

-- impaler of the igniter
local impalerIgniter = Weapon(WEAPON_SWORD)
impalerIgniter:id(22760)
impalerIgniter:level(150)
impalerIgniter:attack(26)
impalerIgniter:defense(31,3)
impalerIgniter:wieldUnproperly(true)
impalerIgniter:extraElement(26, "fire")
impalerIgniter:slotType("two-handed")
impalerIgniter:vocation("knight", true, true)
impalerIgniter:vocation("elite knight")
impalerIgniter:register()
