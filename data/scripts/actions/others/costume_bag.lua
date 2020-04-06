local config = {
	[653] = {"orc warrior", "pirate cutthroat", "dworc voodoomaster", "dwarf guard", "minotaur mage"}, -- common
	[654] = {"quara hydromancer", "diabolic imp", "banshee", "frost giant", "lich"}, -- uncommon
	[655] = {"serpent spawn", "demon", "juggernaut", "behemoth", "ashmunrah"}, -- deluxe
}

local costumeBag = Action()

function costumeBag.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local creatures = config[item.itemid]
	if not creatures then
		return true
	end
	player:setMonsterOutfit(creatures[math.random(#creatures)], 5 * 60 * 10 * 1000)
	item:getPosition():sendMagicEffect(CONST_ME_MAGIC_RED)
	item:remove()
	return true
end

for index, value in pairs(config) do
	costumeBag:id(index)
end

costumeBag:register()
