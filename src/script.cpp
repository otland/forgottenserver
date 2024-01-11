// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "script.h"

#include "configmanager.h"

extern LuaEnvironment g_luaEnvironment;
extern ConfigManager g_config;

Scripts::Scripts() : scriptInterface("Scripts Interface") { scriptInterface.initState(); }

Scripts::~Scripts() { scriptInterface.reInitState(); }

bool Scripts::loadScripts(bool reload)
{
	namespace fs = std::filesystem;

	const fs::path dir = "data/scripts";
	if (!fs::exists(dir) || !fs::is_directory(dir)) {
		std::cout << "[Warning - Scripts::loadScripts] Can not load scripts folder." << std::endl;
		return false;
	}

	fs::recursive_directory_iterator endit;
	std::vector<fs::path> v;
	std::string disable = ("#");
	for (fs::recursive_directory_iterator it(dir); it != endit; ++it) {
		if (fs::is_regular_file(*it) && it->path().extension() == ".lua") {
			size_t found = it->path().filename().string().find(disable);
			if (found != std::string::npos) {
				if (g_config.getBoolean(ConfigManager::SCRIPTS_CONSOLE_LOGS)) {
					std::cout << "> " << it->path().filename().string() << " [disabled]" << std::endl;
				}
				continue;
			}

			if (scriptInterface.loadFile(it->path().string()) == -1) {
				std::cout << "> " << it->path().string() << " [error]" << std::endl;
				std::cout << "^ " << scriptInterface.getLastLuaError() << std::endl;
				continue;
			}

			if (g_config.getBoolean(ConfigManager::SCRIPTS_CONSOLE_LOGS)) {
				if (!reload) {
					std::cout << "> " << it->path().string() << " [loaded]" << std::endl;
				} else {
					std::cout << "> " << it->path().string() << " [reloaded]" << std::endl;
				}
			}
		}
	}

	return true;
}

bool Scripts::loadMonsters()
{
	namespace fs = std::filesystem;

	const fs::path dir = "data/monster/lua";
	if (!fs::exists(dir) || !fs::is_directory(dir)) {
		std::cout << "[Warning - Scripts::loadMonsters] Can not load monster/lua folder." << std::endl;
		return false;
	}

	fs::recursive_directory_iterator endit;
	std::vector<fs::path> v;
	std::string disable = ("#");
	for (fs::recursive_directory_iterator it(dir); it != endit; ++it) {
		if (fs::is_regular_file(*it) && it->path().extension() == ".lua") {
			size_t found = it->path().filename().string().find(disable);
			if (found != std::string::npos) {
				continue;
			}

			if (scriptInterface.loadFile(it->path().string()) == -1) {
				std::cout << "> " << it->path().string() << " [error]" << std::endl;
				std::cout << "^ " << scriptInterface.getLastLuaError() << std::endl;
				continue;
			}
		}
	}

	return true;
}

bool Scripts::loadLibs()
{
	namespace fs = std::filesystem;

	const fs::path dir = "data/lib";
	if (!fs::exists(dir) || !fs::is_directory(dir)) {
		std::cout << "[Warning - Scripts::loadlibs] Can not load lib folder." << std::endl;
		return false;
	}

	fs::recursive_directory_iterator endit;
	std::vector<fs::path> v;
	std::string disable = ("#");
	for (fs::recursive_directory_iterator it(dir); it != endit; ++it) {
		if (fs::is_regular_file(*it) && it->path().extension() == ".lua") {
			size_t found = it->path().filename().string().find(disable);
			if (found != std::string::npos) {
				continue;
			}

			if (scriptInterface.loadFile(it->path().string()) == -1) {
				std::cout << "> " << it->path().string() << " [error]" << std::endl;
				std::cout << "^ " << scriptInterface.getLastLuaError() << std::endl;
				continue;
			}
		}
	}

	return true;
}
