local keywordHandler = KeywordHandler:new()
local npcHandler = NpcHandler:new(keywordHandler)
NpcSystem.parseParameters(npcHandler)

local count = {}
local transfer = {}

function onCreatureAppear(cid)              npcHandler:onCreatureAppear(cid)            end
function onCreatureDisappear(cid)           npcHandler:onCreatureDisappear(cid)         end
function onCreatureSay(cid, type, msg)      npcHandler:onCreatureSay(cid, type, msg)    end
function onThink()                          npcHandler:onThink()                        end

local function greetCallback(cid)
	count[cid], transfer[cid] = nil, nil
	return true
end

local topicList = {
	NONE = 0,
	DEPOSIT_GOLD = 1,
	DEPOSIT_CONSENT = 2,
	WITHDRAW_GOLD = 3,
	WITHDRAW_CONSENT = 4,
	TRANSFER_PLAYER_GOLD = 5,
	TRANSFER_PLAYER_WHO = 6,
	TRANSFER_PLAYER_CONSENT = 7,
	CHANGE_GOLD_CHOOSE = 8,
	CHANGE_GOLD_PLATINUM = 9,
	CHANGE_GOLD_PLATINUM_CONSENT = 10,
	CHANGE_PLATINUM_GOLD_OR_CRYSTAL = 11,
	CHANGE_PLATINUM_GOLD = 12,
	CHANGE_PLATINUM_GOLD_CONSENT = 13,
	CHANGE_PLATINUM_CRYSTAL = 14,
	CHANGE_PLATINUM_CRYSTAL_CONSENT = 15,
	CHANGE_CRYSTAL_PLATINUM = 16,
	CHANGE_CRYSTAL_PLATINUM_CONSENT = 17,
}

local function creatureSayCallback(cid, type, msg)
	if not npcHandler:isFocused(cid) then
		return false
	end
	local player = Player(cid)
	if msgcontains(msg, "bank account") then
		npcHandler:say("Would you like to know more about the {basic} functions of your bank account, the {advanced} functions, or are you already bored, perhaps?", cid)
		npcHandler.topic[cid] = topicList.NONE
		return true
	elseif msgcontains(msg, "balance") then
		npcHandler.topic[cid] = topicList.NONE
		if player:getBankBalance() >= 100000000 then
			npcHandler:say("I think you must be one of the richest inhabitants in the world! Your account balance is " .. player:getBankBalance() .. " gold.", cid)
			return true
		elseif player:getBankBalance() >= 10000000 then
			npcHandler:say("You have made ten millions and it still grows! Your account balance is " .. player:getBankBalance() .. " gold.", cid)
			return true
		elseif player:getBankBalance() >= 1000000 then
			npcHandler:say("Wow, you have reached the magic number of a million gp!!! Your account balance is " .. player:getBankBalance() .. " gold!", cid)
			return true
		elseif player:getBankBalance() >= 100000 then
			npcHandler:say("You certainly have made a pretty penny. Your account balance is " .. player:getBankBalance() .. " gold.", cid)
			return true
		else
			npcHandler:say("Your account balance is " .. player:getBankBalance() .. " gold.", cid)
			return true
		end
	elseif msgcontains(msg, "deposit") then
		count[cid] = player:getMoney()
		if count[cid] < 1 then
			npcHandler:say("You do not have enough gold.", cid)
			npcHandler.topic[cid] = topicList.NONE
			return false
		end
		if msgcontains(msg, "all") then
			npcHandler:say("Would you really like to deposit " .. count[cid] .. " gold?", cid)
			npcHandler.topic[cid] = topicList.DEPOSIT_CONSENT
			return true
		else
			if string.match(msg,"%d+") then
				count[cid] = getMoneyCount(msg)
				if count[cid] < 1 then
					npcHandler:say("You do not have enough gold.", cid)
					npcHandler.topic[cid] = topicList.NONE
					return false
				end
				npcHandler:say("Would you really like to deposit " .. count[cid] .. " gold?", cid)
				npcHandler.topic[cid] = topicList.DEPOSIT_CONSENT
				return true
			else
				npcHandler:say("Please tell me how much gold it is you would like to deposit.", cid)
				npcHandler.topic[cid] = topicList.DEPOSIT_GOLD
				return true
			end
		end
		if not isValidMoney(count[cid]) then
			npcHandler:say("Sorry, but you can't deposit that much.", cid)
			npcHandler.topic[cid] = topicList.NONE
			return false
		end
	elseif npcHandler.topic[cid] == topicList.DEPOSIT_GOLD then
		count[cid] = getMoneyCount(msg)
		if isValidMoney(count[cid]) then
			npcHandler:say("Would you really like to deposit " .. count[cid] .. " gold?", cid)
			npcHandler.topic[cid] = topicList.DEPOSIT_CONSENT
			return true
		else
			npcHandler:say("You do not have enough gold.", cid)
			npcHandler.topic[cid] = topicList.NONE
			return true
		end
	elseif npcHandler.topic[cid] == topicList.DEPOSIT_CONSENT then
		if msgcontains(msg, "yes") then
			if player:getMoney() >= tonumber(count[cid]) then
				player:depositMoney(count[cid])
				npcHandler:say("Alright, we have added the amount of " .. count[cid] .. " gold to your {balance}. You can {withdraw} your money anytime you want to.", cid)
			else
				npcHandler:say("You do not have enough gold.", cid)
			end
		elseif msgcontains(msg, "no") then
			npcHandler:say("As you wish. Is there something else I can do for you?", cid)
		end
		npcHandler.topic[cid] = topicList.NONE
		return true
	elseif msgcontains(msg, "withdraw") then
		if string.match(msg,"%d+") then
			count[cid] = getMoneyCount(msg)
			if isValidMoney(count[cid]) then
				npcHandler:say("Are you sure you wish to withdraw " .. count[cid] .. " gold from your bank account?", cid)
				npcHandler.topic[cid] = topicList.WITHDRAW_GOLD
			else
				npcHandler:say("There is not enough gold in your account.", cid)
				npcHandler.topic[cid] = topicList.NONE
			end
			return true
		else
			npcHandler:say("Please tell me how much gold you would like to withdraw.", cid)
			npcHandler.topic[cid] = topicList.WITHDRAW_CONSENT
			return true
		end
	elseif npcHandler.topic[cid] == topicList.WITHDRAW_CONSENT then
		count[cid] = getMoneyCount(msg)
		if isValidMoney(count[cid]) then
			npcHandler:say("Are you sure you wish to withdraw " .. count[cid] .. " gold from your bank account?", cid)
			npcHandler.topic[cid] = topicList.WITHDRAW_GOLD
		else
			npcHandler:say("There is not enough gold in your account.", cid)
			npcHandler.topic[cid] = topicList.NONE
		end
		return true
	elseif npcHandler.topic[cid] == topicList.WITHDRAW_GOLD then
		if msgcontains(msg, "yes") then
			if player:canCarryMoney(count[cid]) then
				if not player:withdrawMoney(count[cid]) then
					npcHandler:say("There is not enough gold in your account.", cid)
				else
					npcHandler:say("Here you are, " .. count[cid] .. " gold. Please let me know if there is something else I can do for you.", cid)
				end
			else
				npcHandler:say("Whoah, hold on, you have no room in your inventory to carry all those coins. I don't want you to drop it on the floor, maybe come back with a cart!", cid)
			end
			npcHandler.topic[cid] = topicList.NONE
		elseif msgcontains(msg, "no") then
			npcHandler:say("The customer is king! Come back anytime you want to if you wish to {withdraw} your money.", cid)
			npcHandler.topic[cid] = topicList.NONE
		end
		return true
	elseif msgcontains(msg, "transfer") then
		local parts = msg:split(" ")

		if #parts < 3 then
			if #parts == 2 then
				-- Immediate topicList.TRANSFER_PLAYER_GOLD simulation
				count[cid] = getMoneyCount(parts[2])
				if player:getBankBalance() < count[cid] then
					npcHandler:say("There is not enough gold in your account.", cid)
					npcHandler.topic[cid] = topicList.NONE
					return true
				end
				if isValidMoney(count[cid]) then
					npcHandler:say("Who would you like transfer " .. count[cid] .. " gold to?", cid)
					npcHandler.topic[cid] = topicList.TRANSFER_PLAYER_WHO
				else
					npcHandler:say("There is not enough gold in your account.", cid)
					npcHandler.topic[cid] = topicList.NONE
				end
			else
				npcHandler:say("Please tell me the amount of gold you would like to transfer.", cid)
				npcHandler.topic[cid] = topicList.TRANSFER_PLAYER_GOLD
			end
		else -- "transfer 250 playerName" or "transfer 250 to playerName"
			local receiver = ""

			local seed = 3
			if #parts > 3 then
				seed = parts[3] == "to" and 4 or 3
			end
			for i = seed, #parts do
				receiver = receiver .. " " .. parts[i]
			end
			receiver = receiver:trim()

			-- Immediate topicList.TRANSFER_PLAYER_GOLD simulation
			count[cid] = getMoneyCount(parts[2])
			if player:getBankBalance() < count[cid] then
				npcHandler:say("There is not enough gold in your account.", cid)
				npcHandler.topic[cid] = topicList.NONE
				return true
			end
			if isValidMoney(count[cid]) then
				-- Immediate topicList.TRANSFER_PLAYER_WHO simulation
				transfer[cid] = getPlayerDatabaseInfo(receiver)
				if player:getName() == transfer[cid].name then
					npcHandler:say("Why would you want to transfer money to yourself? You already have it!", cid)
					npcHandler.topic[cid] = topicList.NONE
					return true
				end

				if transfer[cid] then
					if transfer[cid].vocation == VOCATION_NONE and Player(cid):getVocation() ~= 0 then
						npcHandler:say("I'm afraid this character only holds a junior account at our bank. Do not worry, though. Once he has chosen his vocation, his account will be upgraded.", cid)
						npcHandler.topic[cid] = topicList.NONE
						return true
					end
					npcHandler:say("So you would like to transfer " .. count[cid] .. " gold to " .. transfer[cid].name .. "?", cid)
					npcHandler.topic[cid] = topicList.TRANSFER_PLAYER_CONSENT
				else
					npcHandler:say("This player does not exist.", cid)
					npcHandler.topic[cid] = topicList.NONE
				end
				-- end topicList.TRANSFER_PLAYER_WHO
			else
				npcHandler:say("There is not enough gold in your account.", cid)
				npcHandler.topic[cid] = topicList.NONE
			end
		end
	elseif npcHandler.topic[cid] == topicList.TRANSFER_PLAYER_GOLD then
		count[cid] = getMoneyCount(msg)
		if player:getBankBalance() < count[cid] then
			npcHandler:say("There is not enough gold in your account.", cid)
			npcHandler.topic[cid] = topicList.NONE
			return true
		end
		if isValidMoney(count[cid]) then
			npcHandler:say("Who would you like transfer " .. count[cid] .. " gold to?", cid)
			npcHandler.topic[cid] = topicList.TRANSFER_PLAYER_WHO
		else
			npcHandler:say("There is not enough gold in your account.", cid)
			npcHandler.topic[cid] = topicList.NONE
		end
	elseif npcHandler.topic[cid] == topicList.TRANSFER_PLAYER_WHO then
		transfer[cid] = getPlayerDatabaseInfo(msg)
		if player:getName() == transfer[cid].name then
			npcHandler:say("Fill in this field with person who receives your gold!", cid)
			npcHandler.topic[cid] = topicList.NONE
			return true
		end

		if transfer[cid] then
			if transfer[cid].vocation == VOCATION_NONE and Player(cid):getVocation() ~= 0 then
				npcHandler:say("I'm afraid this character only holds a junior account at our bank. Do not worry, though. Once he has chosen his vocation, his account will be upgraded.", cid)
				npcHandler.topic[cid] = topicList.NONE
				return true
			end
			npcHandler:say("So you would like to transfer " .. count[cid] .. " gold to " .. transfer[cid].name .. "?", cid)
			npcHandler.topic[cid] = topicList.TRANSFER_PLAYER_CONSENT
		else
			npcHandler:say("This player does not exist.", cid)
			npcHandler.topic[cid] = topicList.NONE
		end
	elseif npcHandler.topic[cid] == topicList.TRANSFER_PLAYER_CONSENT then
		if msgcontains(msg, "yes") then
			if not player:transferMoneyTo(transfer[cid], count[cid]) then
				npcHandler:say("You cannot transfer money to this account.", cid)
			else
				npcHandler:say("Very well. You have transfered " .. count[cid] .. " gold to " .. transfer[cid].name ..".", cid)
				transfer[cid] = nil
			end
		elseif msgcontains(msg, "no") then
			npcHandler:say("Alright, is there something else I can do for you?", cid)
		end
		npcHandler.topic[cid] = topicList.NONE
	elseif msgcontains(msg, "change") then
		local parts = msg:split(" ")
		if #parts > 1 then
			if parts[2]:lower() == "gold" then
				npcHandler:say("How many platinum coins would you like to get?", cid)
				npcHandler.topic[cid] = topicList.CHANGE_GOLD_PLATINUM
			elseif parts[2]:lower() == "platinum" then
				npcHandler:say("Would you like to change your platinum coins into {gold} or {crystal}?", cid)
				npcHandler.topic[cid] = topicList.CHANGE_PLATINUM_GOLD_OR_CRYSTAL
			elseif parts[2]:lower() == "crystal" then
				npcHandler:say("How many crystal coins would you like to change into platinum?", cid)
				npcHandler.topic[cid] = topicList.CHANGE_CRYSTAL_PLATINUM
			end
		else
			npcHandler:say("Would you like to change {gold}, {platinum} or {crystal} coins?", cid)
			npcHandler.topic[cid] = topicList.CHANGE_GOLD_CHOOSE
		end
	elseif msgcontains(msg, "gold") and npcHandler.topic[cid] == topicList.CHANGE_GOLD_CHOOSE then
		npcHandler:say("How many platinum coins would you like to get?", cid)
		npcHandler.topic[cid] = topicList.CHANGE_GOLD_PLATINUM
	elseif msgcontains(msg, "platinum") and npcHandler.topic[cid] == topicList.CHANGE_GOLD_CHOOSE then
		npcHandler:say("Would you like to change your platinum coins into gold or crystal?", cid)
		npcHandler.topic[cid] = topicList.CHANGE_PLATINUM_GOLD_OR_CRYSTAL
	elseif msgcontains(msg, "crystal") and npcHandler.topic[cid] == topicList.CHANGE_GOLD_CHOOSE then
		npcHandler:say("How many crystal coins would you like to change into platinum?", cid)
		npcHandler.topic[cid] = topicList.CHANGE_CRYSTAL_PLATINUM
	elseif npcHandler.topic[cid] == topicList.CHANGE_GOLD_PLATINUM then
		if getMoneyCount(msg) < 1 then
			npcHandler:say("Sorry, you do not have enough gold coins.", cid)
			npcHandler.topic[cid] = topicList.NONE
		else
			count[cid] = getMoneyCount(msg)
			npcHandler:say("So you would like me to change " .. count[cid] * 100 .. " of your gold coins into " .. count[cid] .. " platinum coins?", cid)
			npcHandler.topic[cid] = topicList.CHANGE_GOLD_PLATINUM_CONSENT
		end
	elseif npcHandler.topic[cid] == topicList.CHANGE_GOLD_PLATINUM_CONSENT then
		if msgcontains(msg, "yes") then
			if player:removeItem(ITEM_GOLD_COIN, count[cid] * 100) then
				player:addItem(ITEM_PLATINUM_COIN, count[cid])
				npcHandler:say("Here you are.", cid)
			else
				npcHandler:say("Sorry, you do not have enough gold coins.", cid)
			end
		else
			npcHandler:say("Well, can I help you with something else?", cid)
		end
		npcHandler.topic[cid] = topicList.NONE
	elseif npcHandler.topic[cid] == topicList.CHANGE_PLATINUM_GOLD_OR_CRYSTAL then
		if msgcontains(msg, "gold") then
			npcHandler:say("How many platinum coins would you like to change into gold?", cid)
			npcHandler.topic[cid] = topicList.CHANGE_PLATINUM_GOLD
		elseif msgcontains(msg, "crystal") then
			npcHandler:say("How many crystal coins would you like to get?", cid)
			npcHandler.topic[cid] = topicList.CHANGE_PLATINUM_CRYSTAL
		else
			npcHandler:say("Well, can I help you with something else?", cid)
			npcHandler.topic[cid] = topicList.NONE
		end
	elseif npcHandler.topic[cid] == topicList.CHANGE_PLATINUM_GOLD then
		if getMoneyCount(msg) < 1 then
			npcHandler:say("Sorry, you do not have enough platinum coins.", cid)
			npcHandler.topic[cid] = topicList.NONE
		else
			count[cid] = getMoneyCount(msg)
			npcHandler:say("So you would like me to change " .. count[cid] .. " of your platinum coins into " .. count[cid] * 100 .. " gold coins for you?", cid)
			npcHandler.topic[cid] = topicList.CHANGE_PLATINUM_GOLD_CONSENT
		end
	elseif npcHandler.topic[cid] == topicList.CHANGE_PLATINUM_GOLD_CONSENT then
		if msgcontains(msg, "yes") then
			if player:getFreeCapacity() >= getMoneyWeight(count[cid]) then
				if player:removeItem(ITEM_PLATINUM_COIN, count[cid]) then
					player:addItem(ITEM_GOLD_COIN, count[cid] * 100)
					npcHandler:say("Here you are.", cid)
				else
					npcHandler:say("Sorry, you do not have enough platinum coins.", cid)
				end
			else
				npcHandler:say("Whoah, hold on, you have no room in your inventory to carry all those coins. I don't want you to drop it on the floor, maybe come back with a cart!", cid)
			end
		else
			npcHandler:say("Well, can I help you with something else?", cid)
		end
		npcHandler.topic[cid] = topicList.NONE
	elseif npcHandler.topic[cid] == topicList.CHANGE_PLATINUM_CRYSTAL then
		if getMoneyCount(msg) < 1 then
			npcHandler:say("Sorry, you do not have enough platinum coins.", cid)
			npcHandler.topic[cid] = topicList.NONE
		else
			count[cid] = getMoneyCount(msg)
			npcHandler:say("So you would like me to change " .. count[cid] * 100 .. " of your platinum coins into " .. count[cid] .. " crystal coins for you?", cid)
			npcHandler.topic[cid] = topicList.CHANGE_PLATINUM_CRYSTAL_CONSENT
		end
	elseif npcHandler.topic[cid] == topicList.CHANGE_PLATINUM_CRYSTAL_CONSENT then
		if msgcontains(msg, "yes") then
			if player:removeItem(ITEM_PLATINUM_COIN, count[cid] * 100) then
				player:addItem(ITEM_CRYSTAL_COIN, count[cid])
				npcHandler:say("Here you are.", cid)
			else
				npcHandler:say("Sorry, you do not have enough platinum coins.", cid)
			end
		else
			npcHandler:say("Well, can I help you with something else?", cid)
		end
		npcHandler.topic[cid] = topicList.NONE
	elseif npcHandler.topic[cid] == topicList.CHANGE_CRYSTAL_PLATINUM then
		if getMoneyCount(msg) < 1 then
			npcHandler:say("Sorry, you do not have enough crystal coins.", cid)
			npcHandler.topic[cid] = topicList.NONE
		else
			count[cid] = getMoneyCount(msg)
			npcHandler:say("So you would like me to change " .. count[cid] .. " of your crystal coins into " .. count[cid] * 100 .. " platinum coins for you?", cid)
			npcHandler.topic[cid] = topicList.CHANGE_CRYSTAL_PLATINUM_CONSENT
		end
	elseif npcHandler.topic[cid] == topicList.CHANGE_CRYSTAL_PLATINUM_CONSENT then
		if msgcontains(msg, "yes") then
			if player:getFreeCapacity() >= getMoneyWeight(count[cid]) then
				if player:removeItem(ITEM_CRYSTAL_COIN, count[cid]) then
					player:addItem(ITEM_PLATINUM_COIN, count[cid] * 100)
					npcHandler:say("Here you are.", cid)
				else
					npcHandler:say("Sorry, you do not have enough crystal coins.", cid)
				end
			else
				npcHandler:say("Whoah, hold on, you have no room in your inventory to carry all those coins. I don't want you to drop it on the floor, maybe come back with a cart!", cid)
			end
		else
			npcHandler:say("Well, can I help you with something else?", cid)
		end
		npcHandler.topic[cid] = topicList.NONE
	end
	return true
end

keywordHandler:addKeyword({"money"}, StdModule.say,{
	npcHandler = npcHandler,
	text = "We can {change} money for you. You can also access your {bank account}."
})
keywordHandler:addKeyword({"advanced"}, StdModule.say, {
	npcHandler = npcHandler,
	text = "Your bank account will be used automatically when you want to {rent} a house or place an offer on an item on the {market}. Let me know if you want to know about how either one works."
})
keywordHandler:addKeyword({"help"}, StdModule.say, {
	npcHandler = npcHandler,
	text = "You can check the {balance} of your bank account, {deposit} money or {withdraw} it. You can {transfer} money to other characters, provided that they have a vocation, or {change} the coins in your inventory."
})
keywordHandler:addAliasKeyword({'functions'})
keywordHandler:addAliasKeyword({'basic'})
keywordHandler:addKeyword({"job"}, StdModule.say, {
	npcHandler = npcHandler,
	text = "I work in this bank. I can {change} money for you and help you with your {bank account}."
})
keywordHandler:addKeyword({"rent"}, StdModule.say, {
	npcHandler = npcHandler,
	text = "Once you have acquired a house the rent will be charged automatically from your {bank account} every month."
})
keywordHandler:addKeyword({"market"}, StdModule.say, {
	npcHandler = npcHandler,
	text = "If you buy an item from the market, the required gold will be deducted from your bank account automatically. On the other hand, money you earn for selling items via the market will be added to your account. It's easy!"
})

npcHandler:setMessage(MESSAGE_GREET, "Welcome to the bank, |PLAYERNAME|! Need some help with your {bank account}?")
npcHandler:setCallback(CALLBACK_GREET, greetCallback)
npcHandler:setCallback(CALLBACK_MESSAGE_DEFAULT, creatureSayCallback)
npcHandler:addModule(FocusModule:new())
