local afflictedOutfit = Action()

function afflictedOutfit.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if not player:isPremium() then
		player:sendCancelMessage(RETURNVALUE_YOUNEEDPREMIUMACCOUNT)
		return true
	end
	local hasOutfit = player:getStorageValue(PlayerStorageKeys.afflictedOutfit) == 1
	if item.itemid == 13925 then -- plague mask
		if not hasOutfit then
			return false
		end
		if player:getStorageValue(PlayerStorageKeys.afflictedPlagueMask) == 1 then
			return false
		end
		player:addOutfitAddon(430, 2)
		player:addOutfitAddon(431, 2)
		player:setStorageValue(PlayerStorageKeys.afflictedPlagueMask, 1)
		player:say("You gained a plague mask for your outfit.", TALKTYPE_MONSTER_SAY, false, player)
		if player:hasOutfit(looktype, 3) then
			player:addAchievement("Beak Doctor")
		end
		item:remove(1)
	elseif item.itemid == 13926 then -- plague bell
		if not hasOutfit then
			return false
		end
		if player:getStorageValue(PlayerStorageKeys.addonPlagueBell) == 1 then
			return false
		end
		player:addOutfitAddon(430, 1)
		player:addOutfitAddon(431, 1)
		player:setStorageValue(PlayerStorageKeys.addonPlagueBell, 1)
		player:say("You gained a plague bell for your outfit.", TALKTYPE_MONSTER_SAY, false, player)
		if player:hasOutfit(looktype, 3) then
			player:addAchievement("Beak Doctor")
		end
		item:remove(1)
	else -- outfit
		if hasOutfit then
			return false
		end
		for id = 13540, 13545 do
			if player:getItemCount(id) < 1 then
				return false
			end
		end
		for id = 13540, 13545 do
			player:removeItem(id, 1)
		end
		player:addOutfit(430)
		player:addOutfit(431)
		player:getPosition():sendMagicEffect(CONST_ME_GREEN_RINGS)
		player:setStorageValue(PlayerStorageKeys.afflictedOutfit, 1)
		player:say("You have restored an outfit.", TALKTYPE_MONSTER_SAY, false, player)
	end
	return true
end

afflictedOutfit:id(13540, 13541, 13542, 13543, 13544, 13545, 13925, 13926)
afflictedOutfit:register()
