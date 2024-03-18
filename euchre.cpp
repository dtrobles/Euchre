#include <iostream>
#include "Player.hpp"
#include "Pack.hpp"
#include "Card.hpp"
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class Game {
 public:
  void play();
  Game(int argc, char *argv[]) 
  {


    player_zero = Player_factory(argv[4], 
                        argv[5]);
    player_one = Player_factory(argv[6], 
                        argv[7]);
    player_two = Player_factory(argv[8], 
                        argv[9]);
    player_three = Player_factory(argv[10], 
                        argv[11]);
    
    players.push_back(player_zero);
    players.push_back(player_one);
    players.push_back(player_two);
    players.push_back(player_three);

    team_one.push_back(player_zero);
    team_one.push_back(player_two);
    
    team_two.push_back(player_one);
    team_two.push_back(player_three);

    play_order = players;

    dealer = player_zero; //to start

    ifstream fin(argv[1]);
    Pack temp(fin);
    pack = temp;

    if(string(argv[2]) == "shuffle"){
      shuffled = true;
    }
    else if(string(argv[2]) == "noshuffle"){
      shuffled = false;
    }

    max_points = atoi(argv[3]);
  };

 private:
  bool shuffled;
  Player * player_zero; 
  Player * player_one; 
  Player * player_two;
  Player * player_three;
  Player* dealer;
  vector<Player*> players;
  vector<Player*> play_order; // How to deallocate
  vector<Player*> team_one;
  vector<Player*> team_two;
  vector<Player*> attackers;
  vector<Player*> defenders;
  
  int max_points;
  int attacker_tricks = 0;
  int defender_tricks = 0;
  int team_one_points = 0; 
  int team_two_points = 0;
  int rounds = 0;
  int hand_number;
  Pack pack;
  Card up_card;
  Suit order_up_suit;


  void shuffle();
  void deal_one_player(Player * dealt, const int num_of_cards);
  void deal();
  void shift_dealer();
  void shift_leader(Player * winner);
  bool is_dealer(Player * player);
  void set_upcard();
  void add_tricks(Player * player);
  bool make_trump_one(Player * player, bool is_dealer, int round);
  void outcome();
  void make_trump();
  void play_hand();
  void play_round();
  void round_reset();
  Player * next_player(Player *current_player);
  
  // Player * next_order(Player *current_player)


};

void Game::shuffle(){ //Checked
  if(shuffled){
    pack.shuffle();
  }
}

void Game::shift_dealer(){ // Checked
  //should be shifting play order
  Player * temp_dealer = next_player(dealer); 
  vector <Player*> temp = play_order;
  temp.at(0) = temp_dealer; 
  temp.at(1) = next_player(temp.at(0));
  temp.at(2) = next_player(temp.at(1));
  temp.at(3) = next_player(temp.at(2));

  dealer = temp_dealer;
  play_order = temp;
}

void Game::shift_leader(Player * winner){
  //Checked
  vector <Player*> temp = play_order;
  temp.at(1) = winner; 
  temp.at(2) = next_player(winner);
  temp.at(3) = next_player(next_player(winner));
  temp.at(0) = next_player(next_player(next_player(winner)));
  //0th player plays last
  play_order = temp;
}

void Game::deal_one_player(Player * dealt, const int num_of_cards){
  //Checked
  if(num_of_cards == 2){
    dealt->add_card(pack.deal_one());
    dealt->add_card(pack.deal_one());
  }
  else if(num_of_cards == 3){
    dealt->add_card(pack.deal_one());
    dealt->add_card(pack.deal_one());
    dealt->add_card(pack.deal_one());
  }
  else{}

}

Player * Game::next_player(Player * current_player){
  //Checked
  int current_index;
  for(int i = 0; i < players.size(); i++){
    if(current_player == players.at(i)){
      current_index = i;
    }
  }
  if(current_index == 3){
    return players.at(0);
  }
  return players.at(current_index+1);
}


void Game::deal(){ // Checked
  cout << play_order.at(0)->get_name() << " deals" << endl;
  deal_one_player(play_order.at(1), 3);
  deal_one_player(play_order.at(2), 2);
  deal_one_player(play_order.at(3), 3);
  deal_one_player(play_order.at(0), 2);

  deal_one_player(play_order.at(1), 2);
  deal_one_player(play_order.at(2), 3);
  deal_one_player(play_order.at(3), 2);
  deal_one_player(play_order.at(0), 3);
  //dealer is the last to be fully dealt
}

void Game::set_upcard(){ //Checked
  up_card = pack.deal_one();
  cout << up_card << " turned up" << endl;
  //*Contigent on function used immediately after deal"//
}

bool Game::is_dealer(Player * player){ //Checked
  if(player == dealer){
    return true;
  }
  return false;
}

bool Game::make_trump_one(Player* player, bool is_dealer, int round){ 
  //Checked(tent..)
  bool suit_chosen = player->make_trump(up_card, is_dealer, round, order_up_suit); 
  if(suit_chosen){
    cout << player -> get_name() << " orders up " << order_up_suit << endl;
    if(round == 1){
      dealer->add_and_discard(up_card);
    }
    cout << "\n";
    attackers.push_back(player);
    defenders.push_back(next_player(player));
    attackers.push_back(next_player(next_player(player)));
    defenders.push_back(next_player((next_player(player))));
  }
  else{
    cout << player->get_name() << " passes" << endl;
  }

  return suit_chosen;
}

void Game::make_trump(){ //Checked (tent..)
//order needs to be fixed
  // r1 & players that aren't dealer
    for(int index = 1; index < play_order.size(); index++){
      if(make_trump_one(play_order.at(index), false, 1)){
        return;
      }
    }

  // dealer
    if(make_trump_one(play_order.at(0), true, 1)){
      return;
    }
    // r2 & players that aren't dealer
    for(int index2 = 1; index2 < play_order.size(); index2++){
      if(make_trump_one(play_order.at(index2), false, 2)){
        return;
      }
    }
    // screw dealer
    if(make_trump_one(play_order.at(0), true, 2)){
      return;
    }

}

void Game::add_tricks(Player * player){ // Checked
  if(player == attackers.at(0) || player == attackers.at(1)){
    attacker_tricks++;
  }
  else{
    defender_tricks++;
  }
}

void Game::play_hand(){ //Checked(tent..)
  vector<Card> cards_played;
  Card led_card = play_order.at(1)->lead_card(order_up_suit); 
  cout << led_card << " led by " << play_order.at(1)->get_name() << endl;
  Card play_card1 = play_order.at(2)->play_card(led_card, order_up_suit);
  cout << play_card1 << " played by " << play_order.at(2)->get_name() << endl;
  Card play_card2 = play_order.at(3)->play_card(led_card, order_up_suit);
  cout << play_card2 << " played by " << play_order.at(3)->get_name() << endl;
  Card play_card3 = play_order.at(0)->play_card(led_card, order_up_suit);
  cout << play_card3 << " played by " << play_order.at(0)->get_name() << endl;

  cards_played.push_back(led_card);
  cards_played.push_back(play_card1);
  cards_played.push_back(play_card2);
  cards_played.push_back(play_card3);

  Card winning_card = led_card;
  for(int index = 1; index < cards_played.size(); index++){
    if(Card_less(winning_card, cards_played.at(index), led_card, order_up_suit)){
      winning_card = cards_played.at(index);
  }
  }
  
  if(winning_card == led_card){
    cout << play_order.at(1)->get_name() << " takes the trick" << "\n" << endl;
    add_tricks(play_order.at(1));
    shift_leader(play_order.at(1));
  }
  else if(winning_card == play_card1){
    cout << play_order.at(2)->get_name() << " takes the trick" << "\n" << endl;
    add_tricks(play_order.at(2));
    shift_leader(play_order.at(2));
  }
  else if(winning_card == play_card2){
    cout << play_order.at(3)->get_name() << " takes the trick" << "\n" << endl;
    add_tricks(play_order.at(3));
    shift_leader(play_order.at(3));
  }
  else if(winning_card == play_card3){
    cout << play_order.at(0)->get_name() << " takes the trick" << "\n" << endl;
    add_tricks(play_order.at(0));
    shift_leader(play_order.at(0));
  }
}

void Game::outcome(){
  int attacker_round_points = 0;
  int defender_round_points = 0;
  if(attacker_tricks > 3 && attacker_tricks < 5) {
    attacker_round_points = 1;
  }
  else if(attacker_tricks == 5) {
    attacker_round_points = 2;
    // cout << "march!" << endl;
  }
  else{
    defender_round_points = 2;
    // cout << "euchred" << endl;
  }
  
  if(attackers[0] == team_one[0] || attackers[0] == team_one[1] ){
    // cout << "condition a";
    team_one_points= team_one_points + attacker_round_points;
    team_two_points = team_two_points + defender_round_points;
    if(attacker_tricks> defender_tricks){
      cout << team_one.at(0)->get_name() 
  << " and " << (team_one.at(1))->get_name() << " win the hand" <<endl;
    }
    else{
      cout << team_two.at(0)->get_name() 
  << " and " << (team_two.at(1))->get_name() << " win the hand" <<endl;
    }
  }
  else if(attackers[0] == team_two[0] || attackers[0] == team_two[1]){
    // cout << "condition b";
    team_one_points = team_one_points + defender_round_points;
    team_two_points = team_two_points + attacker_round_points;
    if(attacker_tricks> defender_tricks){
      cout << team_two.at(0)->get_name() 
  << " and " << (team_two.at(1))->get_name() << " win the hand" << endl;
    }
    else{
      cout << team_one.at(0)->get_name() 
  << " and " << (team_one.at(1))->get_name() << " win the hand" << endl;
    }

  }
  else{
  }

  if(attacker_tricks == 5) {
    attacker_round_points = 2;
    cout << "march!" << endl;
  }
  else if(attacker_tricks <= 2){
    defender_round_points = 2;
    cout << "euchred!" << endl;
  }

  cout << (team_one.at(0))->get_name() 
  << " and " << (team_one.at(1))->get_name() 
  << " have " << team_one_points << " points" <<endl;

  cout << (team_two.at(0))->get_name() 
  << " and " << (team_two.at(1))->get_name() 
  << " have " << team_two_points << " points" << endl;

  cout << endl;
  

}
void Game::play(){
  // FOR SOME REASON BUGGING OUT BECAUSE OF TEST
  pack.reset();

  while(team_one_points < max_points && team_two_points < max_points){
    play_round();
    round_reset();

  }
  if(team_one_points >= max_points){
    cout << (team_one.at(0))->get_name() << " and " 
    << (team_one.at(1))->get_name() << " win!" << endl;
  }
  else if (team_two_points >= max_points) {
    cout << (team_two.at(0))->get_name() << " and " 
    << (team_two.at(1))->get_name() << " win!" << endl;
  }
  else{
    // cout << "error";
  }

  for (size_t i = 0; i < players.size(); ++i) {
  delete players[i];
  }

}

void Game::round_reset(){
  // vector<Player*> play_order = ;
  shift_dealer();
  attacker_tricks = 0;
  defender_tricks = 0;
  pack.reset(); // 
  attackers.pop_back();
  attackers.pop_back();
  defenders.pop_back();
  defenders.pop_back();

  // Card up_card; // how do I reset these
  // Suit order_up_suit; //how do I reset these
}

void Game::play_round(){ // need to rewrite this for the entire round
  int tricks = 0;
  cout << "Hand " << rounds << endl;
  shuffle();
  deal();
  set_upcard();
  make_trump();
  while(tricks < 5){
    play_hand(); // shift leader already built in 
    tricks++;
  }
  outcome();
  rounds++;
}

int main(int argc, char *argv[]) { // Have to write error conditions
  if(argc != 12){
      cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
     return 1;
    }
    if(string(argv[2]) != "shuffle" && string(argv[2]) != "noshuffle"){
      cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
     return 1;
    }
    if(atoi(argv[3]) < 1 || atoi(argv[3]) > 100){
      cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
     return 1;
    }

    if(string(argv[5]) != "Human" && string(argv[5]) != "Simple"){
      cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
     return 1;
    } 
    if(string(argv[7]) != "Human" && string(argv[7]) != "Simple"){
      cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
     return 1;
    }

    if(string(argv[9]) != "Human" && string(argv[9]) != "Simple"){
      cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
     return 1;
    } 
    if(string(argv[11]) != "Human" && string(argv[11]) != "Simple"){
      cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
     return 1;
    }

    ifstream fin(argv[1]);
    Pack temp(fin);
    if(!fin.is_open()){
        cout << "Error opening " << argv[1] << endl;
        return 1;
    }


  for(int arg = 0; arg < argc; arg++){
    cout << argv[arg] << " ";
  }
  cout << endl;

  Game game(argc, argv);
  game.play();
  // cout << "I got to here";
  
}
