#define BOOST_TEST_MODULE networkmessage

#include "../otpch.h"

#include "../networkmessage.h"
#include "../position.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_networkmessage_reset)
{
	uint8_t expected = 12;
	uint8_t actual = 0;
	NetworkMessage msg{};
	msg.addByte(expected);
	msg.setBufferPosition(0);
	actual = msg.getByte();
	BOOST_TEST(msg.getLength() == 1);
	BOOST_TEST(msg.getBufferPosition() == 1 + NetworkMessage::INITIAL_BUFFER_POSITION);
	msg.reset();
	BOOST_TEST(msg.getLength() == 0);
	BOOST_TEST(actual == expected);
}


BOOST_AUTO_TEST_CASE(test_networkmessage_getPreviousByte)
{
	NetworkMessage msg{};
	msg.addByte(11);
	msg.addByte(22);
	msg.addByte(33);
	msg.addByte(44);
	BOOST_TEST(msg.getLength() == 4);
	BOOST_TEST(msg.getPreviousByte() == 44);
	BOOST_TEST(msg.getPreviousByte() == 33);
	BOOST_TEST(msg.getPreviousByte() == 22);
	BOOST_TEST(msg.getPreviousByte() == 11);
	// overflow case
	BOOST_TEST(msg.getPreviousByte() == 11);
	BOOST_TEST(msg.getBufferPosition() == NetworkMessage::INITIAL_BUFFER_POSITION);

}

BOOST_AUTO_TEST_CASE(test_networkmessage_add_get_template)
{
	NetworkMessage msg{};
	msg.add<uint16_t>(std::numeric_limits<uint16_t>::max());
	msg.add<uint32_t>(std::numeric_limits<uint32_t>::max());
	msg.add<uint64_t>(std::numeric_limits<uint64_t>::max());
	msg.setBufferPosition(0);
	BOOST_TEST(msg.getLength() == 14);
	BOOST_TEST(msg.get<uint16_t>() == std::numeric_limits<uint16_t>::max());
	BOOST_TEST(msg.get<uint32_t>() == std::numeric_limits<uint32_t>::max());
	BOOST_TEST(msg.get<uint64_t>() == std::numeric_limits<uint64_t>::max());
}

BOOST_AUTO_TEST_CASE(test_networkmessage_string)
{
	NetworkMessage msg{};
	msg.addString("test");
	msg.addString("Msg");
	msg.setBufferPosition(0);
	BOOST_TEST(msg.getLength() == 11);
	BOOST_TEST(msg.getString() == "test");
	BOOST_TEST(msg.getString() == "Msg");
}

BOOST_AUTO_TEST_CASE(test_networkmessage_position)
{
	Position position1(11, 22, 3);
	Position position2(111, 222, 4);
	NetworkMessage msg{};
	msg.addPosition(position1);
	msg.addPosition(position2);
	msg.setBufferPosition(0);
	BOOST_TEST(msg.getLength() == 10);
	BOOST_TEST(msg.getPosition() == position1);
	BOOST_TEST(msg.getPosition() == position2);
}

BOOST_AUTO_TEST_CASE(test_networkmessage_skip)
{
	NetworkMessage msg{};
	msg.addByte(1);
	msg.addByte(2);
	msg.addByte(5);
	msg.setBufferPosition(0);
	msg.skipBytes(2);
	BOOST_TEST(msg.getLength() == 3);
	BOOST_TEST(msg.getByte() == 5);
}

BOOST_AUTO_TEST_CASE(test_networkmessage_byte)
{
	NetworkMessage msg{};
	msg.addByte(12);
	msg.addByte(34);
	msg.setBufferPosition(0);
	BOOST_TEST(msg.getLength() == 2);
	BOOST_TEST(msg.getByte() == 12);
	BOOST_TEST(msg.getByte() == 34);
}

BOOST_AUTO_TEST_CASE(test_networkmessage_bytes)
{
	uint8_t expected1[] = {1, 2, 3};
	NetworkMessage msg1{};
	msg1.addBytes(expected1, 3);
	msg1.setBufferPosition(0);
	BOOST_TEST(msg1.getLength() == 3);
	BOOST_TEST(msg1.getByte() == expected1[0]);
	BOOST_TEST(msg1.getByte() == expected1[1]);
	BOOST_TEST(msg1.getByte() == expected1[2]);

	const char* expected2 = {"abc"};
	NetworkMessage msg2{};
	msg2.addBytes(expected2, 3);
	msg2.setBufferPosition(0);
	BOOST_TEST(msg2.getLength() == 3);
	BOOST_TEST(msg2.getByte() == expected2[0]);
	BOOST_TEST(msg2.getByte() == expected2[1]);
	BOOST_TEST(msg2.getByte() == expected2[2]);
}
