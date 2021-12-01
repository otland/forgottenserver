local goldenOutfitMemorial = Action()

function goldenOutfitMemorial.onUse(player, item, fromPosition, target, toPosition, isHotkey)
    local resultId = db.storeQuery("SELECT `name` FROM `player_storage` INNER JOIN `players` ON `name` = `name` WHERE `key` = " .. PlayerStorageKeys.goldenOutfit .. " AND `value` >= 3;")
    if not resultId then
        player:showTextDialog(item.itemid, "The Golden Outfit has not been acquired by anyone yet.")
        result.free(resultId)
        return true
    end

    local message = "The following characters have spent a fortune on a Golden Outfit:\n\nFull Outfit for 1,000,000,000 gold:\n\n"
    repeat
        message = message .. string.format("- %s\n", result.getString(resultId, "name"))
    until not result.next(resultId)

    result.free(resultId)
    player:showTextDialog(item.itemid, message)
    return true
end

goldenOutfitMemorial:id(34174, 34175, 34176, 34177, 34178, 34179)
goldenOutfitMemorial:register()
