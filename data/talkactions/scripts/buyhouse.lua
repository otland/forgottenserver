function onSay(player, words, param)
	local housePrice = configManager.getNumber(configKeys.HOUSE_PRICE)
	if housePrice == -1 then
		return true
	end

	if not player:isPremium() then
		player:sendCancelMessage("You need a premium account.")
		return false
	end

	local position = player:getPosition()
	position:getNextPosition(player:getDirection())

	local tile = Tile(position)
	local house = tile and tile:getHouse()
	if not house then
		player:sendCancelMessage("You have to be looking at the door of the house you would like to buy.")
		return false
	end

	-- HOUSE_TYPE_NORMAL
	if house:getType() == HOUSE_TYPE_NORMAL then
		if house:getOwnerGuid() > 0 then
			player:sendCancelMessage("This house already has an owner.")
			return false
		end

		if player:getHouse() then
			player:sendCancelMessage("You are already the owner of a house.")
			return false
		end

		local price = house:getTileCount() * housePrice
		if not player:removeMoney(price) then
			player:sendCancelMessage("You do not have enough money.")
			return false
		end

		house:setOwnerGuid(player:getGuid())
		player:sendTextMessage(MESSAGE_INFO_DESCR, "You have successfully bought this house, be sure to have the money for the rent in the bank.")
		return false
	end

	-- HOUSE_TYPE_GUILDHALL
	if house:getOwnerGuild() > 0 then
		player:sendCancelMessage("This guildhall already has an owner.")
		return false
	end

	local guild = player:getGuild()
	if guild:getHouseId() then
		player:sendCancelMessage("Your guild already owns a guildhall.")
		return false
	end

	local price = house:getTileCount() * housePrice
	local balance = guild:getBankBalance()
	if price > balance then
		player:sendCancelMessage("Your guild bank do not have enough money, it is missing ".. price - balance .. " gold.")
		return false
	end

	guild:setBankBalance(balance - price)
	local currentTime = os.time()
	local insertData = table.concat({
		guild:getId(),
		player:getGuid(),
		"'WITHDRAW'",
		"'RENT'",
		price,
		currentTime
	},',')
	db.query("INSERT INTO `guild_transactions` (`guild_id`,`player_associated`,`type`,`category`,`balance`,`time`) VALUES ("..insertData..");")
	local receipt = Game.createItem(ITEM_RECEIPT_SUCCESS, 1)
	receipt:setAttribute(ITEM_ATTRIBUTE_TEXT, "Date: " .. os.date("%d. %b %Y - %H:%M:%S", currentTime) .. "\nType: Guild Withdraw\nCategory: House rent\nHouse: ".. house:getName() .. "\nGold Amount: " .. price .. "\nReceipt Owner: " .. player:getName() .. "\nRecipient: The " .. guild:getName() .. "\n\nWe are happy to inform you that your transfer request was successfully carried out.")
	player:addItemEx(receipt)

	house:setOwnerGuid(player:getGuid())
	player:sendTextMessage(MESSAGE_INFO_DESCR, "You have successfully bought this guildhall, be sure to have the money for the rent in the guild bank.")
	return false
end
