#include "Player.hpp"
#include "Card.hpp"
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

class SimplePlayer: public Player{
  private:
    const string name;
    vector<Card> hand;

  public:
  SimplePlayer(const string &name): name(name){ //should this be passed by 
      //                                    reference|| yes it should be
  } 

  // Maximum number of cards in a player's hand
  static const int MAX_HAND_SIZE = 5;

  //EFFECTS returns player's name
  const std::string & get_name() const{ // CHECKED
    return name;
  }

  //REQUIRES player has less than MAX_HAND_SIZE cards
  //EFFECTS  adds Card c to Player's hand
  void add_card(const Card &c){ // CHECKED
    hand.push_back(c);
  }
  //REQUIRES round is 1 or 2
  //MODIFIES order_up_suit
  //EFFECTS If Player wishes to order up a trump suit then return true and
  //  change order_up_suit to desired suit.  If Player wishes to pass, then do
  //  not modify order_up_suit and return false.
  bool make_trump(const Card &upcard, bool is_dealer,
                          int round, Suit &order_up_suit) const{
      // better approach: determine suit first then count eligible cards?
    int num_of_trump = 0;
    int num_of_trump_selected = 0;
    if(round==1){
      for(int i=0; i < hand.size(); i++){
        if(hand.at(i).is_face_or_ace() && hand.at(i).is_trump(upcard.get_suit())){
            num_of_trump++;
        }
      }
       if(num_of_trump>=2){
            order_up_suit = upcard.get_suit();
            return true;
       }
       else{
        return false;
       } 
    }
    else if(round == 2){
    for(int i=0; i < hand.size(); i++){
        if(hand.at(i).is_face_or_ace() && 
        hand.at(i).is_trump(Suit_next(upcard.get_suit()))){
            num_of_trump_selected++;
        }
      }
      if(num_of_trump_selected>=1 || is_dealer){
            order_up_suit = Suit_next(upcard.get_suit());
            return true;
      }
       else{
           return false;
       } 
    }
    return false; 
}
  //REQUIRES Player has at least one card
  //EFFECTS  Player adds one card to hand and removes one card from hand.
  void add_and_discard(const Card& upcard) { // Checked
      assert(hand.size() > 0); // double checked this; this assert() is good
      Suit trump = upcard.get_suit();
      hand.push_back(upcard); // Add the upcard to the hand

      Card minimum = hand.at(0);
      int minIndex = 0;

      for (int i = 1; i < hand.size(); ++i) {
          if (Card_less(hand.at(i), minimum, trump)) {
              minimum = hand.at(i);
              minIndex = i;
          }
      }

      // Remove the correctly identified lowest card
      hand.erase(hand.begin() + minIndex);
  }




  //REQUIRES Player has at least one card
  //EFFECTS  Leads one Card from Player's hand according to their strategy
  //  "Lead" means to play the first Card in a trick.  The card
  //  is removed the player's hand.
  Card lead_card(Suit trump){ 
    // possibly a more efficient approach exists but its fine
    vector<Card> non_trump;
    vector<Card> trum;
    Card maximum;
    
    for(int i = 0; i < hand.size(); i++){
      if(!(hand.at(i)).is_trump(trump)){
        non_trump.push_back(hand.at(i));
      }
      else{
        trum.push_back(hand.at(i));
      }
    }
    
    if(non_trump.empty()){ //condition that if all cards are trump
      maximum = trum.at(0);
      for(int i = 1; i < trum.size(); i++){
        if(Card_less(maximum, trum.at(i), trump)){
          maximum = trum.at(i);
       }
      }
    }
    else{
      maximum = non_trump.at(0);
      for(int i = 0; i < non_trump.size(); i++){
        if(Card_less(maximum, non_trump.at(i), trump)){
        maximum = non_trump.at(i);
        }
      }
    } 
    for(int index = 0; index < hand.size(); index++){
      if(maximum == hand.at(index)){
        hand.erase(hand.begin() + index);
      }
    }
    return maximum;
  }
    

  //REQUIRES Player has at least one card
  //EFFECTS  Plays one Card from Player's hand according to their strategy.
  //  The card is removed from the player's hand.
  Card play_card(const Card &led_card, Suit trump){ // Checked
    vector<Card> led_suit_cards;
    vector<Card> non_led_suit_cards;
    Card return_card;


      for(int i = 0; i < hand.size(); i++){
        Card current_card = hand.at(i);
        if(current_card.get_suit(trump) == led_card.get_suit(trump))
        {
          led_suit_cards.push_back(hand.at(i));
        }
        else{
          non_led_suit_cards.push_back(hand.at(i));
        }
      }

      if(led_suit_cards.empty()){ //condition to play lowest card
        return_card = non_led_suit_cards.at(0);
        for(int i = 1; i < non_led_suit_cards.size(); i++){
         if(Card_less(non_led_suit_cards.at(i), return_card, trump)){
            return_card = non_led_suit_cards.at(i);
          }
        }
      }
      else{ //condition to play highest suit followed card
        return_card = led_suit_cards.at(0);
        for(int i = 1; i < led_suit_cards.size(); i++){
          if(Card_less(return_card, led_suit_cards.at(i), trump)){
            return_card = led_suit_cards.at(i);
          }
         }
      }

      for(int index = 0; index < hand.size(); index++){
      if(return_card == hand.at(index)){
        hand.erase(hand.begin() + index);
        }
      }
      return return_card;

    }


};

class HumanPlayer: public Player{
  private:
    const string name;
    vector<Card> hand;

  public:
  HumanPlayer(const string &name): name(name){ //should this be passed by reference 
      //                                        || Yes. Checked
  } 

  // Maximum number of cards in a player's hand
  static const int MAX_HAND_SIZE = 5;

  //EFFECTS returns player's name
  const std::string & get_name() const{ // Self explanatory, checked
    return name;
  }

  //REQUIRES player has less than MAX_HAND_SIZE cards
  //EFFECTS  adds Card c to Player's hand
  void add_card(const Card &c){ // Checked
    hand.push_back(c);
    std::sort(hand.begin(), hand.end());
  }
  //REQUIRES round is 1 or 2
  //MODIFIES order_up_suit
  //EFFECTS If Player wishes to order up a trump suit then return true and
  //  change order_up_suit to desired suit.  If Player wishes to pass, then do
  //  not modify order_up_suit and return false.
  bool make_trump(const Card &upcard, bool is_dealer,
  int round, Suit &order_up_suit) const
  {
  print_hand();
  cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
  string decision;
  cin >> decision;

  if (decision != "pass") {
    Suit ordered_up = string_to_suit(decision);
    order_up_suit = ordered_up;
    return true;
  }
  else {
    return false;
  }



  }

  void print_hand() const { // not sorting || newlines checked
  for (size_t i=0; i < hand.size(); ++i)
    cout << "Human player " << name << "'s hand: "
         << "[" << i << "] " << hand[i] << "\n";
  }

  //REQUIRES Player has at least one card
  //EFFECTS  Player adds one card to hand and removes one card from hand.
  void add_and_discard(const Card &upcard){ // REALLY NEED TO CHECK THIS
    print_hand();
    int index;
    cout << "Discard upcard: [-1]\n";
    cout << "Human player " << name << ", please select a card to discard:\n";
    cin >> index;

    // is the below necessary?
    // Input validation: Ensure index is within valid range or is -1
    while (index < -1 || index >= hand.size()) {
        cout << "Human player " << name << ", please select a card to discard:\n";
        cin >> index;
    }
    //
    if(index != -1){
      hand.erase(hand.begin() + index);
      add_card(upcard);
    }
    
    }

  //REQUIRES Player has at least one card
  //EFFECTS  Leads one Card from Player's hand according to their strategy
  //  "Lead" means to play the first Card in a trick.  The card
  //  is removed the player's hand.
  Card lead_card(Suit trump){
    print_hand();
    int index;
    cout << "Human player " << name << ", please select a card:\n";
    cin >> index;

    // is the below necessary?
    // Input validation: Ensure index is within valid range
    while (index < 0 || index >= hand.size()) {
        cout << "Human player " << name << ", please select a card:\n";
        cin >> index;
    }
    //
    Card player = hand.at(index);
    hand.erase(hand.begin() + index);
    return player;

  }
    

  //REQUIRES Player has at least one card
  //EFFECTS  Plays one Card from Player's hand according to their strategy.
  //  The card is removed from the player's hand.
  Card play_card(const Card &led_card, Suit trump){
    print_hand();
    int index;
    cout << "Human player " << name << ", please select a card:\n";
    cin >> index;
 
    // is the below necessary?
    // Input validation: Ensure index is within valid range
    while (index < 0 || index >= hand.size()) {
        cout << "Human player " << name << ", please select a card:\n";
        cin >> index;
    }
    //
    Card player = hand.at(index);
    hand.erase(hand.begin() + index);
    return player;
  }
};

Player * Player_factory(const std::string &name, 
                        const std::string &strategy) {
  // We need to check the value of strategy and return 
  // the corresponding player type.
  if (strategy == "Simple") {
    // The "new" keyword dynamically allocates an object.
    return new SimplePlayer(name);
  }
  if(strategy == "Human") {
    
    return new HumanPlayer(name);
  }
  // Repeat for each other type of Player
  // Invalid strategy if we get here
  assert(false); // Although it doesnt break anything, I don't 
  //                think this is necessary.
  return nullptr;
}


//EFFECTS: Prints player's name to os
std::ostream & operator<<(std::ostream &os, const Player &p){ //Checked
    os << p.get_name();
    return os;
}

