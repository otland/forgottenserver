--[[
Functions:
	getAchievementInfoById(achievement_id)
	getAchievementInfoByName(achievement_name)
	getSecretAchievements()
	getPublicAchievements()
	getAchievements()
	Player:addAchievement(achievement_id/name[, hideMsg])
	Player:removeAchievement(achievement_id/name)
	Player:hasAchievement(achievement_id/name)
	Player:addAllAchievements([hideMsg])
	Player:removeAllAchievements()
	Player:getSecretAchievements()
	Player:getPublicAchievements()
	Player:getAchievements()
	isAchievementSecret(achievement_id/name)
	Player:getAchievementPoints()
	Player:addAchievementProgress()
Storages:
	PlayerStorageKeys.achievementsBase -- base storage
	PlayerStorageKeys.achievementsCounter -- this storage will be used to save the process to obtain the certain achievement
	(Ex: this storage + the id of achievement 'Allowance Collector' to save how many piggy banks has been broken
]]

achievements =
{
	-- 8.6
	[1] = {name = "Allow Cookies?", grade = 1, points = 2, description = "With a perfectly harmless smile you fooled all of those wisecrackers into eating your exploding cookies. Consider a boy or girl scout outfit next time to make the trick even better."},
	[2] = {name = "Allowance Collector", grade = 1, points = 2, secret = true, description = "You certainly have your ways when it comes to acquiring money. Many of them are pink and paved with broken fragments of porcelain."},
	[3] = {name = "Amateur Actor", grade = 1, points = 2, description = "You helped bringing Princess Buttercup, Doctor Dumbness and Lucky the Wonder Dog to life - and will probably dream of them tonight, since you memorised your lines perfectly. What a .. special piece of.. screenplay."},
	[4] = {name = "Animal Activist", grade = 1, points = 2, description = "You have a soft spot for little, weak animals, and you do everything in your power to protect them - even if you probably eat dragons for breakfast."},
	[5] = {name = "Annihilator", grade = 2, points = 5, description = "You've daringly jumped into the infamous Annihilator and survived - taking home fame, glory and your reward."},
	[6] = {name = "Archpostman", grade = 1, points = 3, description = "Delivering letters and parcels has always been a secret passion of yours, and now you can officially put on your blue hat, blow your Post Horn and do what you like to do most. Beware of dogs!"},
	[7] = {name = "Backpack Tourist", grade = 1, points = 1, secret = true, description = "If someone lost a random thing in a random place, you're probably a good person to ask and go find it, even if you don't know what and where."},
	[8] = {name = "Beach Tamer", grade = 1, points = 2, description = "You re-enacted the Taming of the Shrew on a beach setting and proved that you can handle capricious girls quite well. With or without fish tails."},
	[9] = {name = "Bearhugger", grade = 1, points = 1, description = "Warm, furry and cuddly - though that same bear you just hugged would probably rip you into pieces if he had been conscious, he reminded you of that old teddy bear which always slept in your bed when you were still small."},
	[10] = {name = "Blessed!", grade = 1, points = 2, description = "You travelled the world for an almost meaningless prayer - but at least you don't have to do that again and can get a new blessed stake in the blink of an eye."},
	[11] = {name = "Bone Brother", grade = 1, points = 1, description = "You've joined the undead bone brothers - making death your enemy and your weapon as well. Devouring what's weak and leaving space for what's strong is your primary goal."},
	[12] = {name = "Castlemania", grade = 2, points = 5, secret = true, description = "You have an eye for suspicious places and love to read other people's diaries, especially those with vampire stories in it. You're also a dedicated token collector and explorer. Respect!"},
	[13] = {name = "Champion of Chazorai", grade = 2, points = 4, description = "You won the merciless 2 vs. 2 team tournament on the Isle of Strife and wiped out wave after wave of fearsome opponents. Death or victory - you certainly chose the latter."},
	[14] = {name = "Chorister", grade = 1, points = 1, description = "Lalalala... you now know the cult's hymn sung in Liberty Bay"},
	[15] = {name = "Clay Fighter", grade = 1, points = 3, secret = true, description = "You love getting your hands wet and dirty - and covered with clay. Your perfect sculpture of Brog, the raging Titan is your true masterpiece."},
	[16] = {name = "Clay to Fame", grade = 2, points = 6, secret = true, description = "Sculpting Brog, the raging Titan, is your secret passion. Numerous perfect little clay statues with your name on them can be found everywhere around the World."},
	[17] = {name = "Cold as Ice", grade = 2, points = 6, secret = true, description = "Take an ice cube and an obsidian knife and you'll very likely shape something really pretty from it. Mostly cute little mammoths, which are a hit with all the girls."},
	[18] = {name = "Culinary Master", grade = 2, points = 4, description = "Simple hams and bread merely make you laugh. You're the master of the extra-ordinaire, melter of cheese, fryer of bat wings and shaker of shakes. Delicious!"},
	[19] = {name = "Deep Sea Diver", grade = 2, points = 4, secret = true, description = "Under the sea - might not be your natural living space, but you're feeling quite comfortable on the ocean floor. Quara don't scare you anymore and sometimes you sleep with your helmet of the deep still equipped."},
	[20] = {name = "Dread Lord", grade = 3, points = 8, secret = true, description = "You don't care for rules that others set up and shape the world to your liking. Having left behind meaningless conventions and morals, you prize only the power you wield. You're a master of your fate and battle to cleanse the world."},
	[21] = {name = "Efreet Ally", grade = 1, points = 3, description = "Even though the welcomed you only reluctantly and viewed you as \"only a human\" for quite some time, you managed to impress Malor and gained his respect and trade options with the green djinns."},
	[22] = {name = "Elite Hunter", grade = 2, points = 5, description = "You jump at every opportunity for a hunting challenge that's offered to you and carry out those tasks with deadly precision. You're a hunter at heart and a valuable member of the Paw &amp; Fur Society."},
	[23] = {name = "Explorer", grade = 2, points = 4, description = "You've been to places most people don't even know the names of. Collecting botanic, zoologic and ectoplasmic samples is your daily business and you're always prepared to discover new horizons."},
	[24] = {name = "Exquisite Taste", grade = 1, points = 2, secret = true, description = "You love fish - but preferably those caught in the cold north. Even though they're hard to come by you never get tired of picking holes in ice sheets and hanging your fishing rod in."},
	[25] = {name = "Firewalker", grade = 2, points = 4, secret = true, description = "Running barefoot across ember is not for you! You do it the elegant way. Yet, you're kind of drawn to fire and warm surroundings in general - you like it hot!"},
	[26] = {name = "Fireworks in the Sky", grade = 1, points = 2, secret = true, description = "You love the moment right before your rocket takes off and explodes into beautiful colours - not only on new year's eve!"},
	[27] = {name = "Follower of Azerus", grade = 2, points = 4, description = "When you do something, you do it right. You have an opinion and you stand by it - and no one will be able to convince you otherwise. On a sidenote, you're a bit on the brutal and war-oriented side, but that's not a bad thing, is it?"},
	[28] = {name = "Follower of Palimuth", grade = 2, points = 4, description = "You're a peacekeeper and listen to what the small people have to say. You've made up your mind and know who to help and for which reasons - and you do it consistently. Your war is fought with reason rather than weapons."},
	[29] = {name = "Fountain of Life", grade = 1, points = 1, secret = true, description = "You found and took a sip from the Fountain of Life. Thought it didn't grant you eternal life, you feel changed and somehow at peace."},
	[30] = {name = "Friend of the Apes", grade = 2, points = 4, description = "You know Banuta like the back of your hand and are good at destroying caskets and urns. The sight of giant footprints doesn't keep you from exploring unknown areas either."},
	[31] = {name = "Ghostwhisperer", grade = 1, points = 3, description = "You don't hunt them, you talk to them. You know that ghosts might keep secrets that have been long lost among the living, and you're skilled at talking them into revealing them to you."},
	[32] = {name = "Golem in the Gears", grade = 2, points = 4, description = "You're an aspiring mago-mechanic. Science and magic work well together in your eyes - and even though you probably delivered countless wrong charges while working for Telas, you might just have enough knowledge to build your own golem now."},
	[33] = {name = "Green Thumb", grade = 2, points = 4, secret = true, description = "If someone gives you seeds, you usually grow a beautiful plant from it within a few days. You like your house green and decorated with flowers. Probably you also talk to them."},
	[34] = {name = "Greenhorn", grade = 1, points = 2, description = "You wiped out Orcus the Cruel in the Arena of Svargrond. You're still a bit green behind the ears, but there's some great potential."},
	[35] = {name = "Herbicide", grade = 3, points = 8, secret = true, description = "You're one of the brave heroes to face and defeat the mysterious demon oak and all the critters it threw in your face. Wielding your blessed axe no tree dares stand in your way - demonic or not."},
	[36] = {name = "Here, Fishy Fishy!", grade = 1, points = 1, secret = true, description = "Ah, the smell of the sea! Standing at the shore and casting a line is one of your favourite activities. For you, fishingis relaxing - and at the same time, providing easy food. Perfect!"},
	[37] = {name = "High-Flyer", grade = 2, points = 4, secret = true, description = "The breeze in your hair, your fingers clutching the rim of your Carpet - that's how you like to travel. Faster! Higher! And a looping every now and then."},
	[38] = {name = "High Inquisitor", grade = 2, points = 5, description = "You're the one who poses the questions around here, and you know how to get the answers you want to hear. Besides, you're a famous exorcist and slay a few vampires and demons here and there. You and your stake are a perfect team."},
	[39] = {name = "His True Face", grade = 1, points = 3, secret = true, description = "You're one of the few citizens who Armenius chose to actually show his true face to - and he made you fight him. Either that means you're very lucky or very unlucky, but one thing's for sure - it's extremely rare."},
	[40] = {name = "Honorary Barbarian", grade = 1, points = 1, description = "You've hugged bears, pushed mammoths and proved your drinking skills. And even though you have a slight hangover, a partially fractured rib and some greasy hair on your tongue, you're quite proud to call yourself a honorary barbarian from now on."},
	[41] = {name = "Huntsman", grade = 1, points = 2, description = "You're familiar with hunting tasks and have carried out quite a few already. A bright career as hunter for the Paw &amp; Fur society lies ahead!"},
	[42] = {name = "Ice Sculptor", grade = 1, points = 3, secret = true, description = "You love to hang out in cold surroundings and consider ice the best material to be shaped. What a waste to use ice cubes for drinks when you can create a beautiful mammoth statue from it!"},
	[43] = {name = "Interior Decorator", grade = 2, points = 4, secret = true, description = "Your home is your castle - and the furniture in it is just as important. Your friends ask for your advice when decorating their Houses and your probably own every statue, rack and bed there is."},
	[44] = {name = "Jamjam", grade = 2, points = 5, secret = true, description = "When it comes to interracial understanding, you're an expert. You've mastered the language of the Chakoya and made someone really happy with your generosity. Achuq!"},
	[45] = {name = "Jinx", grade = 1, points = 2, secret = true, description = "Sometimes you feel there's a gremlin in there. So many lottery tickets, so many blanks? That's just not fair! Share your misery with the world."},
	[46] = {name = "Just in Time", grade = 1, points = 1, description = "You're a fast runner and are good at delivering wares which are bound to decay just in the nick of time, even if you can't use any means of transportation or if your hands get cold or smelly in the process."},
	[47] = {name = "King Fan", grade = 1, points = 3, description = "You're not sure what it is, but you feel drawn to royalty. Your knees are always a bit grazed from crawling around in front of thrones and you love hanging out in castles. Maybe you should consider applying as a guard?"},
	[48] = {name = "Lord Protector", grade = 3, points = 8, secret = true, description = "You proved yourself - not only in your dreams - and possess a strong and spiritual mind. Your valorous fight against demons and the undead plague has granted you the highest and most respected rank among the Nightmare Knights."},
	[49] = {name = "Lord of the Elements", grade = 2, points = 5, description = "You travelled the surreal realm of the elemental spheres, summoned and slayed the Lord of the Elements, all in order to retrieve neutral matter. And as brave as you were, you couldn't have done it without your team!"},
	[50] = {name = "Lucid Dreamer", grade = 1, points = 2, description = "Dreams - are your reality? Strange visions, ticking clocks, going to bed and waking up somewhere completely else - that was some trip, but you're almost sure you actually did enjoy it."},
	[51] = {name = "Lucky Devil", grade = 2, points = 4, secret = true, description = "That's almost too much luck for one person. If something's really, really rare - it probably falls into your lap sooner or later. Congratulations!"},
	[52] = {name = "Marble Madness", grade = 2, points = 6, secret = true, description = "Your little statues of Godsula have become quite famous around the World and there's few people with similar skills when it comes to shaping marble."},
	[53] = {name = "Marblelous", grade = 1, points = 3, secret = true, description = "You're an aspiring marble sculptor with promising skills - proven by the perfect little Godsula statue you shaped. One day you'll be really famous!"},
	[54] = {name = "Marid Ally", grade = 1, points = 3, description = "You've proven to be a valuable ally to the Marid, and Gabel welcomed you to trade with Haroun and Nah'Bob whenever you want to. Though the Djinn war has still not ended, the Marid can't fail with you on their side."},
	[55] = {name = "Masquerader", grade = 1, points = 3, secret = true, description = "You probably don't know anymore how you really look like - usually when you look into a mirror, some kind of monster stares back at you. On the other hand - maybe that's an improvement?"},
	[56] = {name = "Master Thief", grade = 2, points = 4, description = "Robbing, inviting yourself to VIP parties, faking contracts and pretending to be someone else - you're a jack of all trades when it comes to illegal activities. You take no prisoners, except for the occasional goldfish now and then."},
	[57] = {name = "Master of the Nexus", grade = 2, points = 6, description = "You were able to fight your way through the countless hordes in the Demon Forge. Once more you proved that nothing is impossible."},
	[58] = {name = "Matchmaker", grade = 1, points = 1, description = "You don't believe in romance to be a coincidence or in love at first sight. In fact - love potions, bouquets of flowers and cheesy poems do the trick much better than ever could. Keep those hormones flowing!"},
	[59] = {name = "Mathemagician", grade = 1, points = 1, description = "Sometimes the biggest secrets of life can have a simple solution."},
	[60] = {name = "Ministrel", grade = 1, points = 2, secret = true, description = "You can handle any music instrument you're given - and actually manage to produce a pleasant sound with it. You're a welcome guest and entertainer in most taverns."},
	[61] = {name = "Nightmare Knight", grade = 1, points = 1, description = "You follow the path of dreams and that of responsibility without self-centered power. Free from greed and selfishness, you help others without expecting a reward."},
	[62] = {name = "Party Animal", grade = 1, points = 1, secret = true, description = "Oh my god, it's a paaaaaaaaaaaarty! You're always in for fun, friends and booze and love being the center of attention. There's endless reasons to celebrate! Woohoo!"},
	[63] = {name = "Passionate Kisser", grade = 1, points = 3, description = "For you, a kiss is more than a simple touch of lips. You kiss maidens and deadbeats alike with unmatched affection and faced death and rebirth through the kiss of the banshee queen. Lucky are those who get to share such an intimate moment with you!"},
	[64] = {name = "Perfect Fool", grade = 1, points = 3, description = "You love playing jokes on others and tricking them into looking a little silly. Wagging tongues say that the moment of realisation in your victims' eyes is the reward you feed on, but you're probably just kidding and having fun with them... right??"},
	[65] = {name = "Poet Laureate", grade = 1, points = 2, secret = true, description = "Poems, verses, songs and rhymes you've recited many times. You have passed the cryptic door, raconteur of ancient lore. Even elves you've left impressed, so it seems you're truly blessed."},
	[66] = {name = "Polisher", grade = 2, points = 4, secret = true, description = "If you see a rusty item, you can't resist polishing it. There's always a little flask of rust remover in your inventory - who knows, there might be a golden armor beneath all that dirt!"},
	[67] = {name = "Potion Addict", grade = 2, points = 4, secret = true, description = "Your local magic trader considers you one of his best customers - you usually buy large stocks of potions so you won't wake up in the middle of the night craving for more. Yet, you always seem to run out of them too fast. Cheers!"},
	[68] = {name = "Quick as a Turtle", grade = 1, points = 2, secret = true, description = "There... is... simply... no... better... way - than to travel on the back of a turtle. At least you get to enjoy the beautiful surroundings of Laguna."},
	[69] = {name = "Razing!", grade = 3, points = 7, secret = true, description = "People with sharp canine teeth better beware of you, especially at nighttime, or they might find a stake between their ribs. You're a merciless vampire hunter and have gathered numerous tokens as proof."},
	[70] = {name = "Recognised Trader", grade = 1, points = 3, description = "You're a talented merchant who's able to handle wares with care, finds good offers and digs up rares every now and then. Never late to complete an order, you're a reliable trader - at least in Rashid's eyes."},
	[71] = {name = "Rockstar", grade = 1, points = 3, secret = true, description = "Music just comes to you naturally. You feel comfortable on any stage, at any time, and secretly hope that someday you will be able to defeat your foes by playing music only. Rock on!"},
	[72] = {name = "Ruthless", grade = 2, points = 5, description = "You've touched all thrones of The Ruthless Seven and absorbed some of their evil spirit. It may have changed you forever."},
	[73] = {name = "Scrapper", grade = 1, points = 3, description = "You put out the Spirit of Fire's flames in the arena of Svargrond. Arena fights are for you - fair, square, with simple rules and one-on-one battles."},
	[74] = {name = "Sea Scout", grade = 1, points = 2, description = "Not even the hostile underwater environment stops you from doing your duty for the Explorer Society. Scouting the Quara realm is a piece of cake for you."},
	[75] = {name = "Secret Agent", grade = 1, points = 1, description = "Pack your spy gear and get ready for some dangerous missions in service of a secret agency. You've shown you want to - but can you really do it? Time will tell."},
	[76] = {name = "Shell Seeker", grade = 1, points = 3, secret = true, description = "You found a hundred beautiful pearls in large sea shells. By now that necklace should be finished - and hopefully you didn't get your fingers squeezed too often during the process."},
	[77] = {name = "Ship's Kobold", grade = 2, points = 4, secret = true, description = "You've probably never gotten seasick in your life - you love spending your free time on the ocean and covered quite a lot of miles with ships. Aren't you glad you didn't have to swim all that?"},
	[78] = {name = "Steampunked", grade = 1, points = 2, secret = true, description = "Travelling with the dwarven steamboats through the underground rivers is your preferred way of crossing the lands. No pesky seagulls, and good beer on board!"},
	[79] = {name = "Superstitious", grade = 1, points = 2, secret = true, description = "Fortune tellers and horoscopes guide you through your life. And you probably wouldn't dare going on a big game hunt without your trusty voodoo skull giving you his approval for the day."},
	[80] = {name = "Talented Dancer", grade = 1, points = 1, description = "You're a lord or lady of the dance - and not afraid to use your skills to impress tribal gods. One step to the left, one jump to the right, twist and shout!"},
	[81] = {name = "Territorial", grade = 1, points = 1, secret = true, description = "Your map is your friend - always in your back pocket and covered with countless marks of interesting and useful locations. One could say that you might be lost without it - but luckily there's no way to take it from you."},
	[82] = {name = "The Milkman", grade = 1, points = 2, description = "Who's the milkman? You are!"},
	[83] = {name = "Top AVIN Agent", grade = 2, points = 4, description = "You've proven yourself as a worthy member of the 'family' and successfully carried out numerous spy missions for your 'uncle' to support the Venorean traders and their goals."},
	[84] = {name = "Top CGB Agent", grade = 2, points = 4, description = "Girl power! Whether you're female or not, you've proven absolute loyalty and the willingness to put your life at stake for the girls brigade of Carlin."},
	[85] = {name = "Top TBI Agent", grade = 2, points = 4, description = "Conspiracies and open secrets are your daily bread. You've shown loyalty to the Thaian crown through your courage when facing enemies and completing spy missions. You're an excellent field agent of the TBI."},
	[86] = {name = "Turncoat", grade = 2, points = 4, secret = true, description = "You served Yalahar - but you didn't seem so sure whom to believe on the way. Both Azerus and Palimuth had good reasons for their actions, and thus you followed your gut instinct in the end, even if you helped either of them. May Yalahar prosper!"},
	[87] = {name = "Vanity", grade = 1, points = 3, secret = true, description = "Aren't you just perfectly, wonderfully, beautifully gorgeous? You can't pass a mirror without admiring your looks. Or maybe doing a quick check whether something's stuck in your teeth, perhaps?"},
	[88] = {name = "Vive la Resistance", grade = 1, points = 2, description = "You've always been a rebel - admit it! Supplying prisoners, caring for outcasts, stealing from the rich and giving to the poor - no wait, that was another story."},
	[89] = {name = "Warlord of Svargrond", grade = 2, points = 5, description = "You sent the Obliverator into oblivion in the arena of Svargrond and defeated nine other dangerous enemies on the way. All hail the Warlord of Svargrond!"},
	[90] = {name = "Waverider", grade = 1, points = 2, secret = true, description = "One thing's for sure: You definitely love swimming. Hanging out on the beach with your friends, having ice cream and playing beach ball is splashingly good fun!"},
	[91] = {name = "Wayfarer", grade = 1, points = 3, secret = true, description = "Dragon dreams are golden."},
	[92] = {name = "Worm Whacker", grade = 1, points = 1, secret = true, description = "Weehee! Whack those worms! You sure know how to handle a big hammer."},

	-- 8.61
	[93] = {name = "Cocoon of Doom", grade = 1, points = 3, secret = true, description = "You helped bringing Devovorga's dangerous tentacles and her humongous cocoon down - not stopping her transformation, but ultimately completing a crucial step to her death."},
	[94] = {name = "Daring Trespasser", grade = 1, points = 3, secret = true, description = "You've entered the lair of Devovorga and joined the crew trying to take her down - whether crowned with success or not doesn't matter, but they can't blame you for not trying!"},
	[95] = {name = "Devovorga's Nemesis", grade = 2, points = 5, secret = true, description = "One special hero among many. This year - it was you. Devovorga withdrew in a darker realm because she could not withstand your power - and that of your comrades. Time will tell if the choice you made was good - but for now, it saved your world."},
	[96] = {name = "I Did My Part", grade = 1, points = 2, secret = true, description = "Your world is lucky to have you! You don't hesitate to jump in and help when brave heroes are called to save the world."},
	[97] = {name = "Notorious Worldsaver", grade = 3, points = 8, secret = true, description = "You're in the front line when it comes to saving your world or taking part in social events. Whether you do it noticed or unnoticed by the people, your world can rely on you to dutifully do your part to make it a better place for everyone."},
	[98] = {name = "Slayer of Anmothra", grade = 1, points = 2, secret = true, description = "Souls are like butterflies. The black soul of a living weapon yearning to strike lies shattered beneath your feet."},
	[99] = {name = "Slayer of Chikhaton", grade = 1, points = 2, secret = true, description = "Power lies in the will of her who commands it. You fought it with full force - and were stronger."},
	[100] = {name = "Slayer of Irahsae", grade = 1, points = 2, secret = true, description = "Few things equal the wild fury of a trapped and riven creature. You were a worthy opponent."},
	[101] = {name = "Slayer of Phrodomo", grade = 1, points = 2, secret = true, description = "Blind hatred took physical form, violently rebelling against the injustice it was born into. You were not able to bring justice - but at least temporary peace."},
	[102] = {name = "Slayer of Teneshpar", grade = 1, points = 2, secret = true, description = "The forbidden knowledge of aeons was never meant to invade this world. You silenced its voice before it could be made heard."},
	[103] = {name = "Teamplayer", grade = 1, points = 2, secret = true, description = "You don't consider yourself too good to do the dirty work while someone else might win the laurels for killing Devovorga. They couldn't do it without you!"},

	-- 8.62
	[104] = {name = "Alumni", grade = 2, points = 6, description = "You're considered a first-rate graduate of the Magic Academy in Edron due to your pioneering discoveries and successful studies in the field of experimental magic and spell development. Ever considered teaching the Armageddon spell?"},
	[105] = {name = "Aristocrat", grade = 2, points = 4, description = "You begin your day by bathing in your pot of gold and you don't mind showing off your wealth while strolling the streets in your best clothes - after all it's your hard-earned money! You prefer to be addressed with 'Your Highness'."},
	[106] = {name = "Bad Timing", grade = 1, points = 2, secret = true, description = "Argh! Not now! How is it that those multifunctional tools never fail when you're using them for something completely trivial like squeezing juice, but mess up when you desperately need to climb up a rope spot with a fire-breathing dragon chasing you?"},
	[107] = {name = "Berserker", grade = 1, points = 3, description = "RAWR! Strength running through your body, your heart racing faster and adrenaline fueling your every weapon swing. All in a little bottle. No refund for destroyed furniture. For further questions consult your healer or potion dealer."},
	[108] = {name = "Bluebarian", grade = 1, points = 2, secret = true, description = "You live the life of hunters and gatherers. Well, especially that of a gatherer, and especially of one who gathers lots of blueberries. Have you checked the colour of your tongue lately?"},
	[109] = {name = "Brutal Politeness", grade = 2, points = 6, description = "What is best in life? To crush your enemies. To see them driven before you. And to maybe have a nice cup of tea afterwards."},
	[110] = {name = "Commitment Phobic", grade = 1, points = 2, secret = true, description = "Longterm relationships are just not for you. And each time you think you're in love, you're proven wrong shortly afterwards. Or maybe you just end up with the wrong lover each time - exploited and betrayed. Staying single might just be better."},
	[111] = {name = "Cookie Monster", grade = 1, points = 1, secret = true, description = "You can easily be found by anyone if they just follow the cookie crumb trail. And for you, true love means to give away your last cookie."},
	[112] = {name = "Cursed!", grade = 1, points = 3, secret = true, description = "The wrath of the Noxious Spawn - you accidentally managed to incur it. Your days are counted and your death inevitable. Sometime. Someplace."},
	[113] = {name = "Demonbane", grade = 2, points = 6, description = "You don't carry that stake just for decoration - you're prepared to use it. Usually you're seen hightailing through the deepest dungeons leaving a trail of slain demons. Whoever dares stand in your way should prepare to die."},
	[114] = {name = "Demonic Barkeeper", grade = 1, points = 3, description = "Thick, red - shaken, not stirred - and with a straw in it: that's the way you prefer your demon blood. Served with an onion ring, the subtle metallic aftertaste is almost not noticeable. Beneficial effects on health or mana are welcome."},
	[115] = {name = "The Snowman", grade = 1, points = 1, description = "You love the winter. Fully equipped with scarf and gloves, you like to have fun outside while building lots of snowmen with your friends. Snowball fight, anyone?"},
	[116] = {name = "Do Not Disturb", grade = 1, points = 1, secret = true, description = "Urgh! Close the windows! Shut out the sun rearing its ugly yellow head, shut out the earsplitting laughter of your neighbour's corpulent children. Ahhh. Embrace sweet darkness and silence."},
	[117] = {name = "Exemplary Citizen", grade = 2, points = 4, description = "Every city should be proud to call someone like you its inhabitant. You're keeping the streets clean and help settling the usual disputes in front of the depot. Also, you probably own a cat and like hiking."},
	[118] = {name = "Fool at Heart", grade = 1, points = 3, description = "And remember: Never try to teach a pig to sing. It wastes your time and annoys the pig."},
	[119] = {name = "Free Items!", grade = 1, points = 3, secret = true, description = "Yay! Finders keepers, losers weepers! Who cares where all that stuff came from and if you had to crawl through garbage piles to get it? It's FREE!"},
	[120] = {name = "Godslayer", grade = 2, points = 4, description = "You have defeated the Snake God's incarnations and, with a final powerful swing of the snake sceptre, cut off his life force supply. The story of power, deceit and corruption has come to an end - or... not?"},
	[121] = {name = "Gold Digger", grade = 2, points = 4, secret = true, description = "Hidden treasures below the sand dunes of the desert - you have a nose for finding them and you know where to dig. They might not make you filthy rich, but they're shiny and pretty anyhow."},
	[122] = {name = "Happy Farmer", grade = 1, points = 1, secret = true, description = "Scythe swung over your shoulder, sun burning down on your back - you are a farmer at heart and love working in the fields. Or then again maybe you just create fancy crop circles to scare your fellow men."},
	[123] = {name = "Heartbreaker", grade = 1, points = 1, secret = true, description = "Trust? Love? Faithfulness? Pah! Antiquated sentiments. As long as you have fun, you do not mind stepping on lots of hearts. Preferably while wearing combat boots."},
	[124] = {name = "Homebrewed", grade = 1, points = 1, secret = true, description = "Yo-ho-ho and a bottle of rum - homebrewed, of course, made from handpicked and personally harvested sugar cane plants. Now, let it age in an oak barrel and enjoy it in about 10 years. Or for the impatient ones: Let's have a paaaarty right now!"},
	[125] = {name = "Hunting with Style", grade = 2, points = 6, description = "At daytime you can be found camouflaged in the woods laying traps or chasing big game, at night you're sitting by the campfire and sharing your hunting stories. You eat what you hunted and wear what you skinned. Life could go on like that forever."},
	[126] = {name = "I Need a Hug", grade = 1, points = 2, description = "You and your stuffed furry friends are inseparable, and you're not ashamed to take them to bed with you - who knows when you will wake up in the middle of the night in dire need of a cuddle?"},
	[127] = {name = "In Shining Armor", grade = 2, points = 6, description = "With edged blade and fully equipped in a sturdy full plate armor, you charge at your enemies with both strength and valour. There's always a maiden to save and a dragon to slay for you."},
	[128] = {name = "Joke's on You", grade = 1, points = 1, secret = true, description = "Well - the contents of that present weren't quite what you expected. With friends like these, who needs enemies?"},
	[129] = {name = "Keeper of the Flame", grade = 1, points = 2, secret = true, description = "One of the Lightbearers. One of those who helped to keep the basins burning and worked together against the darkness. The demonic whispers behind the thin veil between the worlds - they were silenced again thanks to your help."},
	[130] = {name = "Let the Sunshine In", grade = 1, points = 1, secret = true, description = "Rise and shine! It's a beautiful new day - open your windows, feel the warm sunlight, watch the birds singing on your windowsill and care for your plants. What reason is there not to be happy?"},
	[131] = {name = "Life on the Streets", grade = 2, points = 4, description = "You're a beggar, homeless, wearing filthy and ragged clothes. But that doesn't mean you have to beg anyone for stuff - and you still kept your pride. Fine feathers do not necessarily make fine birds - what's under them is more important."},
	[132] = {name = "Make a Wish", grade = 1, points = 1, secret = true, description = "But close your eyes and don't tell anyone what you wished for, or it won't come true!"},
	[133] = {name = "Master of War", grade = 2, points = 6, description = "You're not afraid to show your colours in the heat of battle. Enemies fear your lethal lance and impenetrable armor. The list of the wars you've won is impressive. Hail and kill!"},
	[134] = {name = "Mastermind", grade = 1, points = 3, description = "You feel you could solve the hardest riddles within a minute or so. Plus, there's a nice boost on your spell damage. All in a little bottle. Aftereffects - feeling slightly stupid. For further questions consult your healer or potion dealer."},
	[135] = {name = "Mister Sandman", grade = 1, points = 2, secret = true, description = "Tired... so tired... curling up in a warm and cosy bed seems like the perfect thing to do right now. Sweet dreams!"},
	[136] = {name = "Modest Guest", grade = 1, points = 1, secret = true, description = "You don't need much to sleep comfortably. A pile of straw and a roof over your head - with the latter being completely optional - is quite enough to relax. You don't even mind the rats nibbling on your toes."},
	[137] = {name = "Mutated Presents", grade = 1, points = 1, secret = true, description = "Muahahaha it's a... mutated pumpkin! After helping to take it down - you DID help, didn't you? - you claimed your reward and got a more or less weird present. Happy Halloween!"},
	[138] = {name = "Natural Sweetener", grade = 1, points = 1, secret = true, description = "Liberty Bay is the perfect hangout for you and harvesting sugar cane quite a relaxing leisure activity. Would you like some tea with your sugar, hon?"},
	[139] = {name = "Nightmare Walker", grade = 2, points = 6, description = "You do not fear nightmares, you travel in them - facing countless horrors and fighting the fate they're about to bring. Few believe the dark prophecies you bring back from those dreams, but those who do fight alongside you as Nightmare Knights."},
	[140] = {name = "Nothing Can Stop Me", grade = 1, points = 1, secret = true, description = "You laugh at unprepared adventurers stuck in high grass or rush wood. Or maybe you actually do help them out. They call you... 'Machete'."},
	[141] = {name = "Number of the Beast", grade = 1, points = 2, description = "Six. Six. Six."},
	[142] = {name = "Of Wolves and Bears", grade = 2, points = 6, description = "One with nature, one with wildlife. Raw and animalistic power, sharpened senses, howling on the highest cliffs and roaring in the thickest forests - that's you."},
	[143] = {name = "One Thousand and One", grade = 2, points = 6, description = "You feel at home under the hot desert sun with sand between your toes, and your favourite means of travel is a flying carpet. Also, you can probably do that head isolation dance move."},
	[144] = {name = "Oops", grade = 1, points = 2, secret = true, description = "So much for your feathered little friend! Maybe standing in front of the birdcage, squeezing its neck and shouting 'Sing! Sing! Sing!' was a little too much for it?!"},
	[145] = {name = "Out in the Snowstorm", grade = 2, points = 4, description = "Snow heaps and hailstorms can't keep you from where you want to go. You're perfectly equipped for any expedition into the perpetual ice and know how to keep your feet warm. If you're a woman, that's quite an accomplishment, too."},
	[146] = {name = "Peazzekeeper", grade = 2, points = 6, description = "You're a humble warrior who doesn't need wealth or specialised equipment for travelling and fighting. You feel at home in the northern lands of Zao and did your part in fighting its corruption."},
	[147] = {name = "Piece of Cake", grade = 1, points = 1, description = "Life can be so easy with the right cake at the right time - and you mastered baking many different ones, so you should be prepared for almost everything life decides to throw at you."},
	[148] = {name = "Ritualist", grade = 2, points = 6, description = "You could be the author of the magnum opus 'How to Summon the Ultimate Beast from the Infernal Depths, Volume I'. Or, if your mind and heart are pure, you rather summon beings to help others. Or maybe just a little cat to have someone to cuddle."},
	[149] = {name = "Rock Me to Sleep", grade = 1, points = 1, secret = true, description = "Sleeping - you do it with style. You're chilling in your hammock, listening to the sound of the birds and crickets as you slowly drift away into the realm of dreams."},
	[150] = {name = "Rocket in Pocket", grade = 1, points = 1, secret = true, description = "Either you are not a fast learner or you find some pleasure in setting yourself on fire. Or you're just looking for a fancy title. In any case, you should know that passing gas during your little donkey experiments is not recommended."},
	[151] = {name = "Rollercoaster", grade = 1, points = 1, description = "Up and down and up and down... and then the big looping! Wait - they don't build loopings in Kazordoon. But ore wagon rides are still fun!"},
	[152] = {name = "Santa's Li'l Helper", grade = 1, points = 2, secret = true, description = "Christmas is your favourite time of the year, and boy, do you love presents. Buy some nice things for your friends, hide them away until - well, until you decide to actually unwrap them rather yourself."},
	[153] = {name = "Sharpshooter", grade = 1, points = 3, description = "Improved eyesight, arrows and bolts flying at the speed of light and pinning your enemies with extra damage. All in a little bottle. No consumption of carrots required. For further questions consult your healer or potion dealer."},
	[154] = {name = "Skull and Bones", grade = 2, points = 6, description = "Wearing the insignia and dark robes of the Brotherhood of Bones you roam the lands spreading fear and pain, creating new soldiers for the necromantic army which is about to rise soon. Hail the Brotherhood."},
	[155] = {name = "Slim Chance", grade = 1, points = 1, description = "Okay, let's face it - as long as you believe it could potentially lead you to the biggest treasure ever, you won't let go of that map, however fishy it might look. There must be a secret behind all of this!"},
	[156] = {name = "Swashbuckler", grade = 2, points = 6, description = "Ye be a gentleman o' fortune, fightin' and carousin' on the high seas, out fer booty and lassies! Ye no be answerin' to no man or blasted monarchy and yer life ain't fer the lily-livered. Aye, matey!"},
	[157] = {name = "Sweet Tooth", grade = 1, points = 2, secret = true, description = "The famous 'Ode to a Molten Chocolate Cake' was probably written by you. Spending a rainy afternoon in front of the chimney, wrapped in a blanket while indulging in cocoa delights sounds just like something you'd do. Enjoy!"},
	[158] = {name = "Swift Death", grade = 2, points = 6, description = "Stealth kills and backstabbing are you specialty. Your numerous victims are usually unaware of their imminent death, which you bring to them silently and swiftly. Everything is permitted."},
	[159] = {name = "The Cake's the Truth", grade = 1, points = 1, secret = true, description = "And anyone claiming otherwise is a liar."},
	[160] = {name = "The Day After", grade = 1, points = 2, secret = true, description = "Uhm... who's that person who you just woke up beside? Broken cocktail glasses on the floor, flowers all over the room, and why the heck are you wearing a ring? Yesterday must have been a long, weird day..."},
	[161] = {name = "The Undertaker", grade = 1, points = 2, secret = true, description = "You and your shovel - a match made in heaven. Or hell, for that matter. Somewhere down below in any case. You're magically attracted by stone piles and love to open them up and see where those holes lead you. Good biceps as well."},
	[162] = {name = "True Lightbearer", grade = 2, points = 5, secret = true, description = "You're one of the most dedicated Lightbearers - without you, the demons would have torn the veil between the worlds for sure. You've lit each and every basin, travelling high and low, pushing back the otherworldly forces. Let there be light!"},
	[163] = {name = "Warlock", grade = 2, points = 6, description = "You're proficient in the darker ways of magic and are usually found sitting inside a circle of candles and skulls muttering unspeakable words. Don't carry things too far or the demons might come get you."},
	[164] = {name = "Way of the Shaman", grade = 2, points = 6, description = "Shaking your rattle and dancing around the fire to jungle drums sounds like something you like doing. Besides, dreadlocks are a convenient way to wear your hair - no combing required!"},
	[165] = {name = "Wild Warrior", grade = 2, points = 6, description = "Valour is for weaklings - it doesn't matter how you win the battle, as long as you're victorious. Thick armor would just hinder your movements, thus you keep it light and rely on speed and skill instead of hiding in an uncomfortable shell."},
	[166] = {name = "With a Cherry on Top", grade = 1, points = 1, secret = true, description = "You like your cake soft, with fruity bits and a nice sugar icing. And you prefer to make them by yourself. Have you ever considered opening a bakery? You must be really good by now!"},
	[167] = {name = "Yalahari of Power", grade = 1, points = 3, description = "You defend Yalahar with brute force and are ready to lead it into a glorious battle, if necessary. Thanks to you, Yalahar will be powerful enough to stand up against any enemy."},
	[168] = {name = "Yalahari of Wisdom", grade = 1, points = 3, description = "Your deeds for Yalahar are usually characterised by deep insight and thoughtful actions. Thanks to you, Yalahar might have a chance to grow peacefully and with happy people living in it."},

	-- 8.7
	[169] = {name = "Afraid of no Ghost!", grade = 1, points = 2, description = "You passed their test and helped the Spirithunters testing equipment, researching the supernatural and catching ghosts - it's you they're gonna call."},
	[170] = {name = "Ashes to Dust", grade = 2, points = 4, secret = true, description = "Staking vampires and demons has almost turned into your profession. You make sure to gather even the tiniest amount of evil dust particles. Beware of silicosis."},
	[171] = {name = "Baby Sitter", grade = 1, points = 1, secret = true, description = "You have cheered up a demon baby and returned it to its mother. A quick count of your fingers will reveal if you made it through unharmed."},
	[172] = {name = "Banebringers' Bane", grade = 1, points = 2, secret = true, description = "You sacrificed a lot of ingredients to create the protective brew of the witches and played a significant part in the efforts to repel the dreaded banebringers. The drawback is that even the banebringers may take notice of you ..."},
	[173] = {name = "Berry Picker", grade = 2, points = 4, secret = true, description = "The Combined Magical Winterberry Society hereby honours continued selfless dedication and extraordinary efforts in the Annual Autumn Vintage."},
	[174] = {name = "Bunny Slipped", grade = 1, points = 2, description = "Indeed, you have a soft spot for rabbits. Maybe the rabbits you saved today will be the rabbits that will save you tomorrow. When you are really hungry."},
	[175] = {name = "Cake Conqueror", grade = 1, points = 1, description = "You have bravely stepped onto the cake isle. Is there any more beautiful, tasty place to be in the whole world?"},
	[176] = {name = "Dark Voodoo Priest", grade = 1, points = 2, secret = true, description = "Sinister curses, evil magic - you don't shy away from punishing others by questionable means. Someone just gave you a strange look - now where's that needle again?"},
	[177] = {name = "Extreme Degustation", grade = 1, points = 2, secret = true, description = "Almost all the plants you tested for Chartan in Zao where inedible - you tasted them all, yet you're still standing! You should really get some fresh air now, though."},
	[178] = {name = "Fire Devil", grade = 1, points = 3, secret = true, description = "To keep the witches' fire burning, you trashed a lot of the wood the bane bringers animated. Some might find your fascination for fire ... disturbing."},
	[179] = {name = "Fire Lighter", grade = 1, points = 1, secret = true, description = "You have helped to keep the witches fire burning. Just watch your fingers, it's hot!"},
	[180] = {name = "Ghost Sailor", grade = 1, points = 1, secret = true, description = "You have sailed the nether seas with the Ghost Captain. Despite the perils, you and your fellow crewmen have braved the challenge."},
	[181] = {name = "Guinea Pig", grade = 1, points = 2, description = "True scientists know their equipment. Testing new inventions is essential daily work for any hard working researcher. You showed no fear and took all the new equipment from Spectulus and Sinclair for a spin."},
	[182] = {name = "Hidden Powers", grade = 1, points = 2, description = "You've discovered the Ancients' hidden powers - from now on, they will aid you in your adventures."},
	[183] = {name = "Honorary Witch", grade = 2, points = 4, secret = true, description = "Your efforts in fighting back the banebringers has not gone unnoticed. You are a legend amongst the witches and your name is whispered with awe and admiration."},
	[184] = {name = "I Like it Fancy", grade = 1, points = 1, secret = true, description = "You definitely know how to bring out the best in your furniture and decoration pieces. Beautiful."},
	[185] = {name = "Master Shapeshifter", grade = 1, points = 2, secret = true, description = "You have mastered Kuriks challenge in all possible shapes."},
	[186] = {name = "Merry Adventures", grade = 1, points = 2, description = "You went into the forest, met Rottin Wood and the Married Men and helped them out in their camp. Oh, and don't worry about those merchants. They won't dare mentioning the strangely large sums of gold they actually possessed which are missing now."},
	[187] = {name = "Nanny from Hell", grade = 1, points = 3, secret = true, description = "You have cheered up a bunch of demon babies and returned them to their mother. Don't bother the burn marks, don't bother the strains of grey hair, don't bother the nights you wake up screaming. It was worth it ... probably ... somehow."},
	[188] = {name = "Natural Born Cowboy", grade = 1, points = 1, secret = true, description = "Oh, the joy of riding! You've just got your very first own mount. Conveniently enough you don't even need stables, but can summon it any time you like."},
	[189] = {name = "Nether Pirate", grade = 1, points = 3, secret = true, description = "Not fearing death or ghosts you have traveled with the ghost captain several times and are a seasoned traveler of the netherworld. The dead and the living whisper about your exploits with appreciation."},
	[190] = {name = "Nomad Soul", grade = 1, points = 2, secret = true, description = "Home is where your current favourite hunting ground is, and though you might hold certain places more dear than others you never feel attached enough to really stay in one city for long. Pack all your stuff - it's time to move on again."},
	[191] = {name = "Petrologist", grade = 1, points = 2, secret = true, description = "Stones have always fascinated you. So has the chance of finding something really precious inside one of them. Statistically you should've discovered a few nice treasures by now. But then again, most statistics are overriden by Mother Disfortune."},
	[192] = {name = "Pyromaniac", grade = 2, points = 4, secret = true, description = "Love ... fire! So ... shiny! Must ... buuuurrrn!"},
	[193] = {name = "Safely Stored Away", grade = 1, points = 2, secret = true, description = "Don't worry, no one will be able to take it from you. Probably."},
	[194] = {name = "Scourge of Death", grade = 2, points = 5, secret = true, description = "You are a master of the nether sea and have traveled with the ghost captain so many times that you know his ship and the perils of the nether sea inside out. You laugh in the face of death and may return as a ghost pirate yourself in the afterlife!"},
	[195] = {name = "Silent Pet", grade = 1, points = 1, secret = true, description = "Awww. Your very own little goldfish friend - he's cute, he's shiny and he can't complain should you forget to feed him. He'll definitely brighten up your day!"},
	[196] = {name = "Skin-Deep", grade = 2, points = 4, secret = true, description = "You always carry your obsidian knife with you and won't hesitate to use it. You've skinned countless little - and bigger - critters and yeah: they usually don't get any more beautiful on the inside. It's rather blood and gore and all that..."},
	[197] = {name = "Snowbunny", grade = 1, points = 2, secret = true, description = "Hopping, hopping through the snow - that's the funnest way to go! Making footprints in a flurry - it's more fun the more you hurry! Licking icicles all day - Winter, never go away!"},
	[198] = {name = "Something's in There", grade = 1, points = 1, secret = true, description = "By the gods! What was that?"},
	[199] = {name = "Spectral Traveler", grade = 1, points = 2, secret = true, description = "You have sailed the nether seas with the Ghost Captain several times. The dangers of the nether have become familiar to you and unexperienced travelers turn to you for advice."},
	[200] = {name = "True Colours", grade = 1, points = 3, secret = true, description = "You and your friends showed the three wizards your loyalty three times - I am sure at least one of them is probably eternally thankful and exceedingly proud of you."},
	[201] = {name = "Truth Be Told", grade = 1, points = 2, secret = true, description = "You told Jack the truth by explaining you and Spectulus made a mistake when trying to convince him of being a completely different person."},
	[202] = {name = "Witches Lil' Helper", grade = 1, points = 1, secret = true, description = "You sacrificed ingredients to create the protective brew of the witches."},
	[203] = {name = "You Don't Know Jack", grade = 1, points = 2, secret = true, description = "You did not tell Jack the truth about the mistake you and Spectulus made when trying to convince him about being a completely different person. He will live in doubt until the end of his existence."},

	-- 9.1
	[204] = {name = "Askarak Nemesis", grade = 1, points = 1, secret = true, description = "You are now the royal archfiend of the Askarak, prince slayer."},
	[205] = {name = "Beak Doctor", grade = 2, points = 4, description = "You significantly helped the afflicted citizens of Venore in times of dire need. Somehow you still feel close to the victims of the fever outbreak. Your clothes make you one of them, one poor soul amongst the countless afflicted."},
	[206] = {name = "Biodegradable", grade = 1, points = 1, secret = true, description = "You caught fifty rare shimmer swimmers. Getting rid of all those corpses by dumping them into the lake really was worth it, wasn't it? Wait, didn't something move in the water just now...?"},
	[207] = {name = "Deer Hunt", grade = 1, points = 1, secret = true, description = "You managed to kill more than four hundred white deer - it looks like you are one of the main reasons they will soon be considered extinct, way to go!"},
	[208] = {name = "Doctor! Doctor!", grade = 1, points = 2, secret = true, description = "Did someone call a doctor? You delivered 100 medicine bags to Ottokar of the Venore poor house in times of dire need, well done!"},
	[209] = {name = "Eye of the Deep", grade = 1, points = 1, secret = true, description = "You didn't look into it - at least not for too long... but Groam did. And you relieved him. Just don't tell his friend Dronk."},
	[210] = {name = "Firefighter", grade = 1, points = 2, secret = true, description = "You extinguished 500 thornfires! You were there when the Firestarters took over Shadowthorn. You saved the day - and the home of some elves which will try to kill you nonetheless. Isn't it nice to see everything restored just as it was before..?"},
	[211] = {name = "Invader of the Deep", grade = 1, points = 2, secret = true, description = "Many creatures of the deep have lost their lives by your hand. Three hundred have entered the depths of eternity. You should probably fear the revenge of the Eyes of the Deep."},
	[212] = {name = "Mageslayer", grade = 1, points = 1, secret = true, description = "You killed the raging mage in his tower south of Zao. Again. But this one just keeps coming back. The dimensional portal collapsed once more and you know he will eventually return but hey - a raging mage, it's like asking for it..."},
	[213] = {name = "Mystic Fabric Magic", grade = 2, points = 4, description = "You vanquished the mad mage, you subdued the raging mage - no spellweaving self-exposer can stand in your way. Yet you are quite absorbed in magical studies yourself. This very fabric reflects this personal approval of the magic arts."},
	[214] = {name = "Shaburak Nemesis", grade = 1, points = 1, secret = true, description = "You are now the public archenemy of the Shaburak, prince slayer."},
	[215] = {name = "Slimer", grade = 1, points = 1, secret = true, description = "With the assistance of your friendly little helper, you gobbled more than 500 chunks of slime. Well done, Slimer."},

	-- 9.2
	[216] = {name = "Arachnoise", grade = 1, points = 1, description = "You've shattered each of Bloodweb's eight frozen legs. As they say: break a leg, and then some more."},
	[217] = {name = "Back into the Abyss", grade = 1, points = 1, description = "You've cut off a whole lot of tentacles today. Thul was driven back to where he belongs."},
	[218] = {name = "Beautiful Agony", grade = 1, points = 2, description = "Ethershreck's cry of agony kept ringing in your ear for hours after he had dissolved into thin air. He probably moved to another plane of existence... for a while."},
	[219] = {name = "Blood-Red Snapper", grade = 1, points = 1, description = "You've tainted the jungle floor with the Snapper's crimson blood."},
	[220] = {name = "Breaking the Ice", grade = 1, points = 1, description = "You almost made friends with Shardhead... before he died. Poor guy only seems to attract violence with his frosty attitude."},
	[221] = {name = "Choking on Her Venom", grade = 1, points = 1, description = "The Old Widow fell prey to your supreme hunting skills."},
	[222] = {name = "Crawling Death", grade = 1, points = 1, description = "You ripped the ancient scarab Fleshcrawler apart and made sure he didn't get under your skin."},
	[223] = {name = "Hissing Downfall", grade = 1, points = 2, description = "You've vansquished the Noxious Spawn and his serpentine heart."},
	[224] = {name = "Just Cracked Me Up!", grade = 1, points = 2, description = "Stonecracker's head was much softer than the stones he threw at you."},
	[225] = {name = "Meat Skewer", grade = 1, points = 1, description = "You've impaled the big mammoth Bloodtusk with his own tusks."},
	[226] = {name = "No More Hiding", grade = 1, points = 1, description = "You've found a well-hidden spider queen and caught her off guard in the middle of her meal."},
	[227] = {name = "One Less", grade = 1, points = 2, description = "The Many is no more, but how many more are there? One can never know."},
	[228] = {name = "Pwned a Lot of Fur", grade = 3, points = 8, secret = true, description = "You've faced and defeated a lot of the mighty bosses the Paw and Fur society sent you out to kill. All by yourself. What a hunt!"},
	[229] = {name = "Rootless Behaviour", grade = 1, points = 1, description = "You've descended into the swampy depths of Deathbine's lair and made quick work of it."},
	[230] = {name = "Scorched Flames", grade = 1, points = 1, description = "A mighty blaze went out today. It's Flameborn's turn to wait for his rebirth in the eternal cycle of life and death."},
	[231] = {name = "Something Smells", grade = 1, points = 1, description = "You've exinguished the Sulphur Scuttler's gas clouds and made the air in his cave a little better... at least for a while."},
	[232] = {name = "Spareribs for Dinner", grade = 1, points = 1, description = "Ribstride is striding no more. He had quite a few ribs to spare though."},
	[233] = {name = "The Drowned Sea God", grade = 1, points = 2, description = "As the killer of Leviathan, the giant sea serpent, his underwater kingdom is now under your reign."},
	[234] = {name = "The Gates of Hell", grade = 1, points = 2, description = "It seems the gates to the underworld have to remain unprotected for a while. Kerberos, the mighty hellhound, lost his head. All three of them."},
	[235] = {name = "The Serpent's Bride", grade = 1, points = 2, description = "You made a knot with Gorgo's living curls and took her scalp. You couldn't save her countless petrified victims, but at least you didn't become one."},
	[236] = {name = "Twisted Mutation", grade = 1, points = 1, description = "You've slain Esmeralda, the most hideous and aggressive of the mutated rats. No one will know that you almost lost a finger in the process."},

	-- 9.4
	[237] = {name = "Bane of the Hive", grade = 1, points = 2, description = "Countless fights and never tiring effort in the war against the hive grant you the experience to finish your outfit with the last remaining part. Your chitin outfit is a testament of your skills and dedication for the cause."},
	[238] = {name = "Chest Robber", grade = 1, points = 1, description = "You've discovered three nomad camps and stole their supplies. Well, you can probably use them better then they can."},
	[239] = {name = "Chitin Bane", grade = 2, points = 4, description = "You have become competent and efficient in gathering the substance that is needed to fight the hive. You almost smell like dissolved chitin and the Hive Born would tell their children scary stories about you if they could speak."},
	[240] = {name = "Confusion", grade = 1, points = 3, description = "The destruction you have caused by now can be felt throughout the whole hive. The mayhem that follows your step caused significant confusion in the consciousness of the hive."},
	[241] = {name = "Dazzler", grade = 1, points = 3, description = "In the war against the hive, your efforts in blinding it begin to pay off. Your actions have blinded the hive severely and the entity seems to become aware that something dangerous is happening."},
	[242] = {name = "Death Song", grade = 1, points = 3, description = "You hushed the songs of war in the black depths by sliencing more than three hundred Deepling Spellsingers."},
	[243] = {name = "Depth Dwellers", grade = 1, points = 3, description = "By eliminating at least three hundred Deepling Warriors you delivered quite a blow to the amassing armies of the deep."},
	[244] = {name = "Desert Fisher", grade = 1, points = 1, description = "You managed to catch a fish in a surrounding that usually doesn't even carry water. Everything is subject to change, probably..."},
	[245] = {name = "Dog Sitter", grade = 1, points = 1, description = "You showed Noodles the way home. How long will it take this time until he's on the loose again? That dog must be really bored in the throne room by now."},
	[246] = {name = "Down the Drain", grade = 1, points = 2, description = "You've found a secret dungeon in the flooded plains and killed several of its inhabitants. And now you have wet feet."},
	[247] = {name = "Exterminator", grade = 2, points = 4, description = "Efficient and lethal, you have gained significant experience in fighting the elite forces of the hive. Almost single-handed, you have slain the best of the Hive Born and live to tell the tale."},
	[248] = {name = "Fire from the Earth", grade = 1, points = 2, description = "You've survived the Hellgorge eruption and found a way through the flames and lava. You've even managed to kill a few fireborn on the way."},
	[249] = {name = "Gatherer", grade = 1, points = 2, description = "By killing creatures of the hive and gaining weapons for further missions, you started a quite effective way of war. You gathered a lot of dissolved chitin to resupply the war effort."},
	[250] = {name = "Gem Cutter", grade = 1, points = 1, secret = true, description = "You cut your first gem - and it bears your own name! Now that would be a nice gift! This does not make it a \"true\" Heart of the Sea, however..."},
	[251] = {name = "Goldhunter", grade = 1, points = 2, secret = true, description = "	If it wasn't for you, several banks in the World would've gotten bankrupt by now. Keep on chasing bank robbers and no one will have to worry about the World economy!"},
	[252] = {name = "Guard Killer", grade = 1, points = 2, description = "You have proven that you can beat the best of the hive. You have caused first promising breaches in the defence of the hive"},
	[253] = {name = "Guardian Downfall", grade = 2, points = 4, description = "You ended the life of over three hundred Deepling Guards. Not quite the guardian of the Deeplings, are you?"},
	[254] = {name = "Headache", grade = 1, points = 2, description = "Even in the deepest structures of the hive, you began to strike against the mighty foe. Your actions probably already gave the hive a headache."},
	[255] = {name = "Heartburn", grade = 1, points = 3, description = "Never-tiring, you attack the inner organs of the mighty hive. Your attacks on the hive's digestion system begin to cause some trouble."},
	[256] = {name = "Hickup", grade = 1, points = 2, description = "You have grown accustomed to frequenting the hive's stomach system. Your actions have caused the hive some first digestion problems."},
	[257] = {name = "Hive Blinder", grade = 2, points = 4, description = "You have put a lot of time and energy into keeping the hive unaware of what is happening on Quirefang. The hive learnt to fear your actions. It would surely crush you with all its might ... if it could only find you!"},
	[258] = {name = "Hive Fighter", grade = 1, points = 1, description = "You have participated that much in the hive war, that you are able to create some makeshift armor from the remains of dead hive born that can be found in the major hive, to show of your skill."},
	[259] = {name = "Hive Infiltrator", grade = 1, points = 3, description = "The most powerful warriors of the hive were killed by you by the dozens. The hive is not safe anymore because of your actions."},
	[260] = {name = "Hive War Veteran", grade = 1, points = 1, description = "Your invaluable experience in fighting the hive allows you to add another piece of armor to your chitin outfit to proove your dedication for the cause."},
	[261] = {name = "Honest Finder", grade = 1, points = 1, description = "You've stopped the bank robber and returned the bag full of gold. Good to know there are still lawful citizens like you around."},
	[262] = {name = "Ice Harvester", grade = 1, points = 1, description = "You witnessed the thawing of Svargrond and harvested rare seeds from some strange icy plants. They must be good for something."},
	[263] = {name = "Loyal Subject", grade = 1, points = 1, description = "You joined the Kingsday festivities and payed the King your respects. Now, off to party!"},
	[264] = {name = "Manic", grade = 2, points = 4, description = "You have destroyed a significant amount of the hive's vital nerve centres and caused massive destruction to the hive's awareness. You are probably causing the hive horrible nightmares."},
	[265] = {name = "Minor Disturbance", grade = 1, points = 2, description = "Your actions start to make a difference. You have blinded the antennae of the hive often enough to become an annoyance to it."},
	[266] = {name = "Navigational Error", grade = 2, points = 5, secret = true, description = "You confronted the Navigator."},
	[267] = {name = "Pimple", grade = 1, points = 3, description = "You are getting more and more experienced in destroying the supply of the enemy's forces. Your actions caused the hive some severe skin problems."},
	[268] = {name = "Planter", grade = 1, points = 2, description = "The hive has to be fought with might and main, hampering its soldiers is only the first step. You diligently stopped the pores of the hive to spread its warriors."},
	[269] = {name = "Preservationist", grade = 1, points = 1, secret = true, description = "You are a pretty smart thinker and managed to create everlasting flowers. They might become a big hit with all the people who aren't blessed with a green thumb or just forgetful."},
	[270] = {name = "Si, Ariki!", grade = 1, points = 1, description = "You've found the oriental traveller Yasir and were able to trade with him - even if you didn't really understand his language."},
	[271] = {name = "Someone's Bored", grade = 1, points = 1, secret = true, description = "That was NOT a giant spider. There's some witchcraft at work here."},
	[272] = {name = "Spolium Profundis", grade = 2, points = 4, description = "You travelled the depths of this very world. You entered the blackness of the deep sea to conquer the realm of the Deeplings. May this suit remind you of the strange beauty below."},
	[273] = {name = "Stomach Ulcer", grade = 2, points = 4, description = "You severely disrupted the digestion of the hive. The hive should for sure see a doctor. It seems you proved to be more than it can swallow."},
	[274] = {name = "Supplier", grade = 1, points = 3, description = "The need for supplies often decides over loss or victory. Your tireless efforts to resupply the resources keeps the war against the hive going."},
	[275] = {name = "Suppressor", grade = 2, points = 4, description = "A war is won by those who have the best supply of troops. The hive's troops have been dealt a significant blow by your actions. You interrupted the hive's replenishment of troops lastingly and severely."},
	[276] = {name = "Torn Treasures", grade = 1, points = 1, secret = true, description = "Wyda seems to be really, really bored. You also found out that she doesn't really need all those blood herbs that adventurers brought her. Still, she was nice enough to take one from you and gave you something quite cool in exchange."},
	[277] = {name = "Trail of the Ape God", grade = 1, points = 1, secret = true, description = "You've discovered a trail of giant footprints and Terrified Elephants running everywhere. Could it be that the mysterious Ape God is rambling in the jungle?"},
	[278] = {name = "Whistle-Blower", grade = 1, points = 1, secret = true, description = "You can't keep a secret, can you? Then again, you're just fulfilling your duty to the Queen of Carlin as a lawful citizen. That's a good thing, isn't it...?"},

	-- 9.5
	[279] = {name = "Back from the Dead", grade = 1, points = 2, description = "You overcame the undead Zanakeph and sent him back into the darkness that spawned him."},
	[280] = {name = "Dream's Over", grade = 1, points = 1, description = "No more fear and bad dreams. You stabbed Tormentor to death with its scythe leg."},
	[281] = {name = "Enter zze Draken!", grade = 1, points = 2, description = "You gave zzze draken a tazte of your finizzzing move."},
	[282] = {name = "Howly Silence", grade = 1, points = 1, description = "You muted the everlasting howling of Hemming."},
	[283] = {name = "Kapow!", grade = 1, points = 1, description = "No joke, you murdered the bat."},
	[284] = {name = "King of the Ring", grade = 1, points = 2, description = "Bretzecutioner's body just got slammed away. You are a true king of the ring!"},
	[285] = {name = "Pwned All Fur", grade = 3, points = 8, secret = true, description = "You've faced and defeated each of the mighty bosses the Paw and Fur society sent you out to kill. All by yourself. What a hunt!"},
	[286] = {name = "Stepped on a Big Toe", grade = 1, points = 1, description = "This time you knocked out the big one."},
	[287] = {name = "Zzztill Zzztanding!", grade = 1, points = 1, description = "You wiped Fazzrah away - zzeemzz like now you're the captain."},

	-- 9.6
	[288] = {name = "Becoming a Bigfoot", grade = 1, points = 1, description = "You did it! You convinced the reclusive gnomes to accept you as one of their Bigfoots. Now you are ready to help them. With big feet big missions seen to come."},
	[289] = {name = "Bibby's Bloodbath", grade = 1, points = 1, secret = true, description = "You lend a helping hand in defeating invading Orcs by destroying their warcamp along with their leader. Bibby's personal bloodbath..."},
	[290] = {name = "Call Me Sparky", grade = 1, points = 1, description = "Admittedly you enjoyed the killing as usual. But the part with the sparks still gives you shivers ... or is it that there is some charge left on you?"},
	[291] = {name = "Crystal Clear", grade = 1, points = 3, description = "If the gnomes had told you that crystal armor is see-through you had probably changed your underwear in time."},
	[292] = {name = "Crystal Keeper", grade = 1, points = 1, description = "So you repaired the light of some crystals for those gnomes. What's next? Sitting a week in a mushroom bed as a temporary mushroom?"},
	[293] = {name = "Crystals in Love", grade = 1, points = 1, description = "You brought two loving crystals together. Perhaps they might even name one of their children after you. To bad you forgot to leave your calling card."},
	[294] = {name = "Death from Below", grade = 1, points = 2, secret = true, description = "The face of the enemy is unmasked. You have encountered one of 'those below' and survived. More than that, you managed to kill the beast and prove once and for all that the enemy can be beaten."},
	[295] = {name = "Death on Strike", grade = 2, points = 4, secret = true, description = "Again and again Deathstrike has fallen to your prowess. Perhaps it's time for people calling YOU Deathstrike from now on."},
	[296] = {name = "Diplomatic Immunity", grade = 2, points = 4, secret = true, description = "You killed the ambassador of the abyss that often that they might consider sending another one. Perhaps that will one day stop further intrusions."},
	[297] = {name = "Dungeon Cleaner", grade = 1, points = 3, secret = true, description = "Seen it all. Done it all. Your unstoppable force swept through the dungeons and you vanquished their masters. Not to forget the precious loot you took! Now stop reading this and continue hunting! Time is money after all!"},
	[298] = {name = "Fall of the Fallen", grade = 2, points = 4, secret = true, description = "Have you ever wondered how he reappears again and again? You only care for the loot, do you? Gotcha!"},
	[299] = {name = "Final Strike", grade = 1, points = 2, secret = true, description = "The mighty Deathstrike is dead! One legend is dead and you're on your way to become one yourself."},
	[300] = {name = "Funghitastic", grade = 1, points = 3, description = "Finally your dream to become a walking mushroom has come true ... No, wait a minute!"},
	[301] = {name = "Gnome Friend", grade = 1, points = 2, description = "The gnomes are warming up to you. One or two of them might actually bother to remember your name. You're allowed to access their gnomebase alpha. You are prepared to boldly put your gib feet into areas few humans have walked before."},
	[302] = {name = "Gnome Little Helper", grade = 1, points = 1, description = "You think the gnomes start to like you. A little step for a Bigfoot but a big step for humanity."},
	[303] = {name = "Gnomebane's Bane", grade = 1, points = 2, secret = true, description = "The fallen gnome is dead and justice served. But what was it that the gnome whispered with his last breath? He's your father???"},
	[304] = {name = "Gnomelike", grade = 1, points = 3, description = "You have become a household name in gnomish society! Your name is mentioned by gnomes more than once. Of course usually by gnomish mothers whose children refuse to eat their mushroom soup, but you are certainly making some tremendous progress."},
	[305] = {name = "Gnomish Art Of War", grade = 1, points = 3, description = "You have unleashed your inner gnome and slain some of the most fearsome threats that gnomekind has ever faced. Now you can come and go to the warzones as it pleases you. The enemies of gnomekind will never be safe again."},
	[306] = {name = "Goo Goo Dancer", grade = 1, points = 1, secret = true, description = "Seeing a mucus plug makes your heart dance and you can't resist to see what it hides. Goo goo away!"},
	[307] = {name = "Grinding Again", grade = 1, points = 1, description = "Burnt fingers and itching lungs are a small price for bringing those gnomes some lousy stone and getting almost killed! Your mother warned you to better become a farmer."},
	[308] = {name = "Honorary Gnome", grade = 2, points = 4, description = "You accomplished what few humans ever will: you truly impressed the gnomes. This might not change their outlook on humanity as a whole, but at least you can bathe in gnomish respect! And don't forget you're now allowed to enter the warzones!"},
	[309] = {name = "Nestling", grade = 1, points = 1, description = "You cleansed the land from an eight legged nuisance by defeating Mamma Longlegs three times. She won't be back soon... or will she?"},
	[310] = {name = "One Foot Vs. Many", grade = 1, points = 1, description = "One Bigfoot won over thousands of tiny feet. Perhaps the gnomes are wrong and size matters?"},
	[311] = {name = "Spore Hunter", grade = 1, points = 1, description = "After hunting for the correct mushrooms and their spores you're starting to feel like a mushroom yourself. A few times more and you might start thinking like a mushroom, who knows?"},
	[312] = {name = "Substitute Tinker", grade = 1, points = 1, description = "Ring-a-ding! You have visited the golem workshop and lent a hand in repairing them. To know those golems are safe is worth all the bruises, isn't it?"},
	[313] = {name = "The Picky Pig", grade = 1, points = 1, description = "The gnomes decided their pigs need some exclusive diet and you had to do all the dirty work - but wasn't the piglet adorable?"},

	-- 9.8
	[314] = {name = "Task Manager", grade = 1, points = 2, secret = true, description = "Helping a poor, stupid goblin to feed his starving children and wifes feels good ... if you'd only get rid of the strange feeling that you're missing something."},
	[315] = {name = "True Dedication", grade = 2, points = 5, secret = true, description = "You conquered the demon challenge and prevailed... now show off your success in style!"},

	-- 10.1
	[316] = {name = "Gravedigger", grade = 1, points = 3, description = "Assisting Omrabas' sick plan to resurrect made you dig your way through the blood-soaked halls of Drefia. Maybe better he failed!"},
	[317] = {name = "Repenter", grade = 1, points = 1, secret = true, description = "You cleansed your soul in serving the Repenter enclave and purified thine self in completing all tasks in a single day of labour."},

	-- 10.2
	[318] = {name = "Cave Completionist", grade = 1, points = 2, description = "	You have helped the gnomes of the spike in securing the caves and explored enough of the lightles depths to earn you a complete cave explorers outfit. Well done!"},

	-- 10.3
	[319] = {name = "Dream Warden", grade = 2, points = 5, description = "It doesn't matter what noise you would hear... dream, nightmare, illusion - there is nothing you can't vanquish. You are a true Dream Warden."},
	[320] = {name = "Dream Wright", grade = 1, points = 1, description = "You have mended many a broken dream and so, the dream of Roshamuul is safely being told over and over again."},
	[321] = {name = "Ending the Horror", grade = 1, points = 2, description = "You have cleansed the lands of many retching horrors. You sure know how to end a bad dream: forcefully, that's how!"},
	[322] = {name = "Luring Silence", grade = 1, points = 2, description = "What a scientific discovery - they really DO communicate! Using their own communication habits against them, you lured a large pack of silencers away from the walls of Roshamuul."},
	[323] = {name = "Never Surrender", grade = 1, points = 3, description = "You did not show any signs of surrender to any sight of... you get the picture. Even a hundred of them did not pose a threat to you."},
	[324] = {name = "Nevermending Story", grade = 1, points = 3, secret = true, description = "You collected all of the mysterious bottle messages around the island of Roshamuul and located the remains of the first mate. Time will tell if his tale of mending an evil ring holds true."},
	[325] = {name = "Noblesse Obliterated", grade = 2, points = 6, description = "After a battle like this you know who your friends are."},
	[326] = {name = "Prison Break", grade = 3, points = 8, description = "Gaz'haragoth... a day to remember! Your world accomplished someting really big - and you have been part of it!"},
	[327] = {name = "Sleepwalking", grade = 1, points = 1, description = "You know your way, in dream and waking. And how to make tea that transcends the boundaries of conscience."},
	[328] = {name = "Umbral Archer", grade = 2, points = 6, description = "You managed to transform, improve and sacrify your bow into a master state and have proven yourself worthy in a nightmarish world."},
	[329] = {name = "Umbral Berserker", grade = 2, points = 6, description = "You managed to transform, improve and sacrify your hammer into a master state and have proven yourself worthy in a nightmarish world."},
	[330] = {name = "Umbral Bladelord", grade = 2, points = 6, description = "You managed to transform, improve and sacrify your slayer into a master state and have proven yourself worthy in a nightmarish world."},
	[331] = {name = "Umbral Brawler", grade = 2, points = 6, description = "You managed to transform, improve and sacrify your mace into a master state and have proven yourself worthy in a nightmarish world."},
	[332] = {name = "Umbral Executioner", grade = 2, points = 6, description = "You managed to transform, improve and sacrify your chopper into a master state and have proven yourself worthy in a nightmarish world."},
	[333] = {name = "Umbral Harbringer", grade = 2, points = 6, description = "You managed to transform, improve and sacrify your spellbook into a master state and have proven yourself worthy in a nightmarish world."},
	[334] = {name = "Umbral Headsman", grade = 2, points = 6, description = "You managed to transform, improve and sacrify your axe into a master state and have proven yourself worthy in a nightmarish world."},
	[335] = {name = "Umbral Marksman", grade = 2, points = 6, description = "You managed to transform, improve and sacrify your crossbow into a master state and have proven yourself worthy in a nightmarish world."},
	[336] = {name = "Umbral Master", grade = 3, points = 8, description = "You managed to transform, improve and sacrify all kinds of weapons into a master state and have proven yourself worthy in a nightmarish world. Respect!"},
	[337] = {name = "Umbral Swordsman", grade = 2, points = 6, description = "You managed to transform, improve and sacrify your blade into a master state and have proven yourself worthy in a nightmarish world."},

	-- 10.5
	[338] = {name = "Combo Master", grade = 1, points = 1, secret = true, description = "You accomplished 10 or more consecutive chains in a row! That's killing at least 39 creatures in the correct order - now that's combinatorics!"},
	[339] = {name = "Elementary, My Dear", grade = 1, points = 1, description = "Through the spirit of science and exploration, you have discovered how to enter the secret hideout of the renowned Dr Merlay."},
	[340] = {name = "Glooth Engineer", grade = 2, points = 5, description = "Though you might have averted a dire threat for Rathleton, this relative peace may only hold for a while. At least you've scavenged an outfit from some of the poor fellows that have fallen prey to death priest Shagron."},
	[341] = {name = "Rathleton Citizen", grade = 1, points = 1, description = "By having rendered numerous services to the city of Rathleton you have been promoted to the rank of Citizen."},
	[342] = {name = "Rathleton Commoner", grade = 1, points = 1, description = "By having rendered numerous services to the city of Rathleton you have been promoted to the rank of Commoner."},
	[343] = {name = "Rathleton Inhabitant", grade = 1, points = 1, description = "By having rendered numerous services to the city of Rathleton you have been promoted to the rank of Inhabitant."},

	-- 10.56
	[344] = {name = "Seasoned Adventurer", grade = 1, points = 1, description = "Adventure is your middle name. You spent much time in dangerous lands and have seen things others only dream of. You know your way around in the World - you are a seasoned adventurer now. And your journey has only just begun!"},

	-- 10.7
	[345] = {name = "Go with da Lava Flow", grade = 1, points = 1, secret = true, description = "You escaped the glowing hot lava death trap, Professor Maxxen has set for you - Captain Caveworm is indeed proud!"},
	[346] = {name = "Lion's Den Explorer", grade = 1, points = 1, secret = true, description = "You discovered the Lion's Rock, passed the tests to enter the inner sanctum and finally revealed the secrets of the buried temple. You literally put your head in the lion's mouth and survived."},
	[347] = {name = "Mind the Step!", grade = 1, points = 1, description = "You've got a mind ready to draw strange conclusions that defy the laws of logic and sidestep reality. Or maybe it's just a lucky guess - or adventurous recklessness?"},
	[348] = {name = "Plant vs. Minos", grade = 1, points = 4, secret = true, description = "You have defeated the wallbreaker and saved the glooth plant."},
	[349] = {name = "Publicity", grade = 1, points = 1, description = "You are a man of the public. Or of good publicity at least. Through your efforts in advertising the airtight cloth, Zeronex might yet be redeemed - and Rathleton might yet see its first working Gloud Ship."},
	[350] = {name = "Rathleton Squire", grade = 1, points = 1, description = "By having rendered numerous services to the city of Rathleton you have been promoted to the rank of Squire."},
	[351] = {name = "Robo Chop", grade = 1, points = 4, secret = true, description = "You have defeated the glooth bomb and chopped down a lot of metal monsters on your way."},
	[352] = {name = "Rumble in the Plant", grade = 2, points = 4, secret = true, description = "You have defeated the tremor worm - and wonder what kind of fish you'd be able to catch with such a bait."},
	[353] = {name = "Snake Charmer", grade = 1, points = 1, description = "By restoring the Everhungry Altar, you charmed the Fire-Feathered Sea Serpent back into its fitful sleep, twenty miles beneath the sea."},
	[354] = {name = "The Professor's Nut", grade = 1, points = 3, description = "He seriously stored away a wallnut? That was a nutty professor indeed."},
	[355] = {name = "Wail of the Banshee", grade = 1, points = 1, secret = true, description = "You saw the Crystal Gardens with all their stunning beauty and survived the equally impressive monsters there. In the end you discovered a great evil and destroyed it with the help of a banshee who was not even aware of her support."},

	-- 10.8
	[356] = {name = "Bearbaiting", grade = 1, points = 1, description = "Hunter's greeting! Your skillful use of the slingshot actually stunned a large bear. The creature is slightly dazed, but seems susceptible to your commands. Let's declare open season on all our foes!"},
	[357] = {name = "Beneath the Sea", grade = 1, points = 3, description = "Not really twenty thousand miles, but you had to dive a fair way beneath the sea to find your personal Manta Ray."},
	[358] = {name = "Blacknailed", grade = 1, points = 1, description = "Well, you can rest your nailcase now. This gravedigger's fingernails are nice and clean. Though after the next hellride, you might not want to let it hand any food to you."},
	[359] = {name = "Cartography 101", grade = 1, points = 2, description = "You succeeded in finding and charting several previously unexplored landmarks and locations for the Adventurer's Guild, you probably never need to ask anyone for the way - do you?"},
	[360] = {name = "Chequered Teddy", grade = 1, points = 1, description = "Don't let its fluffy appearance deceive you. The panda is a creature of the wild. It will take you to the most distant regions of the World, always in hopes of a little bamboo to nibble on or to check on a possible mate."},
	[361] = {name = "Dragon Mimicry", grade = 1, points = 2, description = "It's not really a dragon, but rather a kind of chimera. Nonetheless a decent mount to impress any passer-by."},
	[362] = {name = "Fabled Construction", grade = 1, points = 3, description = "Finding all the pieces to this complicated vehicle was one kind of a challenge. However, what you built in the end is rather a fabled than a feeble construction."},
	[363] = {name = "Fata Morgana", grade = 1, points = 2, description = "There are many delusions and phantasms in the desert. You saw a false oasis with fruit-bearing palm trees. Instead of water and refreshment, however, you found a dromedary in the end. What a useful Fata Morgana!"},
	[364] = {name = "Fried Shrimp", grade = 1, points = 2, description = "This must be underwater love - this enormous crustacean now does thy bidding. Or maybe it's just in it for a little more of that shrimp barbecue, as that's a little hard to come by in the sea."},
	[365] = {name = "Friend of Elves", grade = 1, points = 1, description = "Kingly deer mostly prefer elves as friends and familiars. This one, however, decided to favour you as a confidant and rider. Well done!"},
	[366] = {name = "Gear Up", grade = 1, points = 3, description = "Installing that control unit was a no-brainer. Now you're in control to make it walk this way or that, or to change tack at any moment if required. Your faithful walker mount obeys your every command."},
	[367] = {name = "Golden Sands", grade = 1, points = 3, description = "Counting ten thousand grains of sand could not have been harder than gaining this impressive mount."},
	[368] = {name = "Hoard of the Dragon", grade = 1, points = 1, secret = true, description = "Your adventurous way through countless dragon lairs earned you a pretty treasure - and surely the enmity of many a dragon."},
	[369] = {name = "Icy Glare", grade = 1, points = 1, description = "Here's looking at you, kid. This ancient creature seems to size you up with its brilliant eyes and barely tolerates you riding it. Maybe it thinks you're the defrosted snack, after all?"},
	[370] = {name = "Knock on Wood", grade = 1, points = 3, description = "It's a wound-up wooden lizard! Well, stranger things have happened, or so you're told. Just hop on and let this wood-and-tin contraption take you anywhere you want to wind down a bit. And hope you don't get hit by lightning underway."},
	[371] = {name = "Lion King", grade = 1, points = 1, description = "By mastering the secrets of Lion's Rock, you proved yourself worthy to face the mighty lions there. One of them even chose to accompany you."},
	[372] = {name = "Little Ball of Wool", grade = 1, points = 1, description = "You found a lost sheep and thus a steady source of black wool. But careful: don't get entangled."},
	[373] = {name = "Lost Palace Raider", grade = 1, points = 2, secret = true, description = "Lifting the secrets of a fabulous palace and defeating a beautiful demon princess was a thrilling experience indeed. This site's marvels nearly matched its terrors. Nearly."},
	[374] = {name = "Lovely Dots", grade = 1, points = 3, description = "Finding a four-leaved clover is always a sign of luck. And as luck would have it, you even baited a lovely dotted ladybug. Lucky you!"},
	[375] = {name = "Loyal Lad", grade = 1, points = 1, description = "Having a loyal friend alongside is comforting to every adventurer. If only this lad was not so stubborn..."},
	[376] = {name = "Lucky Horseshoe", grade = 1, points = 1, description = "'Sweets for my steed' could be your motto. An impressive horse is eating out of your hand. Saddle up and be ready to find adventure, new friends, and maybe someone to shoe your horse now and then."},
	[377] = {name = "Luminous Kitty", grade = 1, points = 3, description = "You made some efforts to bring a little more light into the world. And what a nice present you got in return!"},
	[378] = {name = "Magnetised", grade = 1, points = 2, description = "This magnetic beast attracted you in a very literal way. Or was it attracted by your metal equipment? Anyway, you seem to be stuck together now."},
	[379] = {name = "Mind the Dog!", grade = 1, points = 2, description = "Barking dogs never bite, as the saying goes. But this one clearly tried. In the end, however, you were able to walk the dog - ahem, gnarlhound."},
	[380] = {name = "Out of the Stone Age", grade = 1, points = 3, description = "What a blast from the past! This thankful patient thinks you missed your dentist vocation. It's now ready to take a bite of the future and to carry you to your next adventure, or your next patient."},
	[381] = {name = "Pecking Order", grade = 1, points = 1, description = "Ah, the old carrot-on-a-stick trick. Well done! You've made the racing bird accept you as a rider and provider. Just don't feed it your fingers."},
	[382] = {name = "Personal Nightmare", grade = 1, points = 3, description = "It might come as a shock to you, but this is the mount of your dreams. Not exactly the white steed of Prince Charming, but maybe the ladies will still scream and faint at the sight of you."},
	[383] = {name = "Pig-Headed", grade = 1, points = 2, description = "Whoa, sow long! This boar is like a force of nature, breaking through the undergrowth of all the forests and all records of speed. Hang on!"},
	[384] = {name = "Scales and Tail", grade = 1, points = 2, description = "The Muggy Plains are a dangerous place, often raided by dragons. But that was your luck: thus you found this scaly little guy."},
	[385] = {name = "Slugging Around", grade = 1, points = 2, description = "Drugging a snail can have some beneficial side effects. You're now the proud owner of a snarling, speed-crazy slug. Maybe it'll purr if you stroke it. Anyway, life should be one slick ride from now on."},
	[386] = {name = "Spin-Off", grade = 1, points = 1, description = "Seems like this spider has got a sweet tooth. As a result, eight hairy legs are now at your disposal to crawl and weave at your whim, and strike fear into the hearts of men."},
	[387] = {name = "Starless Night", grade = 1, points = 3, description = "By many it is considered a myth like the Yeti. But you came, saw and tamed it. Now you're the proud rider of a midnight panther, black as a starless night."},
	[388] = {name = "Stuntman", grade = 1, points = 3, description = "A drop of oil and you're good to go. This unique mount will roll merrily in and out of any strange place you want to visit. If you see no exit, you probably ended up in a circus ring. Ah well, the show must go on!"},
	[389] = {name = "Swamp Beast", grade = 1, points = 1, description = "By cleverly using a leech to cool that raging bull's blood, you managed not to get swamped or trampled in a water buffalo stampede. The creature is now docile and follows your every command."},
	[390] = {name = "The Right Tone", grade = 1, points = 1, description = "By setting the right tone you convinced a crystal wolf to accompany you. Remember it is made of crystal, though, so be careful in a banshee's presence."},
	[391] = {name = "Thick-Skinned", grade = 1, points = 2, description = "It's unstoppable! Walls? Fortresses? Obstacles? Objections? Pah! Nothing will stand before the stampor. Arrows and spears bounce off its hide, enemies are trampled by the dozen. Just don't go for the subtle approach or a date on this thing."},
	[392] = {name = "Way to Hell", grade = 1, points = 2, description = "This fiery beast really tried to give you hell. But not even a magma crawler can resist a mug of spicy, hot glow wine. Skol!"},

	-- 10.9
	[393] = {name = "Hat Hunter", grade = 2, points = 5, description = "You sucessfully fought against all odds to protect your world from an ascending god! – You weren't there for the hat only after all?"},
	[394] = {name = "Ogre Chef", grade = 1, points = 1, description = "You didn't manage to become an ogre chief. But at least you are, beyond doubt, a worthy ogre chef."},
	[395] = {name = "Rift Warrior", grade = 1, points = 3, description = "You went through hell. Seven times. You defeated the demons. Countless times. You put an end to Ferumbras claims to ascendancy. Once and for all."},
	[396] = {name = "The Call of the Wild", grade = 1, points = 2, description = "You opposed man-eating ogres and clumsy clomps. You grappled with hungry chieftains, desperate goblins and angry spirits. So you truly overcame the wild vastness of Krailos."},

	-- 10.94
	[397] = {name = "Ender of the End", grade = 2, points = 5, description = "You have entered the heart of destruction and valiantly defeated the world devourer. By your actions you have postponed the end of the world — at least for a while."},
	[398] = {name = "Vortex Tamer", grade = 2, points = 5, description = "After a long journey and dedication you were favoured by fortune and have tamed all three elusive beasts of the vortex. Unless the Vortexion decides you're a tasty morsel you can enjoy your small stable of ravaging beasts from beyond."},
}

ACHIEVEMENT_FIRST = 1
ACHIEVEMENT_LAST = #achievements

function getAchievementInfoById(id)
	for k, v in pairs(achievements) do
		if k == id then
			local targetAchievement = {}
			targetAchievement.id = k
			targetAchievement.actionStorage = PlayerStorageKeys.achievementsCounter + k
			for inf, it in pairs(v) do
				targetAchievement[inf] = it
			end
			return targetAchievement
		end
	end
	return false
end

function getAchievementInfoByName(name)
	for k, v in pairs(achievements) do
		if v.name:lower() == name:lower() then
			local targetAchievement = {}
			targetAchievement.id = k
			targetAchievement.actionStorage = PlayerStorageKeys.achievementsCounter + k
			for inf, it in pairs(v) do
				targetAchievement[inf] = it
			end
			return targetAchievement
		end
	end
	return false
end

function getSecretAchievements()
	local targetAchievement = {}
	for k, v in pairs(achievements) do
		if v.secret then
			targetAchievement[#targetAchievement + 1] = k
		end
	end
	return targetAchievement
end

function getPublicAchievements()
	local targetAchievement = {}
	for k, v in pairs(achievements) do
		if not v.secret then
			targetAchievement[#targetAchievement + 1] = k
		end
	end
	return targetAchievement
end

function getAchievements()
	return achievements
end

function isAchievementSecret(ach)
	local achievement
	if tonumber(ach) ~= nil then
		achievement = getAchievementInfoById(ach)
	else
		achievement = getAchievementInfoByName(ach)
	end
	if not achievement then
		print("[!] -> Invalid achievement \"" .. ach .. "\".")
		return false
	end

	return achievement.secret
end

function Player.hasAchievement(self, ach)
	local achievement
	if tonumber(ach) ~= nil then
		achievement = getAchievementInfoById(ach)
	else
		achievement = getAchievementInfoByName(ach)
	end
	if not achievement then
		print("[!] -> Invalid achievement \"" .. ach .. "\".")
		return false
	end

	return self:getStorageValue(PlayerStorageKeys.achievementsBase + achievement.id) > 0
end

function Player.getAchievements(self)
	local targetAchievement = {}
	for k = 1, #achievements do
		if self:hasAchievement(k) then
			targetAchievement[#targetAchievement + 1] = k
		end
	end
	return targetAchievement
end

function Player.addAchievement(self, ach, hideMsg)
	local achievement
	if tonumber(ach) ~= nil then
		achievement = getAchievementInfoById(ach)
	else
		achievement = getAchievementInfoByName(ach)
	end
	if not achievement then
		print("[!] -> Invalid achievement \"" .. ach .. "\".")
		return false
	end

	if not self:hasAchievement(achievement.id) then
		self:setStorageValue(PlayerStorageKeys.achievementsBase + achievement.id, 1)
		if not hideMsg then
			self:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Congratulations! You earned the achievement \"" .. achievement.name .. "\".")
		end
	end
	return true
end

function Player.removeAchievement(self, ach)
	local achievement
	if tonumber(ach) ~= nil then
		achievement = getAchievementInfoById(ach)
	else
		achievement = getAchievementInfoByName(ach)
	end
	if not achievement then
		print("[!] -> Invalid achievement \"" .. ach .. "\".")
		return false
	end

	if self:hasAchievement(achievement.id) then
		self:setStorageValue(PlayerStorageKeys.achievementsBase + achievement.id, -1)
	end
	return true
end

function Player.addAllAchievements(self, hideMsg)
	for i = ACHIEVEMENT_FIRST, ACHIEVEMENT_LAST do
		self:addAchievement(i, hideMsg)
	end
	return true
end

function Player.removeAllAchievements(self)
	for k = 1, #achievements do
		if self:hasAchievement(k) then
			self:removeAchievement(k)
		end
	end
	return true
end

function Player.getSecretAchievements(self)
	local targetAchievement = {}
	for k, v in pairs(achievements) do
		if self:hasAchievement(k) and v.secret then
			targetAchievement[#targetAchievement + 1] = k
		end
	end
	return targetAchievement
end

function Player.getPublicAchievements(self)
	local targetAchievement = {}
	for k, v in pairs(achievements) do
		if self:hasAchievement(k) and not v.secret then
			targetAchievement[#targetAchievement + 1] = k
		end
	end
	return targetAchievement
end

function Player.getAchievementPoints(self)
	local points = 0
	local list = self:getAchievements()
	if #list > 0 then -- has achievements
		for i = 1, #list do
			local targetAchievement = getAchievementInfoById(list[i])
			if targetAchievement.points > 0 then -- avoid achievements with unknow points
				points = points + targetAchievement.points
			end
		end
	end
	return points
end

function Player.addAchievementProgress(self, ach, value)
	local achievement = tonumber(ach) ~= nil and getAchievementInfoById(ach) or getAchievementInfoByName(ach)
	if not achievement then
		print('[!] -> Invalid achievement "' .. ach .. '".')
		return true
	end

	local storage = PlayerStorageKeys.achievementsCounter + achievement.id
	local progress = self:getStorageValue(storage)
	if progress < value then
		self:setStorageValue(storage, math.max(1, progress) + 1)
	elseif progress == value then
		self:setStorageValue(storage, value + 1)
		self:addAchievement(achievement.id)
	end
	return true
end
