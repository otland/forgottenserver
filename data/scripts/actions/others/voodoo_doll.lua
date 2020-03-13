local voodooDoll = Action()

function voodooDoll.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if target.itemid ~= 1 or not target:isPlayer() then
		return false
	end

	local text = ""
	if math.random(100) <= 5 then
		text = "You concentrate on your victim and hit the needle in the doll."
		toPosition:sendMagicEffect(CONST_ME_DRAWBLOOD, player)
	else
		text = "You concentrate on your victim, hit the needle in the doll.......but nothing happens."
	end

	player:say(text, TALKTYPE_MONSTER_SAY, false, player)
	return true
end

voodooDoll:id(3955)
voodooDoll:register()
