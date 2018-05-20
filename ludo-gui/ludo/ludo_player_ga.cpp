#include "ludo_player_ga.h"
#include <random>
// Goal = 99
// Home = -1
// First in goal strech = 51

// 57 is offboard
ludo_player_ga::ludo_player_ga():
    pos_start_of_turn(16),
    pos_end_of_turn(16),
    dice_roll(0),
    rd(),   // Delete me
    gen(rd()) // Delete me
{
 //   cout<<"init"<<endl;
    //Genome->myChromosomes;
    //initialize_random_genes(myChromosomes.Chromosomes);
}
vector<float> ludo_player_ga::initialize_random_genes(vector<float> SomeGenesToRandomize){
    float tmp;
    for(int i = 0; i < SomeGenesToRandomize.size(); i++){
        tmp = rand() % 10 + 1;
        SomeGenesToRandomize[i] = tmp/100;

        cout<<"Chromosome "<<i<<" has weight of "<< SomeGenesToRandomize[i]<<endl;
    }
    SomeGenesToRandomize[0] +=0.9;
    return SomeGenesToRandomize;
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
        if(MyPiecesPos[pieces] != 99){
            //cout<<"loop pieces"<<endl;
            int index = MyPiecesPos[pieces] + dice_roll; // easy reference for each piece
            // EnterBoard
            if(MyPiecesPos[pieces] == home && dice_roll == 6 && MyPiecesPos[pieces]!=goal){
                moves[pieces][0]= true;
                //cout<<"piece "<<pieces<<" enters board"<<endl;
            }
            // Move to safe zone/ goal stretch
            if(MyPiecesPos[pieces]+dice_roll == goalStretch ){
                moves[pieces][1]= true;
            }
            // SendEnemyHome
            for( int enemyPiece = 0; enemyPiece < EnemyPiecesPos.size(); enemyPiece++)
                if(MyPiecesPos[pieces] + dice_roll == EnemyPiecesPos[enemyPiece] ){
                    moves[pieces][2]= true;
                }
            // Block with friend
            if( ( MyPiecesPos[pieces] +dice_roll == (MyPiecesPos[pieces+1]
                                                  || MyPiecesPos[pieces-1] )  )
                    && MyPiecesPos[pieces] !=-1){
                moves[pieces][3]= true;
            }
            // moveNormal forward, !! Tjek der ikke står 2 brikker !!
            if(MyPiecesPos[pieces]+dice_roll< 52 &&
                    MyPiecesPos[pieces] !=-1  ){
                moves[pieces][4]= true;
            }
            // Move onto a star if possible
            if( (index == 5  ||
                index == 18 ||
                index == 31 ||
                index == 44 ||
                index == 11 ||
                index == 24 ||
                index == 37 ||
                    index == 50 ) && MyPiecesPos[pieces] !=-1){
                 moves[pieces][5]= true;
            }
            // Move onto globe - using index instead of Piece vector container
            if(index < 52 && MyPiecesPos[pieces] !=-1){     //check only the indexes on the board, not in the home streatch
                if(index % 13 == 0 || (index - 8) % 13 == 0){  //doesn't check for more people at same spot
                    moves[pieces][6]= true;
                }
            }
            // Move in Safe zone
            if(MyPiecesPos[pieces] > 51  ){
                moves[pieces][7]= true;
            }
            // Finish piece
            if(MyPiecesPos[pieces] > 51 && MyPiecesPos[pieces]+dice_roll ==56 ){
                moves[pieces][8]= true;
            }
        }
    }


    //cout<<"******** "<<dice_roll<<" *******"<<endl;
    if(dice_roll == 6){
        //cout<<"enter dice roll"<<endl;
        //moves[0][0]=true;
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
int ludo_player_ga::calculateScores(vector<vector<bool> > availibleMoves, vector<float> Chromosome){
    double piece1Fitness = 0;
    double piece2Fitness = 0;
    double piece3Fitness = 0;
    double piece4Fitness = 0;
    vector<float> container;
    int current = 0;
    int prev = 0;
    int choosenPiece = 0;
    //for(int pieces = 0; pieces < 4; pieces++){ // Loop through pieces
        // Det her virker ikke som det skal. jeg skal kigge på brikken lidt bedre.
        for(int move = 0; move < Chromosome.size() ; move ++){ // Loop through availeble moves
            // Multiply by 1 so weights can be changed appropiately i Chromosones
            if(availibleMoves[0][move] == true){
                piece1Fitness+=1* Chromosome[move];
            }
            if(availibleMoves[1][move] == true){
                piece2Fitness+=1* Chromosome[move];
            }
            if(availibleMoves[2][move] == true){
                piece3Fitness+=1* Chromosome[move];
            }
            if(availibleMoves[3][move] == true){
                piece4Fitness+=1* Chromosome[move];
            }
        }
    //}
    container.push_back(piece1Fitness);
    container.push_back(piece2Fitness);
    container.push_back(piece3Fitness);
    container.push_back(piece4Fitness);
    /* Going thorugh the different fitness scores to see which is the biggest and then choose that one */
    for(int i = 0; i<container.size();i++){
        cout<<"Piece "<<i<<" scores "<<container[i]<<endl;
        /*
        current = container[i];
        choosenPiece = i;
        if(i >= 1){
            prev = container[i-1];
            if(current > prev){
                choosenPiece = i;
            }
        }*/
    }
    auto biggest = std::max_element(std::begin(container), std::end(container));
        //std::cout << "Max element is " << *biggest;

    choosenPiece = std::distance(std::begin(container), biggest) ;
    cout<<"chose "<<choosenPiece<<endl;
    return choosenPiece;




}

int ludo_player_ga::make_decision(){
    vector<vector<bool>>myMoves;
    //Genome myPlayer;
    static vector<float> Chromosomes;
    int choice;
    // DEBUGGING STUFF
    //positions_and_dice piece;
    //piecePos = piece.pos;
    //cout<<"piece 1 " <<endl;
    if(init_flag == true){
        Chromosomes = initialize_random_genes(myChromosomes.Chromosomes);
        init_flag = false;
        }

    myMoves = exploreBoard();

    //cout<<Chromosomes[8]<<endl;

    choice = calculateScores(myMoves,Chromosomes);
   // cout<<"made decesion"<<endl;


    /*
    std::vector<int> valid_moves;
    if(dice_roll == 6){
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]<0){
                valid_moves.push_back(i);
            }
        }
    }
    for(int i = 0; i < 4; ++i){
        if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
            valid_moves.push_back(i);
        }
    }
    if(valid_moves.size()==0){
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i] != 99){
                valid_moves.push_back(i);
            }
        }
    }
    std::uniform_int_distribution<> piece(0, valid_moves.size()-1);
    int select = piece(gen);
    return valid_moves[select];
    */
    return choice;

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
