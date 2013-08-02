function onUpdateDatabase()
	print("> Updating database to version 9 (global inbox)")
	db.query("CREATE TABLE IF NOT EXISTS `player_inboxitems` (`player_id` int(11) NOT NULL, `sid` int(11) NOT NULL, `pid` int(11) NOT NULL DEFAULT '0', `itemtype` smallint(6) NOT NULL, `count` smallint(5) NOT NULL DEFAULT '0', `attributes` blob NOT NULL, UNIQUE KEY `player_id_2` (`player_id`,`sid`), KEY `player_id` (`player_id`), FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE) ENGINE=InnoDB DEFAULT CHARSET=latin1")

	-- Delete "market" item
	db.query("DELETE FROM `player_depotitems` WHERE `itemtype` = 14405")

	-- Move up items in depot chests
	local resultId = db.storeQuery("SELECT `player_id`, `pid`, (SELECT `dp2`.`sid` FROM `player_depotitems` AS `dp2` WHERE `dp2`.`player_id` = `dp1`.`player_id` AND `dp2`.`pid` = `dp1`.`sid` AND `itemtype` = 2594) AS `sid` FROM `player_depotitems` AS `dp1` WHERE `itemtype` = 2589")
	if resultId ~= false then
		repeat
			db.query("UPDATE `player_depotitems` SET `pid` = " .. result.getDataInt(resultId, "pid") .. " WHERE `player_id` = " .. result.getDataInt(resultId, "player_id") .. " AND `pid` = " .. result.getDataInt(resultId, "sid"))
		until not result.next(resultId)
		result.free(resultId)
	end

	-- Delete the depot lockers
	db.query("DELETE FROM `player_depotitems` WHERE `itemtype` = 2589")

	-- Delete the depot chests
	db.query("DELETE FROM `player_depotitems` WHERE `itemtype` = 2594")

	resultId = db.storeQuery("SELECT DISTINCT `player_id` FROM `player_depotitems` WHERE `itemtype` = 14404")
	if resultId ~= false then
		repeat
			local playerId = result.getDataInt(resultId, "player_id")

			local runningId = 100

			local stmt = "INSERT INTO `player_inboxitems` (`player_id`, `sid`, `pid`, `itemtype`, `count`, `attributes`) VALUES "

			local resultId2 = db.storeQuery("SELECT `sid` FROM `player_depotitems` WHERE `player_id` = " .. playerId .. " AND `itemtype` = 14404")
			if resultId2 ~= false then
				repeat
					local sids = {}
					sids[#sids + 1] = result.getDataInt(resultId2, "sid")
					while #sids > 0 do
						local sid = sids[#sids]
						sids[#sids] = nil

						local resultId3 = db.storeQuery("SELECT * FROM `player_depotitems` WHERE `player_id` = " .. playerId .. " AND `pid` = " .. sid)
						if resultId3 ~= false then
							repeat
								local attr, attrSize = result.getDataStream(resultId3, "attributes")
								runningId = runningId + 1
								stmt = stmt .. "(" .. playerId .. "," .. runningId .. ",0," .. result.getDataInt(resultId3, "itemtype") .. "," .. result.getDataInt(resultId3, "count") .. "," .. db.escapeBlob(attr, attrSize) .. "),"
								sids[#sids + 1] = result.getDataInt(resultId3, "sid")

								db.query("DELETE FROM `player_depotitems` WHERE `player_id` = " .. result.getDataInt(resultId, "player_id") .. " AND `sid` = " .. result.getDataInt(resultId3, "sid"))
							until not result.next(resultId3)
							result.free(resultId3)
						end
					end
				until not result.next(resultId2)
				result.free(resultId2)
			end

			local stmtLen = string.len(stmt)
			if stmtLen > 102 then
				stmt = string.sub(stmt, 1, stmtLen - 1)
				db.query(stmt)
			end
		until not result.next(resultId)
		result.free(resultId)
	end

	-- Delete the inboxes
	db.query("DELETE FROM `player_depotitems` WHERE `itemtype` = 14404")
	return true
end
