/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "otpch.h"

#include "script.h"
#include <boost/filesystem.hpp>
using namespace std;
using namespace boost::filesystem;

extern LuaEnvironment g_luaEnvironment;

Scripts::Scripts() :
	scriptInterface("Scripts Interface")
{
	scriptInterface.initState();
}

Scripts::~Scripts()
{
	scriptInterface.reInitState();
}

bool Scripts::loadScripts(string folderName)
{
	const auto dir = current_path() / "\\data\\" / folderName / "\\";
	if(!exists(dir) || !is_directory(dir)) {
		cout << "[Warning - Scripts::loadScripts] Can not load folder 'scripts'." << endl;
		return false;
	}

	cout << ">> Loading lua scripts" << endl;
	recursive_directory_iterator endit;
	typedef vector<path> vec;
	vec v;
	string disable = ("#");
	for(recursive_directory_iterator it(dir); it != endit; ++it) {
		if(is_regular_file(*it) && it->path().extension() == ".lua") {
			size_t found = it->path().filename().string().find(disable);
			if (found != string::npos) {
				cout << "> " << it->path().filename().string() << " [disabled]" << endl;
				continue;
			}
			v.push_back(it->path());
		}
	}
	sort(v.begin(), v.end());
	string redir;
	for (vec::const_iterator it(v.begin()); it != v.end(); ++it) {
		const string scriptFile = it->string();
		if (redir.empty() || redir != it->parent_path().string()) {
			auto p = boost::filesystem::path(it->relative_path());
			cout << ">> [" << p.parent_path().filename() << "]" << endl;
			redir = it->parent_path().string();
		}

		if(scriptInterface.loadFile(scriptFile) == -1) {
			cout << "> " << it->filename().string() << " [error]" << endl;
			cout << "^ " << scriptInterface.getLastLuaError() << endl;
			continue;
		}

		cout << "> " << it->filename().string() << " [loaded]" << endl;
	}

	return true;
}
