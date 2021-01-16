local config = {
	{chance = {0.0, 1.54}, transformId = 11342, description = "This little figurine of Brog, the raging Titan, was skillfully made by |PLAYERNAME|.", achievement = true},
	{chance = {1.54, 9.16}, transformId = 11341, description = "It was made by |PLAYERNAME| and is clearly a little figurine of.. hm, one does not recognise that yet."},
	{chance = {9.16, 25.48}, transformId = 11340, description = "It was made by |PLAYERNAME|, whose potter skills could use some serious improvement."},
	{chance = {25.48, 100.0}, remove = true, sound = "Aw man. That did not work out too well."}
}

local clayLump = Action()

function clayLump.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local random, tmpItem = math.random(0, 10000) * 0.01
	for i = 1, #config do
		tmpItem = config[i]
		if random >= tmpItem.chance[1] and random < tmpItem.chance[2] then
			item:getPosition():sendMagicEffect(CONST_ME_POFF)

			if tmpItem.remove then
				item:remove()
			else
				item:transform(tmpItem.transformId)
			end

			if tmpItem.sound then
				player:say(tmpItem.sound, TALKTYPE_MONSTER_SAY, false, player)
			end

			if tmpItem.description then
				item:setAttribute(ITEM_ATTRIBUTE_DESCRIPTION, tmpItem.description:gsub('|PLAYERNAME|', player:getName()))
			end

			if tmpItem.achievement then
				player:addAchievement("Clay Fighter")
				player:addAchievementProgress("Clay to Fame", 5)
			end

			break
		end
	end
	return true
end

clayLump:id(11339)
clayLump:register()
