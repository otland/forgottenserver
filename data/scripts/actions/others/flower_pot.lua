local flowers = {
	{itemid = 7655, watered = false, advance = false, msg = "You should plant some seeds first."},
	{itemid = 7665, watered = true, advance = false, msg = "You watered your plant.", after = 7673},
	{itemid = 7673, watered = false, advance = false, msg = "Your plant doesn't need water."},
	{itemid = 7670, watered = false, advance = false, msg = "Your plant doesn't need water."},
	{itemid = 7680, watered = false, advance = false, msg = "Your plant doesn't need water."},
	{itemid = 7682, watered = false, advance = false, msg = "Your plant doesn't need water."},
	{itemid = 7684, watered = false, advance = false, msg = "Your plant doesn't need water."},
	{itemid = 7686, watered = false, advance = false, msg = "Your plant doesn't need water."},
	{itemid = 7688, watered = false, advance = false, msg = "Your plant doesn't need water."},
	{itemid = 7690, watered = false, advance = false, msg = "Your plant doesn't need water."},
	{itemid = 7992, watered = false, advance = false, msg = "Your plant doesn't need water."},
	{itemid = 7994, watered = false, advance = false, msg = "Your plant doesn't need water."},
	{itemid = 9982, watered = false, advance = false, msg = "Your plant doesn't need water."},
	{itemid = 9990, watered = false, advance = false, msg = "Your plant doesn't need water."},
	{itemid = 7692, watered = false, advance = false, msg = "Your plant doesn't need water."},
	{itemid = 7694, watered = false, advance = false, msg = "Your plant doesn't need water."},
	{itemid = 9986, watered = false, advance = false, msg = "Your plant doesn't need water."},
	{itemid = 9988, watered = false, advance = false, msg = "Your plant doesn't need water."},
	{itemid = 7689, watered = true, advance = false, msg = "You watered your plant.", after = 7688},
	{itemid = 7691, watered = true, advance = false, msg = "You watered your plant.", after = 7690},
	{itemid = 7693, watered = true, advance = false, msg = "You watered your plant.", after = 7692},
	{itemid = 7695, watered = true, advance = false, msg = "You watered your plant.", after = 7694},
	{itemid = 9991, watered = true, advance = false, msg = "You watered your plant.", after = 9990},
	{itemid = 9989, watered = true, advance = false, msg = "You watered your plant.", after = 9988},
	{itemid = 7674, watered = true, advance = false, msg = "You finally remembered to water your plant and it recovered.", after = 7688},
	{itemid = 7675, watered = true, advance = false, msg = "You finally remembered to water your plant and it recovered.", after = 7690},
	{itemid = 7676, watered = true, advance = false, msg = "You finally remembered to water your plant and it recovered.", after = 7692},
	{itemid = 7677, watered = true, advance = false, msg = "You finally remembered to water your plant and it recovered.", after = 7694},
	{itemid = 9984, watered = true, advance = false, msg = "You finally remembered to water your plant and it recovered.", after = 9990},
	{itemid = 9985, watered = true, advance = false, msg = "You finally remembered to water your plant and it recovered.", after = 9988},
	{itemid = 7679, watered = true, advance = true, msg = {"You watered your plant.","Your plant has grown to the next stage!"}, after = {7673, 7670}, chance = 80},
	{itemid = 7681, watered = true, advance = true, msg = {"You watered your plant.","Your plant has grown to the next stage!"}, after = {7680, 7688}, chance = 80},
	{itemid = 7683, watered = true, advance = true, msg = {"You watered your plant.","Your plant has grown to the next stage!"}, after = {7682, 7690}, chance = 80},
	{itemid = 7685, watered = true, advance = true, msg = {"You watered your plant.","Your plant has grown to the next stage!"}, after = {7684, 7692}, chance = 80},
	{itemid = 7687, watered = true, advance = true, msg = {"You watered your plant.","Your plant has grown to the next stage!"}, after = {7686, 7694}, chance = 80},
	{itemid = 9983, watered = true, advance = true, msg = {"You watered your plant.","Your plant has grown to the next stage!"}, after = {9982, 9990}, chance = 80},
	{itemid = 9987, watered = true, advance = true, msg = {"You watered your plant.","Your plant has grown to the next stage!"}, after = {9986, 9988}, chance = 80},
	{itemid = 7678, watered = true, advance = true, msg = {"You watered your plant.","Your plant has grown to the next stage!"}, after = {7670, 7680, 7682, 7684, 7686, 9982, 9986}, chance = 80},
	{itemid = 15444, watered = true, advance = false, msg = "You finally remembered to water your plant and it recovered.", after = 15443},
	{itemid = 15442, watered = true, advance = true, msg = {"You watered your plant.","Your plant has grown to the next stage!"}, after = {15443, 15441}, chance = 80},
	{itemid = 15443, watered = false, advance = false, msg = "Your plant doesn't need water."},
	{itemid = 15441, watered = false, advance = false, msg = "This plant can't wither anymore."},
	{itemid = 15445, watered = false, advance = false, msg = "This plant can't wither anymore."},
	{itemid = 15446, watered = false, advance = false, msg = "This plant can't wither anymore."},
	{itemid = 15447, watered = false, advance = false, msg = "This plant can't wither anymore."},
	{itemid = 15448, watered = false, advance = false, msg = "This plant can't wither anymore."},
	{itemid = 15449, watered = false, advance = false, msg = "This plant can't wither anymore."},
	{itemid = 15450, watered = false, advance = false, msg = "This plant can't wither anymore."}
}

local flowerPot = Action()

function flowerPot.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	for _, flower in pairs(flowers) do
		if target.itemid == flower.itemid then
			if (flower.watered == false and flower.advance == false) then
				player:say(flower.msg, TALKTYPE_MONSTER_SAY)
			elseif (flower.watered == true and flower.advance == false) then
				target:transform(flower.after)
				player:say(flower.msg, TALKTYPE_MONSTER_SAY)
				toPosition:sendMagicEffect(CONST_ME_LOSEENERGY)
				target:decay()
			elseif (flower.watered == true and flower.advance == true) then
				local i = 1
				if (math.random(100) <= flower.chance) then
					i = 2
					target:transform(flower.after[math.random(2, #flower.after)])
				else
					target:transform(flower.after[i])
				end
				toPosition:sendMagicEffect(CONST_ME_LOSEENERGY)
				player:say(flower.msg[i], TALKTYPE_MONSTER_SAY)
				target:decay()
			end
		break
		end
	end
	return true
end

flowerPot:id(7734) -- watering can
flowerPot:register()
