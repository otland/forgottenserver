#include "otpch.h"

#include "shaders.h"

#include "pugicast.h"
#include "tools.h"

bool Shaders::reload()
{
	shaders.clear();
	return loadFromXml();
}

bool Shaders::loadFromXml()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/XML/shaders.xml");
	if (!result) {
		printXMLError("Error - Shaders::loadFromXml", "data/XML/shaders.xml", result);
		return false;
	}

	for (auto shaderNode : doc.child("shaders").children()) {
		shaders.emplace_back(
			static_cast<uint8_t>(pugi::cast<uint16_t>(shaderNode.attribute("id").value())),
			shaderNode.attribute("name").as_string(),
			shaderNode.attribute("premium").as_bool()
		);
	}
	shaders.shrink_to_fit();
	return true;
}

Shader* Shaders::getShaderByID(uint8_t id)
{
	auto it = std::find_if(shaders.begin(), shaders.end(), [id](const Shader& shader) {
		return shader.id == id;
	});

	return it != shaders.end() ? &*it : nullptr;
}

Shader* Shaders::getShaderByName(const std::string& name) {
	auto shaderName = name.c_str();
	for (auto& it : shaders) {
		if (strcasecmp(shaderName, it.name.c_str()) == 0) {
			return &it;
		}
	}

	return nullptr;
}

