// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_SCRIPT_H
#define FS_SCRIPT_H

#include "luascript.h"

class Scripts
{
public:
	Scripts();
	~Scripts();

	bool loadScripts(std::string folderName, bool isLib, bool reload);
	LuaScriptInterface& getScriptInterface() { return scriptInterface; }

private:
	LuaScriptInterface scriptInterface;
};

#endif // FS_SCRIPT_H
