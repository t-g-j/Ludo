#include "ludo_player_ga.h"
#include <random>
// Goal = 99
// Home = -1
// First in goal strech = 51

// 57 is offboard
ludo_player_ga::ludo_player_ga():
    pos_start_of_turn(16),
    pos_end_of_turn(16),
    dice_roll(0)
{
}

vector<bool> ludo_player_ga::exploreBoard(){
    //cout<<"explored board"<<endl;
    static int roundNr;                     // Counter to debug how the dices are moved around.
    roundNr++;
    cout<<"Round "<<roundNr<<endl;          // Debug purposes

    vector<int> MyPiecesPos;                   //container for my pieces
    vector<bool>moves{false,false,false,false,false,false,false,false};                      //Container for possible moves

    positions_and_dice relative;
    pos_start_of_turn = relative.pos;       //Getting all the positions of players
    /**
       Looking through each players piece position
    **/
    for(int pieces = 0; pieces < 4 ; pieces++){
        MyPiecesPos.push_back(pos_start_of_turn[pieces]);
        cout<<"piece "<<pieces<<" pos: "<<MyPiecesPos[pieces]<<endl;
    }

    /**
      looking at board
      **/

    Genome myPlayer; // not used right now
    //cout<<myPlayer.Chromosomes.size()<<endl; // not used right now

    for(int pieces = 0; pieces <4 ; pieces++){  // Lookign through each players possible moves
/*
        if(MyPiecesPos[pieces] == -1 ){              // EnterBoard
            moves[0]= true;
        }
        if(MyPiecesPos[pieces] == -1 ){              // Move Home
            moves[1]= true;
        }
        if(MyPiecesPos[pieces] == -1 ){              // SendEnemyHome
            moves[2]= true;
        }
        if(MyPiecesPos[pieces] == -1 ){              // Block with friend
            moves[3]= true;
        }
        if(MyPiecesPos[pieces] == -1 ){              // moveNormal forward
            moves[4]= true;
        }
        if(MyPiecesPos[pieces] == -1 ){              // Move to next star
            moves[5]= true;
        }
        if(MyPiecesPos[pieces] == -1 ){              // Move to next globe
            moves[6]= true;
        }
        if(MyPiecesPos[pieces] == -1 ){              // Move in Safe zone
            moves[7]= true;
        }*/
    }



    // Goal = 99
    // Home = -1
    // First in goal strech = 51

    // 57 is offboard


    //cout<<"******** "<<dice_roll<<" *******"<<endl;
    if(dice_roll == 6){
        cout<<"******** dice = 6 *******"<<endl;
        moves[0]=true;
    }
    /*
    for(int i = 0; i<moves.size();i++){
        cout<<moves[i];
    }
    cout<<endl;
    */
    int move_counter = 0;

    for(int i = 0; i<myPlayer.Chromosomes.size(); i++){
        //cout<<moves[i]<<endl;
        if(moves[i]== true){


            move_counter++;
        }

    }
   cout<<"Moves Possible: "<<move_counter<<endl;
   move_counter = 0;

    return moves;
}

int ludo_player_ga::make_decision(){
    vector<bool>myMoves;
    positions_and_dice piece;
    //piecePos = piece.pos;
    //cout<<"piece 1 " <<endl;

    myMoves = exploreBoard();
    //cout<<"made decesion"<<endl;



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
    /*
    cout<<"pos_start_turn[0]"<<pos_start_of_turn[0]<<endl;
    cout<<"pos_start_turn[1]"<<pos_start_of_turn[1]<<endl;
    cout<<"pos_start_turn[2]"<<pos_start_of_turn[2]<<endl;
    cout<<"pos_start_turn[3]"<<pos_start_of_turn[3]<<endl;
    cout<<"amount of players"<<pos_start_of_turn.size()<<endl;
    */
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
