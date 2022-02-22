// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_SCRIPTS_H
#define FS_SCRIPTS_H

#include "luascript.h"
#include "enums.h"

class Scripts
{
	public:
		Scripts();
		~Scripts();

		bool loadScripts(std::string folderName, bool isLib, bool reload);
		LuaScriptInterface& getScriptInterface() {
			return scriptInterface;
		}
	private:
		LuaScriptInterface scriptInterface;
};

#endif
