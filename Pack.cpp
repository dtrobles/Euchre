#include "Pack.hpp"
#include <cassert>
#include <string>

using namespace std;

  // EFFECTS: Initializes the Pack to be in the following standard order:
  //          the cards of the lowest suit arranged from lowest rank to
  //          highest rank, followed by the cards of the next lowest suit
  //          in order from lowest to highest rank, and so on. 
  // NOTE: The standard order is the same as that in pack.in.
  // NOTE: Do NOT use pack.in in your implementation of this function
  // NOTE: The pack is initially full, with no cards dealt.

  //////////////////
  Pack::Pack(){ //Checked
    int index = 0;
    next = 0;
    for (int s = SPADES; s <= DIAMONDS; ++s) {
        for (int r = NINE; r <= ACE; ++r) {
            if(index >= 24){
                assert(false); // Error condition
            }
            Rank rank = static_cast<Rank>(r);
            Suit suit = static_cast<Suit>(s);
             Card card(rank, suit);
             cards[index] = card; 
             index++;
        }
    }

  }
  // REQUIRES: pack_input contains a representation of a Pack in the
  //           format required by the project specification
  // MODIFIES: pack_input
  // EFFECTS: Initializes Pack by reading from pack_input.
  // NOTE: The pack is initially full, with no cards dealt.
 
  
    Pack::Pack(std::istream& pack_input){ //Checked
        string input;
        int index = 0;
        next = 0;
        Card card;
        while(pack_input >> card){
           cards[index] = card;
           index++;
        }
    }




  // REQUIRES: cards remain in the Pack
  // EFFECTS: Returns the next card in the pack and increments the next index
    Card Pack::deal_one(){ //Checked
        Card new_card = cards[next];
        next = next + 1;
        return new_card;
    }

  // EFFECTS: Resets next index to first card in the Pack
    void Pack::reset() { //Checked
        next = 0;
    }


  // EFFECTS: Shuffles the Pack and resets the next index. This
  //          performs an in shuffle seven times. See
  //          https://en.wikipedia.org/wiki/In_shuffle.
 
  //////////////////
  // dylan was here; some changes
  void Pack::shuffle(){ // Checked
    next = 0;
    int number_of_shuffles = 0;

    while(number_of_shuffles < 7){
        array<Card, 12> half_one;
        array<Card, 12> half_two;
        for(int i=0; i < 12; i++)
        {
          half_one[i] = cards[i];
          half_two[i] = cards[12+i];
        }
        for(int i=0; i < 24; i++)
        {
            if(i%2 == 0){
             cards[i] = half_two[i / 2];
            }
            else{
             cards[i] = half_one[i / 2];
            }
        }
        number_of_shuffles = number_of_shuffles + 1;
    }


  }

  // EFFECTS: returns true if there are no more cards left in the pack
  bool Pack::empty() const{ //Checked
    if(next > 23){
        return true;
    }
    else{
        return false;
    }
  }