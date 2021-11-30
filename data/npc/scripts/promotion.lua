local keywordHandler = KeywordHandler:new()
local npcHandler = NpcHandler:new(keywordHandler)
NpcSystem.parseParameters(npcHandler)

function onCreatureAppear(cid)              npcHandler:onCreatureAppear(cid)            end
function onCreatureDisappear(cid)           npcHandler:onCreatureDisappear(cid)         end
function onCreatureSay(cid, type, msg)      npcHandler:onCreatureSay(cid, type, msg)    end
function onThink()                          npcHandler:onThink()                        end

local promotion = keywordHandler:addKeyword({'promot'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, text = 'I can promote you for 20000 gold coins. Do you want me to promote you?'})
promotion:addChildKeyword({'yes'}, StdModule.promotePlayer, {npcHandler = npcHandler, cost = 20000, level = 20, text = 'Congratulations! You are now promoted.'})
promotion:addChildKeyword({'no'}, StdModule.say, {npcHandler = npcHandler, onlyFocus = true, text = 'Alright then, come back when you are ready.', reset = true})

local function creatureSayCallback(cid, type, msg)
	if not npcHandler:isFocused(cid) then
		return false
	end

	local player = Player(cid)
	if(msgcontains(msg, "outfit")) or (msgcontains(msg, "addon")) then
		selfSay("In exchange for a truly generous donation, I will offer a special outfit. Do you want to make a donation?", cid)
		npcHandler.topic[cid] = 1
	elseif(msgcontains(msg, "yes")) then
		if npcHandler.topic[cid] == 1 then
			selfSay("Excellent! Now, let me explain. If you donate 1.000.000.000 gold pieces, you will be entitled to wear a unique outfit. ...", cid)
			selfSay("You will be entitled to wear the {armor} for 500.000.000 gold pieces, {helmet} for an additional 250.000.000 and the {boots} for another 250.000.000 gold pieces. ...", cid)
			selfSay("What will it be?", cid)
			npcHandler.topic[cid] = 2
		elseif npcHandler.topic[cid] == 2 then
			selfSay("In that case, return to me once you made up your mind.", cid)
			npcHandler.topic[cid] = 0
		elseif npcHandler.topic[cid] == 3 then
			if player:getStorageValue(Storage.GoldenOutfit) < 1 then
				if player:getTotalMoney() >= 500000000 then
					local storeInbox = player:getStoreInbox()
					if storeInbox then
						local item = Game.createItem(ITEM_DECORATION_KIT, 1)
						local decoKitName = ItemType(34156):getName()
						if item then
							item:setStoreItem(true)
							item:setAttribute("wrapid", 34156)
							item:setAttribute(ITEM_ATTRIBUTE_DESCRIPTION, "Unwrap it in your own house to create a " .. decoKitName .. ".")
							storeInbox:addItemEx(item)
						end

						selfSay("Take this armor as a token of great gratitude. Let us forever remember this day, my friend!", cid)

						player:removeTotalMoney(500000000)
						player:addOutfit(1211)
						player:addOutfit(1210)
						player:getPosition():sendMagicEffect(CONST_ME_EARLY_THUNDER)
						player:setStorageValue(Storage.GoldenOutfit, 1)
					else
						selfSay("Please make sure you have free slots in your store inbox.", cid)
					end
				else
					selfSay("You do not have enough money to donate that amount.", cid)
				end
			else
				selfSay("You alread have that addon.", cid)
			end
			npcHandler.topic[cid] = 2
		elseif npcHandler.topic[cid] == 4 then
			if player:getStorageValue(Storage.GoldenOutfit) == 1 then
				if player:getStorageValue(Storage.GoldenOutfit) < 2 then
					if player:getTotalMoney() >= 250000000 then
						selfSay("Take this helmet as a token of great gratitude. Let us forever remember this day, my friend.", cid)

						player:removeTotalMoney(250000000)
						player:addOutfitAddon(1210, 2)
						player:addOutfitAddon(1211, 2)
						player:getPosition():sendMagicEffect(CONST_ME_EARLY_THUNDER)
						player:setStorageValue(Storage.GoldenOutfit, 2)
						npcHandler.topic[cid] = 2
					else
						selfSay("You do not have enough money to donate that amount.", cid)
						npcHandler.topic[cid] = 2
					end
				else
					selfSay("You alread have that outfit.", cid)
					npcHandler.topic[cid] = 2
				end
			else
				selfSay("You need to donate {armor} outfit first.", cid)
				npcHandler.topic[cid] = 2
			end
			npcHandler.topic[cid] = 2
		elseif npcHandler.topic[cid] == 5 then
			if player:getStorageValue(Storage.GoldenOutfit) == 2 then
				if player:getStorageValue(Storage.GoldenOutfit) < 3 then
					if player:getTotalMoney() >= 250000000 then
						selfSay("Take this boots as a token of great gratitude. Let us forever remember this day, my friend.", cid)

						player:removeTotalMoney(250000000)
						player:addOutfitAddon(1210, 1)
						player:addOutfitAddon(1211, 1)
						player:getPosition():sendMagicEffect(CONST_ME_EARLY_THUNDER)
						player:setStorageValue(Storage.GoldenOutfit, 3)
						npcHandler.topic[cid] = 2
					else
						selfSay("You do not have enough money to donate that amount.", cid)
						npcHandler.topic[cid] = 2
					end
				else
					selfSay("You alread have that outfit.", cid)
					npcHandler.topic[cid] = 2
				end
			else
				selfSay("You need to donate {helmet} addon first.", cid)
				npcHandler.topic[cid] = 2
			end
			npcHandler.topic[cid] = 2
		end
	elseif(msgcontains(msg, "armor")) and npcHandler.topic[cid] == 2 then
		selfSay("So you wold like to donate 500.000.000 gold pieces which in return will entitle you to wear a unique armor?", cid)
		npcHandler.topic[cid] = 3
	elseif(msgcontains(msg, "helmet")) and npcHandler.topic[cid] == 2 then
		selfSay("So you would like to donate 250.000.000 gold pieces which in return will entitle you to wear unique helmet?", cid)
		npcHandler.topic[cid] = 4
	elseif(msgcontains(msg, "boots")) and npcHandler.topic[cid] == 2 then
		selfSay("So you would like to donate 250.000.000 gold pieces which in return will entitle you to wear a unique boots?", cid)
		npcHandler.topic[cid] = 5
	end
end

npcHandler:setCallback(CALLBACK_MESSAGE_DEFAULT, creatureSayCallback)
npcHandler:addModule(FocusModule:new())
