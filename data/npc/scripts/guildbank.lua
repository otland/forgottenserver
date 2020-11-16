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
	TRANSFER_TYPE = 5,
	TRANSFER_PLAYER_GOLD = 6,
	TRANSFER_PLAYER_WHO = 7,
	TRANSFER_PLAYER_CONSENT = 8,
	TRANSFER_GUILD_GOLD = 9,
	TRANSFER_GUILD_WHO = 10,
	TRANSFER_GUILD_CONSENT = 11,
	LEDGER_CONSENT = 12
}

local function creatureSayCallback(cid, type, msg)
	if not npcHandler:isFocused(cid) then
		return false
	end
	local player = Player(cid)
	local guild = player:getGuild()
	if not guild then
		npcHandler:say("I'm too busy serving guilds, perhaps my colleague the {Banker} can assist you with your personal bank account.", cid)
		npcHandler.topic[cid] = topicList.NONE
		return true
	end
	if msgcontains(msg, "balance") then
		npcHandler.topic[cid] = topicList.NONE
		npcHandler:say("The {guild account} balance of " .. guild:getName() .. " is " .. guild:getBankBalance() .. " gold.", cid)
		return true
	elseif msgcontains(msg, "deposit") then
		count[cid] = player:getBankBalance()
		if count[cid] < 1 then
			npcHandler:say("Your {personal} bank account looks awefully empty, please deposit money there first, I don't like dealing with heavy coins.", cid)
			npcHandler.topic[cid] = topicList.NONE
			return false
		end
		if string.match(msg,"%d+") then
			count[cid] = getMoneyCount(msg)
			if count[cid] > 0 and count[cid] <= player:getBankBalance() then
				npcHandler:say("Would you really like to deposit " .. count[cid] .. " gold to the guild " .. guild:getName() .. "?", cid)
				npcHandler.topic[cid] = topicList.DEPOSIT_CONSENT
				return true
			else
				npcHandler:say("You cannot afford to deposit " .. count[cid] .. " gold to the guild " .. guild:getName() .. ". You only have " .. player:getBankBalance() .. " gold in your account!", cid)
				npcHandler.topic[cid] = topicList.NONE
				return false
			end
		else
			npcHandler:say("Please tell me how much gold it is you would like to deposit.", cid)
			npcHandler.topic[cid] = topicList.DEPOSIT_GOLD
			return true
		end
	elseif npcHandler.topic[cid] == topicList.DEPOSIT_GOLD then
		count[cid] = getMoneyCount(msg)
		if count[cid] > 0 and count[cid] <= player:getBankBalance() then
			npcHandler:say("Would you really like to deposit " .. count[cid] .. " gold to the guild " .. guild:getName() .. "?", cid)
			npcHandler.topic[cid] = topicList.DEPOSIT_CONSENT
			return true
		else
			npcHandler:say("You do not have enough gold.", cid)
			npcHandler.topic[cid] = topicList.NONE
			return true
		end
	elseif npcHandler.topic[cid] == topicList.DEPOSIT_CONSENT then
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
					"'CONTRIBUTION'",
					deposit,
					currentTime
				},',')
				db.query("INSERT INTO `guild_transactions` (`guild_id`,`player_associated`,`type`,`category`,`balance`,`time`) VALUES ("..insertData..");")
				local receipt = Game.createItem(ITEM_RECEIPT_SUCCESS, 1)
				receipt:setAttribute(ITEM_ATTRIBUTE_TEXT, "Date: " .. os.date("%d. %b %Y - %H:%M:%S", currentTime) .. "\nType: Guild Deposit\nGold Amount: " .. deposit .. "\nReceipt Owner: " .. player:getName() .. "\nRecipient: The " .. guild:getName() .. "\n\nWe are happy to inform you that your transfer request was successfully carried out.")
				player:addItemEx(receipt)
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
			if count[cid] > 0 and count[cid] <= guild:getBankBalance() then
				npcHandler:say("Are you sure you wish to withdraw " .. count[cid] .. " gold from the guild " .. guild:getName() .. "?", cid)
				npcHandler.topic[cid] = topicList.WITHDRAW_CONSENT
			else
				npcHandler:say("There is not enough gold in the guild " .. guild:getName() .. ". Their available balance is currently " .. guild:getBankBalance() .. ".", cid)
				npcHandler.topic[cid] = topicList.NONE
			end
			return true
		else
			npcHandler:say("Please tell me how much gold you would like to withdraw.", cid)
			npcHandler.topic[cid] = topicList.WITHDRAW_GOLD
			return true
		end
	elseif npcHandler.topic[cid] == topicList.WITHDRAW_GOLD then
		count[cid] = getMoneyCount(msg)
		if count[cid] > 0 and count[cid] <= guild:getBankBalance() then
			npcHandler:say("Are you sure you wish to withdraw " .. count[cid] .. " gold from the guild " .. guild:getName() .. "?", cid)
			npcHandler.topic[cid] = topicList.WITHDRAW_CONSENT
		else
			npcHandler:say("There is not enough gold in the guild " .. guild:getName() .. ". Their available balance is currently " .. guild:getBankBalance() .. ".", cid)
			npcHandler.topic[cid] = topicList.NONE
		end
		return true
	elseif npcHandler.topic[cid] == topicList.WITHDRAW_CONSENT then
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
					db.query("INSERT INTO `guild_transactions` (`guild_id`,`player_associated`,`type`,`balance`,`time`) VALUES ("..insertData..");")
					local receipt = Game.createItem(ITEM_RECEIPT_SUCCESS, 1)
					receipt:setAttribute(ITEM_ATTRIBUTE_TEXT, "Date: " .. os.date("%d. %b %Y - %H:%M:%S", currentTime) .. "\nType: Guild Withdraw\nGold Amount: " .. withdraw .. "\nReceipt Owner: " .. player:getName() .. "\nRecipient: The " .. guild:getName() .. "\n\nWe are happy to inform you that your transfer request was successfully carried out.")
					player:addItemEx(receipt)
				else
					npcHandler:say("Sorry, you are not authorized for withdrawals. Only Leaders and Vice-leaders are allowed to withdraw funds from guild accounts.", cid)
				end
			else
				npcHandler:say("There is not enough gold in the guild " .. guild:getName() .. ". Their available balance is currently " .. guild:getBankBalance() .. ".", cid)
			end
			npcHandler.topic[cid] = topicList.NONE
		elseif msgcontains(msg, "no") then
			npcHandler:say("Come back anytime you want to if you wish to {withdraw} your money.", cid)
			npcHandler.topic[cid] = topicList.NONE
		end
		return true
	elseif msgcontains(msg, "guild transfer") or (npcHandler.topic[cid] == topicList.TRANSFER_TYPE and msgcontains(msg, "guild")) then
		if player:getGuid() ~= guild:getOwnerGUID() then
			npcHandler:say("Sorry, you are not authorized for withdrawals. Only Guild Leaders are allowed to transfer funds between guilds.", cid)
			npcHandler.topic[cid] = topicList.NONE
			return true
		end

		npcHandler:say("Please tell me the amount of gold you would like to transfer to another guild.", cid)
		npcHandler.topic[cid] = topicList.TRANSFER_GUILD_GOLD
	elseif npcHandler.topic[cid] == topicList.TRANSFER_GUILD_GOLD then
		count[cid] = getMoneyCount(msg)
		if count[cid] < 0 or guild:getBankBalance() < count[cid] then
			npcHandler:say("There is not enough gold in your guild account.", cid)
			npcHandler.topic[cid] = topicList.NONE
			return true
		end
		npcHandler:say("Which guild would you like transfer " .. count[cid] .. " gold to?", cid)
		npcHandler.topic[cid] = topicList.TRANSFER_GUILD_WHO
	elseif npcHandler.topic[cid] == topicList.TRANSFER_GUILD_WHO then

		local query = db.storeQuery("SELECT `id`, `name` FROM `guilds` WHERE `name`=" .. db.escapeString(msg))
		if not query then
			npcHandler:say("There are no guild in my record who has the name: ["..msg.."]", cid)
			npcHandler.topic[cid] = topicList.NONE
			return true
		end
		transfer[cid] = {
			["id"] = result.getNumber(query, "id"),
			["name"] = result.getString(query, "name")
		}
		result.free(query)

		if guild:getName() == transfer[cid].name then
			npcHandler:say("Fill in this field with guild who receives your gold!", cid)
			npcHandler.topic[cid] = topicList.NONE
			return true
		end

		npcHandler:say("So you would like to transfer " .. count[cid] .. " gold from the guild " .. guild:getName() .. " to the guild " .. transfer[cid].name .. "?", cid)
		npcHandler.topic[cid] = topicList.TRANSFER_GUILD_CONSENT
	elseif npcHandler.topic[cid] == topicList.TRANSFER_GUILD_CONSENT then
		if msgcontains(msg, "yes") then
			if not transfer[cid] or count[cid] < 1 or count[cid] > guild:getBankBalance() then
				transfer[cid] = nil
				npcHandler:say("Your guild account cannot afford this transfer.", cid)
				npcHandler.topic[cid] = topicList.NONE
				return true
			end

			-- Transfer between the guilds
			local transferAmount = count[cid]
			guild:setBankBalance(guild:getBankBalance() - transferAmount)
			local transferGuild = Guild(transfer[cid].name)
			if transferGuild then
				transferGuild:setBankBalance(transferGuild:getBankBalance() + transferAmount)
			else
				db.query("UPDATE `guilds` SET `balance` = (`balance`+"..transferAmount..") WHERE `id`="..transfer[cid].id)
			end

			-- Log: Withdraw from main guild
			local currentTime = os.time()
			local insertData = table.concat({
				guild:getId(),
				transfer[cid].id,
				player:getGuid(),
				"'WITHDRAW'",
				transferAmount,
				currentTime
			},',')
			db.query("INSERT INTO `guild_transactions` (`guild_id`,`guild_associated`,`player_associated`,`type`,`balance`,`time`) VALUES ("..insertData..");")

			-- Log: Deposit to transfer guild
			insertData = table.concat({
				transfer[cid].id,
				guild:getId(),
				player:getGuid(),
				"'DEPOSIT'",
				transferAmount,
				currentTime
			},',')
			db.query("INSERT INTO `guild_transactions` (`guild_id`,`guild_associated`,`player_associated`,`type`,`balance`,`time`) VALUES ("..insertData..");")

			npcHandler:say("Very well. You have transfered " .. transferAmount .. " gold to " .. transfer[cid].name ..".", cid)
			transfer[cid] = nil
			return true
		elseif msgcontains(msg, "no") then
			npcHandler:say("Alright, is there something else I can do for you?", cid)
		end
		npcHandler.topic[cid] = topicList.NONE
	elseif msgcontains(msg, "player transfer") or (npcHandler.topic[cid] == topicList.TRANSFER_TYPE and msgcontains(msg, "player")) then
		local parts = msg:split(" ")

		if #parts < 3 then
			if #parts == 2 then
				-- Immediate topic 11 simulation
				count[cid] = getMoneyCount(parts[2])
				if count[cid] < 0 or guild:getBankBalance() < count[cid] then
					npcHandler:say("There is not enough gold in your guild account.", cid)
					npcHandler.topic[cid] = topicList.NONE
					return true
				end
				npcHandler:say("Who would you like transfer " .. count[cid] .. " gold to?", cid)
				npcHandler.topic[cid] = topicList.TRANSFER_PLAYER_WHO
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

			-- Immediate topic 11 simulation
			count[cid] = getMoneyCount(parts[2])
			if count[cid] < 0 or guild:getBankBalance() < count[cid] then
				npcHandler:say("There is not enough gold in your guild account.", cid)
				npcHandler.topic[cid] = topicList.NONE
				return true
			end
			-- Topic 12
			transfer[cid] = getPlayerDatabaseInfo(receiver)
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
				npcHandler:say("So you would like to transfer " .. count[cid] .. " gold from the guild " .. guild:getName() .. " to " .. transfer[cid].name .. "?", cid)
				npcHandler.topic[cid] = topicList.TRANSFER_PLAYER_CONSENT
			else
				npcHandler:say("This player does not exist.", cid)
				npcHandler.topic[cid] = topicList.NONE
			end
		end
		return true
	elseif msgcontains(msg, "transfer") then
		if player:getGuid() == guild:getOwnerGUID() or player:getGuildLevel() == 2 then
			npcHandler:say("Would you like to transfer money to a {guild} or a {player}?", cid)
			npcHandler.topic[cid] = topicList.TRANSFER_TYPE
		else
			npcHandler:say("Sorry, you are not authorized for withdrawals. Only Leaders and Vice-leaders are allowed to withdraw funds from guild accounts.", cid)
			npcHandler.topic[cid] = topicList.NONE
		end
		return true
	elseif npcHandler.topic[cid] == topicList.TRANSFER_PLAYER_GOLD then
		count[cid] = getMoneyCount(msg)
		if count[cid] < 0 or guild:getBankBalance() < count[cid] then
			npcHandler:say("There is not enough gold in your guild account.", cid)
			npcHandler.topic[cid] = topicList.NONE
			return true
		end
		npcHandler:say("Who would you like transfer " .. count[cid] .. " gold to?", cid)
		npcHandler.topic[cid] = topicList.TRANSFER_PLAYER_WHO
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
			npcHandler:say("So you would like to transfer " .. count[cid] .. " gold from the guild " .. guild:getName() .. " to " .. transfer[cid].name .. "?", cid)
			npcHandler.topic[cid] = topicList.TRANSFER_PLAYER_CONSENT
		else
			npcHandler:say("This player does not exist.", cid)
			npcHandler.topic[cid] = topicList.NONE
		end
	elseif npcHandler.topic[cid] == topicList.TRANSFER_PLAYER_CONSENT then
		if msgcontains(msg, "yes") then
			if not transfer[cid] or count[cid] < 1 or count[cid] > guild:getBankBalance() then
				transfer[cid] = nil
				npcHandler:say("Your guild account cannot afford this transfer.", cid)
				npcHandler.topic[cid] = topicList.NONE
				return true
			end
			local transferAmount = count[cid]
			guild:setBankBalance(guild:getBankBalance() - transferAmount)
			player:setBankBalance(player:getBankBalance() + transferAmount)
			if not player:transferMoneyTo(transfer[cid], transferAmount) then
				npcHandler:say("You cannot transfer money to this account.", cid)
				player:setBankBalance(player:getBankBalance() - transferAmount)
			else
				npcHandler:say("Very well. You have transfered " .. transferAmount .. " gold to " .. transfer[cid].name ..".", cid)
				transfer[cid] = nil
				local currentTime = os.time()
				local insertData = table.concat({
					guild:getId(),
					player:getGuid(),
					"'WITHDRAW'",
					transferAmount,
					currentTime
				},',')
				db.query("INSERT INTO `guild_transactions` (`guild_id`,`player_associated`,`type`,`balance`,`time`) VALUES ("..insertData..");")
			end
		elseif msgcontains(msg, "no") then
			npcHandler:say("Alright, is there something else I can do for you?", cid)
		end
		npcHandler.topic[cid] = topicList.NONE
	elseif msgcontains(msg, "ledger") then
		if player:getGuid() ~= guild:getOwnerGUID() then
			npcHandler.topic[cid] = topicList.NONE
			npcHandler:say("Sorry, this is confidential between me and your Guild Leader!", cid)
			return true
		end
		npcHandler.topic[cid] = topicList.LEDGER_CONSENT
		npcHandler:say("To your advantage, I'm a man who got his papers sorted out. I have ledger records of all transaction requests for your {guild account}. Would you like to get a copy?", cid)
		return true
	elseif msgcontains(msg, "yes") and npcHandler.topic[cid] == topicList.LEDGER_CONSENT then
		local dbTransactions = db.storeQuery([[
			SELECT
				`g`.`name` as `guild_name`,
				`g2`.`name` as `guild_associated_name`,
				`p`.`name` as `player_name`,
				`t`.`type`,
				`t`.`balance`,
				`t`.`time`
			FROM `guild_transactions` as `t`
			JOIN `guilds` as `g`
				ON `t`.`guild_id` = `g`.`id`
			LEFT JOIN `guilds` as `g2`
				ON `t`.`guild_associated` = `g2`.`id`
			LEFT JOIN `players` as `p`
				ON `t`.`player_associated` = `p`.`id`
			WHERE `guild_id` = ]] .. guild:getId() .. [[
			ORDER BY `t`.`time` DESC
		]])
		local ledger_text = "Ledger Date: " .. os.date("%d. %b %Y - %H:%M:%S", os.time()) .. ".\nOfficial ledger for Guild: " .. guild:getName() .. ".\nGuild balance: " .. guild:getBankBalance() .. ".\n\n"
		local records = {}

		if dbTransactions ~= false then
			repeat
				local guild_name = result.getString(dbTransactions, 'guild_name')
				local guild_associated_name = result.getString(dbTransactions, 'guild_associated_name')
				local player_name = result.getString(dbTransactions, 'player_name')
				local type = (result.getString(dbTransactions, 'type') == "WITHDRAW" and "Withdraw" or "Deposit")
				local balance = result.getNumber(dbTransactions, 'balance')
				local time = result.getNumber(dbTransactions, 'time')
				if guild_associated_name ~= "" then
					guild_associated_name = "\nReceiving Guild: The " .. guild_associated_name
				else
					guild_associated_name = ""
				end
	            table.insert(records, "Date: " .. os.date("%d. %b %Y - %H:%M:%S", time) .. "\nType: Guild "..type.."\nGold Amount: " .. balance .. "\nReceipt Owner: " .. player_name .. "\nReceipt Guild: The " .. guild_name .. guild_associated_name)

			until not result.next(dbTransactions)
			result.free(dbTransactions)
	    else -- No transactions exist
			npcHandler.topic[cid] = topicList.NONE
			npcHandler:say("Ohh, your ledger is actually empty. You should start using your {guild account}!", cid)
			return true
		end

		local ledger = Game.createItem(ITEM_DOCUMENT_RO, 1)
		ledger:setAttribute(ITEM_ATTRIBUTE_TEXT, ledger_text .. table.concat(records, "\n\n"))
		player:addItemEx(ledger)

		npcHandler.topic[cid] = topicList.NONE
		npcHandler:say("Here is your ledger "..player:getName()..". Feel free to come back anytime should you need an updated copy.", cid)

		return true
	elseif msgcontains(msg, "no") and npcHandler.topic[cid] == topicList.LEDGER_CONSENT then
		npcHandler.topic[cid] = topicList.NONE
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
