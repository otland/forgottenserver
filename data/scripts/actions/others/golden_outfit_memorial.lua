local goldenOutfitMemorial = Action()

function goldenOutfitMemorial.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local resultId = db.storeQuery("SELECT `name`, `value` FROM `player_storage` INNER JOIN `players` as `p` ON `p`.`id` = `player_id` WHERE `key` = " .. PlayerStorageKeys.goldenOutfit .. " AND `value` >= 1;")
	if not resultId then
		player:showTextDialog(item.itemid, "The Golden Outfit has not been acquired by anyone yet.")
		result.free(resultId)
		return true
	end

	local playerAddons = {[1] = {}, [2] = {}, [3] = {}}
	repeat
		local addons = result.getNumber(resultId, "value")
		local name = result.getString(resultId, "name")
		table.insert(playerAddons[addons], "- " .. name)
	until not result.next(resultId)

	result.free(resultId)

	local message = "The following characters have spent a fortune on a Golden Outfit:\n"
	if #playerAddons[3] > 0 then
		message = message .. string.format("\nFull Outfit for 1,000,000,000 gold:\n%s", table.concat(playerAddons[3], "\n"))
	end

	if #playerAddons[2] > 0 then
		message = message .. string.format("\n\nWith One Addon for 750,000,000 gold:\n%s", table.concat(playerAddons[2], "\n"))
	end

	if #playerAddons[1] > 0 then
		message = message .. string.format("\n\nBasic Outfit for 500,000,000 gold:\n%s", table.concat(playerAddons[1], "\n"))
	end

	player:showTextDialog(item.itemid, message)
	return true
end

goldenOutfitMemorial:id(34174, 34175, 34176, 34177, 34178, 34179)
goldenOutfitMemorial:register()
