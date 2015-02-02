function onUpdateDatabase()
	print("> Updating database to version 12 (storing players record and message of the day in database)")

	local motdNum = ""
	local motd = ""

	local lastMotdFile = io.open("lastMotd.txt", "r")
	if lastMotdFile ~= nil then
		motdNum = lastMotdFile:read()
		motd = lastMotdFile:read()
		lastMotdFile:close()
	end

	local record = 0

	local playersRecordFile = io.open("playersRecord.txt", "r")
	if playersRecordFile ~= nil then
		record = playersRecordFile:read("*n")
		playersRecordFile:close()
	end

	db.query("INSERT INTO `server_config` (`config`, `value`) VALUES ('players_record', '" .. record .. "'), ('motd_hash', SHA1(" .. db.escapeString(motd) .. ")), ('motd_num', " .. db.escapeString(motdNum) .. ")")
	return true
end
