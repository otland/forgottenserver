local keywordHandler = KeywordHandler:new()
local npcHandler = NpcHandler:new(keywordHandler)
NpcSystem.parseParameters(npcHandler)

local vocation = {}
local town = {}
local destination = {}

function onCreatureAppear(cid)              npcHandler:onCreatureAppear(cid)            end
function onCreatureDisappear(cid)           npcHandler:onCreatureDisappear(cid)         end
function onCreatureSay(cid, type, msg)      npcHandler:onCreatureSay(cid, type, msg)    end
function onThink()                          npcHandler:onThink()                        end

local function greetCallback(cid)
	local player = Player(cid)
	local level = player:getLevel()
	if level < 8 then
		npcHandler:say("CHILD! COME BACK WHEN YOU HAVE GROWN UP!", cid)
		return false
	elseif level > 9 then
		npcHandler:say(player:getName() .. ", I CAN'T LET YOU LEAVE - YOU ARE TOO STRONG ALREADY! YOU CAN ONLY LEAVE WITH LEVEL 9 OR LOWER.", cid)
		return false
	elseif player:getVocation():getId() > 0 then
		npcHandler:say("YOU ALREADY HAVE A VOCATION!", cid)
		return false
	end
	return true
end

local function creatureSayCallback(cid, type, msg)
	if not npcHandler:isFocused(cid) then
		return false
	end

	if msgcontains(msg, "yes") and npcHandler.topic[cid] == 0 then
		npcHandler:say("IN WHICH TOWN DO YOU WANT TO LIVE: {RHYVES}?", cid)
		npcHandler.topic[cid] = 1
	elseif npcHandler.topic[cid] == 1 then
		if msgcontains(msg, "rhyves") then
			town[cid] = 2
			destination[cid] = Position(159, 387, 6)
			npcHandler:say("IN RHYVES! AND WHAT PROFESSION HAVE YOU CHOSEN: {KNIGHT}, {PALADIN}, {SORCERER}, OR {DRUID}?", cid)
			npcHandler.topic[cid] = 2
		else
			npcHandler:say("IN WHICH TOWN DO YOU WANT TO LIVE: {RHYVES}?", cid)
		end
	elseif npcHandler.topic[cid] == 2 then
		if msgcontains(msg, "sorcerer") then
			npcHandler:say("A SORCERER! ARE YOU SURE? THIS DECISION IS IRREVERSIBLE!", cid)
			npcHandler.topic[cid] = 3
			vocation[cid] = 1
		elseif msgcontains(msg, "druid") then
			npcHandler:say("A DRUID! ARE YOU SURE? THIS DECISION IS IRREVERSIBLE!", cid)
			npcHandler.topic[cid] = 3
			vocation[cid] = 2
		elseif msgcontains(msg, "paladin") then
			npcHandler:say("A PALADIN! ARE YOU SURE? THIS DECISION IS IRREVERSIBLE!", cid)
			npcHandler.topic[cid] = 3
			vocation[cid] = 3
		elseif msgcontains(msg, "knight") then
			npcHandler:say("A KNIGHT! ARE YOU SURE? THIS DECISION IS IRREVERSIBLE!", cid)
			npcHandler.topic[cid] = 3
			vocation[cid] = 4
		else
			npcHandler:say("{KNIGHT}, {PALADIN}, {SORCERER}, OR {DRUID}?", cid)
		end
	elseif npcHandler.topic[cid] == 3 then
		if msgcontains(msg, "yes") then
			local player = Player(cid)
			npcHandler:say("SO BE IT!", cid)
			player:setVocation(Vocation(vocation[cid]))
			player:setTown(Town(town[cid]))

			local destination = destination[cid]
			npcHandler:releaseFocus(cid)
			player:teleportTo(destination)
			player:getPosition():sendMagicEffect(CONST_ME_TELEPORT)
			destination:sendMagicEffect(CONST_ME_TELEPORT)
		else
			npcHandler:say("THEN WHAT? {KNIGHT}, {PALADIN}, {SORCERER}, OR {DRUID}?", cid)
			npcHandler.topic[cid] = 2
		end
	end
	return true
end

local function onAddFocus(cid)
	town[cid] = 0
	vocation[cid] = 0
	destination[cid] = 0
end

local function onReleaseFocus(cid)
	town[cid] = nil
	vocation[cid] = nil
	destination[cid] = nil
end

npcHandler:setCallback(CALLBACK_ONADDFOCUS, onAddFocus)
npcHandler:setCallback(CALLBACK_ONRELEASEFOCUS, onReleaseFocus)

npcHandler:setCallback(CALLBACK_GREET, greetCallback)
npcHandler:setCallback(CALLBACK_MESSAGE_DEFAULT, creatureSayCallback)
npcHandler:addModule(FocusModule:new())
