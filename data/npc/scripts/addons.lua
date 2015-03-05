local keywordHandler = KeywordHandler:new()
local npcHandler = NpcHandler:new(keywordHandler)
NpcSystem.parseParameters(npcHandler)

function onCreatureAppear(cid)			npcHandler:onCreatureAppear(cid)			end
function onCreatureDisappear(cid)		npcHandler:onCreatureDisappear(cid)			end
function onCreatureSay(cid, type, msg)	npcHandler:onCreatureSay(cid, type, msg)	end
function onThink()						npcHandler:onThink()						end

function buyAddons(cid, message, keywords, parameters, node)
	--TODO: buyAddons function in modules.lua
	if(not npcHandler:isFocused(cid)) then
		return false
	end

	local addon = parameters.addon
	local cost = parameters.cost
	local premium = (parameters.premium ~= nil and parameters.premium)

	if isPlayerPremiumCallback == nil or (isPlayerPremiumCallback(cid) and premium) then
		if doPlayerRemoveMoney(cid, cost) == TRUE then
			doPlayerAddAddons(cid, addon)
			npcHandler:say('There, you are now able to use all addons!', cid)
		else
			npcHandler:say('Sorry, you do not have enough money.', cid)
		end
	else
		npcHandler:say('I only serve customers with premium accounts.', cid)
	end

	keywordHandler:moveUp(cid, 2)
	return true
end

local node1 = keywordHandler:addKeyword({'first addon'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, text = 'Do you want to buy the first addons set for 5000 gold coins?'})
	node1:addChildKeyword({'yes'}, buyAddons, {addon = 1, cost = 5000, premium = true})
	node1:addChildKeyword({'no'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, moveup = 2, text = 'Too expensive, eh?'})

local node2 = keywordHandler:addKeyword({'second addon'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, text = 'Would you like to buy the second addons set for 10000 gold coins?'})
	node2:addChildKeyword({'yes'}, buyAddons, {addon = 2, cost = 10000, premium = true})
	node2:addChildKeyword({'no'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, moveup = 2, text = 'Too expensive, eh?'})

keywordHandler:addKeyword({'addon'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, text = 'I sell the first addons set for 5000 gold coins and the second addons set for 10000 gold coins.'})

npcHandler:addModule(FocusModule:new())