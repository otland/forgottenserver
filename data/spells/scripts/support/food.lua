local foodIds = {
2666,--meat
2671,--ham
2681,--grape
2674,--apple
2689,--bread
2690,--roll
2696,--cheese
}

function onCastSpell(cid, var)
		local id = math.random(#foodIds)
		
		Player(cid):addItem(foodIds[id])
		Player(cid):getPosition():sendMagicEffect(CONST_ME_MAGIC_GREEN)

end
