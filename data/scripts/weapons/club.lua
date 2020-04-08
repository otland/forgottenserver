-- Items ordered by ClientID

-- giant smithhammer
local giantSmithhammer = Weapon(WEAPON_CLUB)
giantSmithhammer:id(3208)
giantSmithhammer:attack(24)
giantSmithhammer:defense(14)
giantSmithhammer:register()

-- club
local club = Weapon(WEAPON_CLUB)
club:id(3270)
club:attack(7)
club:defense(7)
club:register()

-- war hammer
local warHammer = Weapon(WEAPON_CLUB)
warHammer:id(3279)
warHammer:level(50)
warHammer:attack(45)
warHammer:defense(10)
warHammer:wieldUnproperly(true)
warHammer:slotType("two-handed")
warHammer:vocation("knight", true, true)
warHammer:vocation("elite knight")
warHammer:register()

-- morning star
local morningStar = Weapon(WEAPON_CLUB)
morningStar:id(3282)
morningStar:attack(25)
morningStar:defense(11)
morningStar:register()

-- mace
local mace = Weapon(WEAPON_CLUB)
mace:id(3286)
mace:attack(16)
mace:defense(11)
mace:register()

-- staff
local staff = Weapon(WEAPON_CLUB)
staff:id(3289)
staff:attack(10)
staff:defense(25)
staff:slotType("two-handed")
staff:register()

-- crowbar
local crowbar = Weapon(WEAPON_CLUB)
crowbar:id(3304)
crowbar:attack(5)
crowbar:defense(6)
crowbar:register()

-- battle hammer
local battleHammer = Weapon(WEAPON_CLUB)
battleHammer:id(3305)
battleHammer:attack(24)
battleHammer:defense(14)
battleHammer:register()

-- thunder hammer
local thunderHammer = Weapon(WEAPON_CLUB)
thunderHammer:id(3309)
thunderHammer:level(85)
thunderHammer:attack(49)
thunderHammer:defense(35,1)
thunderHammer:wieldUnproperly(true)
thunderHammer:register()

-- iron hammer
local ironHammer = Weapon(WEAPON_CLUB)
ironHammer:id(3310)
ironHammer:attack(18)
ironHammer:defense(10)
ironHammer:register()

-- clerical mace
local clericalMace = Weapon(WEAPON_CLUB)
clericalMace:id(3311)
clericalMace:level(20)
clericalMace:attack(28)
clericalMace:defense(15,1)
clericalMace:wieldUnproperly(true)
clericalMace:register()

-- silver mace
local silverMace = Weapon(WEAPON_CLUB)
silverMace:id(3312)
silverMace:level(45)
silverMace:attack(41)
silverMace:defense(30,2)
silverMace:wieldUnproperly(true)
silverMace:register()

-- enchanted staff
local enchantedStaff = Weapon(WEAPON_CLUB)
enchantedStaff:id(3321)
enchantedStaff:attack(39)
enchantedStaff:defense(445)
enchantedStaff:duration(60)
enchantedStaff:decayTo(3289)
enchantedStaff:slotType("two-handed")
enchantedStaff:register()

-- dragon hammer
local dragonHammer = Weapon(WEAPON_CLUB)
dragonHammer:id(3322)
dragonHammer:level(25)
dragonHammer:attack(32)
dragonHammer:defense(20)
dragonHammer:wieldUnproperly(true)
dragonHammer:register()

-- skull staff
local skullStaff = Weapon(WEAPON_CLUB)
skullStaff:id(3324)
skullStaff:level(30)
skullStaff:attack(36)
skullStaff:defense(12,1)
skullStaff:wieldUnproperly(true)
skullStaff:register()

-- light mace
local lightMace = Weapon(WEAPON_CLUB)
lightMace:id(3325)
lightMace:attack(14)
lightMace:defense(9)
lightMace:register()

-- daramian mace
local daramianMace = Weapon(WEAPON_CLUB)
daramianMace:id(3327)
daramianMace:attack(21)
daramianMace:defense(12)
daramianMace:register()

-- hammer of wrath
local hammerOfWrath = Weapon(WEAPON_CLUB)
hammerOfWrath:id(3332)
hammerOfWrath:level(65)
hammerOfWrath:attack(48)
hammerOfWrath:defense(12)
hammerOfWrath:wieldUnproperly(true)
hammerOfWrath:slotType("two-handed")
hammerOfWrath:vocation("knight", true, true)
hammerOfWrath:vocation("elite knight")
hammerOfWrath:register()

-- crystal mace
local crystalMace = Weapon(WEAPON_CLUB)
crystalMace:id(3333)
crystalMace:level(35)
crystalMace:attack(38)
crystalMace:defense(16,1)
crystalMace:wieldUnproperly(true)
crystalMace:register()

-- studded club
local studdedClub = Weapon(WEAPON_CLUB)
studdedClub:id(3336)
studdedClub:attack(9)
studdedClub:defense(8)
studdedClub:register()

-- bone club
local boneClub = Weapon(WEAPON_CLUB)
boneClub:id(3337)
boneClub:attack(12)
boneClub:defense(8)
boneClub:register()

-- heavy mace
local heavyMace = Weapon(WEAPON_CLUB)
heavyMace:id(3340)
heavyMace:level(70)
heavyMace:attack(49)
heavyMace:defense(15)
heavyMace:wieldUnproperly(true)
heavyMace:slotType("two-handed")
heavyMace:vocation("knight", true, true)
heavyMace:vocation("elite knight")
heavyMace:register()

-- arcane staff
local arcaneStaff = Weapon(WEAPON_CLUB)
arcaneStaff:id(3341)
arcaneStaff:level(75)
arcaneStaff:attack(50)
arcaneStaff:defense(30)
arcaneStaff:wieldUnproperly(true)
arcaneStaff:slotType("two-handed")
arcaneStaff:register()

-- lich staff
local lichStaff = Weapon(WEAPON_CLUB)
lichStaff:id(3343)
lichStaff:level(40)
lichStaff:attack(40)
lichStaff:defense(30,2)
lichStaff:wieldUnproperly(true)
lichStaff:register()


-- banana staff
local bananaStaff = Weapon(WEAPON_CLUB)
bananaStaff:id(3348)
bananaStaff:attack(25)
bananaStaff:defense(15,2)
bananaStaff:register()

-- brutetamer's staff
local brutetamersStaff = Weapon(WEAPON_CLUB)
brutetamersStaff:id(7379)
brutetamersStaff:level(25)
brutetamersStaff:attack(35)
brutetamersStaff:defense(15)
brutetamersStaff:wieldUnproperly(true)
brutetamersStaff:slotType("two-handed")
brutetamersStaff:register()

-- mammoth whopper
local mammothWhopper = Weapon(WEAPON_CLUB)
mammothWhopper:id(7381)
mammothWhopper:level(20)
mammothWhopper:attack(30)
mammothWhopper:defense(15)
mammothWhopper:wieldUnproperly(true)
mammothWhopper:register()

-- diamond sceptre
local diamondSceptre = Weapon(WEAPON_CLUB)
diamondSceptre:id(7387)
diamondSceptre:level(25)
diamondSceptre:attack(34)
diamondSceptre:defense(18)
diamondSceptre:wieldUnproperly(true)
diamondSceptre:register()

-- orcish maul
local orcishMaul = Weapon(WEAPON_CLUB)
orcishMaul:id(7392)
orcishMaul:level(35)
orcishMaul:attack(42)
orcishMaul:defense(18)
orcishMaul:wieldUnproperly(true)
orcishMaul:slotType("two-handed")
orcishMaul:register()

-- northern star
local northernStar = Weapon(WEAPON_CLUB)
northernStar:id(7409)
northernStar:level(50)
northernStar:attack(42)
northernStar:defense(15,1)
northernStar:wieldUnproperly(true)
northernStar:register()

-- queen's sceptre
local queensSceptre = Weapon(WEAPON_CLUB)
queensSceptre:id(7410)
queensSceptre:level(55)
queensSceptre:attack(43)
queensSceptre:defense(19)
queensSceptre:wieldUnproperly(true)
queensSceptre:register()

-- abyss hammer
local abyssHammer = Weapon(WEAPON_CLUB)
abyssHammer:id(7414)
abyssHammer:level(60)
abyssHammer:attack(47)
abyssHammer:defense(21)
abyssHammer:wieldUnproperly(true)
abyssHammer:slotType("two-handed")
abyssHammer:vocation("knight", true, true)
abyssHammer:vocation("elite knight")
abyssHammer:register()

-- cranial basher
local abyssHammer = Weapon(WEAPON_CLUB)
abyssHammer:id(7415)
abyssHammer:level(60)
abyssHammer:attack(44)
abyssHammer:defense(20,-2)
abyssHammer:wieldUnproperly(true)
abyssHammer:register()

-- onyx flail
local onyxFlail = Weapon(WEAPON_CLUB)
onyxFlail:id(7421)
onyxFlail:level(65)
onyxFlail:attack(45)
onyxFlail:defense(18)
onyxFlail:wieldUnproperly(true)
onyxFlail:register()

-- jade hammer
local jadeHammer = Weapon(WEAPON_CLUB)
jadeHammer:id(7422)
jadeHammer:level(70)
jadeHammer:attack(46)
jadeHammer:defense(20,1)
jadeHammer:wieldUnproperly(true)
jadeHammer:register()

-- skullcrusher
local skullcrusher = Weapon(WEAPON_CLUB)
skullcrusher:id(7423)
skullcrusher:level(85)
skullcrusher:attack(51)
skullcrusher:defense(20)
skullcrusher:wieldUnproperly(true)
skullcrusher:slotType("two-handed")
skullcrusher:vocation("knight", true, true)
skullcrusher:vocation("elite knight")
skullcrusher:register()

-- lunar staff
local lunarStaff = Weapon(WEAPON_CLUB)
lunarStaff:id(7424)
lunarStaff:level(30)
lunarStaff:attack(40)
lunarStaff:defense(25)
lunarStaff:wieldUnproperly(true)
lunarStaff:slotType("two-handed")
lunarStaff:register()

-- taurus mace
local taurusMace = Weapon(WEAPON_CLUB)
taurusMace:id(7425)
taurusMace:level(20)
taurusMace:attack(30)
taurusMace:defense(18)
taurusMace:wieldUnproperly(true)
taurusMace:slotType("two-handed")
taurusMace:register()

-- amber staff
local amberStaff = Weapon(WEAPON_CLUB)
amberStaff:id(7426)
amberStaff:level(40)
amberStaff:attack(43)
amberStaff:defense(25)
amberStaff:wieldUnproperly(true)
amberStaff:slotType("two-handed")
amberStaff:register()

-- chaos mace
local chaosMace = Weapon(WEAPON_CLUB)
chaosMace:id(7427)
chaosMace:level(45)
chaosMace:attack(44)
chaosMace:defense(21)
chaosMace:wieldUnproperly(true)
chaosMace:slotType("two-handed")
chaosMace:register()

-- bonebreaker
local bonebreaker = Weapon(WEAPON_CLUB)
bonebreaker:id(7428)
bonebreaker:level(55)
bonebreaker:attack(46)
bonebreaker:defense(15)
bonebreaker:wieldUnproperly(true)
bonebreaker:slotType("two-handed")
bonebreaker:vocation("knight", true, true)
bonebreaker:vocation("elite knight")
bonebreaker:register()

-- blessed sceptre
local blessedSceptre = Weapon(WEAPON_CLUB)
blessedSceptre:id(7429)
blessedSceptre:level(75)
blessedSceptre:attack(47)
blessedSceptre:defense(21,3)
blessedSceptre:wieldUnproperly(true)
blessedSceptre:register()

-- dragonbone staff
local dragonboneStaff = Weapon(WEAPON_CLUB)
dragonboneStaff:id(7430)
dragonboneStaff:level(30)
dragonboneStaff:attack(35)
dragonboneStaff:defense(18)
dragonboneStaff:wieldUnproperly(true)
dragonboneStaff:register()

-- demonbone
local demonbone = Weapon(WEAPON_CLUB)
demonbone:id(7431)
demonbone:level(80)
demonbone:attack(48)
demonbone:defense(38,2)
demonbone:wieldUnproperly(true)
demonbone:register()

-- furry club
local furryClub = Weapon(WEAPON_CLUB)
furryClub:id(7432)
furryClub:level(20)
furryClub:attack(31)
furryClub:defense(19,1)
furryClub:wieldUnproperly(true)
furryClub:register()

-- sapphire hammer
local sapphireHammer = Weapon(WEAPON_CLUB)
sapphireHammer:id(7437)
sapphireHammer:level(30)
sapphireHammer:attack(37)
sapphireHammer:defense(18)
sapphireHammer:wieldUnproperly(true)
sapphireHammer:register()

-- hammer of prophecy
local hammerOfProphecy = Weapon(WEAPON_CLUB)
hammerOfProphecy:id(7450)
hammerOfProphecy:level(120)
hammerOfProphecy:attack(52)
hammerOfProphecy:defense(35)
hammerOfProphecy:wieldUnproperly(true)
hammerOfProphecy:vocation("knight", true, true)
hammerOfProphecy:vocation("elite knight")
hammerOfProphecy:register()

-- shadow sceptre
local shadowSceptre = Weapon(WEAPON_CLUB)
shadowSceptre:id(7451)
shadowSceptre:level(35)
shadowSceptre:attack(39)
shadowSceptre:defense(17)
shadowSceptre:wieldUnproperly(true)
shadowSceptre:register()

-- spiked squelcher
local spikedSquelcher = Weapon(WEAPON_CLUB)
spikedSquelcher:id(7452)
spikedSquelcher:level(30)
spikedSquelcher:attack(41)
spikedSquelcher:defense(21)
spikedSquelcher:wieldUnproperly(true)
spikedSquelcher:slotType("two-handed")
spikedSquelcher:vocation("knight", true, true)
spikedSquelcher:vocation("elite knight")
spikedSquelcher:register()

-- dark trinity mace
local darkTrinityMace = Weapon(WEAPON_CLUB)
darkTrinityMace:id(8099)
darkTrinityMace:level(120)
darkTrinityMace:attack(51)
darkTrinityMace:defense(32,-1)
darkTrinityMace:wieldUnproperly(true)
darkTrinityMace:register()

-- obsidian truncheon
local obsidianTruncheon = Weapon(WEAPON_CLUB)
obsidianTruncheon:id(8100)
obsidianTruncheon:level(100)
obsidianTruncheon:attack(50)
obsidianTruncheon:defense(30,2)
obsidianTruncheon:wieldUnproperly(true)
obsidianTruncheon:register()

-- stomper
local stomper = Weapon(WEAPON_CLUB)
stomper:id(8101)
stomper:level(100)
stomper:attack(51)
stomper:defense(20)
stomper:wieldUnproperly(true)
stomper:slotType("two-handed")
stomper:vocation("knight", true, true)
stomper:vocation("elite knight")
stomper:register()

-- glutton's mace
local gluttonsMace = Weapon(WEAPON_CLUB)
gluttonsMace:id(9373)
gluttonsMace:attack(16)
gluttonsMace:defense(10)
gluttonsMace:register()

-- stale bread of ancientness
local staleAncientness = Weapon(WEAPON_CLUB)
staleAncientness:id(9376)
staleAncientness:attack(18)
staleAncientness:defense(8)
staleAncientness:register()

-- scythe of the reaper
local scytheReaper = Weapon(WEAPON_CLUB)
scytheReaper:id(9384)
scytheReaper:attack(16)
scytheReaper:defense(6)
scytheReaper:slotType("two-handed")
scytheReaper:register()

-- club of the fury
local clubFury = Weapon(WEAPON_CLUB)
clubFury:id(9385)
clubFury:attack(16)
clubFury:defense(8)
clubFury:register()

-- drachaku
local drachaku = Weapon(WEAPON_CLUB)
drachaku:id(10391)
drachaku:level(55)
drachaku:attack(46)
drachaku:defense(25)
drachaku:wieldUnproperly(true)
drachaku:slotType("two-handed")
drachaku:vocation("knight", true, true)
drachaku:vocation("elite knight")
drachaku:register()

-- snake god's sceptre
local snakeGodsSceptre = Weapon(WEAPON_CLUB)
snakeGodsSceptre:id(11692)
snakeGodsSceptre:level(82)
snakeGodsSceptre:attack(48)
snakeGodsSceptre:defense(29,3)
snakeGodsSceptre:wieldUnproperly(true)
snakeGodsSceptre:register()

-- giant smithhammer
local giantSmithhammer = Weapon(WEAPON_CLUB)
giantSmithhammer:id(12510)
giantSmithhammer:attack(24)
giantSmithhammer:defense(14)
giantSmithhammer:register()

-- deepling staff
local deeplingStaff = Weapon(WEAPON_CLUB)
deeplingStaff:id(13987)
deeplingStaff:level(38)
deeplingStaff:attack(43)
deeplingStaff:defense(23)
deeplingStaff:wieldUnproperly(true)
deeplingStaff:slotType("two-handed")
deeplingStaff:register()

-- ornate mace
local ornateMace = Weapon(WEAPON_CLUB)
ornateMace:id(14001)
ornateMace:level(90)
ornateMace:attack(49)
ornateMace:defense(24)
ornateMace:wieldUnproperly(true)
ornateMace:register()

-- deepling squelcher
local deeplingSquelcher = Weapon(WEAPON_CLUB)
deeplingSquelcher:id(14250)
deeplingSquelcher:level(48)
deeplingSquelcher:attack(44)
deeplingSquelcher:defense(28)
deeplingSquelcher:wieldUnproperly(true)
deeplingSquelcher:register()

-- mycological mace
local mycologicalMace = Weapon(WEAPON_CLUB)
mycologicalMace:id(16162)
mycologicalMace:level(120)
mycologicalMace:attack(50)
mycologicalMace:defense(31,-3)
mycologicalMace:wieldUnproperly(true)
mycologicalMace:register()

-- life preserver
local lifePreserver = Weapon(WEAPON_CLUB)
lifePreserver:id(17813)
lifePreserver:level(15)
lifePreserver:attack(27)
lifePreserver:defense(19,2)
lifePreserver:wieldUnproperly(true)
lifePreserver:register()

-- swampling club
local swamplingClub = Weapon(WEAPON_CLUB)
swamplingClub:id(17824)
swamplingClub:attack(17)
swamplingClub:defense(12)
swamplingClub:register()

-- pair of iron fists
local pairIronFists = Weapon(WEAPON_CLUB)
pairIronFists:id(17828)
pairIronFists:level(50)
pairIronFists:attack(45)
pairIronFists:defense(17)
pairIronFists:wieldUnproperly(true)
pairIronFists:slotType("two-handed")
pairIronFists:vocation("knight", true, true)
pairIronFists:vocation("elite knight")
pairIronFists:register()

-- spiky club
local spikyClub = Weapon(WEAPON_CLUB)
spikyClub:id(17859)
spikyClub:level(20)
spikyClub:attack(31)
spikyClub:defense(14)
spikyClub:wieldUnproperly(true)
spikyClub:register()

-- crude umbral mace
local crudeUmbralMace = Weapon(WEAPON_CLUB)
crudeUmbralMace:id(20076)
crudeUmbralMace:level(75)
crudeUmbralMace:attack(48)
crudeUmbralMace:defense(22,1)
crudeUmbralMace:wieldUnproperly(true)
crudeUmbralMace:vocation("knight", true, true)
crudeUmbralMace:vocation("elite knight")
crudeUmbralMace:register()	

-- umbral mace
local umbralMace = Weapon(WEAPON_CLUB)
umbralMace:id(20077)
umbralMace:level(120)
umbralMace:attack(50)
umbralMace:defense(26,2)
umbralMace:wieldUnproperly(true)
umbralMace:vocation("knight", true, true)
umbralMace:vocation("elite knight")
umbralMace:register()

-- umbral master mace
local umbralMasterMace = Weapon(WEAPON_CLUB)
umbralMasterMace:id(20078)
umbralMasterMace:level(250)
umbralMasterMace:attack(52)
umbralMasterMace:defense(30,3)
umbralMasterMace:wieldUnproperly(true)
umbralMasterMace:vocation("knight", true, true)
umbralMasterMace:vocation("elite knight")
umbralMasterMace:register()

-- crude umbral hammer
local crudeUmbralHammer = Weapon(WEAPON_CLUB)
crudeUmbralHammer:id(20079)
crudeUmbralHammer:level(75)
crudeUmbralHammer:attack(51)
crudeUmbralHammer:defense(27)
crudeUmbralHammer:wieldUnproperly(true)
crudeUmbralHammer:slotType("two-handed")
crudeUmbralHammer:vocation("knight", true, true)
crudeUmbralHammer:vocation("elite knight")
crudeUmbralHammer:register()

-- umbral hammer
local umbralHammer = Weapon(WEAPON_CLUB)
umbralHammer:id(20080)
umbralHammer:level(120)
umbralHammer:attack(53)
umbralHammer:defense(30)
umbralHammer:wieldUnproperly(true)
umbralHammer:slotType("two-handed")
umbralHammer:vocation("knight", true, true)
umbralHammer:vocation("elite knight")
umbralHammer:register()

-- umbral master hammer
local umbralMasterHammer = Weapon(WEAPON_CLUB)
umbralMasterHammer:id(20081)
umbralMasterHammer:level(250)
umbralMasterHammer:attack(55)
umbralMasterHammer:defense(34)
umbralMasterHammer:wieldUnproperly(true)
umbralMasterHammer:slotType("two-handed")
umbralMasterHammer:vocation("knight", true, true)
umbralMasterHammer:vocation("elite knight")
umbralMasterHammer:register()

-- metal bat
local metalBat = Weapon(WEAPON_CLUB)
metalBat:id(21171)
metalBat:level(55)
metalBat:attack(44)
metalBat:defense(20)
metalBat:wieldUnproperly(true)
metalBat:register()

-- glooth whip
local gloothWhip = Weapon(WEAPON_CLUB)
gloothWhip:id(21172)
gloothWhip:level(25)
gloothWhip:attack(33)
gloothWhip:defense(19)
gloothWhip:wieldUnproperly(true)
gloothWhip:register()

-- moohtant cudgel
local moohtantCudgel = Weapon(WEAPON_CLUB)
moohtantCudgel:id(21173)
moohtantCudgel:level(60)
moohtantCudgel:attack(47)
moohtantCudgel:defense(21)
moohtantCudgel:wieldUnproperly(true)
moohtantCudgel:slotType("two-handed")
moohtantCudgel:register()

-- glooth club
local gloothClub = Weapon(WEAPON_CLUB)
gloothClub:id(21178)
gloothClub:level(75)
gloothClub:attack(39)
gloothClub:defense(1)
gloothClub:wieldUnproperly(true)
gloothClub:extraElement(26, "earth")
gloothClub:charges(2, true)
gloothClub:action("removecharge")
gloothClub:slotType("two-handed")
gloothClub:vocation("knight", true, true)
gloothClub:vocation("elite knight")
gloothClub:register()

-- one hit wonder
local oneHitWonder = Weapon(WEAPON_CLUB)
oneHitWonder:id(21219)
oneHitWonder:level(70)
oneHitWonder:attack(49)
oneHitWonder:defense(22)
oneHitWonder:wieldUnproperly(true)
oneHitWonder:slotType("two-handed")
oneHitWonder:register()

-- ogre klubba
local ogreKlubba = Weapon(WEAPON_CLUB)
ogreKlubba:id(22171)
ogreKlubba:level(50)
ogreKlubba:attack(45)
ogreKlubba:defense(25)
ogreKlubba:wieldUnproperly(true)
ogreKlubba:slotType("two-handed")
ogreKlubba:register()

-- maimer
local maimer = Weapon(WEAPON_CLUB)
maimer:id(22762)
maimer:level(150)
maimer:attack(51)
maimer:defense(32)
maimer:wieldUnproperly(true)
maimer:register()

-- ferumbras' staff (blunt)
local ferumbrasStaff = Weapon(WEAPON_CLUB)
ferumbrasStaff:id(22764)
ferumbrasStaff:level(100)
ferumbrasStaff:attack(20)
ferumbrasStaff:defense(30)
ferumbrasStaff:wieldUnproperly(true)
ferumbrasStaff:register()
