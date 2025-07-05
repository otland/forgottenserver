#include "otpch.h"

#include "base64.h"

static constexpr std::string_view alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

std::string tfs::base64::encode(std::string_view input)
{
	std::string output;
	output.reserve((input.size() * 4 + 2) / 3); // Estimated size

	size_t i = 0;
	while (i + 3 <= input.size()) {
		uint32_t chunk = (static_cast<uint8_t>(input[i]) << 16) | (static_cast<uint8_t>(input[i + 1]) << 8) |
		                 static_cast<uint8_t>(input[i + 2]);

		output.push_back(alphabet[(chunk >> 18) & 0x3F]);
		output.push_back(alphabet[(chunk >> 12) & 0x3F]);
		output.push_back(alphabet[(chunk >> 6) & 0x3F]);
		output.push_back(alphabet[chunk & 0x3F]);
		i += 3;
	}

	if (i < input.size()) {
		uint32_t chunk = static_cast<uint8_t>(input[i]) << 16;
		if (i + 1 < input.size()) {
			chunk |= static_cast<uint8_t>(input[i + 1]) << 8;
		}

		output.push_back(alphabet[(chunk >> 18) & 0x3F]);
		output.push_back(alphabet[(chunk >> 12) & 0x3F]);
		if (i + 1 < input.size()) {
			output.push_back(alphabet[(chunk >> 6) & 0x3F]);
		}
	}

	return output;
}

std::string tfs::base64::decode(std::string_view input)
{
	static constexpr auto reverse_table = [] {
		std::array<uint32_t, 256> table{};
		table.fill(-1);
		for (size_t i = 0; i < alphabet.size(); ++i) {
			table[static_cast<uint8_t>(alphabet[i])] = static_cast<uint8_t>(i);
		}
		return table;
	}();

	size_t len = input.size();
	assert(len % 4 != 1);

	std::string output;
	output.reserve((len * 3) / 4); // Estimated

	size_t i = 0;
	while (i + 4 <= len) {
		uint32_t chunk = (reverse_table[static_cast<uint8_t>(input[i])] << 18) |
		                 (reverse_table[static_cast<uint8_t>(input[i + 1])] << 12) |
		                 (reverse_table[static_cast<uint8_t>(input[i + 2])] << 6) |
		                 reverse_table[static_cast<uint8_t>(input[i + 3])];

		output.push_back((chunk >> 16) & 0xFF);
		output.push_back((chunk >> 8) & 0xFF);
		output.push_back(chunk & 0xFF);
		i += 4;
	}

	if (i < len) {
		uint32_t chunk = 0;
		int padding = 0;

		chunk |= reverse_table[static_cast<uint8_t>(input[i])] << 18;
		chunk |= reverse_table[static_cast<uint8_t>(input[i + 1])] << 12;

		if (i + 2 < len) {
			chunk |= reverse_table[static_cast<uint8_t>(input[i + 2])] << 6;
		} else {
			++padding;
		}

		output.push_back((chunk >> 16) & 0xFF);
		if (padding < 1) output.push_back((chunk >> 8) & 0xFF);
	}

	return output;
}
