local config = {
	[9075] = {"orc warrior", "pirate cutthroat", "dworc voodoomaster", "dwarf guard", "minotaur mage"}, -- common
	[9077] = {"serpent spawn", "demon", "juggernaut", "behemoth", "ashmunrah"}, -- deluxe
	[9076] = {"quara hydromancer", "diabolic imp", "banshee", "frost giant", "lich"} -- uncommon
}

local costumeBag = Action()

function costumeBag.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local creatures = config[item.itemid]
	if not creatures then
		return true
	end
	player:setMonsterOutfit(creatures[math.random(#creatures)], 5 * 60 * 10 * 1000)
	player:addAchievementProgress("Masquerader", 100)
	item:getPosition():sendMagicEffect(CONST_ME_MAGIC_RED)
	item:remove()
	return true
end

for k,v in pairs(config) do
	costumeBag:id(k)
end
costumeBag:register()
