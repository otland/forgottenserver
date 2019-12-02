function Item.getType(self)
	return ItemType(self:getId())
end

function Item.isContainer(self)
	return false
end

function Item.isCreature(self)
	return false
end

function Item.isMonster(self)
	return false
end

function Item.isNpc(self)
	return false
end

function Item.isPlayer(self)
	return false
end

function Item.isTeleport(self)
	return false
end

function Item.isTile(self)
	return false
end

StringStream = {}

setmetatable(StringStream, {
	__call = function(self)
		local obj = {
			str = {}
		}
		return setmetatable(obj, {__index = StringStream})
	end
})

function StringStream.append(self, str, ...)
	self.str[#self.str+1] = string.format(str, ...)
end

function StringStream.build(self, sep)
	return table.concat(self.str, sep)
end

function StringStream.isEmpty(self)
	return #self.str == 0
end

function Item.getDefense(self)
	local defense = self:getAttribute(ITEM_ATTRIBUTE_DEFENSE)
	if defense > 0 then
		return defense
	end
	return self:getType():getDefense()
end

function Item.getExtraDefense(self)
	local extraDef = self:getAttribute(ITEM_ATTRIBUTE_EXTRADEFENSE)
	if extraDef > 0 then
		return extraDef
	end
	return self:getType():getExtraDefense()
end

function Item.getAttack(self)
	local attack = self:getAttribute(ITEM_ATTRIBUTE_ATTACK)
	if attack > 0 then
		return attack
	end
	return self:getType():getAttack()
end

function Item.getHitChance(self)
	local hitChance = self:getAttribute(ITEM_ATTRIBUTE_HITCHANCE)
	if hitChance > 0 then
		return hitChance
	end
	return self:getType():getHitChance()
end

function Item.getShootRange(self)
	local shootRange = self:getAttribute(ITEM_ATTRIBUTE_SHOOTRANGE)
	if shootRange > 0 then
		return shootRange
	end
	return self:getType():getShootRange()
end

function Item.getArmor(self)
	local armor = self:getAttribute(ITEM_ATTRIBUTE_ARMOR)
	if armor > 0 then
		return armor
	end
	return self:getType():getArmor()
end

function Item.getDuration(self)
	local duration = self:getAttribute(ITEM_ATTRIBUTE_DURATION)
	if duration > 0 then
		return duration
	end
	return self:getType():getDuration()
end

function Item.getText(self)
	local text = self:getAttribute(ITEM_ATTRIBUTE_TEXT)
	return text ~= '' and text or nil
end

function Item.getDate(self)
	local date = self:getAttribute(ITEM_ATTRIBUTE_DATE)
	return data ~= 0 and date or nil
end

function Item.getWriter(self)
	local writer = self:getAttribute(ITEM_ATTRIBUTE_WRITER)
	return writer ~= '' and writer or nil
end

do -- fix str keys for constant iteration
	local __spellvocation = Spell.vocation
	function Spell.vocation(self, ...)
		local args = {...}
		if #args == 0 then
			local vocMap = __spellvocation(self)
			local tmp = {}
			for k, v in pairs(vocMap) do
				tmp[tonumber(k)] = v
			end
			return tmp
		end
		return __spellvocation(self, ...)
	end
end


function Item.__getNameDescription(self, addArticle)
	local itemType = self:getType()
	local subType = self:getSubType()
	local s = ''
	local name = self:getName()
	if name ~= '' then
		if itemType:isStackable() and subType >  1 then
			if itemType:hasShowCount() then
				s = string.format('%s%d ', s, subType)
			end
			s = string.format('%s%s', s, self:getPluralName())
		else
			if addArticle and self:getArticle() ~= '' then
				s = string.format('%s%s ', s, self:getArticle())
			end
			s = string.format('%s%s', s, self:getName())
		end
	else
		s = 'an item of type '.. self:getId()
	end
	return s
end

do
	local function addSeparator(ss, begin)
		if begin[1] then
			begin[1] = false
			ss:append(' (')
		else
			ss:append(', ')
		end
	end
	local function addGenerics(self, it, abilities, ss, begin)
		if it:getWeaponType() == WEAPON_DISTANCE and it:getAmmoType() ~= 0 then
			ss:append(' (Range:%d', self:getShootRange())
			local attack = self:getAttack()
			local hitChance = self:getHitChance()
			if attack ~= 0 then
				ss:append(', Atk%s%d', showpos(attack), attack)
			end

			if hitChance ~= 0 then
				ss:append(', Hit%%%s%d', showpos(hitChance), hitChance)
			end

			begin[1] = false
		elseif it:getWeaponType() ~= WEAPON_AMMO then
			local attack = self:getAttack()
			local defense = self:getDefense()
			local extraDefense = self:getExtraDefense()

			if attack ~= 0 then
				begin[1] = false
				ss:append(' (Atk:%d', attack)
				
				if abilities.elementType ~= COMBAT_NONE and abilities.elementDamage ~= 0 then
					ss:append(' physical + %d %s', abilities.elementDamage, getCombatName(abilities.elementType))
				end
			end

			if defense ~= 0 or extraDefense ~= 0 then
				addSeparator(ss, begin)
				ss:append('Def:%d', defense)
				if extraDefense ~= 0 then
					ss:append(' %s%d', showpos(extraDefense), extraDefense)
				end
			end
		end

		-- Skills
		for skill, value in ipairs(abilities.skills) do
			if value ~= 0 then
				addSeparator(ss, begin)
				ss:append('%s %s%d', getSkillName(skill - 1), showpos(value), value)
			end
		end

		-- Special Skills
		for specialSkill, value in ipairs(abilities.specialSkills) do
			if value ~= 0 then
				addSeparator(ss, begin)
				ss:append('%s %s%d%%', getSpecialSkillName(specialSkill - 1), showpos(value), value)
			end
		end

		local magicPoints = abilities.stats[4]
		if magicPoints ~= 0 then
			addSeparator(ss, begin)
			ss:append('magic level %s%d', showpos(magicPoints), magicPoints)
		end

		-- Absorb

		local show = abilities.absorbPercent[1]
		if show ~= 0 then
			for _, value in ipairs(abilities.absorbPercent) do
				if value ~= show then
					show = 0
				end
			end
		end
		
		if show == 0 then
			local tmp = true
			for i, value in ipairs(abilities.absorbPercent) do
				if value ~= 0 then
					if tmp then
						tmp = false
						addSeparator(ss, begin)
						ss:append('protection ')
					else
						ss:append(', ')
					end
					ss:append('%s %s%d%%', getCombatName(indexToCombatType(i - 1)), showpos(value), math.abs(value))
				end
			end
		else
			addSeparator(ss, begin)
			ss:append('protection all %s%d%%', showpos(show), show)
		end

		-- Field absorb

		local show = abilities.fieldAbsorbPercent[1]
		if show ~= 0 then
			for _, value in ipairs(abilities.fieldAbsorbPercent) do
				if value ~= show then
					show = 0
				end
			end
		end
		
		if show == 0 then
			local tmp = true
			for i, value in ipairs(abilities.fieldAbsorbPercent) do
				if value ~= 0 then
					if tmp then
						tmp = false
						addSeparator(ss, begin)
						ss:append('protection ')
					else
						ss:append(', ')
					end
					ss:append('%s field %s%d%%', getCombatName(indexToCombatType(i - 1)), showpos(value), math.abs(value))
				end
			end
		else
			addSeparator(ss, begin)
			ss:append('protection all fields %s%d%%', showpos(show), show)
		end

		if abilities.speed ~= 0 then
			addSeparator(ss, begin)
			ss:append('speed %s%d', showpos(abilities.speed), abilities.speed / 2)
		end
	end

	function Item.__getDescription(self, lookDistance)
		local start = os.mtime()
		local it = self:getType()
		local abilities = it:getAbilities()
		local nameDesc = self:__getNameDescription(true)
		local ss = StringStream()
		local subType = self:getSubType()
		local begin = {true} -- reference so we can pass & edit through auxiliary functions

		-- table.print(abilities)

		ss:append(nameDesc)

		if it:isRune() then
			local rune = Spell(self:getId())
			if rune then
				if rune:getLevel() > 0 or rune:getMagicLevel() > 0 then
					local tmpVocMap = rune:vocation()
					local vocMap = {}
					for k, vocName in ipairs(tmpVocMap) do
						local vocation = Vocation(vocName)
						if vocation and vocation:getPromotion() then
							vocMap[#vocMap + 1] = vocName
						end
					end

					ss:append('. %s can only be used by', it:isStackable() and subType > 1 and 'They' or 'It')

					-- Only show base vocations in description; promotions should be a given
					if #vocMap == 0 then
						ss:append(' players')
					else
						for i = 1, #vocMap - 1 do
							local vocName = vocMap[i]
							local vocation = Vocation(vocName)
							ss:append(' %ss', vocName:lower())
							if i + 1 == #vocMap then
								ss:append(' and')
							else
								ss:append(',')
							end
						end
						local vocName = vocMap[#vocMap]
						ss:append(' %ss', vocName:lower())
					end

					ss:append(' with')

					if rune:level() > 0 then
						ss:append(' level %d', rune:level())
					end

					if rune:magicLevel() > 0 then
						if rune:level() > 0 then
							ss:append(' and ')
						end
						ss:append('magic level %d', rune:magicLevel())
					end

					ss:append(' or higher')
				end

				if not begin[1] then
					ss:append(')')
				end
			end
		elseif it:getWeaponType() ~= WEAPON_NONE then
			addGenerics(self, it, abilities, ss, begin)
			if not begin[1] then
				ss:append(')')
			end
		elseif self:getArmor() ~= 0 or it:hasShowAttributes() then
			if self:getArmor() ~= 0 then
				ss:append(' (Arm:%d', self:getArmor())
				begin[1] = false
			end
			addGenerics(self, it, abilities, ss, begin)
			if not begin[1] then
				ss:append(')')
			end
		elseif self:isContainer() then
			local volume = self:getCapacity()
			if volume ~= 0 and not self:hasAttribute(ITEM_ATTRIBUTE_UNIQUEID) then
				ss:append(' (Vol:%d)', volume)
			end
		else
			local found = true

			if abilities.speed > 0 then
				ss:append(' (speed %s%d', showpos(abilities.speed), abilities.speed / 2)
			elseif bit.band(abilities.conditionSuppressions, CONDITION_DRUNK) == 1 then
				ss:append(' (hard drinking)')
			elseif abilities.invisible then
				ss:append(' (invisibility)')
			elseif abilities.regeneration then
				ss:append(' (faster regeneration)')
			elseif abilities.manashield then
				ss:append(' (mana shield)')
			else
				found = false
			end

			if not found then
				if it:getType() == ITEM_TYPE_KEY then
					local aid = self:getActionId()
					ss:append(' (Key:%s)', ('0'):rep(4 - #tostring(aid)) .. aid)
				elseif it:getGroup() == ITEM_GROUP_FLUID then
					if subType > 0 then
						local name = self:getSubTypeName()
						ss:append(' of %s', name ~= '' and name or 'unknown')
					else
						ss:append('. It is empty')
					end
				elseif it:getGroup() == ITEM_GROUP_SPLASH then
					local name = self:getSubTypeName()
					ss:append(' of ')
					if subType > 0 and name ~= '' then
						ss:append(name)
					else
						ss:append('unknown')
					end
				elseif it:hasAllowDistRead() and (it:getId() < 7369 or it:getId() > 7371) then
					ss:append('.\n')
					if lookDistance <= 4 then
						local text = self:getText()
						if text then
							local writer = self:getWriter()
							if writer then
								local date = self:getDate()
								ss:append('%s wrote', writer)
								if date then
									ss:append(' on %s', os.date('%d %b %Y'))
								end
								ss:append(': ')
							else
								ss:append('You read: ')
							end
							ss:append(text)
						else
							ss:append('Nothing is written on it')
						end
					else
						ss:append('You are too far away to read it.')
					end
				elseif it:getLevelDoor() ~= 0 then
					local aid = self:getActionId()
					if aid >= it:getLevelDoor() then
						ss:append(' for level %d', aid - it:getLevelDoor())
					end
				end
			end
		end

		if it:hasShowCharges() then
			ss:append(' that has %d charge%s left', subType, subType ~= -1 and 's' or '')
		end

		-- show duration
		if it:hasShowDuration() then
			local duration = self:getDuration() / 1000
			if self:hasAttribute(ITEM_ATTRIBUTE_DURATION) then
				if duration > 0 then
					ss:append(' that will expire in ')

					if duration >= 86400 then
						local days = math.floor(duration / 86400)
						local hours = math.floor((duration % 86400) / 3600)
						ss:append('%d day%s', days, days ~= 1 and 's' or '')
						if hours > 0 then
							ss:append(' and %d hour%s', hours, hours ~= 1 and 's' or '')
						end
					elseif duration >= 3600 then
						local hours = math.floor(duration / 3600)
						local minutes = math.floor((duration % 3600) / 60)
						ss:append('%d hour%s', hours, hours ~= 1 and 's' or '')
						if minutes > 0 then
							ss:append(' and %d minute%s', minutes, minutes ~= 1 and 's' or '')
						end
					elseif duration >= 60 then
						local minutes = math.floor(duration / 60)
						local seconds = duration % 60
						ss:append('%d minute%s', minutes, minutes ~= 1 and 's' or '')
						if seconds > 0 then
							ss:append(' and %d second%s', seconds, seconds ~= 1 and 's' or '')
						end
					else
						ss:append('%d second%s', duration, duration ~= 1 and 's' or '')
					end
				end
			else
				ss:append(' that is brand-new')
			end
		end

		if not it:hasAllowDistRead() or (it:getId() >= 7369 and it:getId() <= 7371) then
			ss:append('.')
		else
			local text = self:getText()

			if not text then
				ss:append('.')
			end
		end

		local wieldInfo = it:getWieldInfo()
		if wieldInfo ~= 0 then
			ss:append('\nIt can only be wielded properly by ')

			if bit.band(wieldInfo, WIELDINFO_PREMIUM) ~= 0 then
				ss:append('premium ')
			end

			local vocStr = it:getVocationString()
			if vocStr ~= '' then
				ss:append(vocStr)
			else
				ss:append('players')
			end

			if bit.band(wieldInfo, WIELDINFO_LEVEL) ~= 0 then
				ss:append(' of level %d or higher', it:getMinReqLevel())
			end

			if bit.band(wieldInfo, WIELDINFO_MAGLV) ~= 0 then
				if bit.band(wieldInfo, WIELDINFO_LEVEL) ~= 0 then
					ss:append(' and')
				else
					ss:append(' of')
				end
				ss:append(' magic level %d or higher', it:getMinReqMagicLevel())
			end
			ss:append('.')
		end

		if lookDistance <= 1 then
			local weight = self:getWeight()
			if weight ~= 0 and it:isPickupable() then
				ss:append('\n')
				if it:isStackable() and self:getCount() > 1 and it:hasShowCount() then
					ss:append('They weigh ')
				else
					ss:append('It weighs ')
				end
				ss:append('%.2f oz.', weight / 100)
			end
		end

		local specialDesc = self:getSpecialDescription()
		local desc = it:getDescription()
		if specialDesc ~= '' then
			ss:append('\n%s', specialDesc)
		elseif lookDistance <= 1 and desc ~= '' then
			ss:append('\n%s', desc)
		end

		if it:hasAllowDistRead() or (it:getId() >= 7369 and it:getId() <= 7371) then
			local text = self:getText()
			if text then
				ss:append('\n%s', text)
			end
		end

		ss:append('\n\nGenerated in '.. (os.mtime() - start) .. ' milliseconds')
		return ss:build()
	end
end

function Item.getTestDescription(self, player)
	local s = 'You see ' .. self:__getDescription(self:getPosition():getDistance(player:getPosition()))

	return s
end
