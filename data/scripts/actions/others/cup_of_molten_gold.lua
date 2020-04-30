local cupOfMoltenGold = Action()

function cupOfMoltenGold.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local targetId = target.itemid
	if not table.contains({2700, 21428}, targetId) then -- fir tree or fir cone
		return false
	end
	if math.random(100) <= 10 then
		if targetId == 21428 then -- fir cone
			item:transform(13539) -- golden fir cone
		else
			player:addItem(13539, 1)
		end
	else
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Drizzling all over a fir cone you picked from the tree, the molten gold only covers about half of it - not enough.")
		if targetId == 21428 then -- fir cone
			target:remove(1)
		end
		item:remove(1)
	end
	return true
end

cupOfMoltenGold:id(13941) -- cup of molten gold
cupOfMoltenGold:register()
