local keywordHandler = KeywordHandler:new()
local npcHandler = NpcHandler:new(keywordHandler)
NpcSystem.parseParameters(npcHandler)

local vocation = {}
local town = {}

local config = {

	towns = {
		["rhyves"] = 2
	},

	vocations = {
		["sorcerer"] = {
			text = "A SORCERER! ARE YOU SURE? THIS DECISION IS IRREVERSIBLE!",
			vocationId = 1,
			--equipment spellbook, wand of vortex, magician's robe, mage hat, studded legs, leather boots, scarf
			{{2175, 1}, {2190, 1}, {8819, 1}, {8820, 1}, {2468, 1}, {2643, 1}, {2661, 1}},
			--container rope, shovel, mana potion
			{{2120, 1}, {2554, 1}, {7620, 1}}
		},

		["druid"] = {
			text = "A DRUID! ARE YOU SURE? THIS DECISION IS IRREVERSIBLE!",
			vocationId = 2,
			--equipment spellbook, snakebite rod, magician's robe, mage hat, studded legs, leather boots scarf
			{{2175, 1}, {2182, 1}, {8819, 1}, {8820, 1}, {2468, 1}, {2643, 1}, {2661, 1}},
			--container rope, shovel, mana potion
			{{2120, 1}, {2554, 1}, {7620, 1}}
		},

		["paladin"] = {
			text = "A PALADIN! ARE YOU SURE? THIS DECISION IS IRREVERSIBLE!",
			vocationId = 3,
			--equipment dwrven shield, 5 spear, ranger's cloak, ranger legs scarf, legion helmet
			{{2525, 1}, {2389, 5}, {2660, 1}, {8923, 1}, {2643, 1}, {2661, 1}, {2480, 1}},
			--container rope, shovel, health potion, bow, 50 arrow
			{{2120, 1}, {2554, 1}, {7618, 1}, {2456, 1}, {2544, 50}}
		},

		["knight"] = {
			text = "A KNIGHT! ARE YOU SURE? THIS DECISION IS IRREVERSIBLE!",
			vocationId = 4,
			--equipment dwarven shield, steel axe, brass armor, brass helmet, brass legs scarf
			{{2525, 1}, {8601, 1}, {2465, 1}, {2460, 1}, {2478, 1}, {2643, 1}, {2661, 1}},
			--container jagged sword, daramian mace, rope, shovel, health potion
			{{8602, 1}, {2439, 1}, {2120, 1}, {2554, 1}, {7618, 1}}
		}
	}
}

function onCreatureAppear(cid) npcHandler:onCreatureAppear(cid) end
function onCreatureDisappear(cid) npcHandler:onCreatureDisappear(cid) end
function onCreatureSay(cid, type, msg) npcHandler:onCreatureSay(cid, type, msg) end
function onThink() npcHandler:onThink() end

local function greetCallback(cid)
	local player = Player(cid)
	local level = player:getLevel()
	if level < 8 then
		npcHandler:say("CHILD! COME BACK WHEN YOU HAVE GROWN UP!", cid)
		return false
	elseif level > 9 then
		npcHandler:say(string.upper(player:getName()) ..", I CAN'T LET YOU LEAVE - YOU ARE TOO STRONG ALREADY! YOU CAN ONLY LEAVE WITH LEVEL 9 OR LOWER.", cid)
		return false
	elseif player:getVocation():getId() > 0 then
		npcHandler:say("YOU ALREADY HAVE A VOCATION!", cid)
		return false
	else
		npcHandler:setMessage(MESSAGE_GREET, string.upper(player:getName()) ..", ARE YOU PREPARED TO FACE YOUR DESTINY?")
	end
	return true
end

local function creatureSayCallback(cid, type, msg)
	if not npcHandler:isFocused(cid) then
		return false
	end

	local player = Player(cid)
	if npcHandler.topic[cid] == 0 then
		if msgcontains(msg, "yes") then
			npcHandler:say("IN WHICH TOWN DO YOU WANT TO LIVE: {RHYVES}?", cid)
			npcHandler.topic[cid] = 1
		end
	elseif npcHandler.topic[cid] == 1 then
		local cityTable = config.towns[string.lower(msg)]
		if cityTable then
			town[cid] = cityTable
			npcHandler:say("IN ".. string.upper(msg) .."! AND WHAT PROFESSION HAVE YOU CHOSEN: {KNIGHT}, {PALADIN}, {SORCERER}, OR {DRUID}?", cid)
			npcHandler.topic[cid] = 2
		else
			npcHandler:say("IN WHICH TOWN DO YOU WANT TO LIVE: {RHYVES}?", cid)
		end
	elseif npcHandler.topic[cid] == 2 then
		local vocationTable = config.vocations[string.lower(msg)]
		if vocationTable then
			npcHandler:say(vocationTable.text, cid)
			npcHandler.topic[cid] = 3
			vocation[cid] = vocationTable.vocationId
		else
			npcHandler:say("{KNIGHT}, {PALADIN}, {SORCERER}, OR {DRUID}?", cid)
		end
	elseif npcHandler.topic[cid] == 3 then
		if msgcontains(msg, "yes") then
			npcHandler:say("SO BE IT!", cid)
			player:setVocation(Vocation(vocation[cid]))
			player:setTown(Town(town[cid]))
			player:getPosition():sendMagicEffect(CONST_ME_TELEPORT)
			player:teleportTo(Town(town[cid]):getTemplePosition())
			player:getPosition():sendMagicEffect(CONST_ME_TELEPORT)
			local targetVocation = config.vocations[string.lower(Vocation(vocation[cid]):getName())]
			for i = 1, #targetVocation[1] do
				player:addItem(targetVocation[1][i][1], targetVocation[1][i][2])
			end
			local backpack = player:addItem(1988)
			for i = 1, #targetVocation[2] do
				backpack:addItem(targetVocation[2][i][1], targetVocation[2][i][2])
			end
		else
			npcHandler:say("THEN WHAT? {KNIGHT}, {PALADIN}, {SORCERER}, OR {DRUID}?", cid)
			npcHandler.topic[cid] = 2
		end
	end
	return true
end

local function onAddFocus(cid)
	town[cid] = 0
	vocation[cid] = 0
end

local function onReleaseFocus(cid)
	town[cid] = nil
	vocation[cid] = nil
end

npcHandler:setCallback(CALLBACK_ONADDFOCUS, onAddFocus)
npcHandler:setCallback(CALLBACK_ONRELEASEFOCUS, onReleaseFocus)

npcHandler:setCallback(CALLBACK_GREET, greetCallback)
npcHandler:setMessage(MESSAGE_FAREWELL, "COME BACK WHEN YOU ARE PREPARED TO FACE YOUR DESTINY!")
npcHandler:setMessage(MESSAGE_WALKAWAY, "COME BACK WHEN YOU ARE PREPARED TO FACE YOUR DESTINY!")
npcHandler:setCallback(CALLBACK_MESSAGE_DEFAULT, creatureSayCallback)
npcHandler:addModule(FocusModule:new())
