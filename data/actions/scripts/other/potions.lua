local ultimateHealthPot = 8473
local greatHealthPot = 7591
local greatManaPot = 7590
local greatSpiritPot = 8472
local strongHealthPot = 7588
local strongManaPot = 7589
local healthPot = 7618
local manaPot = 7620
local smallHealthPot = 8704
local antidotePot = 8474
local greatEmptyPot = 7635
local strongEmptyPot = 7634
local emptyPot = 7636

local exhaust = createConditionObject(CONDITION_EXHAUST_HEAL)
setConditionParam(exhaust, CONDITION_PARAM_TICKS, (configManager.getNumber(configKeys.EX_ACTIONS_DELAY_INTERVAL) - 100))
-- 1000 - 100 due to exact condition timing. -100 doesn't hurt us, and players don't have reminding ~50ms exhaustion.

function onUse(cid, item, fromPosition, itemEx, toPosition)
	if itemEx.itemid ~= 1 or itemEx.type ~= THING_TYPE_PLAYER then
		return true
	end

	if(getCreatureCondition(cid, CONDITION_EXHAUST_HEAL) == true) then
		doPlayerSendDefaultCancel(cid, RETURNVALUE_YOUAREEXHAUSTED)
		return true
	end

	if(item.itemid == antidotePot) then
		doRemoveCondition(itemEx.uid, CONDITION_POISON)
		doAddCondition(cid, exhaust)
		doCreatureSay(itemEx.uid, "Aaaah...", TALKTYPE_ORANGE_1)
		doRemoveItem(item.uid, 1)
		doPlayerAddItem(cid, emptyPot, 1)
	elseif(item.itemid == smallHealthPot) then
		if(doTargetCombatHealth(0, itemEx.uid, COMBAT_HEALING, 50, 100, CONST_ME_MAGIC_BLUE) == LUA_ERROR) then
			return false
		end
		doAddCondition(cid, exhaust)
		doCreatureSay(itemEx.uid, "Aaaah...", TALKTYPE_ORANGE_1)
		doRemoveItem(item.uid, 1)
		doPlayerAddItem(cid, emptyPot, 1)
	elseif(item.itemid == healthPot) then
		if(doTargetCombatHealth(0, itemEx.uid, COMBAT_HEALING, 100, 200, CONST_ME_MAGIC_BLUE) == LUA_ERROR) then
			return false
		end
		doAddCondition(cid, exhaust)
		doCreatureSay(itemEx.uid, "Aaaah...", TALKTYPE_ORANGE_1)
		doRemoveItem(item.uid, 1)
		doPlayerAddItem(cid, emptyPot, 1)
	elseif(item.itemid == manaPot) then
		if(doTargetCombatMana(0, itemEx.uid, 70, 130, CONST_ME_MAGIC_BLUE) == LUA_ERROR) then
			return false
		end
		doAddCondition(cid, exhaust)
		doCreatureSay(itemEx.uid, "Aaaah...", TALKTYPE_ORANGE_1)
		doRemoveItem(item.uid, 1)
		doPlayerAddItem(cid, emptyPot, 1)
	elseif(item.itemid == strongHealthPot) then
		if(not(isKnight(itemEx.uid) or isPaladin(itemEx.uid)) or (getPlayerLevel(itemEx.uid) < 50)) and not(getPlayerGroupId(cid) >= 2) then
			doCreatureSay(cid, "This potion can only be consumed by paladins and knights of level 50 or higher.", TALKTYPE_ORANGE_1)
			return true
		end

		if(doTargetCombatHealth(0, itemEx.uid, COMBAT_HEALING, 200, 400, CONST_ME_MAGIC_BLUE) == LUA_ERROR) then
			return false
		end
		doAddCondition(cid, exhaust)
		doCreatureSay(itemEx.uid, "Aaaah...", TALKTYPE_ORANGE_1)
		doRemoveItem(item.uid, 1)
		doPlayerAddItem(cid, strongEmptyPot, 1)
	elseif(item.itemid == strongManaPot) then
		if(not(isSorcerer(itemEx.uid) or isDruid(itemEx.uid) or isPaladin(itemEx.uid)) or (getPlayerLevel(itemEx.uid) < 50)) and not(getPlayerGroupId(cid) >= 2) then
			doCreatureSay(cid, "This potion can only be consumed by sorcerers, druids and paladins of level 50 or higher.", TALKTYPE_ORANGE_1)
			return true
		end

		if(doTargetCombatMana(0, itemEx.uid, 110, 190, CONST_ME_MAGIC_BLUE) == LUA_ERROR) then
			return false
		end
		doAddCondition(cid, exhaust)
		doCreatureSay(itemEx.uid, "Aaaah...", TALKTYPE_ORANGE_1)
		doRemoveItem(item.uid, 1)
		doPlayerAddItem(cid, strongEmptyPot, 1)
	elseif(item.itemid == greatSpiritPot) then
		if(not(isPaladin(itemEx.uid)) or (getPlayerLevel(itemEx.uid) < 80)) and not(getPlayerGroupId(cid) >= 2) then
			doCreatureSay(cid, "This potion can only be consumed by paladins of level 80 or higher.", TALKTYPE_ORANGE_1)
			return true
		end

		if(doTargetCombatHealth(0, itemEx.uid, COMBAT_HEALING, 200, 400, CONST_ME_MAGIC_BLUE) == LUA_ERROR or doTargetCombatMana(0, itemEx.uid, 110, 190, CONST_ME_MAGIC_BLUE) == LUA_ERROR) then
			return false
		end
		doAddCondition(cid, exhaust)
		doCreatureSay(itemEx.uid, "Aaaah...", TALKTYPE_ORANGE_1)
		doRemoveItem(item.uid, 1)
		doPlayerAddItem(cid, greatEmptyPot, 1)
	elseif(item.itemid == greatHealthPot) then
		if(not(isKnight(itemEx.uid)) or (getPlayerLevel(itemEx.uid) < 80)) and not(getPlayerGroupId(cid) >= 2) then
			doCreatureSay(cid, "This potion can only be consumed by knights of level 80 or higher.", TALKTYPE_ORANGE_1)
			return true
		end

		if(doTargetCombatHealth(0, itemEx.uid, COMBAT_HEALING, 500, 700, CONST_ME_MAGIC_BLUE) == LUA_ERROR) then
			return false
		end
		doAddCondition(cid, exhaust)
		doCreatureSay(itemEx.uid, "Aaaah...", TALKTYPE_ORANGE_1)
		doRemoveItem(item.uid, 1)
		doPlayerAddItem(cid, greatEmptyPot, 1)
	elseif(item.itemid == greatManaPot) then
		if(not(isSorcerer(itemEx.uid) or isDruid(itemEx.uid)) or (getPlayerLevel(itemEx.uid) < 80)) and not(getPlayerGroupId(cid) >= 2) then
			doCreatureSay(cid, "This potion can only be consumed by sorcerers and druids of level 80 or higher.", TALKTYPE_ORANGE_1)
			return true
		end

		if(doTargetCombatMana(0, itemEx.uid, 200, 300, CONST_ME_MAGIC_BLUE) == LUA_ERROR) then
			return false
		end
		doAddCondition(cid, exhaust)
		doCreatureSay(itemEx.uid, "Aaaah...", TALKTYPE_ORANGE_1)
		doRemoveItem(item.uid, 1)
		doPlayerAddItem(cid, greatEmptyPot, 1)
	elseif(item.itemid == ultimateHealthPot) then
		if(not(isKnight(itemEx.uid)) or (getPlayerLevel(itemEx.uid) < 130)) and not(getPlayerGroupId(cid) >= 2) then
			doCreatureSay(cid, "This potion can only be consumed by knights of level 130 or higher.", TALKTYPE_ORANGE_1)
			return true
		end

		if(doTargetCombatHealth(0, itemEx.uid, COMBAT_HEALING, 800, 1000, CONST_ME_MAGIC_BLUE) == LUA_ERROR) then
			return false
		end
		doAddCondition(cid, exhaust)
		doCreatureSay(itemEx.uid, "Aaaah...", TALKTYPE_ORANGE_1)
		doRemoveItem(item.uid, 1)
		doPlayerAddItem(cid, greatEmptyPot, 1)
	end
	return true
end
