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
 //   cout<<"init"<<endl;
    //Genome->myChromosomes;
    //initialize_random_genes(myChromosomes.Chromosomes);
}
vector<Chromosomes> ludo_player_ga::initialize_random_genes(vector<Chromosomes> PopulationContainer){
    //cout<<"here"<<endl;
    vector<float>SomeGenesToRandomize; // tmp vector for randommizing gene pool
    float tmp;                          // tmp variable for each chromosome.
    for(int pop = 0; pop < ChromosomePool; pop++){
        //SomeGenesToRandomize = PopulationContainer[pop].Chromosomes;
        //cout<<PopulationContainer[pop].Chromosomes[0]<<endl;
        for(int i = 0; i < PopulationContainer[pop].Weights.size(); i++){
            //cout<<"still here"<<endl;
            tmp = rand() % 10 + 1;
            //cout<<"rand "<<tmp/1000<<endl;
            PopulationContainer[pop].Weights[i] = tmp/10;

            //cout<<"Chromosome "<<i<<" has weight of "<< PopulationContainer[pop].Chromosomes[i]<<endl;
        }
    //SomeGenesToRandomize[0] +=0.9;
    }
    return PopulationContainer;
}
void ludo_player_ga::initialize_population(){
    Chromosomes myGen;
    for(int pop = 0; pop <ChromosomePool; pop++){
        generation.push_back(myGen);
    }
}
vector<vector<bool>>  ludo_player_ga::exploreBoard(){
   // cout<<"explored board"<<endl;

    //static int roundNr;                     // Counter to debug how the dices are moved around.
    //roundNr++;
    //cout<<"Round "<<roundNr<<endl;          // Debug purposes
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
        //cout<<"piece "<<pieces<<" pos: "<<MyPiecesPos[pieces]<<endl;
        }

        else{
            EnemyPiecesPos.push_back(pos_start_of_turn[pieces]);
        }
    }

    /*****************************
      looking at board
      *****************************/

    Chromosomes myPlayer; // not used right now
    //cout<<myPlayer.Chromosomes.size()<<endl; // not used right now

    for(int pieces = 0; pieces <4 ; pieces++){  // Lookign through each players possible moves
        if(MyPiecesPos[pieces] != 99){
            //cout<<"loop pieces"<<endl;
            int index = MyPiecesPos[pieces] + dice_roll; // easy reference for each piece
            // EnterBoard
            if(MyPiecesPos[pieces] == home && dice_roll == 6 && MyPiecesPos[pieces]!=goal){
                moves[pieces][0]= true;
                enterBoardCounter+=1;
                //cout<<"piece "<<pieces<<" enters board"<<endl;
            }
            // Move to safe zone/ goal stretch
            if(MyPiecesPos[pieces]+dice_roll == goalStretch ){
                moves[pieces][1]= true;
                moveToSafeZoneCounter+=1;
            }
            // SendEnemyHome
            for( int enemyPiece = 0; enemyPiece < EnemyPiecesPos.size(); enemyPiece++)
                if(MyPiecesPos[pieces] + dice_roll == EnemyPiecesPos[enemyPiece] ){
                    moves[pieces][2]= true;
                    sendEnemyHomeCounter+=1;
                }
            // Block with friend
            if( ( MyPiecesPos[pieces] +dice_roll == (MyPiecesPos[pieces+1]
                                                  || MyPiecesPos[pieces-1] )  )
                    && MyPiecesPos[pieces] !=-1){
                moves[pieces][3]= true;
                blockCounter+=1;
            }
            // moveNormal forward, !! Tjek der ikke står 2 brikker !!
            if(MyPiecesPos[pieces]+dice_roll< 52 &&
                    MyPiecesPos[pieces] !=-1  ){
                moves[pieces][4]= true;
                moveNormalCounter+=1;
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
                 move2StarCounter+=1;
            }
            // Move onto globe - using index instead of Piece vector container
            if(index < 52 && MyPiecesPos[pieces] !=-1){     //check only the indexes on the board, not in the home streatch
                if(index % 13 == 0 || (index - 8) % 13 == 0){  //doesn't check for more people at same spot
                    moves[pieces][6]= true;
                    move2GlobeCounter+=1;
                }
            }
            // Move in Safe zone
            if(MyPiecesPos[pieces] >= 51  ){
                moves[pieces][7]= true;
                moveInSafeZoneCounter+=1;
            }
            // Finish piece
            if(MyPiecesPos[pieces] > 51 && MyPiecesPos[pieces]+dice_roll ==56 ){
                moves[pieces][8]= true;
                finishPieceCounter+=1;
            }
        }
    }


    //cout<<"******** "<<dice_roll<<" *******"<<endl;
    //if(dice_roll == 6){
        //cout<<"enter dice roll"<<endl;
        //moves[0][0]=true;
       // cout<<"******** dice = 6 *******"<<endl;
    //}

    /****************************************************************

    Print move matrix

      ***************************************************************/
/*
    for( int pieces = 0;pieces < 4; pieces++){

        for(int aMove = 0; aMove<9; aMove ++){

            cout<<moves[pieces][aMove]<<" ";
        }
        cout<<endl;
    }
    */

    /*
    for(int i = 0; i<moves.size();i++){
        cout<<moves[i];
    }
    cout<<endl;
    */


    return moves;
   //return tmp;
}
int ludo_player_ga::calculateScores(vector<vector<bool> > availibleMoves, vector<float> Weights){
    double piece1Fitness = 0;
    double piece2Fitness = 0;
    double piece3Fitness = 0;
    double piece4Fitness = 0;
    vector<float> container;
    int choosenPiece = 0;
    //for(int pieces = 0; pieces < 4; pieces++){ // Loop through pieces
        // Det her virker ikke som det skal. jeg skal kigge på brikken lidt bedre.
        for(int move = 0; move < Weights.size() ; move ++){ // Loop through availeble moves
            // Multiply by 1 so weights can be changed appropiately i Chromosones
            if(availibleMoves[0][move] == true){
                piece1Fitness+=1 * Weights[move];
            }
            if(availibleMoves[1][move] == true){
                piece2Fitness+=1 * Weights[move];
            }
            if(availibleMoves[2][move] == true){
                piece3Fitness+=1 * Weights[move];
            }
            if(availibleMoves[3][move] == true){
                piece4Fitness+=1 * Weights[move];
            }
        }
    //}
    container.push_back(piece1Fitness);
    container.push_back(piece2Fitness);
    container.push_back(piece3Fitness);
    container.push_back(piece4Fitness);
    /* Going thorugh the different fitness scores to see which is the biggest and then choose that one */
    for(int i = 0; i<container.size();i++){
        //cout<<"Piece "<<i<<" scores "<<container[i]<<endl;
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
   // cout<<"chose "<<choosenPiece<<endl;
    return choosenPiece;

}

int ludo_player_ga::make_decision(){

    vector<vector<bool>>myMoves;
    static vector<float> Weights;
    int choice;
    // DEBUGGING STUFF
    //positions_and_dice piece;
    //piecePos = piece.pos;
    //cout<<"piece 1 " <<endl;

    if(init_flag == true){
        initialize_population();
        generation = initialize_random_genes(generation);

        for(int pop = 0; pop < ChromosomePool; pop++){
            cout<<"Chromosome "<< pop<< endl;
            for(int chromo = 0 ; chromo < generation[pop].Weights.size() ;  chromo++){
                cout<<"Gene "<<chromo<<" has weight of "<<
                   generation[pop].Weights[chromo]<<endl;
            }
        }
        init_flag = false;
        }

    myMoves = exploreBoard();

    if(Training !=true){
        Perfect_GA_Player myPlayer;
        choice = calculateScores(myMoves,myPlayer.Weights);
    }else{
    choice = calculateScores(myMoves,generation[chromosomeCounterGlobal].Weights);
    }

    //cout<<"Returning choice for decesion"<<endl;
    //child1 = generation[0].Weights;
    //child2 = generation[1].Weights;
    //makeChildren(child1,child2);
    //cout<<"made children"<<endl;
    //printGeneration();
    //mutateGenes(0.02);
    //printGeneration();
   // cout<<"made decesion"<<endl;


    return choice;

}


void ludo_player_ga::printGeneration(){
    for(int chromo = 0; chromo < ChromosomePool; chromo ++){
        cout<<"Chromosone: "<<chromo<<endl;
        for(int gene = 0; gene<generation[chromo].Weights.size(); gene++){
            cout<<"\thas gene weights :"<<generation[chromo].Weights[gene]<<endl;
        }
        cout<<endl;
    }
}

void ludo_player_ga::mutateGenes(double influenceFactor){
    int mutatees = ChromosomePool * 0.7;
    double randNumber;
    double giveOrTake;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); // Get a "random" seed

    shuffle (generation.begin(), generation.end(), std::default_random_engine(seed)); // Shuffle chromosomes
       for(int i=0; i<mutatees; ++i) {      // Loop through the 70% chromosomes that should be shuffled
           for(int gene = 0; gene < generation[i].Weights.size(); gene++){ //loop through their genes
               std::default_random_engine generator;
               std::normal_distribution<double> distribution(generation[i].Weights[gene],influenceFactor);
               double number = distribution(generator);
               //randNumber = ((double) rand() / (RAND_MAX));    // Random number between 0 & 1
                //if(randNumber >= 0.5){
               generation[i].Weights[gene]+= number; // add mutation to gene
               /*
                    giveOrTake = ((double) rand() / (RAND_MAX)); // Decide to add or subtrackt mutation
                    if(giveOrTake >= 0.5){ //add a little something to gene
                        double tmp = ((double) rand() / (RAND_MAX));
                        //generation[i].Weights[gene]+= tmp/10; // add mutation to gene

                    }
                    else{                   // retract a little something from gene
                        double tmp = ((double) rand() / (RAND_MAX));
                        generation[i].Weights[gene]-= tmp/10; // subtract mutation from gene
                  //  }
                }
                    */
           }
       }
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
void ludo_player_ga::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;

    dice_roll = relative.dice;

    int decision = make_decision();
    emit select_piece(decision);
}


void ludo_player_ga::updateFitnessScore(int winner){
    static int gameChromoCounter = 0;
    static int chromosomeCounter;

    static long long int totalGames = 0;
    static int gene= 0;
    int tmp = winner;
    //cout<<" chromosomeCounter\t"<<chromosomeCounter<<endl;
    //cout<<" chromosomeCounteGlobal\t"<<chromosomeCounterGlobal<<endl;
    /* Winner statistics*/

    if(tmp == 0){
        myWinners[0]+=1;
    }

    if(tmp == 1){
        myWinners[1]+=1;
    }
    if(tmp == 2){
        myWinners[2]+=1;
    }
    if(tmp ==3){
        myWinners[3]+=1;
    }

    roundNr = 0;

    if(gameChromoCounter == chromosimeLifeTime-1){
        //cout<<"HERE again"<<endl;
        generation[chromosomeCounter].fitnessScore= myWinners[0] / (chromosimeLifeTime);
        cout<<"i've won "<<myWinners[0]<<" out of total "<<gameChromoCounter<<endl<<endl;

        //cout<<"delete me"<<myWinners[0]; //(gameCounter+1)<<endl;
        //cout<<"PLAYED 10 GAMES"<<endl;
        //cout<<"Fitness score "<<generation[chromosomeCounter].fitnessScore<<endl;

        chromosomeCounter+=1;

        if(chromosomeCounter<ChromosomePool){
            myWinners[0] = 0;
        }
        cout<<endl<<"Game nr: "<<totalGames<<endl;
        cout<<"TRYING Chromosome NR. "<<chromosomeCounter<<endl;
        if(chromosomeCounter == ChromosomePool-1){
            generation[chromosomeCounter].fitnessScore= myWinners[0] / (chromosimeLifeTime);
            myWinners[0] = 0;



            //cout<<endl<<"fitness score in IF "<<generation[chromosomeCounter].fitnessScore;
            //cout<<"counter"<<chromosomeCounter<<endl;

            cout<<"Generation: "<<gene<<endl;

            sortFitness();
            cout<<""<<endl;

            for(int chromo = 0; chromo < ChromosomePool ;chromo++){
                cout<<"fitness after sort "<<generation[chromo].fitnessScore<<endl;
            }

            /*
            cout<<"after sort min "<<generation[0].fitnessScore<<endl;
            cout<<"after sort max "<<generation[ChromosomePool-1].fitnessScore<<endl;
            */
            cout<<""<<endl;

            vector<float>parent1 = generation[ChromosomePool-1].Weights;
            vector<float>parent2 = generation[ChromosomePool-2].Weights;
            makeChildren(parent1,parent2);
            mutateGenes(0.2);
            //printGeneration();
            chromosomeCounter = 0;
            //printMoves();
            gene+=1;

        }
        //cout<<"a game has ended \t"<<gameCounter+1<<endl;
        gameChromoCounter=0;
    }
    gameChromoCounter+=1;
    //cout<<"a game has ended \t"<<gameCounter<<endl;
    //cout<<"winner"<<winner<<endl;
    chromosomeCounterGlobal = chromosomeCounter;
    totalGames+=1;
    if(totalGames==19999){
        printGeneration();
        cout<<"Chromosome "<<gameChromoCounter<<"\tFitness"<<endl;
        for(int chromo = 0; chromo < ChromosomePool; chromo++){
            cout<<"\t"<<generation[chromo].fitnessScore<<endl;
        }
    }

}
void ludo_player_ga::makeChildren(vector<float>parent1,vector<float>parent2){
        //generation.clear();
        float newGenWeight;
        double randNumber;
    //take two best childen and scramble their genes into a new ChromosomePool
        for(int chromo = 0; chromo <ChromosomePool; chromo++){
            generation[chromo].Weights.clear();
            for(int gene = 0; gene <parent1.size(); gene ++){
                randNumber = ((double) rand() / (RAND_MAX));
                //cout<<randNumber<<" ";
                if(randNumber >0.5 ){   // Randomly chose parent 1
                    newGenWeight = parent1[gene] ;//random new gene from a parent
                    }
                else{                   // Randomly chose parent 2
                    newGenWeight = parent2[gene];//random new gene from a parent
                }
              generation[chromo].Weights.push_back(newGenWeight);
            }
        }
        //printGeneration();

}
void ludo_player_ga::sortFitness(){
    sort(generation.begin(),generation.end(),less_than_key() );
}
void ludo_player_ga::printMoves(){
    cout<<"Entered Board\t"<<enterBoardCounter << endl;
    cout<<"MoveTosafeZone\t"<< moveToSafeZoneCounter<< endl;
    cout<<"sendEnemyHome\t"<< sendEnemyHomeCounter<< endl;
    cout<<"block\t\t"<< blockCounter<< endl;
    cout<<"moveNormal\t\t"<< moveNormalCounter<< endl;
    cout<<"move2Star\t\t"<< move2StarCounter<< endl;
    cout<<"move2Globe\t\t"<< move2GlobeCounter<< endl;
    cout<<"moveInSafe\t\t"<< moveInSafeZoneCounter<< endl;
    cout<<"FinishPiece\t\t"<< finishPieceCounter<< endl;
}
