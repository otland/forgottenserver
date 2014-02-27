function onUse(cid, item, fromPosition, itemEx, toPosition)
	local count = getPlayerInstantSpellCount(cid)
	local text = ""
	local t = {}
	for i = 0, count - 1 do
		local spell = getPlayerInstantSpellInfo(cid, i)
		if spell.level ~= 0 then
			if spell.manapercent > 0 then
				spell.mana = spell.manapercent .. "%"
			end
			t[#t+1] = spell
		end
	end
	table.sort(t, function(a, b) return a.level < b.level end)
	local prevLevel = -1
	for i, spell in ipairs(t) do
		local line = ""
		if prevLevel ~= spell.level then
			if i ~= 1 then
				line = "\n"
			end
			line = ("%sSpells for Level %s\n"):format(line, spell.level)
			prevLevel = spell.level
		end
		text = ("%s%s  %s - %s : %s\n"):format(text, line, spell.words, spell.name, spell.mana)
	end
	Player(cid):showTextDialog(item.itemid, text)
	return true
end
