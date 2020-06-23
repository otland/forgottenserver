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

local function creatureSayCallback(cid, type, msg)
	if not npcHandler:isFocused(cid) then
		return false
	end
	local player = Player(cid)
	local guild = player:getGuild()
	if not guild then
		npcHandler:say("I'm too busy serving guilds, perhaps my colleague the {Banker} can assist you with your personal bank account.", cid)
		npcHandler.topic[cid] = 0
		return true
	end
	if msgcontains(msg, "balance") then
		npcHandler.topic[cid] = 0
		npcHandler:say("The {guild account} balance of " .. guild:getName() .. " is " .. guild:getBankBalance() .. " gold.", cid)
		return true
	elseif msgcontains(msg, "deposit") then
		count[cid] = player:getBankBalance()
		if count[cid] < 1 then
			npcHandler:say("Your {personal} bank account looks awefully empty, please deposit money there first, I don't like dealing with heavy coins.", cid)
			npcHandler.topic[cid] = 0
			return false
		end
		if string.match(msg,"%d+") then
			count[cid] = getMoneyCount(msg)
			if count[cid] > 0 and count[cid] <= player:getBankBalance() then
				npcHandler:say("Would you really like to deposit " .. count[cid] .. " gold to the guild " .. guild:getName() .. "?", cid)
				npcHandler.topic[cid] = 2
				return true
			else
				npcHandler:say("You cannot afford to deposit " .. count[cid] .. " gold to the guild " .. guild:getName() .. ". You only have " .. player:getBankBalance() .. " gold in your account!", cid)
				npcHandler.topic[cid] = 0
				return false
			end
		else
			npcHandler:say("Please tell me how much gold it is you would like to deposit.", cid)
			npcHandler.topic[cid] = 1
			return true
		end
	elseif npcHandler.topic[cid] == 1 then
		count[cid] = getMoneyCount(msg)
		if count[cid] > 0 and count[cid] <= player:getBankBalance() then
			npcHandler:say("Would you really like to deposit " .. count[cid] .. " gold to the guild " .. guild:getName() .. "?", cid)
			npcHandler.topic[cid] = 2
			return true
		else
			npcHandler:say("You do not have enough gold.", cid)
			npcHandler.topic[cid] = 0
			return true
		end
	elseif npcHandler.topic[cid] == 2 then
		if msgcontains(msg, "yes") then
			local deposit = tonumber(count[cid])
			if deposit > 0 and player:getBankBalance() >= deposit then
				player:setBankBalance(player:getBankBalance() - deposit)
				player:save()
				guild:setBankBalance(guild:getBankBalance() + deposit)
				npcHandler:say("Alright, we have added the amount of " .. deposit .. " gold to the guild " .. guild:getName() .. ".", cid)
				local currentTime = os.time()
				local insertData = table.concat({
					guild:getId(),
					player:getGuid(),
					"'DEPOSIT'",
					deposit,
					currentTime
				},',')
				db.query("INSERT INTO `guild_transactions` (`to_guild`,`player_id`,`type`,`balance`,`time`) VALUES ("..insertData..");")
				local receipt = Game.createItem(ITEM_RECEIPT_SUCCESS, 1)
				receipt:setAttribute(ITEM_ATTRIBUTE_TEXT, "Date: " .. os.date("%d. %b %Y - %H:%M:%S", currentTime) .. "\nType: Guild Deposit\nGold Amount: " .. deposit .. "\nReceipt Owner: " .. player:getName() .. "\nRecipient: The " .. guild:getName() .. "\n\nWe are happy to inform you that your transfer request was successfully carried out.")
				player:addItemEx(receipt)
			else
				npcHandler:say("You do not have enough gold.", cid)
			end
		elseif msgcontains(msg, "no") then
			npcHandler:say("As you wish. Is there something else I can do for you?", cid)
		end
		npcHandler.topic[cid] = 0
		return true
	elseif msgcontains(msg, "withdraw") then
		if string.match(msg,"%d+") then
			count[cid] = getMoneyCount(msg)
			if count[cid] > 0 and count[cid] <= guild:getBankBalance() then
				npcHandler:say("Are you sure you wish to withdraw " .. count[cid] .. " gold from the guild " .. guild:getName() .. "?", cid)
				npcHandler.topic[cid] = 7
			else
				npcHandler:say("There is not enough gold in the guild " .. guild:getName() .. ". Their available balance is currently " .. guild:getBankBalance() .. ".", cid)
				npcHandler.topic[cid] = 0
			end
			return true
		else
			npcHandler:say("Please tell me how much gold you would like to withdraw.", cid)
			npcHandler.topic[cid] = 6
			return true
		end
	elseif npcHandler.topic[cid] == 6 then
		count[cid] = getMoneyCount(msg)
		if count[cid] > 0 and count[cid] <= guild:getBankBalance() then
			npcHandler:say("Are you sure you wish to withdraw " .. count[cid] .. " gold from the guild " .. guild:getName() .. "?", cid)
			npcHandler.topic[cid] = 7
		else
			npcHandler:say("There is not enough gold in the guild " .. guild:getName() .. ". Their available balance is currently " .. guild:getBankBalance() .. ".", cid)
			npcHandler.topic[cid] = 0
		end
		return true
	elseif npcHandler.topic[cid] == 7 then
		if msgcontains(msg, "yes") then
			local withdraw = count[cid]
			if withdraw > 0 and withdraw <= guild:getBankBalance() then
				if player:getGuid() == guild:getOwnerGUID() or player:getGuildLevel() == 2 then
					guild:setBankBalance(guild:getBankBalance() - withdraw)
					player:setBankBalance(player:getBankBalance() + withdraw)
					player:save()
					npcHandler:say("Alright, we have removed the amount of " .. withdraw .. " gold from the guild " .. guild:getName() .. ", and added it to your {personal} account.", cid)
					local currentTime = os.time()
					local insertData = table.concat({
						guild:getId(),
						player:getGuid(),
						"'WITHDRAW'",
						withdraw,
						currentTime
					},',')
					db.query("INSERT INTO `guild_transactions` (`to_guild`,`player_id`,`type`,`balance`,`time`) VALUES ("..insertData..");")
					local receipt = Game.createItem(ITEM_RECEIPT_SUCCESS, 1)
					receipt:setAttribute(ITEM_ATTRIBUTE_TEXT, "Date: " .. os.date("%d. %b %Y - %H:%M:%S", currentTime) .. "\nType: Guild Withdraw\nGold Amount: " .. withdraw .. "\nReceipt Owner: " .. player:getName() .. "\nRecipient: The " .. guild:getName() .. "\n\nWe are happy to inform you that your transfer request was successfully carried out.")
					player:addItemEx(receipt)
				else
					npcHandler:say("Sorry, you are not authorized for withdrawals. Only Leaders and Vice-leaders are allowed to withdraw funds from guild accounts.", cid)
				end
			else
				npcHandler:say("There is not enough gold in the guild " .. guild:getName() .. ". Their available balance is currently " .. guild:getBankBalance() .. ".", cid)
			end
			npcHandler.topic[cid] = 0
		elseif msgcontains(msg, "no") then
			npcHandler:say("Come back anytime you want to if you wish to {withdraw} your money.", cid)
			npcHandler.topic[cid] = 0
		end
		return true
	elseif msgcontains(msg, "transfer") then
		local parts = msg:split(" ")

		if #parts < 3 then
			if #parts == 2 then
				-- Immediate topic 11 simulation
				count[cid] = getMoneyCount(parts[2])
				if count[cid] < 0 or guild:getBankBalance() < count[cid] then
					npcHandler:say("There is not enough gold in your guild account.", cid)
					npcHandler.topic[cid] = 0
					return true
				end
				npcHandler:say("Who would you like transfer " .. count[cid] .. " gold to?", cid)
				npcHandler.topic[cid] = 12
			else
				npcHandler:say("Please tell me the amount of gold you would like to transfer.", cid)
				npcHandler.topic[cid] = 11
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

			-- Immediate topic 11 simulation
			count[cid] = getMoneyCount(parts[2])
			if count[cid] < 0 or guild:getBankBalance() < count[cid] then
				npcHandler:say("There is not enough gold in your guild account.", cid)
				npcHandler.topic[cid] = 0
				return true
			end
			-- Topic 12
			transfer[cid] = getPlayerDatabaseInfo(receiver)
			if player:getName() == transfer[cid].name then
				npcHandler:say("Fill in this field with person who receives your gold!", cid)
				npcHandler.topic[cid] = 0
				return true
			end

			if transfer[cid] then
				if transfer[cid].vocation == VOCATION_NONE and Player(cid):getVocation() ~= 0 then
					npcHandler:say("I'm afraid this character only holds a junior account at our bank. Do not worry, though. Once he has chosen his vocation, his account will be upgraded.", cid)
					npcHandler.topic[cid] = 0
					return true
				end
				npcHandler:say("So you would like to transfer " .. count[cid] .. " gold from the guild " .. guild:getName() .. " to " .. transfer[cid].name .. "?", cid)
				npcHandler.topic[cid] = 13
			else
				npcHandler:say("This player does not exist.", cid)
				npcHandler.topic[cid] = 0
			end
		end
		return true
	elseif npcHandler.topic[cid] == 11 then
		count[cid] = getMoneyCount(msg)
		if count[cid] < 0 or guild:getBankBalance() < count[cid] then
			npcHandler:say("There is not enough gold in your guild account.", cid)
			npcHandler.topic[cid] = 0
			return true
		end
		npcHandler:say("Who would you like transfer " .. count[cid] .. " gold to?", cid)
		npcHandler.topic[cid] = 12
	elseif npcHandler.topic[cid] == 12 then
		transfer[cid] = getPlayerDatabaseInfo(msg)
		if player:getName() == transfer[cid].name then
			npcHandler:say("Fill in this field with person who receives your gold!", cid)
			npcHandler.topic[cid] = 0
			return true
		end

		if transfer[cid] then
			if transfer[cid].vocation == VOCATION_NONE and Player(cid):getVocation() ~= 0 then
				npcHandler:say("I'm afraid this character only holds a junior account at our bank. Do not worry, though. Once he has chosen his vocation, his account will be upgraded.", cid)
				npcHandler.topic[cid] = 0
				return true
			end
			npcHandler:say("So you would like to transfer " .. count[cid] .. " gold from the guild " .. guild:getName() .. " to " .. transfer[cid].name .. "?", cid)
			npcHandler.topic[cid] = 13
		else
			npcHandler:say("This player does not exist.", cid)
			npcHandler.topic[cid] = 0
		end
	elseif npcHandler.topic[cid] == 13 then
		if msgcontains(msg, "yes") then
			if not transfer[cid] or count[cid] < 1 or count[cid] > guild:getBankBalance() then
				transfer[cid] = nil
				npcHandler:say("Your guild account has cant afford this transfer.", cid)
				npcHandler.topic[cid] = 0
				return true
			end

			guild:setBankBalance(guild:getBankBalance() - count[cid])
			player:setBankBalance(player:getBankBalance() + count[cid])
			if not player:transferMoneyTo(transfer[cid], count[cid]) then
				npcHandler:say("You cannot transfer money to this account.", cid)
				player:setBankBalance(player:getBankBalance() - count[cid])
			else
				npcHandler:say("Very well. You have transfered " .. count[cid] .. " gold to " .. transfer[cid].name ..".", cid)
				transfer[cid] = nil
			end
		elseif msgcontains(msg, "no") then
			npcHandler:say("Alright, is there something else I can do for you?", cid)
		end
		npcHandler.topic[cid] = 0
	elseif msgcontains(msg, "ledger") then
		if player:getGuid() ~= guild:getOwnerGUID() then
			npcHandler.topic[cid] = 0
			npcHandler:say("Sorry, this is confidential between me and your Guild Leader!", cid)
			return true
		end
		npcHandler.topic[cid] = 14
		npcHandler:say("To your advantage, I'm a man who got his papers sorted out. I have ledger records of all transaction requests for your {guild account}. Would you like to get a copy?", cid)
		return true
	elseif msgcontains(msg, "yes") and npcHandler.topic[cid] == 14 then
		local dbTransactions = db.storeQuery([[
			SELECT
				`g`.`name` as `to_guild_name`,
				`g2`.`name` as `from_guild_name`,
				`p`.`name` as `player_name`,
				`t`.`type`,
				`t`.`balance`,
				`t`.`time`
			FROM `guild_transactions` as `t`
			JOIN `guilds` as `g`
				ON `t`.`to_guild` = `g`.`id`
			LEFT JOIN `guilds` as `g2`
				ON `t`.`from_guild` = `g2`.`id`
			LEFT JOIN `players` as `p`
				ON `t`.`player_id` = `p`.`id`
			WHERE `to_guild` = ]] .. guild:getId() .. [[
			ORDER BY `t`.`time` DESC
		]])
		local ledger_text = "Ledger Date: " .. os.date("%d. %b %Y - %H:%M:%S", os.time()) .. ".\nOfficial ledger for Guild: " .. guild:getName() .. ".\nGuild balance: " .. guild:getBankBalance() .. ".\n\n"
		local records = {}

		if dbTransactions ~= false then
			repeat
				local to_guild_name = result.getString(dbTransactions, 'to_guild_name')
				local from_guild_name = result.getString(dbTransactions, 'from_guild_name')
				local player_name = result.getString(dbTransactions, 'player_name')
				local type = (result.getString(dbTransactions, 'type') == "WITHDRAW" and "Withdraw" or "Deposit")
				local balance = result.getNumber(dbTransactions, 'balance')
				local time = result.getNumber(dbTransactions, 'time')

	            table.insert(records, "Date: " .. os.date("%d. %b %Y - %H:%M:%S", time) .. "\nType: Guild "..type.."\nGold Amount: " .. balance .. "\nReceipt Owner: " .. player_name .. "\nRecipient: The " .. to_guild_name)

			until not result.next(dbTransactions)
			result.free(dbTransactions)
	    else -- No transactions exist
			npcHandler.topic[cid] = 0
			npcHandler:say("Ohh, your ledger is actually empty. You should start using your {guild account}!", cid)
			return true
		end

		local ledger = Game.createItem(ITEM_DOCUMENT_RO, 1)
		ledger:setAttribute(ITEM_ATTRIBUTE_TEXT, ledger_text .. table.concat(records, "\n\n"))
		player:addItemEx(ledger)

		npcHandler.topic[cid] = 0
		npcHandler:say("Here is your ledger "..player:getName()..". Feel free to come back anytime should you need an updated copy.", cid)

		return true
	elseif msgcontains(msg, "no") and npcHandler.topic[cid] == 14 then
		npcHandler.topic[cid] = 0
		npcHandler:say("No worries, I will keep it updated for a later date then.", cid)
		return true
	end
	return true
end

keywordHandler:addKeyword({"help"}, StdModule.say, {
	npcHandler = npcHandler,
	text = "You can check the {balance} of your guild account and {deposit} money to it. Guild Leaders and Vice-leaders can also {withdraw}, Guild Leaders can {transfer} money to other guilds and check their guild {ledger}."
})
keywordHandler:addAliasKeyword({'money'})
keywordHandler:addAliasKeyword({'guild account'})

keywordHandler:addKeyword({"job"}, StdModule.say, {
	npcHandler = npcHandler,
	text = "I work in this bank. I can {help} you with your {guild account}."
})
keywordHandler:addAliasKeyword({'functions'})
keywordHandler:addAliasKeyword({'basic'})

keywordHandler:addKeyword({"rent"}, StdModule.say, {
	npcHandler = npcHandler,
	text = "Once you have acquired a guildhall the rent will be charged automatically from your {guild account} every month."
})

keywordHandler:addKeyword({"personal"}, StdModule.say, {
	npcHandler = npcHandler,
	text = "Head over to my colleague known as {Banker}, he will help you get your funds into your own bank account."
})

keywordHandler:addKeyword({"banker"}, StdModule.say, {
	npcHandler = npcHandler,
	text = "Banker is my colleague, he loves flipping coins between his fingers. He will help you exchange money, check your balance and help you withdraw and deposit your funds."
})

npcHandler:setMessage(MESSAGE_GREET, "Welcome to the bank, |PLAYERNAME|! Need some help with your {guild account}?")
npcHandler:setCallback(CALLBACK_GREET, greetCallback)
npcHandler:setCallback(CALLBACK_MESSAGE_DEFAULT, creatureSayCallback)
npcHandler:addModule(FocusModule:new())
