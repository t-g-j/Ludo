#include "ludo_player_ga.h"
#include <random>

ludo_player_ga::ludo_player_ga():
    pos_start_of_turn(16),
    pos_end_of_turn(16),
    dice_roll(0)
{
}

vector<bool> ludo_player_ga::exploreBoard(){
    for(int player = 0; player<4;player++){//Looking through each players possible moves

    }
}

int ludo_player_ga::make_decision(){
   // exploreBoard();


    if(dice_roll == 6){
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]<0){
                return i;
            }
        }
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
                return i;
            }
        }
    } else {
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
                return i;
            }
        }
        for(int i = 0; i < 4; ++i){ //maybe they are all locked in
            if(pos_start_of_turn[i]<0){
                return i;
            }
        }
    }
    return -1;

}

void ludo_player_ga::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;

    int decision = make_decision();
    emit select_piece(decision);
}

void ludo_player_ga::post_game_analysis(std::vector<int> relative_pos){
    pos_end_of_turn = relative_pos;
    bool game_complete = true;
    for(int i = 0; i < 4; ++i){
        if(pos_end_of_turn[i] < 99){
            game_complete = false;
        }
    }
    emit turn_complete(game_complete);
}
