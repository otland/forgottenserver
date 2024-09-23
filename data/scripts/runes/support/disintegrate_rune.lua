local corpseIds = {3058, 3059, 3060, 3061, 3064, 3065, 3066}
local removalLimit = 500

local spell = Spell(SPELL_RUNE)

function spell.onCastSpell(creature, variant, isHotkey)
	local position = variant:getPosition()
	local tile = Tile(position)
	if tile then
		if tile:getHouse() then
			position:sendMagicEffect(CONST_ME_POFF)
			return true
		end
		local items = tile:getItems()
		if items then
			for i, item in ipairs(items) do
				if item:getType():isMovable() and item:getUniqueId() > 65535 and item:getActionId() == 0 and not table.contains(corpseIds, item:getId()) and not item:isPodium() and not item:isStoreItem() then
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

spell:group("support")
spell:id(78)
spell:name("Disintegrate Rune")
spell:runeId(2310)
spell:charges(3)
spell:runeLevel(21)
spell:runeMagicLevel(4)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:range(1)
spell:register()
