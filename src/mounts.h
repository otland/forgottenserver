// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_MOUNTS_H_73716D11906A4C5C9F4A7B68D34C9BA6
#define FS_MOUNTS_H_73716D11906A4C5C9F4A7B68D34C9BA6

struct Mount
{
	Mount(uint8_t id, uint16_t clientId, std::string name, int32_t speed, bool premium) :
		name(std::move(name)), speed(speed), clientId(clientId), id(id), premium(premium) {}

	std::string name;
	int32_t speed;
	uint16_t clientId;
	uint8_t id;
	bool premium;
};

class Mounts
{
	public:
		bool reload();
		bool loadFromXml();
		Mount* getMountByID(uint8_t id);
		Mount* getMountByName(const std::string& name);
		Mount* getMountByClientID(uint16_t clientId);

		const std::vector<Mount>& getMounts() const {
			return mounts;
		}

	private:
		std::vector<Mount> mounts;
};

#endif
