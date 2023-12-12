#include "test.h"

#include "util/Hash.h"

#include <cstdint>
#include <limits>

class HashTest : public LabTest {};

TEST_F(HashTest, test_one_a_one) {
	const auto value_1 = std::uint64_t{ 5'647'095'006'226'412'969 };
	const auto value_2 = std::uint64_t{ 41'413'938'183'913'153 };
	const auto value_3 = std::uint64_t{ 6'225'658'194'131'981'369 };
	const auto value_4 = std::uint64_t{ 0 };
	const auto value_5 = std::numeric_limits<std::uint64_t>::max();

	const auto expected_hash_1 = std::uint64_t{ 7'186'050'654'621'429'857 };
	const auto expected_hash_2 = std::uint64_t{ 13'665'997'653'369'079'497 };
	const auto expected_hash_3 = std::uint64_t{ 15'217'633'926'995'236'288 };
	const auto expected_hash_4 = std::uint64_t{ 17'229'166'815'727'011'488 };
	const auto expected_hash_5 = std::uint64_t{ 6'738'362'524'631'178'469 };

	const auto calculated_hash_1 = Hash::hash(value_1);
	const auto calculated_hash_2 = Hash::hash(value_2);
	const auto calculated_hash_3 = Hash::hash(value_3);
	const auto calculated_hash_4 = Hash::hash(value_4);
	const auto calculated_hash_5 = Hash::hash(value_5);

	ASSERT_EQ(expected_hash_1, calculated_hash_1);
	ASSERT_EQ(expected_hash_2, calculated_hash_2);
	ASSERT_EQ(expected_hash_3, calculated_hash_3);
	ASSERT_EQ(expected_hash_4, calculated_hash_4);
	ASSERT_EQ(expected_hash_5, calculated_hash_5);
}
