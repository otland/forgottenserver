local annihilatorReward = {1990, 2400, 2431, 2494}
function onUse(cid, item, fromPosition, itemEx, toPosition)
	if item.uid > 1000 and item.uid < 10000 then
		local itemWeight = getItemWeight(item.uid, 1, FALSE)
		local playerCap = getPlayerFreeCap(cid)
		if isInArray(annihilatorReward, item.uid) == TRUE then
			if getPlayerStorageValue(cid, 30015) == -1 then
				if playerCap >= itemWeight then
					if item.uid == 1990 then
						local container = doPlayerAddItem(cid, 1990, 1)
						doAddContainerItem(container, 2326, 1)
					else
						doPlayerAddItem(cid, item.uid, 1)
					end
					doPlayerSendTextMessage(cid, MESSAGE_INFO_DESCR, 'You have found a ' .. getItemName(item.uid) .. '.')
					setPlayerStorageValue(cid, 30015, 1)
				else
					doPlayerSendTextMessage(cid, MESSAGE_INFO_DESCR, 'You have found a ' .. getItemName(item.uid) .. ' weighing ' .. itemWeight .. ' oz it\'s too heavy.')
				end
			else
				doPlayerSendTextMessage(cid, MESSAGE_INFO_DESCR, "It is empty.")
			end
		elseif getPlayerStorageValue(cid, item.uid) == -1 then
			if playerCap >= itemWeight then
				doPlayerSendTextMessage(cid, MESSAGE_INFO_DESCR, 'You have found a ' .. getItemName(item.uid) .. '.')
				doPlayerAddItem(cid, item.uid, 1)
				setPlayerStorageValue(cid, item.uid, 1)
			else
				doPlayerSendTextMessage(cid, MESSAGE_INFO_DESCR, 'You have found a ' .. getItemName(item.uid) .. ' weighing ' .. itemWeight .. ' oz it\'s too heavy.')
			end
		else
			doPlayerSendTextMessage(cid, MESSAGE_INFO_DESCR, "It is empty.")
		end
	else
		return FALSE
	end
	return TRUE
end