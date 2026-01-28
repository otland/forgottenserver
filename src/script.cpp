// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "script.h"

#include "configmanager.h"
#include "logger.h"

extern LuaEnvironment g_luaEnvironment;

Scripts::Scripts() : scriptInterface("Scripts Interface") { scriptInterface.initState(); }

Scripts::~Scripts() { scriptInterface.reInitState(); }

bool Scripts::loadScripts(std::string folderName, bool isLib, bool reload)
{
	namespace fs = std::filesystem;

	const auto dir = fs::current_path() / "data" / folderName;
	if (!fs::exists(dir) || !fs::is_directory(dir)) {
		getLogger().warn("[Scripts::loadScripts] Can not load folder '{}'.", folderName);
		return false;
	}

	fs::recursive_directory_iterator endit;
	std::vector<fs::path> v;
	std::string disable = ("#");
	for (fs::recursive_directory_iterator it(dir); it != endit; ++it) {
		auto fn = it->path().parent_path().filename();
		if (fn == "lib" && !isLib) {
			continue;
		}
		if (fs::is_regular_file(*it) && it->path().extension() == ".lua") {
			size_t found = it->path().filename().string().find(disable);
			if (found != std::string::npos) {
				if (getBoolean(ConfigManager::SCRIPTS_CONSOLE_LOGS)) {
					getLogger().info("> {} [disabled]", it->path().filename().string());
				}
				continue;
			}
			v.push_back(it->path());
		}
	}
	sort(v.begin(), v.end());
	std::string redir;
	for (auto it = v.begin(); it != v.end(); ++it) {
		const std::string scriptFile = it->string();
		if (!isLib) {
			if (redir.empty() || redir != it->parent_path().string()) {
				auto p = fs::path(it->relative_path());
				if (getBoolean(ConfigManager::SCRIPTS_CONSOLE_LOGS)) {
					getLogger().info(">> [{}]", p.parent_path().filename().string());
				}
				redir = it->parent_path().string();
			}
		}

		if (scriptInterface.loadFile(scriptFile) == -1) {
			getLogger().error("> {} [error]", it->filename().string());
			getLogger().error("^ {}", scriptInterface.getLastLuaError());
			continue;
		}

		if (getBoolean(ConfigManager::SCRIPTS_CONSOLE_LOGS)) {
			if (!reload) {
				getLogger().info("> {} [loaded]", it->filename().string());
			} else {
				getLogger().info("> {} [reloaded]", it->filename().string());
			}
		}
	}

	return true;
}
