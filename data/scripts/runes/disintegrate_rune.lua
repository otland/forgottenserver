local corpseIds = {3058, 3059, 3060, 3061, 3064, 3065, 3066}
local removalLimit = 500

local desintegrateRune = Spell("rune")

function desintegrateRune.onCastSpell(creature, variant, isHotkey)
	local position = variant:getPosition()
	local tile = Tile(position)
	if tile then
		local items = tile:getItems()
		if items then
			for i, item in ipairs(items) do
				if item:getType():isMovable() and item:getUniqueId() > 65535 and item:getActionId() == 0 and not table.contains(corpseIds, item:getId()) then
					item:remove()
				end

				if i == removalLimit then
					break
				end
			end
		end
	end

	position:sendMagicEffect(CONST_ME_POFF)
	return true
end

desintegrateRune:group("support")
desintegrateRune:id(78)
desintegrateRune:name("Disintegrate Rune")
desintegrateRune:runeId(2278)
desintegrateRune:allowFarUse(true)
desintegrateRune:charges(3)
desintegrateRune:level(21)
desintegrateRune:magicLevel(4)
desintegrateRune:cooldown(2000)
desintegrateRune:groupCooldown(2000)
desintegrateRune:range(5)
desintegrateRune:register()
