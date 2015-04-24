print(">> Loading lua monsters")
function MonsterType.register(self, mask)
	
	if mask.description then
		self:setNameDescription(mask.description)
	end
	
	if mask.experience then
		self:setExperience(mask.experience)
	end
	
	if mask.outfit then
		self:setOutfit(mask.outfit)
	end
	
	if mask.maxHealth then
		self:setMaxHealth(mask.maxHealth)
	end
	
	if mask.health then
		self:setHealth(mask.health)
	end
	
	if mask.runHealth then
		self:setRunHealth(mask.runHealth)
	end
	
	if mask.maxSummons then
		self:setMaxSummons(mask.maxSummons)
	end
	
	if mask.race then
		self:setRace(mask.race)
	end
	
	if mask.manaCost then
		self:setManaCost(mask.manaCost)
	end
	
	if mask.speed then
		self:setBaseSpeed(mask.speed)
	end
	
	if mask.corpse then
		self:setCorpseId(mask.corpse)
	end
	
	if mask.flags then
		if mask.flags.attackable then
			self:setIsAttackable(mask.flags.attackable)
		end
		if mask.flags.healthHidden then
			self:setIsHealthHidden(mask.flags.healthHidden)
		end
		if mask.flags.convinceable then
			self:setIsConvinceable(mask.flags.convinceable)
		end
		if mask.flags.illusionable then
			self:setIsIllusionable(mask.flags.illusionable)
		end
		if mask.flags.hostile then
			self:setIsHostile(mask.flags.hostile)
		end
		if mask.flags.pushable then
			self:setIsPushable(mask.flags.pushable)
		end
		if mask.flags.canPushItems then
			self:setCanPushItems(mask.flags.canPushItems)
		end
		if mask.flags.canPushCreatures then
			self:setCanPushCreatures(mask.flags.canPushCreatures)
		end
		if mask.flags.targetDistance then
			self:setTargetDistance(mask.flags.targetDistance)
		end
		if mask.flags.staticAttack then
			self:setStaticAttack(mask.flags.staticAttack)
		end
	end
	
	if mask.skull then
		self:setSkull(mask.skull)
	end
	
	if mask.light then
		if mask.light.color then
			local color = mask.light.color
		end
		if mask.light.level then
			self:setLight(color, mask.light.level)
		end
	end
	
	if mask.changeTarget then
		if mask.changeTarget.chance then
			local chance = mask.changeTarget.chance
		end
		if mask.changeTarget.interval then
			self:setChangeTarget(chance, mask.changeTarget.interval)
		end
	end
	
	if type(mask.voices) == "table" then
		for k, v in pairs(mask.voices) do
			self:addVoice(v.text, v.interval, v.chance, v.yell)
		end
	end
	
	if type(mask.summons) == "table" then
		for k, v in pairs(mask.summons) do
			self:addSummon(v.name, v.interval, v.chance)
		end
	end
	
	if type(mask.events) == "table" then
		for k, v in pairs(mask.events) do
			self:registerEvent(v)
		end
	end
	
	if type(mask.loot) == "table" then
		for _, loot in pairs(mask.loot) do
			local parent = Loot()
			parent:setId(loot.id)
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
					child:setId(children.id)
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
			self:addLoot(parent)
		end
	end
	
	if type(mask.elements) == "table" then
		for _, element in pairs(mask.elements) do
			if element.type and element.percent then
				self:addElement(element.type, element.percent)
			end
		end
	end
	
	if type(mask.immunities) == "table" then
		for _, immunity in pairs(mask.immunities) do
			if immunity.type and immunity.combat then
				self:addCombatImmunity(immunity.type)
			end
			if immunity.type and immunity.condition then
				self:addConditionImmunity(immunity.type)
			end
		end
	end
	
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
			self:addAttack(spell)
		end
	end
	
	if type(mask.defenses) == "table" then
		for _, defense in pairs(mask.defenses) do
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
			self:addDefense(spell)
		end
	end
end