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
	DEPOSIT_GEMS = 1, -- Depositing gems.
	DEPOSIT_CONSENT = 2, -- Making sure you want to deposit the gems.
	WITHDRAWAL_GEMS = 3, -- Withdrawaling gems.
	WITHDRAWAL_CONSENT = 4 -- Making sure you want to withdrawal the gems.
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
		if player:getTharianBankBalance() >= 100000000 then
			npcHandler:say("I think you must be one of the richest inhabitants in the world! Your account balance is " .. player:getTharianBankBalance() .. " gems.", cid)
			return true
		elseif player:getTharianBankBalance() >= 10000000 then
			npcHandler:say("You have made ten millions and it still grows! Your account balance is " .. player:getTharianBankBalance() .. " gems.", cid)
			return true
		elseif player:getTharianBankBalance() >= 1000000 then
			npcHandler:say("Wow, you have reached the magic number of a million gp!!! Your account balance is " .. player:getTharianBankBalance() .. " gems!", cid)
			return true
		elseif player:getTharianBankBalance() >= 100000 then
			npcHandler:say("You certainly have made a pretty penny. Your account balance is " .. player:getTharianBankBalance() .. " gems.", cid)
			return true
		else
			npcHandler:say("Your account balance is " .. player:getTharianBankBalance() .. " gems.", cid)
			return true
		end
	elseif msgcontains(msg, "deposit") then
		count[cid] = player:getTharianGems()
		if count[cid] < 1 then
			npcHandler:say("You do not have enough gems.", cid)
			npcHandler.topic[cid] = topicList.NONE
			return false
		end
		if msgcontains(msg, "all") then
			count[cid] = player:getTharianGems()
			npcHandler:say("Would you like to deposit " .. count[cid] .. " gems?", cid)
			npcHandler.topic[cid] = topicList.DEPOSIT_CONSENT
			return true
		else
			if string.match(msg,"%d+") then
				count[cid] = getTharianGemsCount(msg)
				if count[cid] < 1 then
					npcHandler:say("You do not have enough gems.", cid)
					npcHandler.topic[cid] = topicList.NONE
					return false
				end
				npcHandler:say("Would you like to deposit " .. count[cid] .. " gems?", cid)
				npcHandler.topic[cid] = topicList.DEPOSIT_CONSENT
				return true
			else
				npcHandler:say("Please tell me how much gems it is you would like to deposit.", cid)
				npcHandler.topic[cid] = topicList.DEPOSIT_GEMS
				return true
			end
		end
		if not isValidTharianGems(count[cid]) then
			npcHandler:say("Sorry, but you can't deposit that much.", cid)
			npcHandler.topic[cid] = topicList.NONE
			return false
		end
	elseif npcHandler.topic[cid] == topicList.DEPOSIT_GEMS then
		count[cid] = getTharianGemsCount(msg)
		if isValidTharianGems(count[cid]) then
			npcHandler:say("Would you really like to deposit " .. count[cid] .. " gems?", cid)
			npcHandler.topic[cid] = topicList.DEPOSIT_CONSENT
			return true
		else
			npcHandler:say("You do not have enough gems.", cid)
			npcHandler.topic[cid] = topicList.NONE
			return true
		end
	elseif npcHandler.topic[cid] == topicList.DEPOSIT_CONSENT then
		if msgcontains(msg, "yes") then
			if player:getTharianGems() >= tonumber(count[cid]) then
				player:depositTharianGems(count[cid])
				npcHandler:say("Alright, we have added the amount of " .. count[cid] .. " gems to your {balance}. You can {withdrawal} your money anytime you want to.", cid)
			else
				npcHandler:say("You do not have enough gems.", cid)
			end
		elseif msgcontains(msg, "no") then
			npcHandler:say("As you wish. Is there something else I can do for you?", cid)
		end
		npcHandler.topic[cid] = topicList.NONE
		return true
	elseif msgcontains(msg, "withdrawal") then
		if string.match(msg,"%d+") then
			count[cid] = getTharianGemsCount(msg)
			if isValidTharianGems(count[cid]) then
				npcHandler:say("Are you sure you wish to withdrawal " .. count[cid] .. " gems from your bank account?", cid)
				npcHandler.topic[cid] = topicList.WITHDRAWAL_GEMS
			else
				npcHandler:say("There is not enough gems in your account.", cid)
				npcHandler.topic[cid] = topicList.NONE
			end
			return true
		else
			npcHandler:say("Please tell me how much gems you would like to withdrawal.", cid)
			npcHandler.topic[cid] = topicList.WITHDRAWAL_CONSENT
			return true
		end
	elseif npcHandler.topic[cid] == topicList.WITHDRAWAL_CONSENT then
		count[cid] = getTharianGemsCount(msg)
		if isValidTharianGems(count[cid]) then
			npcHandler:say("Are you sure you wish to withdrawal " .. count[cid] .. " gems from your bank account?", cid)
			npcHandler.topic[cid] = topicList.WITHDRAWAL_GEMS
		else
			npcHandler:say("There is not enough gems in your account.", cid)
			npcHandler.topic[cid] = topicList.NONE
		end
		return true
	elseif npcHandler.topic[cid] == topicList.WITHDRAWAL_GEMS then
		if msgcontains(msg, "yes") then
			if player:canCarryMoney(count[cid]) then
				if not player:withdrawTharianGems(count[cid]) then
					npcHandler:say("There is not enough gems in your account.", cid)
				else
					npcHandler:say("Here you are, " .. count[cid] .. " gems. Please let me know if there is something else I can do for you.", cid)
				end
			else
				npcHandler:say("Whoah, hold on, you have no room in your inventory to carry all those coins. I don't want you to drop it on the floor, maybe come back with a cart!", cid)
			end
			npcHandler.topic[cid] = topicList.NONE
		elseif msgcontains(msg, "no") then
			npcHandler:say("The customer is king! Come back anytime you want to if you wish to {withdrawal} your money.", cid)
			npcHandler.topic[cid] = topicList.NONE
		end
		return true
	end
end
keywordHandler:addKeyword({"gems"}, StdModule.say,{
	npcHandler = npcHandler,
	text = "Secure your Tharian gems in our one of a kind {bank account}."
})
keywordHandler:addKeyword({"help"}, StdModule.say, {
	npcHandler = npcHandler,
	text = "You may check the {balance} of your Tharian bank account as well as {deposit} or {withdrawal} your gems."
})
keywordHandler:addAliasKeyword({'functions'})
keywordHandler:addAliasKeyword({'basic'})
keywordHandler:addKeyword({"job"}, StdModule.say, {
	npcHandler = npcHandler,
	text = "I am a gem banker for the Tharian Bank. My job is to help you secure your earned gems in a personal {bank account}."
})

npcHandler:setMessage(MESSAGE_GREET, "Welcome to the Tharian Bank, |PLAYERNAME|! Need some help with your {bank account}?")
npcHandler:setCallback(CALLBACK_GREET, greetCallback)
npcHandler:setCallback(CALLBACK_MESSAGE_DEFAULT, creatureSayCallback)
npcHandler:addModule(FocusModule:new())
