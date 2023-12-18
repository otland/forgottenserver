-- player request add vip
local add = PacketHandler(0xDC)

function add.onReceive(player, msg)
	local name = msg:getString()
	player:addVip(name)
end

add:register()

-- player request remove Vip
local remove = PacketHandler(0xDD)

function remove.onReceive(player, msg)
	local vipGuid = msg:getU32()
	player:removeVip(vipGuid)
end

remove:register()

-- player request edit Vip
local edit = PacketHandler(0xDE)

function edit.onReceive(player, msg)
	local vipGuid = msg:getU32()
	local description = msg:getString()
	local icon = math.min(10, msg:getU32()) -- 10 is max icon in 9.63
	local notify = msg:getByte() ~= 0
	player:editVip(vipGuid, description, icon, notify)
end

edit:register()
