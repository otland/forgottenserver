local goldenOutfitMemorial = Action()

function goldenOutfitMemorial.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local resultId = db.storeQuery("SELECT `player_id` FROM `player_storage` WHERE `key` = " .. Storage.GoldenOutfit .. " AND `value` >= 3;")
	if resultId ~= false then
		local message = "The following characters have spent a fortune on a Golden Outfit:\n\nFull Outfit for 1,000,000,000 gold:\n\n"
		repeat
			local resultId2 = db.storeQuery("SELECT `name` FROM `players` WHERE `id` = " .. result.getDataInt(resultId, "player_id") .. ";")
			if resultId2 ~= false then
				local playerName = result.getString(resultId2, "name")
				message = ""..message.."- ".. playerName .."\n"
				result.free(resultId2)
			end
		until not result.next(resultId)

		result.free(resultId)
		player:showTextDialog(item.itemid, message)
	else
		player:showTextDialog(item.itemid, "The Golden Outfit has not been acquired by anyone yet.")
	end
	return true
end

goldenOutfitMemorial:id(34174-34179)
goldenOutfitMemorial:register()
