local spellbooks = {
	2175, -- spellbook
	6120, -- Dragha's spellbook
	8900, -- spellbook of enlightenment
	8901, -- spellbook of warding
	8902, -- spellbook of mind control
	8903, -- spellbook of lost souls
	8904, -- spellscroll of prophecies
	8918, -- spellbook of dark mysteries
	16112, -- spellbook of ancient arcana
	18401, -- spellbook of vigilance
	22422, -- crude umbral spellbook
	22423, -- umbral spellbook
	22424, -- umbral master spellbook
	23771 -- spellbook of the novice
}

local spellbook = Action()

function spellbook.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local text = {}
	local spells = {}
	for _, spell in ipairs(player:getInstantSpells()) do
		if spell.level ~= 0 then
			if spell.manapercent > 0 then
				spell.mana = spell.manapercent .. "%"
			end
			spells[#spells + 1] = spell
		end
	end

	table.sort(spells, function(a, b) return a.level < b.level end)

	local prevLevel = -1
	for i, spell in ipairs(spells) do
		if prevLevel ~= spell.level then
			if i == 1 then
				text[not #text and 1 or #text + 1] = "Spells for Level "
			else
				text[#text + 1] = "\nSpells for Level "
			end
			text[#text + 1] = spell.level .. "\n"
			prevLevel = spell.level
		end
		text[#text + 1] = spell.words .. " - " .. spell.name .. " : " .. spell.mana .. "\n"
	end

	player:showTextDialog(item:getId(), table.concat(text))
	return true
end

spellbook:id(spellbooks)
spellbook:register()
