highscoresMaxResults = 100
highscoresPageSize = 20
highscoresCacheTime = 30 * 60 -- time in seconds
highscoresCache = {}
clientIdsVocation = {}
relatedIdsVocation = {}

function setUpHighscores()
	local next = next
	if next(clientIdsVocation) == nil then
		for _, vocation in pairs(Game.getVocations()) do
			clientIdsVocation[vocation:getId()] = vocation:getClientId()
		end
	end
	if next(relatedIdsVocation) == nil then
		for _, vocation in pairs(Game.getUnpromotedVocations()) do
			if vocation:getId() ~= VOCATION_NONE then
				local relatedIds = {}
				for _, related in pairs(vocation:getRelated()) do
					relatedIds[#relatedIds + 1] = related:getId()
				end
				relatedIdsVocation[vocation:getId()] = relatedIds
			end
		end
	end
end

highscoresExcludedGroups = {4, 5, 6}

HIGHSCORES_BATTLEYE_NOT_PROTECTED = 0
HIGHSCORES_BATTLEYE_PROTECTED = 1
HIGHSCORES_BATTLEYE_INITIALLY_PROTECTED = 2
HIGHSCORES_BATTLEYE_ALL = 0xFF

HIGHSCORES_WORLDTYPE_NONE = 0

HIGHSCORES_CATEGORY_LEVEL = 1
HIGHSCORES_CATEGORY_MAGLEVEL = 2
HIGHSCORES_CATEGORY_FIST_FIGHTING = 3
HIGHSCORES_CATEGORY_AXE_FIGHTING = 4
HIGHSCORES_CATEGORY_CLUB_FIGHTING = 5
HIGHSCORES_CATEGORY_SWORD_FIGHTING = 6
HIGHSCORES_CATEGORY_DISTANCE_FIGHTING = 7
HIGHSCORES_CATEGORY_SHIELDING = 8
HIGHSCORES_CATEGORY_FISHING = 9
HIGHSCORES_CATEGORY_ACHIEVEMENTS = 10

HIGHSCORES_ACTION_BROWSE = 0
HIGHSCORES_ACTION_OWN = 1

HIGHSCORES_TYPE_SKILLS = 0
HIGHSCORES_TYPE_POINTS = 1
HIGHSCORES_TYPE_SCORE = 2

HIGHSCORES_CATEGORIES = {
	[HIGHSCORES_CATEGORY_LEVEL] = { name = "Experience Points", type = HIGHSCORES_TYPE_POINTS },
	[HIGHSCORES_CATEGORY_MAGLEVEL] = { name = "Magic Level", type = HIGHSCORES_TYPE_SKILLS },
	[HIGHSCORES_CATEGORY_FIST_FIGHTING] = { name = "Fist Fighting", type = HIGHSCORES_TYPE_SKILLS },
	[HIGHSCORES_CATEGORY_AXE_FIGHTING] = { name = "Axe Fighting", type = HIGHSCORES_TYPE_SKILLS },
	[HIGHSCORES_CATEGORY_CLUB_FIGHTING] = { name = "Club Fighting", type = HIGHSCORES_TYPE_SKILLS },
	[HIGHSCORES_CATEGORY_SWORD_FIGHTING] = { name = "Sword Fighting", type = HIGHSCORES_TYPE_SKILLS },
	[HIGHSCORES_CATEGORY_DISTANCE_FIGHTING] = { name = "Distance Fighting", type = HIGHSCORES_TYPE_SKILLS },
	[HIGHSCORES_CATEGORY_SHIELDING] = { name = "Shielding", type = HIGHSCORES_TYPE_SKILLS },
	[HIGHSCORES_CATEGORY_FISHING] = { name = "Fishing", type = HIGHSCORES_TYPE_SKILLS },
	[HIGHSCORES_CATEGORY_ACHIEVEMENTS] = { name = "Achievement Points", type = HIGHSCORES_TYPE_POINTS }
}

HIGHSCORES_QUERIES = {
	[HIGHSCORES_CATEGORY_LEVEL] = [[
		SELECT `id`, `name`, `vocation`, `level`, `experience` AS `points`
		FROM `players` WHERE `deletion` = 0 %s
		ORDER BY `points` DESC, `name` ASC LIMIT ]] .. highscoresMaxResults,
	[HIGHSCORES_CATEGORY_MAGLEVEL] = [[
		SELECT `id`, `name`, `vocation`, `level`, `maglevel` AS `points`
		FROM `players` WHERE `deletion` = 0 %s
		ORDER BY `points` DESC, `manaspent` DESC LIMIT ]] .. highscoresMaxResults,
	[HIGHSCORES_CATEGORY_FIST_FIGHTING] = [[
		SELECT `id`, `name`, `vocation`, `level`, `skill_fist` AS `points`
		FROM `players` WHERE `deletion` = 0 %s
		ORDER BY `points` DESC, `skill_fist_tries` DESC LIMIT ]] .. highscoresMaxResults,
	[HIGHSCORES_CATEGORY_AXE_FIGHTING] = [[
		SELECT `id`, `name`, `vocation`, `level`, `skill_axe` AS `points`
		FROM `players` WHERE `deletion` = 0 %s
		ORDER BY `points` DESC, `skill_axe_tries` DESC LIMIT ]] .. highscoresMaxResults,
	[HIGHSCORES_CATEGORY_CLUB_FIGHTING] = [[
		SELECT `id`, `name`, `vocation`, `level`, `skill_club` AS `points`
		FROM `players` WHERE `deletion` = 0 %s
		ORDER BY `points` DESC, `skill_club_tries` DESC LIMIT ]] .. highscoresMaxResults,
	[HIGHSCORES_CATEGORY_SWORD_FIGHTING] = [[
		SELECT `id`, `name`, `vocation`, `level`, `skill_sword` AS `points`
		FROM `players` WHERE `deletion` = 0 %s
		ORDER BY `points` DESC, `skill_sword_tries` DESC LIMIT ]] .. highscoresMaxResults,
	[HIGHSCORES_CATEGORY_DISTANCE_FIGHTING] = [[
		SELECT `id`, `name`, `vocation`, `level`, `skill_dist` AS `points`
		FROM `players` WHERE `deletion` = 0 %s
		ORDER BY `points` DESC, `skill_dist_tries` DESC LIMIT ]] .. highscoresMaxResults,
	[HIGHSCORES_CATEGORY_SHIELDING] = [[
		SELECT `id`, `name`, `vocation`, `level`, `skill_shielding` AS `points`
		FROM `players` WHERE `deletion` = 0 %s
		ORDER BY `points` DESC, `skill_shielding_tries` DESC LIMIT ]] .. highscoresMaxResults,
	[HIGHSCORES_CATEGORY_FISHING] = [[
		SELECT `id`, `name`, `vocation`, `level`, `skill_fishing` AS `points`
		FROM `players` WHERE `deletion` = 0 %s
		ORDER BY `points` DESC, `skill_fishing_tries` DESC LIMIT ]] .. highscoresMaxResults,
	[HIGHSCORES_CATEGORY_ACHIEVEMENTS] = [[
		SELECT `id`, `name`, `vocation`, `level`, `storages`.`value` AS `points`
		FROM `player_storage` AS `storages`
		LEFT JOIN `players` ON `players`.`id` = `storages`.`player_id`
		WHERE `storages`.`key` = ]] .. PlayerStorageKeys.achievementsTotal .. [[
		AND `deletion` = 0 %s
		ORDER BY `points` DESC, `name` ASC LIMIT ]] .. highscoresMaxResults
}

local function chunk(source, index, size)
	local chunks = math.ceil(#source / size)
	if index > chunks then
		return source
	end

	local i = 1 + ((index * size) - size)
	local j = index * size

	return table.slice(source, i, j)
end

local function get(self)
	return highscoresCache[self.params.category][self.params.vocation or 0xFFFFFF] or false
end

local function set(self, data)
	highscoresCache[self.params.category][self.params.vocation] = data
end

local function findPlayer(self, playerId)
	local entries = self:get()
	if not entries then
		return 1
	end

	local rank = 0
	for _, entry in pairs(entries.data) do
		if entry.id == playerId then
			rank = entry.rank
			break
		end
	end

	return rank == 0 and 1 or math.ceil(rank / highscoresPageSize)
end

local function refresh(self)
	local entries = self:get()
	if not entries or ((os.time() - entries.ts) >= highscoresCacheTime) then
		entries = self:fetch()
		self:set(entries)
	end
end

local function render(self, player)
	local entries = self:get()
	if not entries then
		return
	end

	if self.params.action == HIGHSCORES_ACTION_OWN then
		self.params.page = self:findPlayer(player:getGuid())
		self.params.action = HIGHSCORES_ACTION_BROWSE
	end
	
	self.params.totalPages = math.ceil(#entries.data / highscoresPageSize)
	if self.params.page > self.params.totalPages then
		self.params.page = 1
	end

	self.params.worldType = HIGHSCORES_WORLDTYPE_NONE
	self.params.battlEye = HIGHSCORES_BATTLEYE_ALL

	local filtered = chunk(entries.data, self.params.page, highscoresPageSize)
	player:sendHighscores({ data = filtered, ts = entries.ts }, self.params)
end

local function fetch(self)
	local query = HIGHSCORES_QUERIES[self.params.category]
	if not query then
		return
	end

	local entries = {}
	local world = configManager.getString(configKeys.SERVER_NAME)
	local rank = 1

	local extras = {"AND `group_id` NOT IN (" .. table.concat(highscoresExcludedGroups, ",") .. ")"}
	if self.params.vocation ~= 0xFFFFFFFF then
		local vocations = relatedIdsVocation[self.params.vocation] or {VOCATION_NONE}
		table.insert(extras, "AND `vocation` IN(" .. table.concat(vocations, ",") .. ")")
	end

	local resultId = db.storeQuery(string.format(query, table.concat(extras, " ")))
	if resultId then
		repeat
			entries[rank] = {
				id = result.getNumber(resultId, "id"),
				rank = rank,
				name = result.getString(resultId, "name"),
				title = "", -- TODO: loyalty system
				vocation = clientIdsVocation[result.getNumber(resultId, "vocation")] or VOCATION_NONE,
				world = world,
				level = result.getNumber(resultId, "level"),
				points = result.getNumber(resultId, "points")
			}
			rank = rank + 1
		until not result.next(resultId)		
		result.free(resultId)
	end
	
	self.params.ts = os.time()
	return { data = entries, ts = self.params.ts }
end

function Highscores(params)
	if not highscoresCache[params.category] then
		highscoresCache[params.category] = {}
	end

	return {
		get = get,
		fetch = fetch,
		findPlayer = findPlayer,
		params = params,
		refresh = refresh,
		render = render,
		set = set
	}
end
