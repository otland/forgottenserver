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

npcHandler.shopMode = SHOPMODULE_MODE_BOTH
local shopModule = ShopModule:new()
npcHandler:addModule(shopModule)

shopModule:addBuyableItem({'backpack'},		1988, 20, 'backpack')
shopModule:addBuyableItem({'pick'},			2553, 10, 'pick')
shopModule:addBuyableItem({'shovel'},		2554, 20, 'shovel')
shopModule:addBuyableItem({'rope'},			2120, 50, 'rope')

npcHandler.shopMode = SHOPMODULE_MODE_TALK
shopModule:addSellableItem({'rope'},		2120, 7, 'rope')

npcHandler:addModule(FocusModule:new())
