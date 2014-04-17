local keywordHandler = KeywordHandler:new()
local npcHandler = NpcHandler:new(keywordHandler)
NpcSystem.parseParameters(npcHandler)
local talkState = {}

function onCreatureAppear(cid)			npcHandler:onCreatureAppear(cid)			end
function onCreatureDisappear(cid)		npcHandler:onCreatureDisappear(cid)			end
function onCreatureSay(cid, type, msg)	npcHandler:onCreatureSay(cid, type, msg)	end
function onThink()						npcHandler:onThink()						end
function onPlayerEndTrade(cid)			npcHandler:onPlayerEndTrade(cid)			end
function onPlayerCloseChannel(cid)		npcHandler:onPlayerCloseChannel(cid)		end

function thinkCallback()
	if math.random(100000) < 1300 then
		Npc():say("Hey, send a letter to your friend now and then. Keep in touch, you know.", TALKTYPE_YELL)
	end
	return true
end

local shopModule = ShopModule:new()
npcHandler:addModule(shopModule)
shopModule:addBuyableItem({'Parcel'},		ITEM_PARCEL, 15, 'Parcel')
shopModule:addBuyableItem({'Letter'},		ITEM_LETTER, 8, 'Letter')
shopModule:addBuyableItem({'Label'},		ITEM_LABEL, 1, 'Label')

npcHandler:setMessage(MESSAGE_GREET, "Hello. How may I help you |PLAYERNAME|? Ask me for a {trade} if you want to buy something.")
npcHandler:setMessage(MESSAGE_SENDTRADE, "Sure, take a look. You may buy {parcels}, {labels} and {letters} here.")

npcHandler:setCallback(CALLBACK_ONTHINK, thinkCallback)
npcHandler:addModule(FocusModule:new())
