registerMonsterType = {}
setmetatable(registerMonsterType,
{
	__call =
	function(self, mtype, mask)
		for _,parse in pairs(self) do
			parse(mtype, mask)
		end
	end
})

MonsterType.register = function(self, mask)
	return registerMonsterType(self, mask)
end

registerMonsterType.description = function(mtype, mask)
	if mask.description then
		mtype:nameDescription(mask.description)
	end
end
registerMonsterType.experience = function(mtype, mask)
	if mask.experience then
		mtype:experience(mask.experience)
	end
end
registerMonsterType.skull = function(mtype, mask)
	if mask.skull then
		mtype:skull(mask.skull)
	end
end
registerMonsterType.outfit = function(mtype, mask)
	if mask.outfit then
		mtype:outfit(mask.outfit)
	end
end
registerMonsterType.maxHealth = function(mtype, mask)
	if mask.maxHealth then
		mtype:maxHealth(mask.maxHealth)
	end
end
registerMonsterType.health = function(mtype, mask)
	if mask.health then
		mtype:health(mask.health)
	end
end
registerMonsterType.runHealth = function(mtype, mask)
	if mask.runHealth then
		mtype:runHealth(mask.runHealth)
	end
end
registerMonsterType.maxSummons = function(mtype, mask)
	if mask.maxSummons then
		mtype:maxSummons(mask.maxSummons)
	end
end
registerMonsterType.race = function(mtype, mask)
	if mask.race then
		mtype:race(mask.race)
	end
end
registerMonsterType.manaCost = function(mtype, mask)
	if mask.manaCost then
		mtype:manaCost(mask.manaCost)
	end
end
registerMonsterType.speed = function(mtype, mask)
	if mask.speed then
		mtype:baseSpeed(mask.speed)
	end
end
registerMonsterType.corpse = function(mtype, mask)
	if mask.corpse then
		mtype:corpseId(mask.corpse)
	end
end
registerMonsterType.flags = function(mtype, mask)
	if mask.flags then
		if mask.flags.attackable ~= nil then
			mtype:isAttackable(mask.flags.attackable)
		end
		if mask.flags.healthHidden ~= nil then
			mtype:isHealthHidden(mask.flags.healthHidden)
		end
		if mask.flags.convinceable ~= nil then
			mtype:isConvinceable(mask.flags.convinceable)
		end
		if mask.flags.illusionable ~= nil then
			mtype:isIllusionable(mask.flags.illusionable)
		end
		if mask.flags.hostile ~= nil then
			mtype:isHostile(mask.flags.hostile)
		end
		if mask.flags.pushable ~= nil then
			mtype:isPushable(mask.flags.pushable)
		end
		if mask.flags.canPushItems ~= nil then
			mtype:canPushItems(mask.flags.canPushItems)
		end
		if mask.flags.canPushCreatures ~= nil then
			mtype:canPushCreatures(mask.flags.canPushCreatures)
		end
		if mask.flags.targetDistance then
			mtype:targetDistance(mask.flags.targetDistance)
		end
		if mask.flags.staticAttackChance then
			mtype:staticAttackChance(mask.flags.staticAttackChance)
		end
	end
end
registerMonsterType.light = function(mtype, mask)
	if mask.light then
		if mask.light.color then
			local color = mask.light.color
		end
		if mask.light.level then
			mtype:light(color, mask.light.level)
		end
	end
end
registerMonsterType.changeTarget = function(mtype, mask)
	if mask.changeTarget then
		if mask.changeTarget.chance then
			mtype:changeTargetChance(mask.changeTarget.chance)
		end
		if mask.changeTarget.interval then
			mtype:changeTargetSpeed(mask.changeTarget.interval)
		end
	end
end
registerMonsterType.voices = function(mtype, mask)
	if type(mask.voices) == "table" then
		local interval, chance
		if mask.voices.interval then
			interval = mask.voices.interval
		end
		if mask.voices.chance then
			chance = mask.voices.chance
		end
		for k, v in pairs(mask.voices) do
			if type(v) == "table" then
				mtype:addVoice(v.text, interval, chance, v.yell)
			end
		end
	end
end
registerMonsterType.summons = function(mtype, mask)
	if type(mask.summons) == "table" then
		for k, v in pairs(mask.summons) do
			mtype:addSummon(v.name, v.interval, v.chance)
		end
	end
end
registerMonsterType.events = function(mtype, mask)
	if type(mask.events) == "table" then
		for k, v in pairs(mask.events) do
			mtype:registerEvent(v)
		end
	end
end
registerMonsterType.loot = function(mtype, mask)
	if type(mask.loot) == "table" then
		local lootError = false
		for _, loot in pairs(mask.loot) do
			local parent = Loot()
			if not parent:setId(loot.id) then
				lootError = true
			end
			if loot.chance then
				parent:setChance(loot.chance)
			end
			if loot.maxCount then
				parent:setMaxCount(loot.maxCount)
			end
			if loot.aid or loot.actionId then
				parent:setActionId(loot.aid or loot.actionId)
			end
			if loot.subType or loot.charges then
				parent:setSubType(loot.subType or loot.charges)
			end
			if loot.text or loot.description then
				parent:setDescription(loot.text or loot.description)
			end
			if loot.child then
				for _, children in pairs(loot.child) do
					local child = Loot()
					if not child:setId(children.id) then
						lootError = true
					end
					if children.chance then
						child:setChance(children.chance)
					end
					if children.maxCount then
						child:setMaxCount(children.maxCount)
					end
					if children.aid or children.actionId then
						child:setActionId(children.aid or children.actionId)
					end
					if children.subType or children.charges then
						child:setSubType(children.subType or children.charges)
					end
					if children.text or children.description then
						child:setDescription(children.text or children.description)
					end
					parent:addChildLoot(child)
				end
			end
			mtype:addLoot(parent)
		end
		if lootError then
			print("[Warning - end] Monster: \"".. mtype:name() .. "\" loot could not correctly be load.")
		end
	end
end
registerMonsterType.elements = function(mtype, mask)
	if type(mask.elements) == "table" then
		for _, element in pairs(mask.elements) do
			if element.type and element.percent then
				mtype:addElement(element.type, element.percent)
			end
		end
	end
end
registerMonsterType.immunities = function(mtype, mask)
	if type(mask.immunities) == "table" then
		for _, immunity in pairs(mask.immunities) do
			if immunity.type and immunity.combat then
				mtype:combatImmunities(immunity.type)
			end
			if immunity.type and immunity.condition then
				mtype:conditionImmunities(immunity.type)
			end
		end
	end
end
registerMonsterType.attacks = function(mtype, mask)
	if type(mask.attacks) == "table" then
		for _, attack in pairs(mask.attacks) do
			local spell = MonsterSpell()
			if attack.name then
				if attack.name == "melee" then
					spell:setType("melee")
					if attack.attack and attack.skill then
						spell:setAttackValue(attack.attack, attack.skill)
					end
					if attack.interval then
						spell:setInterval(attack.interval)
					end
					if attack.effect then
						spell:setCombatEffect(attack.effect)
					end
					if attack.condition then
						if attack.condition.type then
							spell:setConditionType(attack.condition.type)
						end
						local startDamnage = 0
						if attack.condition.startDamage then
							startDamage = attack.condition.startDamage
						end
						if attack.condition.minDamage and attack.condition.maxDamage then
							spell:setConditionDamage(attack.condition.minDamage, attack.condition.maxDamage, startDamage)
						end
						if attack.condition.duration then
							spell:setConditionDuration(attack.condition.duration)
						end
						if attack.condition.interval then
							spell:setConditionTickInterval(attack.condition.interval)
						end
					end
				else
					spell:setType(attack.name)
					if attack.type then
						if attack.name == "combat" then
							spell:setCombatType(attack.type)
						else
							spell:setConditionType(attack.type)
						end
					end
					if attack.interval then
						spell:setInterval(attack.interval)
					end
					if attack.chance then
						spell:setChance(attack.chance)
					end
					if attack.range then
						spell:setRange(attack.range)
					end
					if attack.duration then
						spell:setConditionDuration(attack.duration)
					end
					if attack.speed then
						spell:setConditionSpeedChange(attack.speed)
					end
					if attack.target then
						spell:setNeedTarget(attack.target)
					end
					if attack.length then
						spell:setCombatLength(attack.length)
					end
					if attack.spread then
						spell:setCombatSpread(attack.spread)
					end
					if attack.radius then
						spell:setCombatRadius(attack.radius)
					end
					if attack.minDamage and attack.maxDamage then
						if attack.name == "combat" then
							spell:setCombatValue(attack.minDamage, attack.maxDamage)
						else
							local startDamage = 0
							if attack.startDamage then
								startDamage = attack.startDamage
							end
							spell:setConditionDamage(attack.minDamage, attack.maxDamage, startDamage)
						end
					end
					if attack.effect then
						spell:setCombatEffect(attack.effect)
					end
					if attack.shootEffect then
						spell:setCombatShootEffect(attack.shootEffect)
					end
				end
			elseif attack.script then
				spell:setScriptName(attack.script)
				if attack.interval then
					spell:setInterval(attack.interval)
				end
				if attack.chance then
					spell:setChance(attack.chance)
				end
				if attack.minDamage and attack.maxDamage then
					spell:setCombatValue(attack.minDamage, attack.maxDamage)
				end
				if attack.target then
					spell:setNeedTarget(attack.target)
				end
			end
			mtype:addAttack(spell)
		end
	end
end
registerMonsterType.defenses = function(mtype, mask)
	if type(mask.defenses) == "table" then
		if mask.defenses.defense then
			mtype:defense(mask.defenses.defense)
		end
		if mask.defenses.armor then
			mtype:armor(mask.defenses.armor)
		end
		for _, defense in pairs(mask.defenses) do
			if type(defense) == "table" then
				local spell = MonsterSpell()
				if defense.name then
					if defense.name == "melee" then
						spell:setType("melee")
						if defense.attack and defense.skill then
							spell:setAttackValue(defense.attack, defense.skill)
						end
						if defense.interval then
							spell:setInterval(defense.interval)
						end
						if defense.effect then
							spell:setCombatEffect(defense.effect)
						end
						if defense.condition then
							if defense.condition.type then
								spell:setConditionType(defense.condition.type)
							end
							local startDamnage = 0
							if defense.condition.startDamage then
								startDamage = defense.condition.startDamage
							end
							if defense.condition.minDamage and defense.condition.maxDamage then
								spell:setConditionDamage(defense.condition.minDamage, defense.condition.maxDamage, startDamage)
							end
							if defense.condition.duration then
								spell:setConditionDuration(defense.condition.duration)
							end
							if defense.condition.interval then
								spell:setConditionTickInterval(defense.condition.interval)
							end
						end
					else
						spell:setType(defense.name)
						if defense.type then
							if defense.name == "combat" then
								spell:setCombatType(defense.type)
							else
								spell:setConditionType(defense.type)
							end
						end
						if defense.interval then
							spell:setInterval(defense.interval)
						end
						if defense.chance then
							spell:setChance(defense.chance)
						end
						if defense.range then
							spell:setRange(defense.range)
						end
						if defense.duration then
							spell:setConditionDuration(defense.duration)
						end
						if defense.speed then
							spell:setConditionSpeedChange(defense.speed)
						end
						if defense.target then
							spell:setNeedTarget(defense.target)
						end
						if defense.length then
							spell:setCombatLength(defense.length)
						end
						if defense.spread then
							spell:setCombatSpread(defense.spread)
						end
						if defense.radius then
							spell:setCombatRadius(defense.radius)
						end
						if defense.minDamage and defense.maxDamage then
							if defense.name == "combat" then
								spell:setCombatValue(defense.minDamage, defense.maxDamage)
							else
								local startDamage = 0
								if defense.startDamage then
									startDamage = defense.startDamage
								end
								spell:setConditionDamage(defense.minDamage, defense.maxDamage, startDamage)
							end
						end
						if defense.effect then
							spell:setCombatEffect(defense.effect)
						end
						if defense.shootEffect then
							spell:setCombatShootEffect(defense.shootEffect)
						end
					end
				elseif defense.script then
					spell:setScriptName(defense.script)
					if defense.interval then
						spell:setInterval(defense.interval)
					end
					if defense.chance then
						spell:setChance(defense.chance)
					end
					if defense.minDamage and defense.maxDamage then
						spell:setCombatValue(defense.minDamage, defense.maxDamage)
					end
					if defense.target then
						spell:setNeedTarget(defense.target)
					end
				end
				mtype:addDefense(spell)
			end
		end
	end
end
