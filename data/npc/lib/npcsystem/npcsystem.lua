-- Advanced NPC System by Jiddo

shop_amount = {}
shop_cost = {}
shop_rlname = {}
shop_itemid = {}
shop_container = {}
shop_npcuid = {}
shop_eventtype = {}
shop_subtype = {}
shop_destination = {}
shop_premium = {}

npcs_loaded_shop = {}
npcs_loaded_travel = {}

if not NpcSystem then
	-- Loads the underlying classes of the npcsystem.
	dofile('data/npc/lib/npcsystem/keywordhandler.lua')
	dofile('data/npc/lib/npcsystem/npchandler.lua')
	dofile('data/npc/lib/npcsystem/modules.lua')

	-- Global npc constants:

	-- Greeting and unGreeting keywords. For more information look at the top of modules.lua
	FOCUS_GREETWORDS = {'hi', 'hello'}
	FOCUS_FAREWELLWORDS = {'bye', 'farewell'}

	-- The word for requesting trade window. For more information look at the top of modules.lua
	SHOP_TRADEREQUEST = {'trade'}

	-- The word for accepting/declining an offer. CAN ONLY CONTAIN ONE FIELD! For more information look at the top of modules.lua
	SHOP_YESWORD = {'yes'}
	SHOP_NOWORD = {'no'}

	-- Pattern used to get the amount of an item a player wants to buy/sell.
	PATTERN_COUNT = '%d+'

	-- Talkdelay behavior. For more information, look at the top of npchandler.lua.
	NPCHANDLER_TALKDELAY = TALKDELAY_ONTHINK

	-- Constant strings defining the keywords to replace in the default messages.
	--	For more information, look at the top of npchandler.lua...
	TAG_PLAYERNAME = '|PLAYERNAME|'
	TAG_ITEMCOUNT = '|ITEMCOUNT|'
	TAG_TOTALCOST = '|TOTALCOST|'
	TAG_ITEMNAME = '|ITEMNAME|'

	NpcSystem = {}

	-- Gets an npcparameter with the specified key. Returns nil if no such parameter is found.
	function NpcSystem.getParameter(key)
		local ret = getNpcParameter(tostring(key))
		if (type(ret) == 'number' and ret == 0) then
			return nil
		else
			return ret
		end
	end

	-- Parses all known parameters for the npc. Also parses parseable modules.
	function NpcSystem.parseParameters(npcHandler)
		local ret = NpcSystem.getParameter('idletime')
		if ret then
			npcHandler.idleTime = tonumber(ret)
		end
		local ret = NpcSystem.getParameter('talkradius')
		if ret then
			npcHandler.talkRadius = tonumber(ret)
		end
		local ret = NpcSystem.getParameter('message_greet')
		if ret then
			npcHandler:setMessage(MESSAGE_GREET, ret)
		end
		local ret = NpcSystem.getParameter('message_farewell')
		if ret then
			npcHandler:setMessage(MESSAGE_FAREWELL, ret)
		end
		local ret = NpcSystem.getParameter('message_decline')
		if ret then
			npcHandler:setMessage(MESSAGE_DECLINE, ret)
		end
		local ret = NpcSystem.getParameter('message_needmorespace')
		if ret then
			npcHandler:setMessage(MESSAGE_NEEDMORESPACE, ret)
		end
		local ret = NpcSystem.getParameter('message_needspace')
		if ret then
			npcHandler:setMessage(MESSAGE_NEEDSPACE, ret)
		end
		local ret = NpcSystem.getParameter('message_sendtrade')
		if ret then
			npcHandler:setMessage(MESSAGE_SENDTRADE, ret)
		end
		local ret = NpcSystem.getParameter('message_noshop')
		if ret then
			npcHandler:setMessage(MESSAGE_NOSHOP, ret)
		end
		local ret = NpcSystem.getParameter('message_oncloseshop')
		if ret then
			npcHandler:setMessage(MESSAGE_ONCLOSESHOP, ret)
		end
		local ret = NpcSystem.getParameter('message_onbuy')
		if ret then
			npcHandler:setMessage(MESSAGE_ONBUY, ret)
		end
		local ret = NpcSystem.getParameter('message_onsell')
		if ret then
			npcHandler:setMessage(MESSAGE_ONSELL, ret)
		end
		local ret = NpcSystem.getParameter('message_missingmoney')
		if ret then
			npcHandler:setMessage(MESSAGE_MISSINGMONEY, ret)
		end
		local ret = NpcSystem.getParameter('message_needmoney')
		if ret then
			npcHandler:setMessage(MESSAGE_NEEDMONEY, ret)
		end
		local ret = NpcSystem.getParameter('message_missingitem')
		if ret then
			npcHandler:setMessage(MESSAGE_MISSINGITEM, ret)
		end
		local ret = NpcSystem.getParameter('message_needitem')
		if ret then
			npcHandler:setMessage(MESSAGE_NEEDITEM, ret)
		end
		local ret = NpcSystem.getParameter('message_idletimeout')
		if ret then
			npcHandler:setMessage(MESSAGE_IDLETIMEOUT, ret)
		end
		local ret = NpcSystem.getParameter('message_walkaway')
		if ret then
			npcHandler:setMessage(MESSAGE_WALKAWAY, ret)
		end
		local ret = NpcSystem.getParameter('message_alreadyfocused')
		if ret then
			npcHandler:setMessage(MESSAGE_ALREADYFOCUSED, ret)
		end
		local ret = NpcSystem.getParameter('message_buy')
		if ret then
			npcHandler:setMessage(MESSAGE_BUY, ret)
		end
		local ret = NpcSystem.getParameter('message_sell')
		if ret then
			npcHandler:setMessage(MESSAGE_SELL, ret)
		end
		local ret = NpcSystem.getParameter('message_bought')
		if ret then
			npcHandler:setMessage(MESSAGE_BOUGHT, ret)
		end
		local ret = NpcSystem.getParameter('message_sold')
		if ret then
			npcHandler:setMessage(MESSAGE_SOLD, ret)
		end
		local ret = NpcSystem.getParameter('message_walkaway_male')
		if ret then
			npcHandler:setMessage(MESSAGE_WALKAWAY_MALE, ret)
		end
		local ret = NpcSystem.getParameter('message_walkaway_female')
		if ret then
			npcHandler:setMessage(MESSAGE_WALKAWAY_FEMALE, ret)
		end

		-- Parse modules.
		for parameter, module in pairs(Modules.parseableModules) do
			local ret = NpcSystem.getParameter(parameter)
			if ret then
				local number = tonumber(ret)
				if number ~= 0 and module.parseParameters then
					local instance = module:new()
					npcHandler:addModule(instance)
					instance:parseParameters()
				end
			end
		end
	end
end
