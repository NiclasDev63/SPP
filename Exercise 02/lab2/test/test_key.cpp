#include "test.h"

#include "encryption/Key.h"

class KeyTest : public LabTest {};

TEST_F(KeyTest, test_three_a_one) {
}

TEST_F(KeyTest, test_three_b_one) {
	const auto& standard_key = Key::get_standard_key();

	const auto standard_hash = Key::hash(standard_key);
	const auto expected_standard_hash = std::uint64_t{ 11'361'817'798'440'499'784 };

	ASSERT_EQ(standard_hash, expected_standard_hash);
}

TEST_F(KeyTest, test_three_c_one) {
}

TEST_F(KeyTest, test_three_c_two) {
	auto keys = std::vector<Key::key_type>{};
	keys.reserve(1024);

	keys.emplace_back(Key::get_standard_key());
	for (auto i = std::size_t(0); i < 1023; i++) {
		keys.emplace_back(Key::produce_new_key(keys[i]));
	}

	const auto minimum_hash = Key::get_smallest_hash(keys);
	const auto expected_minimum_hash = std::uint64_t{ 30'403'176'973'399'313 };

	ASSERT_EQ(minimum_hash, expected_minimum_hash);
}

TEST_F(KeyTest, test_three_e_one) {

}

TEST_F(KeyTest, test_three_f_two) {
}
