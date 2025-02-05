local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_FIREDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HITBYFIRE)

combat:setArea(createCombatArea({{0, 1, 1, 1, 0}, {1, 1, 1, 1, 1}, {1, 1, 3, 1, 1}, {1, 1, 1, 1, 1}, {0, 1, 1, 1, 0}}))

function spellCallback(param)
	local tile = Tile(Position(param.pos))
	if tile then
		if tile:getTopCreature() and tile:getTopCreature():isMonster() then
			if tile:getTopCreature():getName():lower() == "the duke of the depths" then
				tile:getTopCreature():addHealth(math.random(0, 2000))
			end
		end
	end
end

function onTargetTile(cid, pos)
	local param = {}
	param.cid = cid
	param.pos = pos
	param.count = 0
	spellCallback(param)
end

combat:setCallback(CALLBACK_PARAM_TARGETTILE, "onTargetTile")

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("aggresive lava bomb")
spell:words("###75")
spell:isSelfTarget(true)
spell:needLearn(true)
spell:register()
