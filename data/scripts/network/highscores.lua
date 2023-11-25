local handler = PacketHandler(0xB1)

function handler.onReceive(player, msg)
	local params = {}
	params.action = msg:getByte()
	params.category = msg:getByte()
	params.vocation = msg:getU32()
	params.world = msg:getString()
	params.worldType = msg:getByte()
	params.battlEye = msg:getByte()
	params.page = msg:getU16()
	params.totalPages = msg:getU16()

	if params.category == 0 then
		params.category = 1
	end

	local highscores = Highscores(params)
	highscores:refresh()
	highscores:render(player)
end

handler:register()
