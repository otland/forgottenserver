// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "talkaction.h"

#include "player.h"

TalkActions::TalkActions() : scriptInterface("TalkAction Interface") { scriptInterface.initState(); }

TalkActions::~TalkActions() { clear(); }

void TalkActions::clear()
{
	talkActions.clear();

	scriptInterface.reInitState();
}

bool TalkActions::registerLuaEvent(TalkAction_shared_ptr talkAction)
{
	if (!talkAction->getWordsMap().empty()) {
		const auto& words = talkAction->getWordsMap();
		for (auto& word : words) {
			auto result = talkActions.emplace(word, talkAction);
			if (!result.second) {
				std::cout << "[Warning - Talkctions::registerLuaEvent] Duplicate registered word: " << word
				          << std::endl;
			}
		}
		talkAction->clearWords();
	} else {
		return false;
	}

	return true;
}

TalkAction_shared_ptr TalkActions::getTalkActionEvent(const std::string& word)
{
	auto it = talkActions.find(word);
	if (it != talkActions.end()) {
		return it->second;
	}

	return nullptr;
}

TalkActionResult_t TalkActions::playerSaySpell(Player* player, SpeakClasses type, const std::string& words) const
{
	size_t wordsLength = words.length();
	for (auto it = talkActions.begin(); it != talkActions.end();) {
		const std::string& talkactionWords = it->first;
		if (!caseInsensitiveStartsWith(words, talkactionWords)) {
			++it;
			continue;
		}

		std::string param;
		if (wordsLength != talkactionWords.size()) {
			param = words.substr(talkactionWords.size());
			if (param.front() != ' ') {
				++it;
				continue;
			}
			boost::algorithm::trim_left(param);

			std::string separator = it->second->getSeparator();
			if (separator != " ") {
				if (!param.empty()) {
					if (param != separator) {
						++it;
						continue;
					} else {
						param.erase(param.begin());
					}
				}
			}
		}

		if (it->second->getNeedAccess() && !player->getGroup()->access) {
			return TALKACTION_CONTINUE;
		}

		if (player->getAccountType() < it->second->getRequiredAccountType()) {
			return TALKACTION_CONTINUE;
		}

		if (it->second->executeSay(player, talkactionWords, param, type)) {
			return TALKACTION_CONTINUE;
		}
		return TALKACTION_BREAK;
	}
	return TALKACTION_CONTINUE;
}

bool TalkAction::executeSay(Player* player, const std::string& words, const std::string& param, SpeakClasses type) const
{
	// onSay(player, words, param, type)
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - TalkAction::executeSay] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(scriptId, scriptInterface);

	lua_State* L = scriptInterface->getLuaState();

	scriptInterface->pushFunction(scriptId);

	LuaScriptInterface::pushUserdata<Player>(L, player);
	LuaScriptInterface::setMetatable(L, -1, "Player");

	LuaScriptInterface::pushString(L, words);
	LuaScriptInterface::pushString(L, param);
	lua_pushnumber(L, type);

	return scriptInterface->callFunction(4);
}
