#include "Player.hpp"
#include "unit_test_framework.hpp"
#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <sstream>
using namespace std;

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(test_player_insertion) {
  // Create a Human player
  Player * human = Player_factory("NotRobot", "Human");

  // Print the player using the stream insertion operator
  ostringstream oss1;
  oss1 << * human;

  // Verify that the output is the player's name
  ASSERT_EQUAL(oss1.str(), "NotRobot");

  // Create a Simple player
  Player * alice = Player_factory("Alice", "Simple");

  // Print the player using the stream insertion operator
  ostringstream oss2;
  oss2 << *alice;
  ASSERT_EQUAL(oss2.str(), "Alice");

  // Clean up players that were created using Player_factory()
  delete human;
  delete alice;
}
TEST(test_simple_player_make_trump) {
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, SPADES));

  // Bob makes tump
  Card nine_spades(NINE, SPADES);
  Suit trump;
  bool orderup = bob->make_trump(
    nine_spades,    // Upcard
    true,           // Bob is also the dealer
    1,              // First round
    trump           // Suit ordered up (if any)
  );

  // Verify Bob's order up and trump suit
  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, SPADES);

  delete bob;
}

TEST(test_simple_player_lead_card) {
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, SPADES));

  // Bob adds a card to his hand and discards one card
  bob->add_and_discard(
    Card(NINE, HEARTS) // upcard
  );

  // Bob leads
  Card card_led = bob->lead_card(HEARTS);

  // Verify the card Bob selected to lead
  Card ace_spades(ACE, SPADES);
  ASSERT_EQUAL(card_led, ace_spades); //check led card

  delete bob;
}

TEST(test_simple_player_play_card) {
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, SPADES));

  // Bob plays a card
  Card nine_diamonds(NINE, DIAMONDS);
  Card card_played = bob->play_card(
    nine_diamonds,  // Nine of Diamonds is led
    HEARTS    // Trump suit
  ); 

  // Verify the card Bob played
  ASSERT_EQUAL(card_played, Card(NINE, SPADES));
  delete bob;
}

// MAKE TRUMP FUNCITON TESTS BEGIN

TEST(test_simple_player_make_trump_2_trump) {
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, CLUBS));
  bob->add_card(Card(TEN, DIAMONDS));
  bob->add_card(Card(QUEEN, HEARTS));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, SPADES));

  // Bob makes tump
  Card nine_spades(NINE, SPADES);
  Suit trump;
  bool orderup = bob->make_trump(
    nine_spades,    // Upcard
    true,           // Bob is also the dealer
    1,              // First round
    trump           // Suit ordered up (if any)
  );

  // Verify Bob's order up and trump suit
  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, SPADES);

  delete bob;
}

TEST(test_simple_player_make_trump_1_trump_1_bauer) {
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, CLUBS));
  bob->add_card(Card(TEN, DIAMONDS));
  bob->add_card(Card(QUEEN, HEARTS));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(JACK, CLUBS));

  // Bob makes tump
  Card nine_spades(NINE, SPADES);
  Suit trump;
  bool orderup = bob->make_trump(
    nine_spades,    // Upcard
    true,           // Bob is also the dealer
    1,              // First round
    trump           // Suit ordered up (if any)
  );

  // Verify Bob's order up and trump suit
  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, SPADES);

  delete bob;
}

TEST(test_simple_player_make_trump_1_trump) { //does not account for if the player is dealer and will gain the additional card
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, CLUBS));
  bob->add_card(Card(TEN, DIAMONDS));
  bob->add_card(Card(QUEEN, HEARTS));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(JACK, CLUBS));

  // Bob makes tump
  Card king_hearts(KING, HEARTS);
  Suit trump;
  bool orderup = bob->make_trump(
    king_hearts,    // Upcard
    true,           // Bob is also the dealer
    1,              // First round
    trump           // Suit ordered up (if any)
  );

  // Verify Bob's order up and trump suit
  ASSERT_FALSE(orderup);

  delete bob;
}
TEST(test_simple_player_make_trump_1_trump_not_dealer) { //does not account for if the player is dealer and will gain the additional card
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, CLUBS));
  bob->add_card(Card(TEN, DIAMONDS));
  bob->add_card(Card(QUEEN, HEARTS));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(JACK, CLUBS));

  // Bob makes tump
  Card king_hearts(KING, HEARTS);
  Suit trump;
  bool orderup = bob->make_trump(
    king_hearts,    // Upcard
    false,           // Bob is also the dealer
    1,              // First round
    trump           // Suit ordered up (if any)
  );

  // Verify Bob's order up and trump suit
  ASSERT_FALSE(orderup);

  delete bob;
}


TEST(test_simple_player_make_trump_0_trump) {
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, CLUBS));
  bob->add_card(Card(TEN, DIAMONDS));
  bob->add_card(Card(QUEEN, HEARTS));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(JACK, CLUBS));

  // Bob makes tump
  Card king_diamonds(KING, DIAMONDS);
  Suit trump;
  bool orderup = bob->make_trump(
    king_diamonds,    // Upcard
    true,           // Bob is also the dealer
    1,              // First round
    trump           // Suit ordered up (if any)
  );

  // Verify Bob's order up and trump suit
  ASSERT_FALSE(orderup);

  delete bob;
}

// DYLAN TESTS START
// Test case for make_trump function when no cards are face or ace in round 1
TEST(test_make_trump_no_face_or_ace_round1) {
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(TWO, SPADES));
    player->add_card(Card(THREE, CLUBS));
    player->add_card(Card(FOUR, DIAMONDS));
    player->add_card(Card(FIVE, HEARTS));
    player->add_card(Card(SIX, SPADES));
    Suit order_up_suit = SPADES;
    bool result = player->make_trump(Card(QUEEN, SPADES), false, 1, order_up_suit);
    ASSERT_FALSE(result);
    delete player;
    
}

// test case for make_trump function when exactly two face or ace cards in round 1
TEST(test_make_trump_two_face_or_ace_round1) {
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(KING, SPADES));
    player->add_card(Card(QUEEN, SPADES));
    player->add_card(Card(FIVE, HEARTS));
    player->add_card(Card(SIX, SPADES));
    player->add_card(Card(SEVEN, CLUBS));
    Suit order_up_suit;
    bool result = player->make_trump(Card(ACE, SPADES), false, 1, order_up_suit);
    ASSERT_TRUE(result);
    ASSERT_EQUAL(order_up_suit, SPADES);
    delete player;
}

// test case for make_trump function in round 2 when player is dealer
TEST(test_make_trump_round2_is_dealer) {
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(TWO, CLUBS));
    player->add_card(Card(THREE, CLUBS));
    player->add_card(Card(FOUR, DIAMONDS));
    player->add_card(Card(FIVE, HEARTS));
    player->add_card(Card(SIX, SPADES));
    Suit order_up_suit;
    bool result = player->make_trump(Card(QUEEN, CLUBS), true, 2, order_up_suit);
    ASSERT_TRUE(result);
    ASSERT_EQUAL(order_up_suit, SPADES); // Assuming Suit_next function correctly changes CLUBS to SPADES
    delete player;
}

// test case for make_trump function in round 2 when player has one face or ace card
TEST(test_make_trump_one_face_or_ace_round2) {
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(KING, DIAMONDS));
    player->add_card(Card(THREE, CLUBS));
    player->add_card(Card(FOUR, DIAMONDS));
    player->add_card(Card(FIVE, HEARTS));
    player->add_card(Card(SIX, SPADES));
    Suit order_up_suit;
    bool result = player->make_trump(Card(QUEEN, HEARTS), false, 2, order_up_suit);
    ASSERT_TRUE(result);
    ASSERT_EQUAL(order_up_suit, DIAMONDS); // Assuming Suit_next function correctly changes HEARTS to DIAMONDS
    delete player;
}

// test case for make_trump function when player decides to pass in round 2 without being dealer and no face or ace
TEST(test_make_trump_pass_round2_not_dealer_no_face_ace) {
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(TWO, CLUBS));
    player->add_card(Card(THREE, CLUBS));
    player->add_card(Card(FOUR, DIAMONDS));
    player->add_card(Card(FIVE, HEARTS));
    player->add_card(Card(SIX, SPADES));
    Suit order_up_suit = HEARTS; // Initially set to HEARTS for testing
    bool result = player->make_trump(Card(QUEEN, CLUBS), false, 2, order_up_suit);
    ASSERT_FALSE(result);
    ASSERT_EQUAL(order_up_suit, HEARTS); // order_up_suit should not change
    delete player;
}

// MAKE TRUMP FUNCTION TESTS END

// ADD AND DISCARD FUNCTION TESTS BEGIN

// 
TEST(test_add_and_discard_with_full_trump) {
    // Setup: Player's hand is full and must discard the least valuable card
    Player* player = Player_factory("TestPlayer", "Simple");

    Card card_one(ACE, SPADES);
    Card card_two(KING, SPADES);
    Card card_three(QUEEN, SPADES);
    Card card_four(JACK, SPADES);
    Card card_five(TEN, SPADES);

    player->add_card(card_one);
    player->add_card(card_two);
    player->add_card(card_three);
    player->add_card(card_four);
    player->add_card(card_five);

    Card new_card(NINE, SPADES);

    Card led_card = player->lead_card(SPADES);
    ASSERT_EQUAL(led_card, Card(JACK, SPADES));
    led_card = player->lead_card(SPADES);
    ASSERT_EQUAL(led_card, Card(ACE, SPADES));
    led_card = player->lead_card(SPADES);
    ASSERT_EQUAL(led_card, Card(KING, SPADES));
    led_card = player->lead_card(SPADES);
    ASSERT_EQUAL(led_card, Card(QUEEN, SPADES));
    led_card = player->lead_card(SPADES);
    ASSERT_EQUAL(led_card, Card(TEN, SPADES));
    
    delete player;

    // Expect that the NINE of SPADES is discarded, and hand remains unchanged
    // Assert through behavior: After attempting to lead a card, the JACK of SPADES should still be present

}

TEST(test_add_and_discard_with_no_trump) {
    // Setup: Player's hand is full and must discard the least valuable card
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(ACE, SPADES));
    player->add_card(Card(KING, SPADES));
    player->add_card(Card(QUEEN, SPADES));
    player->add_card(Card(JACK, SPADES));
    player->add_card(Card(TEN, SPADES));

    // Card to be added is lower than any in the hand
    Card new_card(NINE, DIAMONDS);
    player->add_and_discard(new_card);

    // Expect that the NINE of SPADES is discarded, and hand remains unchanged
    // Assert through behavior: After attempting to lead a card, the ACE of SPADES should still be present
    Suit trump = DIAMONDS;
    Card led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(ACE, SPADES));
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(KING, SPADES));
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(QUEEN, SPADES));
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(JACK, SPADES));
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(NINE, trump));

    delete player;
}


TEST(test_add_and_discard_with_all_but_one_trump) { 
    // Setup: Player's hand has a mix of values, and a high-value card is added
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(NINE, HEARTS));
    player->add_card(Card(TEN, HEARTS));
    player->add_card(Card(QUEEN, HEARTS));
    player->add_card(Card(KING, HEARTS));
    player->add_card(Card(ACE, DIAMONDS));

    // Card to be added has higher value
    Card new_card(ACE, HEARTS); // DIAMONDS IS TRUMP HERE......
    //this is the trump suit for adding and discarding
    player->add_and_discard(new_card);

    Suit trump = HEARTS;
    Card led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(ACE, trump));
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(KING, trump));
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(QUEEN, trump));
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(TEN, trump));
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(NINE, trump));


    delete player;
}

TEST(test_add_and_discard_with_all_but_two_trump) { 
    // Setup: Player's hand has a mix of values, and a high-value card is added
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(NINE, HEARTS));
    player->add_card(Card(TEN, HEARTS));
    player->add_card(Card(QUEEN, HEARTS));
    player->add_card(Card(ACE, DIAMONDS));
    player->add_card(Card(ACE, SPADES));

    // Card to be added has higher value
    Card new_card(ACE, HEARTS); // DIAMONDS IS TRUMP HERE......
    //this is the trump suit for adding and discarding
    player->add_and_discard(new_card);

    Suit trump = HEARTS;
    Card led_card = player->lead_card(trump);
    ASSERT_NOT_EQUAL(led_card, Card(ACE, SPADES));

    delete player;
}

TEST(test_add_and_discard_with_bauers) { 
    // Setup: Player's hand has a mix of values, and a high-value card is added
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(JACK, CLUBS));
    player->add_card(Card(TEN, SPADES));
    player->add_card(Card(KING, SPADES));
    player->add_card(Card(QUEEN, SPADES));
    player->add_card(Card(ACE, SPADES));

    // Card to be added has higher value
    Card new_card(JACK, SPADES); // DIAMONDS IS TRUMP HERE......
    //this is the trump suit for adding and discarding
    player->add_and_discard(new_card);

    Suit trump = SPADES;
    Card led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(JACK, trump));
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(JACK, CLUBS));
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(ACE, trump));
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(KING, trump));
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(QUEEN, trump));


    player->add_card(Card(JACK, CLUBS));
    player->add_card(Card(TEN, SPADES));
    player->add_card(Card(KING, SPADES));
    player->add_card(Card(QUEEN, SPADES));
    player->add_card(Card(ACE, SPADES));
    delete player;
}

TEST(test_add_and_discard_three_suited) {
    // Setup: Player's hand will have the lowest card as trump, ensuring it doesn't get discarded incorrectly
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(NINE, CLUBS)); // This should be the lowest non-trump
    player->add_card(Card(TEN, HEARTS));
    player->add_card(Card(JACK, SPADES));
    player->add_card(Card(QUEEN, DIAMONDS));
    player->add_card(Card(KING, DIAMONDS));

    // Card to be added is trump and should cause a non-trump to be discarded
    Card up_card(ACE, SPADES); // SPADES is trump
    player->add_and_discard(up_card);

    Suit trump = SPADES;
    Card led_card = player->lead_card(SPADES);
    ASSERT_EQUAL(led_card, Card(KING, DIAMONDS));
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(QUEEN, DIAMONDS));
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(TEN, HEARTS));
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(JACK, trump));
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(ACE, trump));

    delete player;
}

TEST(test_add_and_discard_discard_upcard_if_lowest) {
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(QUEEN, CLUBS));  // Existing card, not lowest
    player->add_card(Card(KING, HEARTS));  // Existing card, not lowest
    player->add_card(Card(ACE, SPADES));   // Existing card, not lowest
    player->add_card(Card(JACK, DIAMONDS));  // Existing card, not lowest
    player->add_card(Card(TEN, HEARTS));   // Existing card, not lowest

    Card up_card(NINE, DIAMONDS); // DIAMONDS is trump, potentially lowest card
    player->add_and_discard(up_card);

    Suit trump = DIAMONDS; 
    ASSERT_NOT_EQUAL(player->lead_card(trump), up_card); // Assert up_card is not present

    delete player;
}
TEST(test_add_and_discard_discard_upcard_if_only_lower_trumps_in_hand) {
    // Player's hand consists entirely of trump cards higher than the upcard
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(TEN, SPADES)); 
    player->add_card(Card(JACK, SPADES)); 
    player->add_card(Card(QUEEN, SPADES)); 
    player->add_card(Card(KING, SPADES)); 
    player->add_card(Card(ACE, SPADES));  

    Card up_card(NINE, SPADES); 
    player->add_and_discard(up_card);

    Suit trump = SPADES;
    Card led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(JACK, SPADES)); 
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(ACE, SPADES)); 
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(KING, SPADES));
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(QUEEN, SPADES));
    led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(TEN, SPADES)); 


    delete player;
}






// ADD AND DISCARD FUNCTION TESTS END


// LEAD CARD FUNCTION TESTS BEGIN
// test case for leading a card from a hand with multiple non-trump cards and no trump cards
TEST(test_lead_card_with_non_trump_cards) {
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(TWO, HEARTS));
    player->add_card(Card(THREE, DIAMONDS));
    player->add_card(Card(FOUR, CLUBS));
    player->add_card(Card(FIVE, SPADES)); 
    Suit trump = CLUBS; 
    Card led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(FIVE, SPADES)); // Highest non-trump
    delete player;
}

// test case for leading a card from a hand with all trump cards
TEST(test_lead_card_with_all_trump_cards) {
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(TWO, SPADES));
    player->add_card(Card(THREE, SPADES));
    player->add_card(Card(FOUR, SPADES));
    Suit trump = SPADES;
    Card led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(FOUR, SPADES)); // Highest trump
    delete player;
}

// test case for leading a card from a mixed hand of trump and non-trump cards
TEST(test_lead_card_with_mixed_hand) {
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(TWO, CLUBS));
    player->add_card(Card(THREE, DIAMONDS));
    player->add_card(Card(FOUR, SPADES)); // Trump card
    player->add_card(Card(FIVE, HEARTS));
    Suit trump = SPADES;
    Card led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(FIVE, HEARTS)); // Highest non-trump, assuming strategy prefers non-trump if available
    delete player;
}

// test case for leading a card from a hand with exactly one card
TEST(test_lead_card_with_one_card) {
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(TWO, CLUBS));
    Suit trump = DIAMONDS;
    Card led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(TWO, CLUBS)); // Only card should be led
    delete player;
}

// test case for leading a card from a hand after adding and discarding cards
TEST(test_lead_card_after_add_and_discard) {
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(TWO, HEARTS));
    player->add_card(Card(THREE, DIAMONDS));
    player->add_and_discard(Card(ACE, CLUBS)); 
    Suit trump = SPADES; 
    Card led_card = player->lead_card(trump);
    ASSERT_EQUAL(led_card, Card(ACE, CLUBS)); // Assuming ACE of CLUBS is now the highest card
    delete player;
}
// LEAD CARD FUNCTION TESTS END


// TEST CASES FOR PLAY_CARD FUNCTION BEGIN
// test case when player can follow suit
TEST(test_play_card_follow_suit) {
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(TEN, CLUBS));
    player->add_card(Card(JACK, CLUBS)); // this is trump because spades makes jack of clubs trump
    player->add_card(Card(QUEEN, SPADES));
    player->add_card(Card(KING, SPADES));
    player->add_card(Card(ACE, HEARTS));

    // led card is of suit clubs, player should follow suit
    Card led_card(NINE, CLUBS);
    Suit trump = SPADES;
    Card played_card = player->play_card(led_card, trump);

    // expect that the player plays the ten of clubs
    ASSERT_EQUAL(played_card, Card(TEN, CLUBS));

    Card led_card2(JACK, SPADES);
    Card played_card2 = player->play_card(led_card2, trump);
    ASSERT_EQUAL(played_card2, Card(JACK, CLUBS));

    Card led_card3(ACE, SPADES);
    Card played_card3 = player->play_card(led_card3, trump);
    ASSERT_EQUAL(played_card3, Card(KING, SPADES));

    Card led_card4(TEN, SPADES);
    Card played_card4 = player->play_card(led_card4, trump);
    ASSERT_EQUAL(played_card4, Card(QUEEN, SPADES));

    Card led_card5(NINE, SPADES);
    Card played_card5 = player->play_card(led_card5, trump);
    ASSERT_EQUAL(played_card5, Card(ACE, HEARTS));

    delete player;
}

// test case when player cannot follow suit and plays lowest off-suit card
TEST(test_play_card_no_match_play_lowest_offsuit) {
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(NINE, DIAMONDS));
    player->add_card(Card(TEN, HEARTS));
    player->add_card(Card(JACK, DIAMONDS));
    player->add_card(Card(QUEEN, HEARTS));
    player->add_card(Card(KING, DIAMONDS));

    // led card is of suit clubs, but player has no clubs, so they should play lowest off-suit card
    Card led_card(TEN, CLUBS);
    Suit trump = SPADES;
    Card played_card = player->play_card(led_card, trump);
    ASSERT_EQUAL(played_card, Card(NINE, DIAMONDS));

    Card led_card2(JACK, SPADES);
    Card played_card2 = player->play_card(led_card2, trump);
    ASSERT_EQUAL(played_card2, Card(TEN, HEARTS));

    Card led_card3(ACE, SPADES);
    Card played_card3 = player->play_card(led_card3, trump);
    ASSERT_EQUAL(played_card3, Card(JACK, DIAMONDS));

    Card led_card4(TEN, SPADES);
    Card played_card4 = player->play_card(led_card4, trump);
    ASSERT_EQUAL(played_card4, Card(QUEEN, HEARTS));

    Card led_card5(NINE, SPADES);
    Card played_card5 = player->play_card(led_card5, trump);
    ASSERT_EQUAL(played_card5, Card(KING, DIAMONDS));

    delete player;
}

// test case when player has multiple cards of the led suit and chooses the highest
TEST(test_play_card_multiple_of_led_suit_choose_highest) {
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(NINE, CLUBS));
    player->add_card(Card(QUEEN, CLUBS));
    player->add_card(Card(JACK, CLUBS)); // this is the left bower, so its trump is effectively SPADES
    player->add_card(Card(QUEEN, SPADES));
    player->add_card(Card(KING, SPADES));

    // led card is of suit clubs
    Card led_card(TEN, CLUBS);
    Suit trump = SPADES;
    Card played_card = player->play_card(led_card, trump);
    ASSERT_EQUAL(played_card, Card(QUEEN, CLUBS));

    Card led_card2(ACE, DIAMONDS);
    Card played_card2 = player->play_card(led_card2, trump);
    ASSERT_EQUAL(played_card2, Card(NINE, CLUBS));

    Card led_card3(ACE, HEARTS);
    Card played_card3 = player->play_card(led_card3, trump);
    ASSERT_EQUAL(played_card3, Card(QUEEN, SPADES));

    Card led_card4(KING, HEARTS);
    Card played_card4 = player->play_card(led_card4, trump);
    ASSERT_EQUAL(played_card4, Card(KING, SPADES));

    Card led_card5(QUEEN, HEARTS);
    Card played_card5 = player->play_card(led_card5, trump);
    ASSERT_EQUAL(played_card5, Card(JACK, CLUBS));

    delete player;
}

// test case when player has only one card, must play it regardless of suit
TEST(test_play_card_only_one_card) {
    Player* player = Player_factory("TestPlayer", "Simple");
    player->add_card(Card(ACE, DIAMONDS));

    // led card is of suit CLUBS, but player has only one card
    Card led_card(TEN, CLUBS);
    Suit trump = SPADES;
    Card played_card = player->play_card(led_card, trump);

    // expect that the player plays the ace of diamonds, their only card
    ASSERT_EQUAL(played_card, Card(ACE, DIAMONDS));

    delete player;
}

// TEST CASES FOR PLAY_CARD FUNCTION END


// Add more tests here

TEST_MAIN()
