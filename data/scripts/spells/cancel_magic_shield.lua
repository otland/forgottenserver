if configManager.getBoolean(configKeys.MANASHIELD_BREAKABLE) then
	local combat = Combat()
	combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
	combat:setParameter(COMBAT_PARAM_AGGRESSIVE, 0)

	local cancelMagicShield = Spell(SPELL_INSTANT)

	function cancelMagicShield.onCastSpell(creature, var)
		creature:removeCondition(CONDITION_MANASHIELD_BREAKABLE)
		return combat:execute(creature, var)
	end

	cancelMagicShield:name("Cancel Magic Shield")
	cancelMagicShield:id(44)
	cancelMagicShield:words("exana vita")
	cancelMagicShield:level(14)
	cancelMagicShield:mana(50)
	cancelMagicShield:group("support")
	cancelMagicShield:isAggressive(false)
	cancelMagicShield:isSelfTarget(true)
	cancelMagicShield:cooldown(2000)
	cancelMagicShield:groupCooldown(2000)
	cancelMagicShield:needLearn(false)
	cancelMagicShield:vocation("druid;true", "elder druid;true", "sorcerer;true", "master sorcerer;true")
	cancelMagicShield:register()
end
