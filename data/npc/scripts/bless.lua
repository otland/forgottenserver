local keywordHandler = KeywordHandler:new()
local npcHandler = NpcHandler:new(keywordHandler)
NpcSystem.parseParameters(npcHandler)

function onCreatureAppear(cid)			npcHandler:onCreatureAppear(cid)			end
function onCreatureDisappear(cid)		npcHandler:onCreatureDisappear(cid)			end
function onCreatureSay(cid, type, msg)	npcHandler:onCreatureSay(cid, type, msg)	end
function onThink()						npcHandler:onThink()						end
function onPlayerCloseChannel(cid)		npcHandler:onPlayerCloseChannel(cid)		end

local blessNode = keywordHandler:addKeyword({'spiritual shielding'}, StdModule.say, {npcHandler = npcHandler, text = 'Do you want to buy {The Spiritual Shielding} blessing for 2000 (plus level depending amount) gold?'})
	blessNode:addChildKeyword({'yes'}, StdModule.bless, {npcHandler = npcHandler, number = 1, premium = false, name = "Spiritual Shielding"})
	blessNode:addChildKeyword({'no'}, StdModule.say, {npcHandler = npcHandler, text = 'Too expensive, eh?'})

local blessNode = keywordHandler:addKeyword({'embrace of tibia'}, StdModule.say, {npcHandler = npcHandler, text = 'Do you want to buy {The Embrace of Tibia} blessing?'})
	blessNode:addChildKeyword({'yes'}, StdModule.bless, {npcHandler = npcHandler, number = 2, premium = false, name = "Embrace of Tibia"})
	blessNode:addChildKeyword({'no'}, StdModule.say, {npcHandler = npcHandler, text = 'Too expensive, eh?'})

local blessNode = keywordHandler:addKeyword({'fire of the suns'}, StdModule.say, {npcHandler = npcHandler, text = 'Do you want to buy {The Fire of the Suns} blessing?'})
	blessNode:addChildKeyword({'yes'}, StdModule.bless, {npcHandler = npcHandler, number = 3, premium = false, name = "Fire of the Suns"})
	blessNode:addChildKeyword({'no'}, StdModule.say, {npcHandler = npcHandler, text = 'Too expensive, eh?'})

local blessNode = keywordHandler:addKeyword({'wisdom of solitude'}, StdModule.say, {npcHandler = npcHandler, text = 'Do you want to buy {The Wisdom of Solitude} blessing?'})
	blessNode:addChildKeyword({'yes'}, StdModule.bless, {npcHandler = npcHandler, number = 4, name = "Wisdom of Solitude"})
	blessNode:addChildKeyword({'no'}, StdModule.say, {npcHandler = npcHandler, text = 'Too expensive, eh?'})

local blessNode = keywordHandler:addKeyword({'spark of the phoenix'}, StdModule.say, {npcHandler = npcHandler, text = 'Do you want to buy {The Spark of the Phoenix} blessing?'})
	blessNode:addChildKeyword({'yes'}, StdModule.bless, {npcHandler = npcHandler, number = 5, premium = false, name = "Spark of the Phoenix"})
	blessNode:addChildKeyword({'no'}, StdModule.say, {npcHandler = npcHandler, text = 'Too expensive, eh?'})

local blessNode = keywordHandler:addKeyword({'twist of fate'}, StdModule.say, {npcHandler = npcHandler, text = 'Do you want to buy {Twist of Fate} blessing for 2000 (plus level depending amount) gold?'})
	blessNode:addChildKeyword({'yes'}, StdModule.bless, {npcHandler = npcHandler, number = 6, name = "god of war"})
	blessNode:addChildKeyword({'no'}, StdModule.say, {npcHandler = npcHandler, text = 'Too expensive, eh?'})

npcHandler:addModule(FocusModule:new())