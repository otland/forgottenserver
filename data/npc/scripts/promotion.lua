local keywordHandler = KeywordHandler:new()
local npcHandler = NpcHandler:new(keywordHandler)
NpcSystem.parseParameters(npcHandler)

function onCreatureAppear(cid)				npcHandler:onCreatureAppear(cid)			end
function onCreatureDisappear(cid)			npcHandler:onCreatureDisappear(cid)			end
function onCreatureSay(cid, type, msg)		npcHandler:onCreatureSay(cid, type, msg)	end
function onThink()							npcHandler:onThink()						end

local node1 = keywordHandler:addKeyword({'promot'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, text = 'I can promote you for 20000 gold coins. Do you want me to promote you?'})
	node1:addChildKeyword({'yes'}, StdModule.promotePlayer, {npcHandler = npcHandler, cost = 20000, level = 20, text = 'Congratulations! You are now promoted.'})
	node1:addChildKeyword({'no'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, text = 'Alright then, come back when you are ready.', reset = true})

local function creatureSayCallback(cid, type, msg)
	if not npcHandler:isFocused(cid) then
		return false
	end

	local player = Player(cid)
	if msgcontains(msg, "outfit") or msgcontains(msg, "addon") then
		npcHandler:say("In exchange for a truly generous donation, I will offer a special outfit. Do you want to make a donation?", cid)
		npcHandler.topic[cid] = 1
	elseif msgcontains(msg, "yes") then
		if npcHandler.topic[cid] == 1 then
			npcHandler:say({
				"Excellent! Now, let me explain. If you donate 1.000.000.000 gold pieces, you will be entitled to wear a unique outfit. ...",
				"You will be entitled to wear the {armor} for 500.000.000 gold pieces, {helmet} for an additional 250.000.000 and the {boots} for another 250.000.000 gold pieces. ...",
				"What will it be?"
			}, cid)
			npcHandler.topic[cid] = 2
		elseif npcHandler.topic[cid] == 2 then
			npcHandler:say("In that case, return to me once you made up your mind.", cid)
			npcHandler.topic[cid] = 0
		elseif npcHandler.topic[cid] == 3 then
			if player:getStorageValue(PlayerStorageKeys.goldenOutfit) < 1 then
				if player:getTotalMoney() >= 500000000 then
					local storeInbox = player:getStoreInbox()
					if storeInbox then
						local item = Game.createItem(ITEM_DECORATION_KIT, 1)
						if item then
							item:setStoreItem(true)
							item:setAttribute("wrapid", 34156)
							item:setAttribute(ITEM_ATTRIBUTE_DESCRIPTION, "Unwrap it in your own house to create a " .. ItemType(34156):getName() .. ".")
							storeInbox:addItemEx(item)
						end

						npcHandler:say("Take this armor as a token of great gratitude. Let us forever remember this day, my friend!", cid)
						player:removeTotalMoney(500000000)
						player:addOutfit(1211)
						player:addOutfit(1210)
						player:getPosition():sendMagicEffect(CONST_ME_EARLY_THUNDER)
						player:setStorageValue(PlayerStorageKeys.goldenOutfit, 1)
						npcHandler.topic[cid] = 2
					end
				else
					npcHandler:say("You do not have enough money to donate that amount.", cid)
					npcHandler.topic[cid] = 2
				end
			else
				npcHandler:say("You already own this outfit.", cid)
				npcHandler.topic[cid] = 0
			end
			npcHandler.topic[cid] = 2
		elseif npcHandler.topic[cid] == 4 then
			if player:getStorageValue(PlayerStorageKeys.goldenOutfit) == 1 then
				if player:getStorageValue(PlayerStorageKeys.goldenOutfit) < 2 then
					if player:getTotalMoney() >= 250000000 then
						npcHandler:say("Take this helmet as a token of great gratitude. Let us forever remember this day, my friend.", cid)
						player:removeTotalMoney(250000000)
						player:addOutfitAddon(1210, 2)
						player:addOutfitAddon(1211, 2)
						player:getPosition():sendMagicEffect(CONST_ME_EARLY_THUNDER)
						player:setStorageValue(PlayerStorageKeys.goldenOutfit, 2)
						npcHandler.topic[cid] = 2
					else
						npcHandler:say("You do not have enough money to donate that amount.", cid)
						npcHandler.topic[cid] = 2
					end
				else
					npcHandler:say("You already own this addon.", cid)
					npcHandler.topic[cid] = 2
				end
			else
				npcHandler:say("You need to donate for the {armor} outfit first.", cid)
				npcHandler.topic[cid] = 2
			end
			npcHandler.topic[cid] = 2
		elseif npcHandler.topic[cid] == 5 then
			if player:getStorageValue(PlayerStorageKeys.goldenOutfit) == 2 then
				if player:getStorageValue(PlayerStorageKeys.goldenOutfit) < 3 then
					if player:getTotalMoney() >= 250000000 then
						npcHandler:say("Take this boots as a token of great gratitude. Let us forever remember this day, my friend.", cid)
						player:removeTotalMoney(250000000)
						player:addOutfitAddon(1210, 1)
						player:addOutfitAddon(1211, 1)
						player:getPosition():sendMagicEffect(CONST_ME_EARLY_THUNDER)
						player:setStorageValue(PlayerStorageKeys.goldenOutfit, 3)
						npcHandler.topic[cid] = 2
					else
						npcHandler:say("You do not have enough money to donate that amount.", cid)
						npcHandler.topic[cid] = 2
					end
				else
					npcHandler:say("You already own this addon.", cid)
					npcHandler.topic[cid] = 2
				end
			else
				npcHandler:say("You need to donate for the {helmet} addon first.", cid)
				npcHandler.topic[cid] = 2
			end
			npcHandler.topic[cid] = 2
		end
	elseif msgcontains(msg, "armor") and npcHandler.topic[cid] == 2 then
		npcHandler:say("So you would like to donate 500.000.000 gold pieces which in return will entitle you to wear a unique armor?", cid)
		npcHandler.topic[cid] = 3
	elseif msgcontains(msg, "helmet") and npcHandler.topic[cid] == 2 then
		npcHandler:say("So you would like to donate 250.000.000 gold pieces which in return will entitle you to wear a unique helmet?", cid)
		npcHandler.topic[cid] = 4
	elseif msgcontains(msg, "boots") and npcHandler.topic[cid] == 2 then
		npcHandler:say("So you would like to donate 250.000.000 gold pieces which in return will entitle you to wear a unique boots?", cid)
		npcHandler.topic[cid] = 5
	end
end

npcHandler:setCallback(CALLBACK_MESSAGE_DEFAULT, creatureSayCallback)
npcHandler:addModule(FocusModule:new())
