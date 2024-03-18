#include "Pack.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_pack_default_ctor) {
    Pack pack;
    Card first = pack.deal_one();
    ASSERT_EQUAL(NINE, first.get_rank());
    ASSERT_EQUAL(SPADES, first.get_suit());
}


// test for deal_one function returning the correct card and moving to the next
TEST(test_pack_deal_one) {
    Pack pack;
    Card first = pack.deal_one(); // should be 9 of Spades by default order
    ASSERT_EQUAL(NINE, first.get_rank());
    ASSERT_EQUAL(SPADES, first.get_suit());
    Card second = pack.deal_one(); // should be Ten of Spades, not Jack
    ASSERT_EQUAL(TEN, second.get_rank());
    ASSERT_EQUAL(SPADES, second.get_suit());
}


// test for reset function
TEST(test_pack_reset) {
    Pack pack;
    pack.deal_one();
    pack.reset();
    Card first = pack.deal_one(); // should be back to 9 of Spades
    ASSERT_EQUAL(NINE, first.get_rank());
    ASSERT_EQUAL(SPADES, first.get_suit());
}

// test for empty function
TEST(test_pack_empty) {
    Pack pack;
    for (int i = 0; i < 24; ++i) {
        pack.deal_one(); // deal all cards
    }
    ASSERT_TRUE(pack.empty()); // should be empty after 24 cards are dealt
}


TEST_MAIN()
