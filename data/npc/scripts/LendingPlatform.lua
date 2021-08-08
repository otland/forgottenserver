-----------------------------------------------------------------------------------------------------------------------
-- NPC Functions
local keywordHandler = KeywordHandler:new()
local npcHandler = NpcHandler:new(keywordHandler)
NpcSystem.parseParameters(npcHandler)
-----------------------------------------------------------------------------------------------------------------------
-- Variable Definitions
local p, pID, pBalance, pBankBalance, lAPY, bAPR, lFees, bFees, iPrd
-----------------------------------------------------------------------------------------------------------------------
-- NPC Functions
function onCreatureAppear(cid)              npcHandler:onCreatureAppear(cid)            end
function onCreatureDisappear(cid)           npcHandler:onCreatureDisappear(cid)         end
function onCreatureSay(cid, type, msg)      npcHandler:onCreatureSay(cid, type, msg)    end
function onThink()                          npcHandler:onThink()                        end
-----------------------------------------------------------------------------------------------------------------------
-- Get number from message
function numFromMsg(s)
	local b, e = s:find('%d+')
	return b and e and math.min(4294967295, tonumber(s:sub(b, e))) or -1
end -- function numFromMsg(s)
-----------------------------------------------------------------------------------------------------------------------
-- NPC Greet Callback
-- Possibly unused
local function greetCallback(cid)
	return true
end -- local function greetCallback(cid)
-----------------------------------------------------------------------------------------------------------------------
-- NPC Say Callback
function creatureSayCallback(cid, type, msg)
	if not npcHandler:isFocused(cid) then
		return false
	end -- if not npcHandler:isFocused(cid) then
-- function continues to the end of the code.
-----------------------------------------------------------------------------------------------------------------------
-- Variable Initializations
p = Player(cid) -- Player Object
pID = p:getGuid() -- Player's GUID
pBalance = p:getTharianGems() -- Player's total Tharian Gems
pBankBalance = p:getTharianBankBalance() -- Player's total Tharian bank balance
lAPY = 200 -- Annualized interest rate for lending
bAPR = 200 -- Annualized interest rate for borrowing
lFees = 0.05 -- Fee to lend gems
bFees = 0.05 -- Fee to borrow gems
iPrd = 86400 -- Seconds in a day
-----------------------------------------------------------------------------------------------------------------------
-- Topic Initialization
local topicList = {
	NONE = 0,
	TRANSACTION_MAIN = 1,
	TRANSACTION_LENDING = 2, 
	LENDING_CONSENT = 3, 
	TRANSACTION_BORROWING = 4, 
	BORROWING_CONSENT = 5, 
	REPAY_CONSENT = 6,
	WITHDRAWAL_CONSENT = 7
}
-----------------------------------------------------------------------------------------------------------------------
-- Return total borrowers
	local function db_gBorrowers()
		local r = db.storeQuery("SELECT COUNT(borrowing_balance) FROM `margin_book` WHERE `ended` IS NULL AND `lending_balance` = 0 ")
		local gBorrowers = result.getNumber(r, "COUNT(borrowing_balance)")
		return gBorrowers
	end -- local function db_gBorrowers()
-----------------------------------------------------------------------------------------------------------------------
-- Return total gems being borrowed
	local function db_gBorrowing()
		local r = db.storeQuery("SELECT SUM(borrowing_balance) FROM `margin_book` WHERE `ended` IS NULL AND `lending_balance` = 0 ")
		local gBorrowed = result.getNumber(r, "SUM(borrowing_balance)")
		return gBorrowed
	end -- local function db_gBorrowing()
-----------------------------------------------------------------------------------------------------------------------
-- Return total lenders
	local function db_gLenders()
		local r = db.storeQuery("SELECT COUNT(lending_balance) FROM `margin_book` WHERE `ended` IS NULL AND `borrowing_balance` = 0 ")
		local gLenders = result.getNumber(r, "COUNT(lending_balance)")
		return gLenders
	end -- local function db_gLenders()
-----------------------------------------------------------------------------------------------------------------------
-- Return total gems being lended
	local function db_gLending()
		local r = db.storeQuery("SELECT SUM(lending_balance) FROM `margin_book` WHERE `ended` IS NULL AND `borrowing_balance` = 0 ")
		local gLending = result.getNumber(r, "SUM(lending_balance)")
		return gLending
	end -- local function db_gLending()
-----------------------------------------------------------------------------------------------------------------------
-- Return global funding (total lending - total borrowing)
	local function db_gFunding()
		local gFunding = db_gLending() - db_gBorrowing()
		return gFunding
	end -- local function db_gFunding()
-----------------------------------------------------------------------------------------------------------------------
-- Return time that the player started borrowing
	local function db_bStart()
		local r = db.storeQuery("SELECT `borrow_start` FROM `players` WHERE `id` = " .. pID)
		local bStart = result.getNumber(r, "borrow_start")
		return bStart
	end -- local function db_bStart()
-----------------------------------------------------------------------------------------------------------------------
-- Return time that the player stopped borrowing
	local function db_bEnd()
		local r = db.storeQuery("SELECT `borrow_end` FROM `players` WHERE `id` = " .. pID)
		local bEnd = result.getNumber(r, "borrow_end")
		return bEnd
	end -- local function db_bEnd()
-----------------------------------------------------------------------------------------------------------------------
-- Return the player's current borrowing balance
	local function db_bBalance()
		local r = db.storeQuery("SELECT `borrow_balance` FROM `players` WHERE `id` = " .. pID)
		local bBalance = result.getNumber(r, "borrow_balance")
		return bBalance
	end -- local function db_bBalance()
-----------------------------------------------------------------------------------------------------------------------
-- Return interest accrued on the player's borrowed gems
	local function db_bAccrued()
		local r = db.storeQuery("SELECT `borrow_accrued` FROM `players` WHERE `id` = " .. pID)
		local bAccrued = result.getNumber(r, "borrow_accrued")
		return bAccrued
	end -- local function db-bAccrued()
-----------------------------------------------------------------------------------------------------------------------
-- Return time that the player started lending
	local function db_lStart()
		local r = db.storeQuery("SELECT `lend_start` FROM `players` WHERE `id` = " .. pID)
		local lStart = result.getNumber(r, "lend_start")
		return lStart
	end -- local function db_lStart()
-----------------------------------------------------------------------------------------------------------------------
--	Return time that the player stopped lending
	local function db_lEnd()
		local r = db.storeQuery("SELECT `lend_end` FROM `players` WHERE `id` = " .. pID)
		local lEnd = result.getNumber(r, "lend_end")
		return lEnd
	end -- local function db_lEnd()
-----------------------------------------------------------------------------------------------------------------------
-- Return the player's current lending balance
	local function db_lBalance()
		local r = db.storeQuery("SELECT `lend_balance` FROM `players` WHERE `id` = " .. pID)
		local lBalance = result.getNumber(r, "lend_balance")
		return lBalance
	end -- local function db_lBalance()
-----------------------------------------------------------------------------------------------------------------------
-- Return interest accrued on the player's lended gems
	local function db_lAccrued()
		local r = db.storeQuery("SELECT `lend_rewards` FROM `players` WHERE `id` = " .. pID)
		local lAccrued = result.getNumber(r, "lend_rewards")
		return lAccrued
	end -- local function db_lAccrued()
-----------------------------------------------------------------------------------------------------------------------
-- Update the player's lending information in the database when creating a lend
	local function db_lCreateP(a, s)
		db.asyncQuery("UPDATE `players` SET `lend_start` = " .. s .. " WHERE `id` = '" .. pID .. "'")
		db.asyncQuery("UPDATE `players` SET `lend_balance` = " .. a .. " WHERE `id` = '" .. pID .. "'")
	end -- local function db_lCreateP(a, s)
-----------------------------------------------------------------------------------------------------------------------
-- Update the player's borrowing information in the database when creating a borrow
	local function db_bCreateP(a, s)
		db.asyncQuery("UPDATE `players` SET `borrow_start` = " .. s .. " WHERE `id` = '" .. pID .. "'")
		db.asyncQuery("UPDATE `players` SET `borrow_balance` = " .. a .. " WHERE `id` = '" .. pID .. "'")
	end -- local function db_bCreateP(a, s)
-----------------------------------------------------------------------------------------------------------------------
-- Update the margin book with the player's borrowing information when creating a borrow
	local function db_bCreateM(a, s)
		db.asyncQuery("INSERT INTO `margin_book` (`player_id`, `borrowing_balance`, `created`) VALUES (" .. pID .. ", " .. a .. ", " .. s .. ")")
	end -- local function db_bCreateM(a, s)
-----------------------------------------------------------------------------------------------------------------------
-- Update the margin book with the player's lending information when creating a lend
	local function db_lCreateM(a, s)
		db.asyncQuery("INSERT INTO `margin_book` (`player_id`, `lending_balance`, `created`) VALUES (" .. pID .. ", " .. a .. ", " .. s .. ")")
	end -- local function db_lCreateM(a, s)
-----------------------------------------------------------------------------------------------------------------------
-- Update the player's lending information in the database when ending a lend
	local function db_lRemoveP(e)
		db.asyncQuery("UPDATE `players` SET `lend_end` = " .. e .. " WHERE `id` = '" .. pID .. "'")
		db.asyncQuery("UPDATE `players` SET `lend_balance` = 0 WHERE `id` = '" .. pID .. "'")
		db.asyncQuery("UPDATE `players` SET `lend_rewards` = 0 WHERE `id` = '" .. pID .. "'")
	end -- local function db_lRemoveP(e)
-----------------------------------------------------------------------------------------------------------------------
-- Update the player's borrowing information in the database when ending a borrow
	local function db_bRemoveP(e)
		db.asyncQuery("UPDATE `players` SET `borrow_end` = " .. e .. " WHERE `id` = '" .. pID .. "'")
		db.asyncQuery("UPDATE `players` SET `borrow_balance` = 0 WHERE `id` = '" .. pID .. "'")
		db.asyncQuery("UPDATE `players` SET `borrow_accrued` = 0 WHERE `id` = '" .. pID .. "'")
	end -- local function db_bRemoveP(e)
-----------------------------------------------------------------------------------------------------------------------
-- Update the margin book with the player's borrowing information when ending a borrow
	local function db_bRemoveM(e)
		local sTime = db_bStart()
		local dif_bTime = sTime - e
		db.asyncQuery("UPDATE `margin_book` SET `ended` = " .. e .. " WHERE `player_id` = '" .. pID .. "'")
		db.asyncQuery("UPDATE `margin_book` SET `borrowing_balance` = 0 WHERE `player_id` = '" .. pID .. "'")
		db.asyncQuery("UPDATE `margin_book` SET `loan_time` = " .. dif_bTime .. " WHERE `player_id` = '" .. pID .. "'")
	end -- local function db_bRemoveM(e)
-----------------------------------------------------------------------------------------------------------------------
-- Update the margin book with the player's lending information when ending a lend
	local function db_lRemoveM(e)
		local sTime = db_lStart()
		local dif_lTime = sTime - e
		db.asyncQuery("UPDATE `margin_book` SET `ended` = " .. e .. " WHERE `player_id` = '" .. pID .. "'")
		db.asyncQuery("UPDATE `margin_book` SET `lending_balance` = 0 WHERE `player_id` = '" .. pID .. "'")
		db.asyncQuery("UPDATE `margin_book` SET `loan_time` = " .. dif_lTime .. " WHERE `player_id` = '" .. pID .. "'")
	end -- local function db_lRemoveM(e)
-----------------------------------------------------------------------------------------------------------------------
-- Update the player's margin balance in the database
	local function db_pMargin(m)
		local margin = m
		db.asyncQuery("UPDATE `players` SET `margin` = " .. margin .. " WHERE `id` = '" .. pID .. "'")
	end
-----------------------------------------------------------------------------------------------------------------------
-- Calculate interest from player's borrow
	local function calc_bInterest()
		local sTime = db_bStart()
		local eTime = os.time()
		local dif_bTime = sTime - eTime
		local toDaily_difBTime = dif_bTime / 86400 
		local balance = db_bBalance() 
		local toDecimal_bAPR = bAPR / 100
		local toDaily_bAPR = toDecimal_bAPR / 365
		local daily_bInterest = balance * toDaily_bAPR 
		local interest = daily_bInterest * toDaily_difBTime
		db.asyncQuery("UPDATE `players` SET `borrow_accrued` = " .. interest .. " WHERE `id` = '" .. pID .. "'")
		return interest
	end -- local function calc_bInterest()
-----------------------------------------------------------------------------------------------------------------------
-- Calculate interest from player's lend
	local function calc_lInterest()
		local sTime = db_lStart()
		local eTime = os.time()
		local dif_lTime = sTime - eTime
		local toDaily_diflTime = dif_lTime / 86400 
		local balance = db_lBalance() 
		local toDecimal_lAPY = lAPY / 100
		local toDaily_lAPY = toDecimal_lAPY / 365
		local daily_lInterest = balance * toDaily_lAPY 
		local interest = daily_lInterest * toDaily_diflTime
		db.asyncQuery("UPDATE `players` SET `lend_rewards` = " .. interest .. " WHERE `id` = '" .. pID .. "'")
		return interest
	end --local function calc_lInterest()
-----------------------------------------------------------------------------------------------------------------------
-- Calculate borrowed balance + accrued interest
	local function calc_bTotal()
		local balance = db_bBalance()
		local accrued = calc_bInterest()
		local total = balance + accrued
		return total
	end -- local function calc_bTotal()
-----------------------------------------------------------------------------------------------------------------------
-- Calculate lending balance + accrued interest
	local function calc_lTotal()
		local balance = db_lBalance()
		local accrued = calc_lInterest()
		local total = balance + accrued
		return total
	end -- local function calc_lTotal()
-----------------------------------------------------------------------------------------------------------------------
-- Calculate player margin balance (lending - borrowing)
	local function calc_pMargin()
		local borrowing = calc_bTotal()
		local lending = calc_lTotal()
		local margin = lending - borrowing
		db_pMargin(margin)
		return margin
	end
-----------------------------------------------------------------------------------------------------------------------
-- Calculate borrowing fee
	local function calc_bFee(a)
		local amount = a
		local bFee = amount * bFees
		return bFee
	end -- local function calc_bFee(a)
-----------------------------------------------------------------------------------------------------------------------
-- Calculate lending fee
	local function calc_lFee(a)
		local amount = a
		local lFee = amount * lFees
		return lFee
	end -- local function calc_lFee(a)
-----------------------------------------------------------------------------------------------------------------------
-- Start borrowing gems from the global fund
	local function act_bStart(a)
		local start = os.time()
		local amount = a
		db_bCreateP(amount, start)
		db_bCreateM(amount, start)
		return true
	end -- local function act_bStart(a)
-----------------------------------------------------------------------------------------------------------------------
-- Stop borrowing gems from the global fund
	local function act_bStop()
		local stop = os.time()
		local balance = db_bBalance()
		local interest = calc_bInterest()
		local total = balance + interest
		db_bRemoveP(stop)
		db_bRemoveM(stop)
			return true
	end -- local function act_bStop()
-----------------------------------------------------------------------------------------------------------------------
-- Start lending gems to the global fund
	local function act_lStart(a)
		local start = os.time()
		local amount = a
		db_lCreateP(amount, start)
		db_lCreateM(amount, start)
		return true
	end -- local function act_lStart(a)
-----------------------------------------------------------------------------------------------------------------------
-- Stop lending gems to the global fund
	local function act_lStop()
		local stop = os.time()
		local balance = db_lBalance()
		local interest = calc_lInterest()
		local total = balance + interest
		db_lRemoveP(stop)
		db_lRemoveM(stop)
		return true
	end -- local function act_lStop()
-----------------------------------------------------------------------------------------------------------------------
-- Lending:
	if msgcontains(msg, "transaction") then
		npcHandler.topic[cid] = topicList.NONE
		npcHandler:say("Is this a {lend} or {borrow} transaction?", cid)
	elseif msgcontains(msg, "lend") then
		if pBankBalance < 1 then
			npcHandler:say("I'm afraid you don't have any gems.", cid)
			npcHandler.topic[cid] = topicList.NONE
			return false
		end
		if msgcontains(msg, "all") then
			npcHandler:say("Would you like to lend " .. pBankBalance .. " gems?", cid)
			npcHandler:say("This will include a lending fee of " .. calc_lFee(pBankBalance) .. " gems.", cid)
			npcHandler.topic[cid] = topicList.LENDING_CONSENT
			return true
		else 
			if string.match(msg,"%d+") then
				toLend = getTharianGemsCount(msg)
				if toLend <= pBankBalance then
					npcHandler:say("Would you like to lend " .. toLend .. " gems?", cid)
					npcHandler:say("This will include a lending fee of " .. calc_lFee(toLend) .. " gems.", cid)
					npcHandler.topic[cid] = topicList.LENDING_CONSENT
				return true
				elseif toLend > pBankBalance then
					npcHandler:say("Sorry, you don't have enough gems to afford this lend.", cid)
				return false
				end
			else
				npcHandler:say("Please tell me how many gems you would like to lend.", cid)
				npcHandler.topic[cid] = topicList.TRANSACTION_LENDING
				return true
			end
		end
		if not isValidTharianGems(pBankBalance) then
			npcHandler:say("Sorry, but you can't lend that much.", cid)
			npcHandler.topic[cid] = topicList.NONE
			return false
		end
	elseif npcHandler.topic[cid] == topicList.TRANSACTION_LENDING then
		toLend = getTharianGemsCount(msg)
		if isValidTharianGems(pBankBalance) then
			npcHandler:say("Would you really like to lend " .. toLend .. " gems?", cid)
			npcHandler:say("This will include a lending fee of " .. calc_lFee(toLend) .. " gems.", cid)
			npcHandler.topic[cid] = topicList.LENDING_CONSENT
			return true
		else
			npcHandler:say("You do not have enough gems.", cid)
			npcHandler.topic[cid] = topicList.NONE
			return true
		end
	elseif npcHandler.topic[cid] == topicList.LENDING_CONSENT then
		if msgcontains(msg, "yes") then
			if pBankBalance >= toLend then
				p:setTharianBankBalance(pBankBalance - toLend - (calc_bFee(toLend)))
				act_lStart(toLend)
				npcHandler:say("Okay! You've started lending " .. toLend .. " gems.", cid)
				npcHandler:say("You also paid a lending fee of " .. calc_lFee(toLend) .. " gems.", cid)
			else
				npcHandler:say("You do not have enough gems.", cid)
			end
		elseif msgcontains(msg, "no") then
			npcHandler:say("Sure thing, is there anything else I can {help} you with?", cid)
		end
		npcHandler.topic[cid] = topicList.NONE
		return true
-----------------------------------------------------------------------------------------------------------------------
-- Borrowing:
	elseif msgcontains(msg, "borrow") then
		if string.match(msg,"%d+") then
			toBorrow = getTharianGemsCount(msg)
			if toBorrow > db_gFunding() then
				npcHandler:say("I'm afraid the global funding market cannot afford this transaction.", cid)
				npcHandler.topic[cid] = topicList.NONE
				return false
			end
			npcHandler:say("Would you like to borrow " .. toBorrow .. " gems?", cid)
			npcHandler:say("This will include a borrowing fee of " .. calc_bFee(toBorrow) .. " gems.", cid)
			npcHandler.topic[cid] = topicList.BORROWING_CONSENT
			return true
		else 
			npcHandler:say("Please tell me how many gems you would like to borrow.", cid)
			npcHandler.topic[cid] = topicList.TRANSACTION_BORROWING
			return true
		end
		if not isValidTharianGems(toBorrow) then
			npcHandler:say("Sorry, but you can't borrow that much.", cid)
			npcHandler.topic[cid] = topicList.NONE
			return false
		end
	elseif npcHandler.topic[cid] == topicList.TRANSACTION_BORROWING then
		if isValidTharianGems(toBorrow) then
			npcHandler:say("Would you really like to borrow " .. toBorrow .. " gems?", cid)
			npcHandler:say("This will include a borrowing fee of " .. calc_bFee(toBorrow) .. " gems.", cid)
			npcHandler.topic[cid] = topicList.BORROWING_CONSENT
			return true
		else
			npcHandler:say("You do not have enough gems.", cid)
			npcHandler.topic[cid] = topicList.NONE
			return true
		end
	elseif npcHandler.topic[cid] == topicList.BORROWING_CONSENT then
		if msgcontains(msg, "yes") then
			p:setTharianBankBalance(pBankBalance + toBorrow - (calc_bFee(toBorrow)))
			act_bStart(toBorrow)
			npcHandler:say("Okay! You've started borrowing " .. toBorrow .. " gems.", cid)
			npcHandler:say("You also paid a borrowing fee of " .. calc_bFee(toBorrow) .. " gems.", cid)
		elseif msgcontains(msg, "no") then
			npcHandler:say("Sure thing, is there anything else I can {help} you with?", cid)
		end
		npcHandler.topic[cid] = topicList.NONE
		return true
-----------------------------------------------------------------------------------------------------------------------
-- Repay:
	elseif msgcontains(msg, "repay") then
		npcHandler:say("Would you like to repay all of your borrowed debt?", cid)
		npcHandler.topic[cid] = topicList.REPAY_CONSENT
		return true
	elseif npcHandler.topic[cid] == topicList.REPAY_CONSENT then
		if msgcontains(msg, "yes") then
			if pBankBalance >= calc_bTotal() then
				p:setTharianBankBalance(pBankBalance - (calc_bTotal()))
				act_bStop()
				npcHandler:say("Alright, you've repayed your debt of " .. calc_bTotal() .. " gems.", cid)
			else
				npcHandler:say("You do not have enough gems to repay this debt.", cid)
			end
		elseif msgcontains(msg, "no") then
			npcHandler:say("Sure thing, is there anything else I can {help} you with?", cid)
		end
		npcHandler.topic[cid] = topicList.NONE
		return true
-----------------------------------------------------------------------------------------------------------------------
-- Withdrawal:
	elseif msgcontains(msg, "withdrawal") then
		npcHandler:say("Would you like to withdrawal all of your lending balance?", cid)
		npcHandler.topic[cid] = topicList.WITHDRAWAL_CONSENT
		return true
	elseif npcHandler.topic[cid] == topicList.WITHDRAWAL_CONSENT then
		if msgcontains(msg, "yes") then
			if calc_lTotal() > 1 then
				p:setTharianBankBalance(pBankBalance + (calc_lTotal()))
				act_lStop()
				npcHandler:say("Alright, you've withdrawn your lend of " .. calc_lTotal() .. " gems.", cid)
			else
				npcHandler:say("It looks like you're not lending any gems.", cid)
			end
		elseif msgcontains(msg, "no") then
			npcHandler:say("Sure thing, is there anything else I can {help} you with?", cid)
		end
		npcHandler.topic[cid] = topicList.NONE
		return true
-----------------------------------------------------------------------------------------------------------------------
-- Minor NPC Interactions
-- Help Menu
	elseif msgcontains(msg, "help") then
		npcHandler.topic[cid] = topicList.NONE
		npcHandler:say("Here you may lend gems and gain interest rewards as time passes, like a savings account or borrow gems from the lenders and pay back the gem debt plus interest.", cid)
		npcHandler:say("Lending gems gives you the opportunity to gain interest on the gems you lend to the global lending market.", cid)
		npcHandler:say("When someone asks to borrow gems, they depend on the lenders to supply the global funding available to borrow.", cid)
		npcHandler:say("You may also view the global {funding} lenders have made available to borrowers, or check your the {balances} on your lending and funding accounts.", cid)
		npcHandler:say("If you're running low on gems and need a loan, you can borrow gems from the lenders who {lend gems} to the global funding market.", cid)
		npcHandler:say("However, you will have to repay the amount you borrowed plus the interest that accrues during the period the debt remains unpaid.", cid)
		npcHandler:say("Just ask if there is anything else I can {help} you with.", cid)
		npcHandler.topic[cid] = topicList.NONE
		return true	

	-- Balances Menu
	elseif msgcontains(msg, "balances") then
		npcHandler:say("Global Lending APY: " .. lAPY .. "%", cid)
		npcHandler:say("Global Borrowing APR: " .. bAPR .. "%", cid)
		npcHandler:say("Lending Balance: " .. db_lBalance() .. ".", cid)
		npcHandler:say("Lending Interest: " .. db_lAccrued() .. ".", cid)
		npcHandler:say("Borrowing Balance: " .. db_bBalance() .. ".", cid)
		npcHandler:say("Borrowing Interest: " .. db_bAccrued() .. ".", cid)
		npcHandler:say("Margin Balance: " .. calc_pMargin() .. ".", cid)
		npcHandler:say("Just ask if there is anything else I can {help} you with.", cid)
		npcHandler.topic[cid] = topicList.NONE
		return true

	-- Global Funding Balance
	elseif msgcontains(msg, "funding") then
		npcHandler:say("Total Lenders: " .. db_gLenders() .. ".", cid)
		npcHandler:say("Total Lending Balance: " .. db_gLending() .. ".", cid)
		npcHandler:say("Total Borrowers: " .. db_gBorrowers() .. ".", cid)
		npcHandler:say("Total Borrwed Balance: " .. db_gBorrowing() .. ".", cid)
		npcHandler:say("Total Global Funding: " .. db_gFunding() .. ".", cid)
		npcHandler:say("Just ask if there is anything else I can {help} you with.", cid)
		npcHandler.topic[cid] = topicList.NONE
		return true

	-- NPC Job
	elseif msgcontains(msg, "job") then
		npcHandler:say("I am a gem broker for the Tharian Bank's lending and borrowing section. My job is to help bank patrons securely lend and borrow the gems they earn in the world. Would you like {help} getting started?", cid)
		npcHandler:say("Just ask if there is anything else I can {help} you with.", cid)
		npcHandler.topic[cid] = topicList.NONE
		return true
	end
end
-----------------------------------------------------------------------------------------------------------------------
npcHandler:setMessage(MESSAGE_GREET, "Welcome to the Tharian Kingdom Bank's lending and borrowing section. Would you like to start a {transaction}, {repay} or {withdrawal} a loan or do you need {help} getting started?")
npcHandler:setCallback(CALLBACK_GREET, greetCallback)
npcHandler:setCallback(CALLBACK_MESSAGE_DEFAULT, creatureSayCallback)
npcHandler:addModule(FocusModule:new())
-----------------------------------------------------------------------------------------------------------------------
-- Extra
	-- Send green text across screen
		-- player:sendTextMessage(MESSAGE_INFO_DESCR, "You started borrowing gems from the Tharian Bank.")
