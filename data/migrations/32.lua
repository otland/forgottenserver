function onUpdateDatabase()
	print("> Updating database to version 32 (Update Blessing)")
	
	db.query("ALTER TABLE `players` CHANGE `blessings` `blessings1` INT NOT NULL DEFAULT '0';")
	db.query([[
		ALTER TABLE `players`
            ADD COLUMN `blessings2` INT  DEFAULT 0 NOT NULL AFTER `blessings1`,
            ADD COLUMN `blessings3` INT  DEFAULT 0 NOT NULL AFTER `blessings2`,
            ADD COLUMN `blessings4` INT  DEFAULT 0 NOT NULL AFTER `blessings3`,
            ADD COLUMN `blessings5` INT  DEFAULT 0 NOT NULL AFTER `blessings4`,
            ADD COLUMN `blessings6` INT  DEFAULT 0 NOT NULL AFTER `blessings5`,
            ADD COLUMN `blessings7` INT  DEFAULT 0 NOT NULL AFTER `blessings6`,
            ADD COLUMN `blessings8` INT  DEFAULT 0 NOT NULL AFTER `blessings7`
			]])

	return true
end
