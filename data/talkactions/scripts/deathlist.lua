function onSay(cid, words, param)
	local resultId = db.storeQuery("SELECT `id`, `name` FROM `players` WHERE `name` = " .. db.escapeString(param))
	if resultId ~= false then
		local targetGUID = result.getDataInt(resultId, "id")
		local targetName = result.getDataString(resultId, "name")
		result.free(resultId)
		local str = ""
		local breakline = ""

		local resultId = db.storeQuery("SELECT `time`, `level`, `killed_by`, `is_player` FROM `player_deaths` WHERE `player_id` = " .. targetGUID .. " ORDER BY `time` DESC")
		if resultId ~= false then
			repeat
				if str ~= "" then
					breakline = "\n"
				end
				local date = os.date("*t", result.getDataInt(resultId, "time"))

				local article = ""
				local killed_by = result.getDataString(resultId, "killed_by")
				if result.getDataInt(resultId, "is_player") == 0 then
					article = getArticle(killed_by) .. " "
					killed_by = string.lower(killed_by)
				end

				if date.day < 10 then date.day = "0" .. date.day end
				if date.hour < 10 then date.hour = "0" .. date.hour end
				if date.min < 10 then date.min = "0" .. date.min end
				if date.sec < 10 then date.sec = "0" .. date.sec end
				str = str .. breakline .. " " .. date.day .. getMonthDayEnding(date.day) .. " " .. getMonthString(date.month) .. " " .. date.year .. " " .. date.hour .. ":" .. date.min .. ":" .. date.sec .. "   Died at Level " .. result.getDataInt(resultId, "level") .. " by " .. article .. killed_by .. "."
			until not result.next(resultId)
			result.free(resultId)
		end

		if str == "" then
			str = "No deaths."
		end
		doPlayerPopupFYI(cid, "Deathlist for player, " .. targetName .. ".\n\n" .. str)
	else
		doPlayerSendCancel(cid, "A player with that name does not exist.")
	end
end
