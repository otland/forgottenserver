#include "../otpch.h"

#include "../luaapi.h"
#include "../luameta.h"
#include "../luascript.h"
#include "../tools.h"
#include "luaregister.h"

namespace {

using namespace tfs::lua;

int luaCreateXmlDocument(lua_State* L)
{
	// XMLDocument(filename)
	std::string filename = getString(L, 2);
	if (filename.empty()) {
		lua_pushnil(L);
		return 1;
	}

	auto doc = std::make_unique<pugi::xml_document>();
	if (auto result = doc->load_file(filename.c_str())) {
		pushUserdata<pugi::xml_document>(L, doc.release());
		setMetatable(L, -1, "XMLDocument");
	} else {
		printXMLError("Error - luaCreateXmlDocument", filename, result);
		lua_pushnil(L);
	}
	return 1;
}

int luaDeleteXmlDocument(lua_State* L)
{
	// doc:delete() or doc:__gc()
	pugi::xml_document** document = getRawUserdata<pugi::xml_document>(L, 1);
	if (document && *document) {
		delete *document;
		*document = nullptr;
	}
	return 1;
}

int luaXmlDocumentChild(lua_State* L)
{
	// doc:child(name)
	pugi::xml_document* document = getUserdata<pugi::xml_document>(L, 1);
	if (!document) {
		lua_pushnil(L);
		return 1;
	}

	std::string name = getString(L, 2);
	if (name.empty()) {
		lua_pushnil(L);
		return 1;
	}

	auto node = std::make_unique<pugi::xml_node>(document->child(name.c_str()));
	pushUserdata<pugi::xml_node>(L, node.release());
	setMetatable(L, -1, "XMLNode");
	return 1;
}

int luaDeleteXmlNode(lua_State* L)
{
	// node:delete() or node:__gc()
	pugi::xml_node** node = getRawUserdata<pugi::xml_node>(L, 1);
	if (node && *node) {
		delete *node;
		*node = nullptr;
	}
	return 1;
}

int luaXmlNodeAttribute(lua_State* L)
{
	// node:attribute(name)
	pugi::xml_node* node = getUserdata<pugi::xml_node>(L, 1);
	if (!node) {
		lua_pushnil(L);
		return 1;
	}

	std::string name = getString(L, 2);
	if (name.empty()) {
		lua_pushnil(L);
		return 1;
	}

	pugi::xml_attribute attribute = node->attribute(name.c_str());
	if (attribute) {
		pushString(L, attribute.value());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaXmlNodeName(lua_State* L)
{
	// node:name()
	pugi::xml_node* node = getUserdata<pugi::xml_node>(L, 1);
	if (!node) {
		lua_pushnil(L);
		return 1;
	}

	pushString(L, node->name());
	return 1;
}

int luaXmlNodeFirstChild(lua_State* L)
{
	// node:firstChild()
	pugi::xml_node* node = getUserdata<pugi::xml_node>(L, 1);
	if (!node) {
		lua_pushnil(L);
		return 1;
	}

	auto firstChild = node->first_child();
	if (!firstChild) {
		lua_pushnil(L);
		return 1;
	}

	auto newNode = std::make_unique<pugi::xml_node>(std::move(firstChild));
	pushUserdata<pugi::xml_node>(L, newNode.release());
	setMetatable(L, -1, "XMLNode");
	return 1;
}

int luaXmlNodeNextSibling(lua_State* L)
{
	// node:nextSibling()
	pugi::xml_node* node = getUserdata<pugi::xml_node>(L, 1);
	if (!node) {
		lua_pushnil(L);
		return 1;
	}

	auto nextSibling = node->next_sibling();
	if (!nextSibling) {
		lua_pushnil(L);
		return 1;
	}

	auto newNode = std::make_unique<pugi::xml_node>(std::move(nextSibling));
	pushUserdata<pugi::xml_node>(L, newNode.release());
	setMetatable(L, -1, "XMLNode");
	return 1;
}

} // namespace

void tfs::lua::registerXml(LuaScriptInterface& lsi)
{
	// XML
	lsi.registerClass("XMLDocument", "", luaCreateXmlDocument);
	lsi.registerMetaMethod("XMLDocument", "__gc", luaDeleteXmlDocument);
	lsi.registerMethod("XMLDocument", "delete", luaDeleteXmlDocument);

	lsi.registerMethod("XMLDocument", "child", luaXmlDocumentChild);

	lsi.registerClass("XMLNode", "");
	lsi.registerMetaMethod("XMLNode", "__gc", luaDeleteXmlNode);
	lsi.registerMethod("XMLNode", "delete", luaDeleteXmlNode);

	lsi.registerMethod("XMLNode", "attribute", luaXmlNodeAttribute);
	lsi.registerMethod("XMLNode", "name", luaXmlNodeName);
	lsi.registerMethod("XMLNode", "firstChild", luaXmlNodeFirstChild);
	lsi.registerMethod("XMLNode", "nextSibling", luaXmlNodeNextSibling);
};
