local spellbook = Action()

function spellbook.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local text = {}
	local spells = player:getInstantSpells()
	table.sort(spells, function(a, b) return a:level() < b:level() end)

	local prevLevel = -1
	for i, spell in ipairs(spells) do
		local mana = ""
		local words = ""

		if spell:level() ~= 0 then
			if spell:manaPercent() > 0 then
				mana = " : "..spell:manaPercent().."%"
			else
				mana = " : "..spell:mana()..""
			end
			if spell:hasParams() then
				words = ""..spell:words().." para"
			else
				words = spell:words()
			end

			if prevLevel ~= spell:level() then
				if prevLevel == -1 then
					text[not #text and 1 or #text + 1] = "Spells for Level "
				else
					text[#text + 1] = "\nSpells for Level "
				end
				text[#text + 1] = spell:level() .. "\n"
				prevLevel = spell:level()
			end
			text[#text + 1] = ""..words.." - "..spell:name()..""..mana.."\n"
		end
	end

	player:showTextDialog(item:getId(), table.concat(text))
	return true
end

spellbook:id(2175, 6120, 8900, 8901, 8902, 8903, 8904, 8918, 12647, 16112, 18401, 22422, 22423, 22424, 23771)
spellbook:register()
