// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_TALKACTION_H
#define FS_TALKACTION_H

#include "baseevents.h"
#include "const.h"
#include "luascript.h"

class TalkAction;
using TalkAction_shared_ptr = std::shared_ptr<TalkAction>;

enum TalkActionResult_t
{
	TALKACTION_CONTINUE,
	TALKACTION_BREAK,
	TALKACTION_FAILED,
};

class TalkAction : public Event
{
public:
	explicit TalkAction(LuaScriptInterface* interface) : Event(interface) {}

	const std::string& getWords() const { return words; }
	void clearWords() { wordsMap.clear(); }
	const std::vector<std::string>& getWordsMap() const { return wordsMap; }
	void setWords(std::string_view word)
	{
		words = word;
		wordsMap.emplace_back(word);
	}
	std::string getSeparator() const { return separator; }
	void setSeparator(std::string sep) { separator = sep; }

	// scripting
	bool executeSay(Player* player, const std::string& words, const std::string& param, SpeakClasses type) const;

	AccountType_t getRequiredAccountType() const { return requiredAccountType; }

	void setRequiredAccountType(AccountType_t reqAccType) { requiredAccountType = reqAccType; }

	bool getNeedAccess() const { return needAccess; }

	void setNeedAccess(bool b) { needAccess = b; }

private:
	std::string_view getScriptEventName() const override { return "onSay"; }

	std::string words;
	std::vector<std::string> wordsMap;
	std::string separator = "\"";
	bool needAccess = false;
	AccountType_t requiredAccountType = ACCOUNT_TYPE_NORMAL;
};

class TalkActions
{
public:
	TalkActions();
	~TalkActions();

	// non-copyable
	TalkActions(const TalkActions&) = delete;
	TalkActions& operator=(const TalkActions&) = delete;

	TalkActionResult_t playerSaySpell(Player* player, SpeakClasses type, const std::string& words) const;

	bool registerLuaEvent(TalkAction_shared_ptr event);
	TalkAction_shared_ptr getTalkActionEvent(const std::string& word);
	void clear();

private:
	std::map<std::string, TalkAction_shared_ptr> talkActions;

	LuaScriptInterface scriptInterface;
};

#endif // FS_TALKACTION_H
