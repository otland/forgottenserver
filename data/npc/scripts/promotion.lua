local keywordHandler = KeywordHandler:new()
local npcHandler = NpcHandler:new(keywordHandler)
NpcSystem.parseParameters(npcHandler)
local talkState = {}
function onCreatureAppear(cid)
	npcHandler:onCreatureAppear(cid)
end
function onCreatureDisappear(cid)
	npcHandler:onCreatureDisappear(cid)
end
function onCreatureSay(cid, type, msg)
	npcHandler:onCreatureSay(cid, type, msg)
end
function onThink()
	npcHandler:onThink()
end
function creatureSayCallback(cid, type, msg)
	if(not(npcHandler:isFocused(cid))) then
		return false
	end
	
	local talkUser = NPCHANDLER_CONVBEHAVIOR == CONVERSATION_DEFAULT and 0 or cid
	
	if msgcontains(msg, "promot") then
		npcHandler:say("Do you want to be promoted in your vocation for 20000 gold?", cid)
		Topic[talkUser] = 1
	elseif msgcontains(msg, "yes") and Topic[talkUser] == 1 then
		if(getPlayerStorageValue(cid, 30018) == 1) then
			npcHandler:say('You are already promoted.', cid)
		elseif(getPlayerLevel(cid) < 20) then
			npcHandler:say('You need to be at least level 20 in order to be promoted.', cid)
		elseif getPlayerMoney(cid) < 20000 then
			npcHandler:say('You do not have enough money.', cid)
		elseif getConfigInfo("freePremium") == "yes" or isPremium(cid) == TRUE then
			npcHandler:say("Congratulations! You are now promoted. You have learned new spells.", cid)
			local promotedVoc = getPromotedVocation(getPlayerVocation(cid))
			doPlayerSetVocation(cid, promotedVoc)
			doPlayerRemoveMoney(cid, 20000)
		else
			npcHandler:say("You need a premium account in order to promote.", cid)
		end
		Topic[talkUser] = 0
	end
	return true
end

npcHandler:setCallback(CALLBACK_MESSAGE_DEFAULT, creatureSayCallback)
npcHandler:addModule(FocusModule:new())
