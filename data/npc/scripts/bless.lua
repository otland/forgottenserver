local keywordHandler = KeywordHandler:new()
local npcHandler = NpcHandler:new(keywordHandler)
NpcSystem.parseParameters(npcHandler)

function onCreatureAppear(cid)				npcHandler:onCreatureAppear(cid)			end
function onCreatureDisappear(cid)			npcHandler:onCreatureDisappear(cid)			end
function onCreatureSay(cid, type, msg)		npcHandler:onCreatureSay(cid, type, msg)	end
function onThink()							npcHandler:onThink()						end

local node0 = keywordHandler:addKeyword({'twist of fate'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, text = 'Do you want to buy the Twist of Fate blessing for 0 gold?'})
	node0:addChildKeyword({'yes'}, StdModule.bless, {npcHandler = npcHandler, bless = 1, premium = false, cost = 0})
	node0:addChildKeyword({'no'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, reset = true, text = 'Too expensive, eh?'})

local node1 = keywordHandler:addKeyword({'first bless'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, text = 'Do you want to buy the first blessing for 10000 gold?'})
	node1:addChildKeyword({'yes'}, StdModule.bless, {npcHandler = npcHandler, bless = 2, premium = true, cost = 10000})
	node1:addChildKeyword({'no'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, reset = true, text = 'Too expensive, eh?'})

local node2 = keywordHandler:addKeyword({'second bless'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, text = 'Do you want to buy the second blessing for 10000 gold?'})
	node2:addChildKeyword({'yes'}, StdModule.bless, {npcHandler = npcHandler, bless = 3, premium = true, cost = 10000})
	node2:addChildKeyword({'no'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, reset = true, text = 'Too expensive, eh?'})

local node3 = keywordHandler:addKeyword({'third bless'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, text = 'Do you want to buy the third blessing for 10000 gold?'})
	node3:addChildKeyword({'yes'}, StdModule.bless, {npcHandler = npcHandler, bless = 4, premium = true, cost = 10000})
	node3:addChildKeyword({'no'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, reset = true, text = 'Too expensive, eh?'})

local node4 = keywordHandler:addKeyword({'fourth bless'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, text = 'Do you want to buy the fourth blessing for 10000 gold?'})
	node4:addChildKeyword({'yes'}, StdModule.bless, {npcHandler = npcHandler, bless = 5, premium = true, cost = 10000})
	node4:addChildKeyword({'no'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, reset = true, text = 'Too expensive, eh?'})

local node5 = keywordHandler:addKeyword({'fifth bless'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, text = 'Do you want to buy the fifth blessing for 10000 gold?'})
	node5:addChildKeyword({'yes'}, StdModule.bless, {npcHandler = npcHandler, bless = 6, premium = true, cost = 10000})
	node5:addChildKeyword({'no'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, reset = true, text = 'Too expensive, eh?'})

local node6 = keywordHandler:addKeyword({'sixth bless'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, text = 'Do you want to buy the sixth blessing for 10000 gold?'})
	node6:addChildKeyword({'yes'}, StdModule.bless, {npcHandler = npcHandler, bless = 7, premium = true, cost = 10000})
	node6:addChildKeyword({'no'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, reset = true, text = 'Too expensive, eh?'})
	
local node7 = keywordHandler:addKeyword({'seventh bless'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, text = 'Do you want to buy the seventh blessing for 10000 gold?'})
	node7:addChildKeyword({'yes'}, StdModule.bless, {npcHandler = npcHandler, bless = 8, premium = true, cost = 10000})
	node7:addChildKeyword({'no'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, reset = true, text = 'Too expensive, eh?'})

npcHandler:addModule(FocusModule:new())
