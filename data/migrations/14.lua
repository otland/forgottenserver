function onUpdateDatabase()
	print("> Updating database to version 15 (moving groups to data/XML/groups.xml)")

	db.query("ALTER TABLE players DROP FOREIGN KEY players_ibfk_2")
	db.query("DROP INDEX group_id ON players")

	db.query("ALTER TABLE accounts DROP FOREIGN KEY accounts_ibfk_1")
	db.query("DROP INDEX group_id ON accounts")
	db.query("ALTER TABLE `accounts` DROP `group_id`")

	local groupsFile = io.open("data/XML/groups.xml", "w")
	if groupsFile then
		groupsFile:write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n")
		groupsFile:write("<groups>\r\n")

		local resultId = db.storeQuery("SELECT `id`, `name`, `flags`, `access`, `maxdepotitems`, `maxviplist` FROM `groups` ORDER BY `id` ASC")
		if resultId ~= false then
			repeat
				groupsFile:write("\t<group id=\"" .. result.getNumber(resultId, "id") .. "\" name=\"" .. result.getString(resultId, "name") .. "\" flags=\"" .. string.format("%u", result.getNumber(resultId, "flags")) .. "\" access=\"" .. result.getNumber(resultId, "access") .. "\" maxdepotitems=\"" .. result.getNumber(resultId, "maxdepotitems") .. "\" maxvipentries=\"" .. result.getNumber(resultId, "maxviplist") .. "\" />\r\n")
			until not result.next(resultId)
			result.free(resultId)
		end

		groupsFile:write("</groups>\r\n")
		groupsFile:close()

		db.query("DROP TABLE `groups`")
	end
	return true
end
