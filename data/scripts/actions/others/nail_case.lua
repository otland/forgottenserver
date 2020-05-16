local messages = {
	[1] = "You carefully cut the nail of the gravedigger's thumb.",
	[2] = "You successfully manicured the forefinger of the gravedigger.",
	[3] = "You successfully manicured the middlefinger of the gravedigger.",
	[4] = "You actually manicured the ring finger of the gravedigger without a problem."
}

local nailCase = Action()

function nailCase.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if not target:isCreature() or not target:isMonster() or target:getMaster() then
		return false
	end

	if not player:isPremium() then
		player:sendCancelMessage(RETURNVALUE_YOUNEEDPREMIUMACCOUNT)
		return true
	end

	if target:getName():lower() ~= "gravedigger" then
		return true
	end

	if player:hasMount(39) then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You already have the obedience of the hellgrip.")
		return true
	end

	if math.random(100) >= 80 then
		target:remove()
		player:say("You failed to manicure the nails of the sensitive gravedigger. Before you can make amends, the creature runs away in agony.", TALKTYPE_MONSTER_SAY)
		player:setStorageValue(PlayerStorageKeys.nailCaseUseCount, 0)
		return true
	end

	player:setStorageValue(PlayerStorageKeys.nailCaseUseCount, player:getStorageValue(PlayerStorageKeys.nailCaseUseCount) + 1)
	local count = player:getStorageValue(PlayerStorageKeys.nailCaseUseCount)
	local message = messages[count]
	if message then
		player:say(message, TALKTYPE_MONSTER_SAY)
		return true
	end
	
	player:say("You did it! A completely manicured gravedigger is ready to follow you as your own personal trusty mount until the bitter end!", TALKTYPE_MONSTER_SAY)
	item:remove(1)
	target:remove()
	player:addMount(39)
	player:addAchievement("Natural Born Cowboy")
	player:addAchievement("Blacknailed")
	return true
end

nailCase:id(21452)
nailCase:register()
