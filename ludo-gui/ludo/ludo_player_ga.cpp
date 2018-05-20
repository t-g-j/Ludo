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

vector<vector<bool>>  ludo_player_ga::exploreBoard(){
   // cout<<"explored board"<<endl;

    static int roundNr;                     // Counter to debug how the dices are moved around.
    roundNr++;
    cout<<"Round "<<roundNr<<endl;          // Debug purposes
    vector<int> MyPiecesPos;                   //container for my pieces
    vector<int> EnemyPiecesPos;                   //container for enemy pieces
    /**
       Initialize move matrix for each piece
    **/
    vector<vector<bool> >moves;
    vector<bool> tmp{false,false,false,false,false,false,false,false,false};      //Container for possible moves
    //vector<int> test{0,0,0,0,0,0,0,0};
    for( int pieces = 0;pieces < 4; pieces++){
        moves.push_back(tmp);
    }


    positions_and_dice relative;
    pos_start_of_turn = relative.pos;       //Getting all the positions of players
    /**
       Looking through each players piece position
    **/
    for(int pieces = 0; pieces < 16; pieces++){
        if(pieces < 4 ){
        MyPiecesPos.push_back(pos_start_of_turn[pieces]);
        cout<<"piece "<<pieces<<" pos: "<<MyPiecesPos[pieces]<<endl;
        }

        else{
            EnemyPiecesPos.push_back(pos_start_of_turn[pieces]);
        }
    }

    /**
      looking at board
      **/

    Genome myPlayer; // not used right now
    //cout<<myPlayer.Chromosomes.size()<<endl; // not used right now

    for(int pieces = 0; pieces <4 ; pieces++){  // Lookign through each players possible moves

        int index = MyPiecesPos[pieces] + dice_roll; // easy reference for each piece

        if(MyPiecesPos[pieces] == home && dice_roll == 6){        // EnterBoard
            moves[pieces][0]= true;
            cout<<"piece "<<pieces<<" enters board"<<endl;

        }

        if(MyPiecesPos[pieces]+dice_roll == goalStretch ){      // Move to safe zone/ goal stretch
            moves[pieces][1]= true;
        }

        if(MyPiecesPos[pieces] + dice_roll == 5 ){              // SendEnemyHome
            moves[pieces][2]= true;
        }

        if(MyPiecesPos[pieces] +dice_roll == (MyPiecesPos[pieces+1]
                                              || MyPiecesPos[pieces-1] )  ){// Block with friend
            moves[pieces][3]= true;
        }

        if(MyPiecesPos[pieces]+dice_roll< 52 &&
                MyPiecesPos[pieces]+dice_roll !=-1  ){              // moveNormal forward, Tjek der ikke står 2 brikker
            moves[pieces][4]= true;
        }

        if( index == 5  ||
            index == 18 ||
            index == 31 ||
            index == 44 ||
            index == 11 ||
            index == 24 ||
            index == 37 ||
                index == 50 ){
             moves[pieces][5]= true;
        }

        // Move onto globe - using index instead of Piece vector container
        if(index < 52){     //check only the indexes on the board, not in the home streatch
            if(index % 13 == 0 || (index - 8) % 13 == 0){  //doesn't check for more people at same spot
                moves[pieces][6]= true;
            }
        }



        if(MyPiecesPos[pieces] > 51  ){              // Move in Safe zone
            moves[pieces][7]= true;
        }

        if(MyPiecesPos[pieces] > 51 && MyPiecesPos[pieces]+dice_roll ==56 ){              // Finish piece
            moves[pieces][8]= true;
        }

    //cout<<"exit for loop"<<endl;
    }



    // Goal = 99
    // Home = -1
    // First in goal strech = 51

    // 57 is offboard


    //cout<<"******** "<<dice_roll<<" *******"<<endl;
    if(dice_roll == 6){
        //cout<<"enter dice roll"<<endl;
        moves[0][0]=true;
        cout<<"******** dice = 6 *******"<<endl;
    }

    /**
      Print move matrix
      **/

    for( int pieces = 0;pieces < 4; pieces++){

        for(int aMove = 0; aMove<9; aMove ++){

            cout<<moves[pieces][aMove]<<" ";
        }
        cout<<endl;
    }
    /*
    for(int i = 0; i<moves.size();i++){
        cout<<moves[i];
    }
    cout<<endl;
    */
    int move_counter = 0;
    for(int i = 0; i<4; i++){
       // cout<<"enter move counter"<<endl;
        //cout<<moves[i]<<endl;
        if(moves[i][0]== true){
         //   move_counter++;
        }
    //cout<<"exit move counter"<<endl;
    }
   //cout<<"Moves Possible: "<<move_counter<<endl;
   move_counter = 0;
    //cout<<"returning"<<endl;
    return moves;
   //return tmp;
}

int ludo_player_ga::make_decision(){
    vector<vector<bool>>myMoves;
    positions_and_dice piece;
    //piecePos = piece.pos;
    //cout<<"piece 1 " <<endl;

    myMoves = exploreBoard();
   // cout<<"made decesion"<<endl;



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
