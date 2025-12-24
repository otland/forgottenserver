local handler = PacketHandler(0xE6)

local BUG_CATEGORY_MAP = 0
local BUG_CATEGORY_TYPO = 1
local BUG_CATEGORY_TECHNICAL = 2
local BUG_CATEGORY_OTHER = 3

function handler.onReceive(player, msg)
	local category = msg:getByte()
	local message = msg:getString()

	local position
	if category == BUG_CATEGORY_MAP then
		position = msg:getPosition()
	end

	if player:getAccountType() == ACCOUNT_TYPE_NORMAL then
		return
	end

	local file = io.open("data/reports/bugs/" .. player:getName() .. " report.txt", "a")
	if not file then
		player:sendTextMessage(MESSAGE_EVENT_DEFAULT, "There was an error when processing your report, please contact a gamemaster.")
		return
	end

	io.output(file)
	io.write("------------------------------\n")
	io.write("Name: " .. player:getName())
	if position ~= nil then
		io.write(" [Map position: " .. position.x .. ", " .. position.y .. ", " .. position.z .. "]")
	end
	local playerPosition = player:getPosition()
	io.write(" [Player Position: " .. playerPosition.x .. ", " .. playerPosition.y .. ", " .. playerPosition.z .. "]\n")
	io.write("Comment: " .. message .. "\n")
	io.close(file)

	player:sendTextMessage(MESSAGE_EVENT_DEFAULT, "Your report has been sent to " .. configManager.getString(configKeys.SERVER_NAME) .. ".")
end

handler:register()
