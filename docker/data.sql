-- creates an account with e-mail: tfs@tfs password: tfs
INSERT INTO `accounts` (`id`, `name`, `password`, `secret`, `type`, `premium_ends_at`, `email`, `creation`)
VALUES ('1', 'tfs', 'e65018eee3ab1e3ecdfc67a91a2aa327434ab5b5', NULL, '6', '0', 'tfs@tfs', '0');

-- creates character: GOD
INSERT INTO `players` (`name`, `group_id`, `account_id`, `level`, `vocation`, `health`, `healthmax`, `experience`,
                       `lookbody`, `lookfeet`, `lookhead`, `looklegs`, `looktype`, `lookaddons`, `lookmount`,
                       `lookmounthead`, `lookmountbody`, `lookmountlegs`, `lookmountfeet`, `currentmount`,
                       `randomizemount`, `direction`, `maglevel`, `mana`, `manamax`, `manaspent`, `soul`, `town_id`,
                       `posx`, `posy`, `posz`, `conditions`, `cap`, `sex`, `lastlogin`, `lastip`, `save`, `skull`,
                       `skulltime`, `lastlogout`, `blessings`, `onlinetime`, `deletion`, `balance`,
                       `offlinetraining_time`, `offlinetraining_skill`, `stamina`, `skill_fist`, `skill_fist_tries`,
                       `skill_club`, `skill_club_tries`, `skill_sword`, `skill_sword_tries`, `skill_axe`,
                       `skill_axe_tries`, `skill_dist`, `skill_dist_tries`, `skill_shielding`, `skill_shielding_tries`,
                       `skill_fishing`, `skill_fishing_tries`)
VALUES ('GOD', '6', '1', '100', '0', '150', '150', '0', '0', '0', '0', '0', '136', '0', '0', '0', '0', '0', '0', '0',
        '0', '2', '0', '0', '0', '0', '0', '1', '0', '0', '0', NULL, '400', '0', '0', 0x30, '1', '0', '0', '0', '0',
        '0', '0', '0', '43200', '-1', '2520', '10', '0', '10', '0', '10', '0', '10', '0', '10', '0', '10', '0', '10',
        '0');

-- creates characters for each vocation
INSERT INTO `players` (`name`, `group_id`, `account_id`, `level`, `vocation`, `health`, `healthmax`, `experience`,
                       `lookbody`, `lookfeet`, `lookhead`, `looklegs`, `looktype`, `lookaddons`, `lookmount`,
                       `lookmounthead`, `lookmountbody`, `lookmountlegs`, `lookmountfeet`, `currentmount`,
                       `randomizemount`, `direction`, `maglevel`, `mana`, `manamax`, `manaspent`, `soul`, `town_id`,
                       `posx`, `posy`, `posz`, `conditions`, `cap`, `sex`, `lastlogin`, `lastip`, `save`, `skull`,
                       `skulltime`, `lastlogout`, `blessings`, `onlinetime`, `deletion`, `balance`,
                       `offlinetraining_time`, `offlinetraining_skill`, `stamina`, `skill_fist`, `skill_fist_tries`,
                       `skill_club`, `skill_club_tries`, `skill_sword`, `skill_sword_tries`, `skill_axe`,
                       `skill_axe_tries`, `skill_dist`, `skill_dist_tries`, `skill_shielding`, `skill_shielding_tries`,
                       `skill_fishing`, `skill_fishing_tries`)
VALUES ('Sorcerer', '1', '1', '100', '1', '10000', '10000', '0', '0', '0', '0', '0', '136', '0', '0', '0', '0', '0', '0',
        '0', '0', '2', '80', '30000', '30000', '0', '100', '1', '0', '0', '0', NULL, '10000', '0', '0', 0x30, '1', '0', '0',
        '0', '0', '0', '0', '0', '43200', '-1', '2520', '10', '0', '10', '0', '10', '0', '10', '0', '10', '0', '10',
        '0', '10', '0'),
    ('Druid', '1', '1', '100', '2', '10000', '10000', '0', '0', '0', '0', '0', '136', '0', '0', '0', '0', '0', '0',
        '0', '0', '2', '80', '30000', '30000', '0', '100', '1', '0', '0', '0', NULL, '10000', '0', '0', 0x30, '1', '0', '0',
        '0', '0', '0', '0', '0', '43200', '-1', '2520', '10', '0', '10', '0', '10', '0', '10', '0', '10', '0', '10',
        '0', '10', '0'),
    ('Paladin', '1', '1', '100', '3', '20000', '20000', '0', '0', '0', '0', '0', '136', '0', '0', '0', '0', '0', '0',
        '0', '0', '2', '15', '10000', '10000', '0', '100', '1', '0', '0', '0', NULL, '10000', '0', '0', 0x30, '1', '0', '0',
        '0', '0', '0', '0', '0', '43200', '-1', '2520', '10', '0', '10', '0', '10', '0', '10', '0', '90', '0', '60',
        '0', '10', '0'),
    ('Knight', '1', '1', '100', '4', '30000', '30000', '0', '0', '0', '0', '0', '136', '0', '0', '0', '0', '0', '0',
        '0', '0', '2', '9', '1000', '5000', '0', '100', '1', '0', '0', '0', NULL, '10000', '0', '0', 0x30, '1', '0', '0',
        '0', '0', '0', '0', '0', '43200', '-1', '2520', '60', '0', '90', '0', '90', '0', '90', '0', '10', '0', '80',
        '0', '10', '0');
