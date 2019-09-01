local corpseIds = {3058, 3059, 3060, 3061, 3064, 3065, 3066}
local removalLimit = 500

local spell = Spell("rune")

function spell.onCastSpell(creature, variant, isHotkey)
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

spell:name("Disintegrate Rune")
spell:group("support")
spell:id(78)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(21)
spell:range(1)
spell:magicLevel(4)
spell:runeId(2310)
spell:charges(3)
spell:isAggressive(false)
spell:allowFarUse(false)
spell:register()
