#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;

// Public Tests //
TEST(test_card_suit_and_rank) {
    Card two_spades = Card();
    ASSERT_EQUAL(two_spades.get_rank(), TWO);
    ASSERT_EQUAL(two_spades.get_suit(), SPADES);

    Card three_spades = Card(THREE, SPADES);
    ASSERT_EQUAL(three_spades.get_rank(), THREE);
    ASSERT_EQUAL(three_spades.get_suit(), SPADES);
    ASSERT_EQUAL(three_spades.get_suit(CLUBS), SPADES);
}

TEST(test_card_type) {
    Card three_spades = Card(THREE, SPADES);
    ASSERT_FALSE(three_spades.is_face_or_ace());
    ASSERT_FALSE(three_spades.is_right_bower(CLUBS));
    ASSERT_FALSE(three_spades.is_left_bower(CLUBS));
    ASSERT_FALSE(three_spades.is_trump(CLUBS));
}

TEST(test_card_self_comparison) {
    Card three_spades = Card(THREE, SPADES);
    ASSERT_FALSE(three_spades < three_spades);
    ASSERT_TRUE(three_spades <= three_spades);
    ASSERT_FALSE(three_spades > three_spades);
    ASSERT_TRUE(three_spades >= three_spades);
    ASSERT_TRUE(three_spades == three_spades);
    ASSERT_FALSE(three_spades != three_spades);
}

TEST(test_Suit_next) {
    ASSERT_EQUAL(Suit_next(CLUBS), SPADES);
}

TEST(test_Card_less_self) {
    Card three_spades = Card(THREE, SPADES);
    ASSERT_FALSE(Card_less(three_spades, three_spades, CLUBS));
    ASSERT_FALSE(Card_less(three_spades, three_spades, three_spades,
                           CLUBS));
}

TEST(test_card_insertion) {
    Card three_spades = Card(THREE, SPADES);
    ostringstream oss;
    oss << three_spades;
    ASSERT_EQUAL(oss.str(), "Three of Spades");
}

TEST(test_card_extraction) {
    istringstream iss("Three of Spades");
    Card c;
    iss >> c;
    ASSERT_EQUAL(c, Card(THREE, SPADES));
}

//End of Public tests ?? 
TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

//OPERATOR OVERLOAD TESTS START
TEST(test_less_than) {
    Card ace_hearts(ACE, HEARTS);
    Card ace_diamonds(ACE, DIAMONDS);
    ASSERT_TRUE(ace_hearts < ace_diamonds);
}

TEST(test_equals) {
    Card ace_hearts(ACE, HEARTS);
    Card ace_hearts_two(ACE, HEARTS);
    ASSERT_TRUE(ace_hearts == ace_hearts_two);
}

TEST(test_less_than_or_equal) {
    Card ace_hearts(ACE, HEARTS);
    Card ace_diamonds(ACE, DIAMONDS);
    ASSERT_TRUE(ace_hearts <= ace_diamonds);
}

TEST(test_greater_than) {
    Card ace_hearts(ACE, HEARTS);
    Card ace_diamonds(ACE, DIAMONDS);
    ASSERT_TRUE(ace_diamonds > ace_hearts);
}

// test for different cases
TEST(test_card_extraction_two) {
    istringstream iss("Four of Hearts");
    Card c;
    iss >> c;
    ASSERT_EQUAL(c, Card(FOUR, HEARTS));
}

TEST(test_card_insertion_two) {
    Card king_diamonds = Card(KING, DIAMONDS);
    ostringstream oss;
    oss << king_diamonds;
    ASSERT_EQUAL(oss.str(), "King of Diamonds");
}
// OPERATOR OVERLOAD TESTS END

//CARD LESS TESTS BEGIN
TEST(test_Card_less_trump_and_nontrump) { 
    Card three_spades = Card(THREE, SPADES);
    Card two_diamonds = Card(TWO, DIAMONDS);
    ASSERT_TRUE(Card_less(three_spades, two_diamonds, DIAMONDS));
}

TEST(test_Card_less_trump_and_trump) { 
    Card three_diamonds = Card(THREE, DIAMONDS);
    Card two_diamonds = Card(TWO, DIAMONDS);
    ASSERT_FALSE(Card_less(three_diamonds, two_diamonds, DIAMONDS));
}
TEST(test_Card_less_nontrump_and_nontrump) { 
    Card three_spades = Card(THREE, SPADES);
    Card two_diamonds = Card(TWO, DIAMONDS);
    ASSERT_FALSE(Card_less(three_spades, two_diamonds, CLUBS));
}

TEST(test_Card_less_left_and_right_bower) {
    // Setting up the bower cards
    Card right_bower = Card(JACK, DIAMONDS); 
    Card left_bower = Card(JACK, HEARTS); 

    // Setting up non-bower cards
    Card high_trump = Card(ACE, DIAMONDS); // High card in the trump suit.
    Card low_trump = Card(NINE, DIAMONDS); // Low card in the trump suit.
    Card high_nontrump = Card(ACE, CLUBS); // High card in a non-trump, non-same color suit.

    Suit trump = DIAMONDS;

    // Right bower should not be less than the left bower
    ASSERT_FALSE(Card_less(right_bower, left_bower, trump));

    // Left bower should be considered less than right bower
    ASSERT_TRUE(Card_less(left_bower, right_bower, trump));

    // Both bowers should be considered greater than any other trump card.
    ASSERT_FALSE(Card_less(right_bower, high_trump, trump));
    ASSERT_FALSE(Card_less(left_bower, high_trump, trump));
    ASSERT_FALSE(Card_less(right_bower, low_trump, trump));
    ASSERT_FALSE(Card_less(left_bower, low_trump, trump));

    // Both bowers should be greater than any non-trump card.
    ASSERT_FALSE(Card_less(right_bower, high_nontrump, trump));
    ASSERT_FALSE(Card_less(left_bower, high_nontrump, trump));

    // Non-trump, non-bower cards should be less than both bowers.
    ASSERT_TRUE(Card_less(high_nontrump, right_bower, trump));
    ASSERT_TRUE(Card_less(high_nontrump, left_bower, trump));
}


TEST(test_Card_less_low_trump_vs_high_nontrump) {
    // This test case checks that a low-rank trump card is considered higher 
    // than a high-rank card of any other suit.
    Card low_trump = Card(TWO, DIAMONDS);
    Card high_nontrump = Card(ACE, SPADES);

    ASSERT_FALSE(Card_less(low_trump, high_nontrump, DIAMONDS));
}

TEST(test_Card_less_led_and_nonled) {
    // This test checks that a card of the led suit is considered higher than a card of a non-led, non-trump suit.
    Card led_card = Card(QUEEN, CLUBS); // Assume clubs is the led suit but not trump
    Card non_led_card = Card(KING, HEARTS);
    Suit trump = SPADES; // Spades is the trump suit

    ASSERT_TRUE(Card_less(non_led_card, led_card, led_card, trump));
}

TEST(test_Card_less_trump_vs_led_when_different) {
    // This test checks that a trump card is considered higher than a card of the led suit when they are not the same.
    Card trump_card = Card(JACK, SPADES); // Spades is trump
    Card led_card = Card(ACE, CLUBS); // Clubs is led but not trump
    Suit trump = SPADES; // Spades is the trump suit

    ASSERT_FALSE(Card_less(trump_card, led_card, led_card, trump));
}


TEST(test_Card_less_trump_led_same_suit) {
    // This test verifies the behavior when the trump suit is also the led suit.
    Card lower_rank_trump = Card(NINE, HEARTS);
    Card higher_rank_trump = Card(KING, HEARTS);
    Card led_card = Card(TEN, HEARTS); // Hearts is both led and trump
    Suit trump = HEARTS;

    ASSERT_TRUE(Card_less(lower_rank_trump, higher_rank_trump, led_card, trump));
}

TEST(test_Card_less_nontrump_higher_rank_vs_lower_rank_led) {
    // This test checks that a higher-rank non-trump card is considered less than a lower-rank led card.
    Card higher_rank_nontrump = Card(KING, DIAMONDS); // Diamonds is neither trump nor led
    Card lower_rank_led = Card(NINE, CLUBS); // Clubs is the led suit
    Card led_card = lower_rank_led;
    Suit trump = SPADES; // Spades is the trump suit

    ASSERT_TRUE(Card_less(higher_rank_nontrump, lower_rank_led, led_card, trump));
}

TEST(test_Card_less_trump_and_led_identical) {
    // Tests the edge case where a card is both trump and led, ensuring it is considered higher than any other card.
    Card trump_and_led_card = Card(JACK, DIAMONDS); // This card is both trump and led
    Card non_trump_non_led = Card(ACE, CLUBS); // High card but neither trump nor led
    Card led_card = trump_and_led_card;
    Suit trump = DIAMONDS;

    ASSERT_FALSE(Card_less(trump_and_led_card, non_trump_non_led, led_card, trump));
}



// CARD_LESS TEST ENDS 

// FACE OR ACE TEST CASES


// dylan was here (lines 151 - 220)
// test for Jack of each suit
TEST(test_Card_Jack_face_or_ace) {
    Card jackClubs = Card(JACK, CLUBS);
    ASSERT_TRUE(jackClubs.is_face_or_ace());

    Card jackDiamonds = Card(JACK, DIAMONDS);
    ASSERT_TRUE(jackDiamonds.is_face_or_ace());

    Card jackHearts = Card(JACK, HEARTS);
    ASSERT_TRUE(jackHearts.is_face_or_ace());

    Card jackSpades = Card(JACK, SPADES);
    ASSERT_TRUE(jackSpades.is_face_or_ace());
}

// test for Queen of each suit
TEST(test_Card_Queen_face_or_ace) {
    Card queenClubs = Card(QUEEN, CLUBS);
    ASSERT_TRUE(queenClubs.is_face_or_ace());

    Card queenDiamonds = Card(QUEEN, DIAMONDS);
    ASSERT_TRUE(queenDiamonds.is_face_or_ace());

    Card queenHearts = Card(QUEEN, HEARTS);
    ASSERT_TRUE(queenHearts.is_face_or_ace());

    Card queenSpades = Card(QUEEN, SPADES);
    ASSERT_TRUE(queenSpades.is_face_or_ace());
}

// test for King of each suit
TEST(test_Card_King_face_or_ace) {
    Card kingClubs = Card(KING, CLUBS);
    ASSERT_TRUE(kingClubs.is_face_or_ace());

    Card kingDiamonds = Card(KING, DIAMONDS);
    ASSERT_TRUE(kingDiamonds.is_face_or_ace());

    Card kingHearts = Card(KING, HEARTS);
    ASSERT_TRUE(kingHearts.is_face_or_ace());

    Card kingSpades = Card(KING, SPADES);
    ASSERT_TRUE(kingSpades.is_face_or_ace());
}

// test for Ace of each suit
TEST(test_Card_Ace_face_or_ace) {
    Card aceClubs = Card(ACE, CLUBS);
    ASSERT_TRUE(aceClubs.is_face_or_ace());

    Card aceDiamonds = Card(ACE, DIAMONDS);
    ASSERT_TRUE(aceDiamonds.is_face_or_ace());

    Card aceHearts = Card(ACE, HEARTS);
    ASSERT_TRUE(aceHearts.is_face_or_ace());

    Card aceSpades = Card(ACE, SPADES);
    ASSERT_TRUE(aceSpades.is_face_or_ace());
}

// test for non-face cards (should return false)
TEST(test_Card_non_face_or_ace) {
    Card card2 = Card(TWO, CLUBS);
    ASSERT_FALSE(card2.is_face_or_ace());

    Card card10 = Card(TEN, DIAMONDS);
    ASSERT_FALSE(card10.is_face_or_ace());
}
// FACE OR ACE TESTS END

// SUIT NEXT TEST CASES
TEST(test_Card_suit_next) { 
    Card card1 = Card(JACK, CLUBS);
    Card card2 = Card(JACK, SPADES);
    Card card3 = Card(JACK, HEARTS);
    Card card4 = Card(JACK, DIAMONDS);
    ASSERT_EQUAL(Suit_next(card1.get_suit()), SPADES);
    ASSERT_EQUAL(Suit_next(card2.get_suit()), CLUBS);
    ASSERT_EQUAL(Suit_next(card3.get_suit()), DIAMONDS);
    ASSERT_EQUAL(Suit_next(card4.get_suit()), HEARTS);
}

// IS TRUMP
TEST(test_Card_is_trump) { 
    Card card1 = Card(JACK, CLUBS);
    Card card2 = Card(KING, CLUBS);
    ASSERT_TRUE(card1.is_trump(CLUBS));
    ASSERT_TRUE(card1.is_trump(SPADES));
    ASSERT_TRUE(card2.is_trump(CLUBS));
    ASSERT_FALSE(card2.is_trump(SPADES));
}

// IS LEFT BOWER
TEST(test_Card_leftbower) { 
    Card card1 = Card(JACK, CLUBS);
    ASSERT_TRUE(card1.is_left_bower(SPADES));
}

// IS RIGHT BOWER
TEST(test_Card_right_bower) { 
    Card card1 = Card(JACK, CLUBS);
    ASSERT_TRUE(card1.is_right_bower(CLUBS));
}

//GET SUIT(WITHOUT TRUMP)
TEST(test_Card_get_suit) { 
    Card card1 = Card(JACK, CLUBS);
    Card card2 = Card(QUEEN, HEARTS);
    ASSERT_EQUAL(card1.get_suit(), CLUBS)
    ASSERT_EQUAL(card2.get_suit(), HEARTS)
}
//GET SUIT(WITH TRUMP)
TEST(test_Card_get_suit_with_trump) { 
    Card card1 = Card(JACK, CLUBS);
    ASSERT_EQUAL(card1.get_suit(SPADES), SPADES);
}

//GET RANK TEST
TEST(test_Card_get_rank) { 
    Card card1 = Card(JACK, CLUBS);
    Card card2 = Card(QUEEN, HEARTS);
    ASSERT_EQUAL(card1.get_rank(), JACK)
    ASSERT_EQUAL(card2.get_rank(), QUEEN)
}
// CARD LESS WITH LEAD CARD TEST CASES
TEST(test_Card_less) { 
    Card card1 = Card(JACK, CLUBS);
    Card card2 = Card(QUEEN, HEARTS);
    ASSERT_TRUE(Card_less(card2,card1, SPADES));
    ASSERT_TRUE(Card_less(card2,card1, CLUBS));
    ASSERT_FALSE(Card_less(card2,card1, HEARTS));
    ASSERT_FALSE(Card_less(card2,card1, DIAMONDS));
}

TEST(test_Card_less_led_card) { 
    Card led_card = Card(KING, CLUBS);
    Card card1 = Card(NINE, CLUBS);
    Card card2 = Card(QUEEN, HEARTS);
    ASSERT_TRUE(Card_less(card2,card1, led_card, SPADES));
}
// CONSTRUCTOR AND DEFAULT CONSTRUCTOR TEST CASES


// Add more test cases here

TEST_MAIN()
