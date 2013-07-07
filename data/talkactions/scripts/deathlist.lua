dofile("./config.lua")

function onSay(cid, words, param)
	if sqlType == "mysql" then
		env = luasql.mysql()
		sql = env:connect(mysqlDatabase, mysqlUser, mysqlPass, mysqlHost, mysqlPort)
	else -- sqlite
		env = luasql.sqlite3()
		sql = env:connect(sqliteDatabase)
	end
	local res = sql:execute("SELECT `name`, `id` FROM `players` WHERE `name` = '" .. escapeString(param) .. "';")
	local row = res:fetch({}, "a")
	res:close()
	if row ~= nil then
		local targetName = row.name
		local targetGUID = row.id
		local str = ""
		local breakline = ""
		local result = sql:execute("SELECT `time`, `level`, `killed_by`, `is_player` FROM `player_deaths` WHERE `player_id` = " .. targetGUID .. " ORDER BY `time` DESC;")
		row = result:fetch()
		while row do
			if str ~= "" then
				breakline = "\n"
			end
			local date = os.date("*t", row.time)

			local article = ""
			if tonumber(row.is_player) ~= TRUE then
				row.killed_by = string.lower(row.killed_by)
				article = getArticle(row.killed_by) .. " "
			end

			if date.day < 10 then date.day = "0" .. date.day end
			if date.hour < 10 then date.hour = "0" .. date.hour end
			if date.min < 10 then date.min = "0" .. date.min end
			if date.sec < 10 then date.sec = "0" .. date.sec end
			str = str .. breakline .. " " .. date.day .. getMonthDayEnding(date.day) .. " " .. getMonthString(date.month) .. " " .. date.year .. " " .. date.hour .. ":" .. date.min .. ":" .. date.sec .. "   Died at Level " .. row.level .. " by " .. article .. row.killed_by .. "."
			row = result:fetch()
		end
		result:close()
		if str == "" then
			str = "No deaths."
		end
		doPlayerPopupFYI(cid, "Deathlist for player, " .. targetName .. ".\n\n" .. str)
	else
		doPlayerSendCancel(cid, "A player with that name does not exist.")
	end
	sql:close()
	env:close()
end
