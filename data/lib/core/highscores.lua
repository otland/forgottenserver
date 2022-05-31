highscoresCache = {}
highscoresTimestamps = {}
highscoresMaxResults = 100
highscoresCacheTime = 30 * 60 -- 30 minutes
highscoresExcludedGroups = {4, 5, 6}

-- VocationId : VocationClientId
local vocationIdsToClientIds = {
    [0] = 0,
    [1] = 3,
    [2] = 4,
    [3] = 2,
    [4] = 1,
    [5] = 13,
    [6] = 14,
    [7] = 12,
    [8] = 11
}

highscoresQueries = {
    [HIGHSCORES_CATEGORY_LEVEL] = [[
        SELECT `id`, `name`, `vocation`, `level`, `experience` AS `points` FROM `players`
        WHERE `deletion` = 0 %s
        AND `group_id` NOT IN (]] .. table.concat(highscoresExcludedGroups, ", ") .. [[)
        ORDER BY `points` DESC LIMIT %d, %d
    ]],
    [HIGHSCORES_CATEGORY_MAGLEVEL] = [[
        SELECT `id`, `name`, `vocation`, `level`, `maglevel` AS `points` FROM `players`
        WHERE `deletion` = 0 %s
        AND `group_id` NOT IN (]] .. table.concat(highscoresExcludedGroups, ", ") .. [[)
        ORDER BY `points` DESC, `manaspent` DESC LIMIT %d, %d
    ]],
    [HIGHSCORES_CATEGORY_FIST_FIGHTING] = [[
        SELECT `id`, `name`, `vocation`, `level`, `skill_fist` AS `points` FROM `players`
        WHERE `deletion` = 0 %s
        AND `group_id` NOT IN (]] .. table.concat(highscoresExcludedGroups, ", ") .. [[)
        ORDER BY `points` DESC, `skill_fist_tries` DESC LIMIT %d, %d
    ]],
    [HIGHSCORES_CATEGORY_AXE_FIGHTING] = [[
        SELECT `id`, `name`, `vocation`, `level`, `skill_axe` AS `points` FROM `players`
        WHERE `deletion` = 0 %s
        AND `group_id` NOT IN (]] .. table.concat(highscoresExcludedGroups, ", ") .. [[)
        ORDER BY `points` DESC, `skill_axe_tries` DESC LIMIT %d, %d
    ]],
    [HIGHSCORES_CATEGORY_CLUB_FIGHTING] = [[
        SELECT `id`, `name`, `vocation`, `level`, `skill_club` AS `points` FROM `players`
        WHERE `deletion` = 0 %s
        AND `group_id` NOT IN (]] .. table.concat(highscoresExcludedGroups, ", ") .. [[)
        ORDER BY `points` DESC, `skill_club_tries` DESC LIMIT %d, %d
    ]],
    [HIGHSCORES_CATEGORY_SWORD_FIGHTING] = [[
        SELECT `id`, `name`, `vocation`, `level`, `skill_sword` AS `points` FROM `players`
        WHERE `deletion` = 0 %s
        AND `group_id` NOT IN (]] .. table.concat(highscoresExcludedGroups, ", ") .. [[)
        ORDER BY `points` DESC, `skill_sword_tries` DESC LIMIT %d, %d
    ]],
    [HIGHSCORES_CATEGORY_DISTANCE_FIGHTING] = [[
        SELECT `id`, `name`, `vocation`, `level`, `skill_dist` AS `points` FROM `players`
        WHERE `deletion` = 0 %s
        AND `group_id` NOT IN (]] .. table.concat(highscoresExcludedGroups, ", ") .. [[)
        ORDER BY `points` DESC, `skill_dist_tries` DESC LIMIT %d, %d
    ]],
    [HIGHSCORES_CATEGORY_SHIELDING] = [[
        SELECT `id`, `name`, `vocation`, `level`, `skill_shielding` AS `points` FROM `players`
        WHERE `deletion` = 0 %s
        AND `group_id` NOT IN (]] .. table.concat(highscoresExcludedGroups, ", ") .. [[)
        ORDER BY `points` DESC, `skill_shielding_tries` DESC LIMIT %d, %d
    ]],
    [HIGHSCORES_CATEGORY_FISHING] = [[
        SELECT `id`, `name`, `vocation`, `level`, `skill_fishing` AS `points` FROM `players`
        WHERE `deletion` = 0 %s
        AND `group_id` NOT IN (]] .. table.concat(highscoresExcludedGroups, ", ") .. [[)
        ORDER BY `points` DESC, `skill_fishing_tries` DESC LIMIT %d, %d
    ]],
    [HIGHSCORES_CATEGORY_ACHIEVEMENTS] = [[
        SELECT `id`, `name`, `vocation`, `level`, `value` AS `points` FROM `player_storage`
        LEFT JOIN `players` ON `players`.`id` = `player_storage`.`player_id`
        WHERE `deletion` = 0 %s
        AND `player_storage`.`key` = ]] .. PlayerStorageKeys.achievementPoints .. [[
        AND `group_id` NOT IN (]] .. table.concat(highscoresExcludedGroups, ", ") .. [[)
        ORDER BY `points` DESC, `name` ASC LIMIT %d, %d
    ]]
    -- CUSTOM HIGHSCORE
    --[[,
    [HIGHSCORES_CATEGORY_DEATHS] = [[
        SELECT DISTINCT(`id`) `id`, `name`, `vocation`, `players`.`level`,
        (SELECT COUNT(*) FROM `player_deaths` WHERE `player_deaths`.`player_id` = `players`.`id`) AS `points`
        FROM `players`
        LEFT JOIN `player_deaths` ON `player_deaths`.`player_id` = `players`.`id`
        WHERE `deletion` = 0 %s
        AND `group_id` NOT IN (4, 5, 6)
        ORDER BY `points` DESC, `name` ASC LIMIT %d, %d
    ]]
    --]]
}

function getHighscores(params)
    if not highscoresQueries[params.category] then
        return {}, os.time()
    end

    local from = (params.page - 1) * 20
    local to = params.page * 20

    local vocWhere = ""
    if params.vocation ~= 0xFFFFFFFF then
        local ids = Vocation(params.vocation):getPromotions() or {VOCATION_NONE}
        vocWhere = "AND `vocation` IN (" .. table.concat(ids, ", ") .. ")"
    end

    local entries = {}
    local rank = from + 1
    local resultId = db.storeQuery(string.format(highscoresQueries[params.category], vocWhere, from, to))
    if resultId then
        repeat
            table.insert(entries, {
                id = result.getNumber(resultId, "id"),
                rank = rank,
                name = result.getString(resultId, "name"),
                title = "",
                vocation = vocationIdsToClientIds[result.getNumber(resultId, "vocation")],
                world = configManager.getString(configKeys.SERVER_NAME),
                level = result.getNumber(resultId, "level"),
                points = result.getNumber(resultId, "points")
            })
            rank = rank + 1
        until not result.next(resultId)
        result.free(resultId)
    end
    return entries
end

function getHighscoresCacheKey(params)
    if (params.world == "OWN" or params.world == "") then
        params.world = configManager.getString(configKeys.SERVER_NAME)
    end
    return string.format("%d%s%d%d", params.category, params.world, params.vocation, params.page)
end
