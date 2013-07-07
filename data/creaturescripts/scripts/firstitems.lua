local firstItems = {2050, 2382}

function onLogin(cid)
	if getPlayerLastLoginSaved(cid) == 0 then
		for i = 1, table.maxn(firstItems) do
			doPlayerAddItem(cid, firstItems[i], 1)
		end
		doPlayerAddItem(cid, getPlayerSex(cid) == 0 and 2651 or 2650, 1)
		local bag = doPlayerAddItem(cid, 1987, 1)
		doAddContainerItem(bag, 2674, 1)
	end
	return true
end
