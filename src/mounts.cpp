// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "mounts.h"

#include "pugicast.h"
#include "tools.h"

bool Mounts::reload()
{
	mounts.clear();
	return loadFromXml();
}

bool Mounts::loadFromXml()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/XML/mounts.xml");
	if (!result) {
		printXMLError("Error - Mounts::loadFromXml", "data/XML/mounts.xml", result);
		return false;
	}

	for (auto mountNode : doc.child("mounts").children()) {
		uint16_t nodeId = pugi::cast<uint16_t>(mountNode.attribute("id").value());
		if (nodeId == 0 || nodeId > std::numeric_limits<uint8_t>::max()) {
			std::cout << "[Notice - Mounts::loadFromXml] Mount id \"" << nodeId << "\" is not within 1 and 255 range" <<  std::endl;
			continue;
		}

		if (getMountByID(nodeId)) {
			std::cout << "[Notice - Mounts::loadFromXml] Duplicate mount with id: " << nodeId << std::endl;
			continue;
		}

		mounts.emplace_back(
			static_cast<uint8_t>(nodeId),
			pugi::cast<uint16_t>(mountNode.attribute("clientid").value()),
			mountNode.attribute("name").as_string(),
			pugi::cast<int32_t>(mountNode.attribute("speed").value()),
			mountNode.attribute("premium").as_bool()
		);
	}
	mounts.shrink_to_fit();
	return true;
}

Mount* Mounts::getMountByID(uint8_t id)
{
	auto it = std::find_if(mounts.begin(), mounts.end(), [id](const Mount& mount) {
		return mount.id == id;
	});

	return it != mounts.end() ? &*it : nullptr;
}

Mount* Mounts::getMountByName(const std::string& name) {
	auto mountName = name.c_str();
	for (auto& it : mounts) {
		if (strcasecmp(mountName, it.name.c_str()) == 0) {
			return &it;
		}
	}

	return nullptr;
}

Mount* Mounts::getMountByClientID(uint16_t clientId)
{
	auto it = std::find_if(mounts.begin(), mounts.end(), [clientId](const Mount& mount) {
		return mount.clientId == clientId;
	});

	return it != mounts.end() ? &*it : nullptr;
}
