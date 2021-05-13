local voodooDoll = Action()

function voodooDoll.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if target:getId() ~= 1 or not target:isPlayer() then
		return false
	end

	local msg = ""
	if math.random(100) <= 5 then
		msg = "You concentrate on your victim and hit the needle in the doll."
		player:addAchievement("Dark Voodoo Priest")
		toPosition:sendMagicEffect(CONST_ME_DRAWBLOOD, player)
	else
		msg = "You concentrate on your victim, hit the needle in the doll.......but nothing happens."
	end

	player:say(msg, TALKTYPE_MONSTER_SAY, false, player)
	return true
end

voodooDoll:id(3955) -- voodoo doll
voodooDoll:register()
