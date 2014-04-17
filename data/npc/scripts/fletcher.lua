local keywordHandler = KeywordHandler:new()
local npcHandler = NpcHandler:new(keywordHandler)
NpcSystem.parseParameters(npcHandler)

Npc():setSpeechBubble(2)
function onCreatureAppear(cid)			npcHandler:onCreatureAppear(cid)			end
function onCreatureDisappear(cid)		npcHandler:onCreatureDisappear(cid)			end
function onCreatureSay(cid, type, msg)	npcHandler:onCreatureSay(cid, type, msg)	end
function onThink()						npcHandler:onThink()						end
function onPlayerEndTrade(cid)			npcHandler:onPlayerEndTrade(cid)			end
function onPlayerCloseChannel(cid)		npcHandler:onPlayerCloseChannel(cid)		end

local shopModule = ShopModule:new()
npcHandler:addModule(shopModule)
shopModule:addBuyableItem({'crossbow'},		2455, 150, 'crossbow')
shopModule:addBuyableItem({'bow'},			2456, 130, 'bow')

shopModule:addSellableItem({'crossbow'},	2455, 360, 'crossbow')
shopModule:addSellableItem({'bow'},			2456, 200, 'bow')
shopModule:addSellableItem({'spear'},		2389, 20, 'spear')
shopModule:addSellableItem({'poison arrow'}, 2545, 18, 'poison arrow')
shopModule:addSellableItem({'bolt'},		2543, 3, 'bolt')
shopModule:addSellableItem({'arrow'},		2544, 2, 'arrow')

npcHandler:addModule(FocusModule:new())
