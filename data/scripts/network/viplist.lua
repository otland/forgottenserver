-- player request add vip
local add = PacketHandler(0xDC)

function add.onReceive(player, msg)
	local name = msg:getString()
	player:addVIP(name)
end

add:register()

-- player request remove vip
local remove = PacketHandler(0xDD)

function remove.onReceive(player, msg)
	local vipGuid = msg:getU32()
	player:removeVIP(vipGuid)
end

remove:register()

-- player request edit vip
local edit = PacketHandler(0xDE)

function edit.onReceive(player, msg)
	local vipGuid = msg:getU32()
	local description = msg:getString()
	local icon = math.min(10, msg:getU32()) -- 10 is max icon in 9.63
	local notify = msg:getByte() ~= 0
	player:editVIP(vipGuid, description, icon, notify)
end

edit:register()
