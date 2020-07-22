local spellbooks = {
	--{itemid, client_version}
	{25411, 1090}, -- book of lies
	{23771, 1055}, -- spellbook of the novice
	{22424, 1030}, -- umbral master spellbook
	{22423, 1030}, -- umbral spellbook
	{22422, 1030}, -- crude umbral spellbook
	{18401, 960}, -- spellbook of vigilance
	{16112, 960}, -- spellbook of ancient arcana
	{12647, 860}, -- snake god's wristguard
	{8918, 820}, -- spellbook of dark mysteries
	{8904, 820}, -- spellscroll of prophecies
	{8903, 820}, -- spellbook of lost souls
	{8902, 820}, -- spellbook of mind control
	{8901, 820}, -- spellbook of warding
	{8900, 820}, -- spellbook of enlightenment
	{6120, 780}, -- Dragha's spellbook
	{2175, 0} -- spellbook
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
				text[#text == nil and 1 or #text+1] = "Spells for Level "
			else
				text[#text+1] = "\nSpells for Level "
			end
			text[#text+1] = spell.level .. "\n"
			prevLevel = spell.level
		end
		text[#text+1] = spell.words .. " - " .. spell.name .. " : " .. spell.mana .. "\n"
	end

	player:showTextDialog(item:getId(), table.concat(text))
	return true
end

for _, s in ipairs(spellbooks) do
	if Game.getClientVersion().min >= s[2] then
		spellbook:id(s[1])
	end
end
spellbook:register()
