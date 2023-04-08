// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_SCRIPTMANAGER_H
#define FS_SCRIPTMANAGER_H

class ScriptingManager
{
public:
	ScriptingManager() = default;
	~ScriptingManager();

	// non-copyable
	ScriptingManager(const ScriptingManager&) = delete;
	ScriptingManager& operator=(const ScriptingManager&) = delete;

	static ScriptingManager& getInstance()
	{
		static ScriptingManager instance;
		return instance;
	}

	bool loadScriptSystems();
};

#endif // FS_SCRIPTMANAGER_H
