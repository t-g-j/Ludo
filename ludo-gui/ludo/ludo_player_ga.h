#ifndef LUDO_PLAYER_GA_H
#define LUDO_PLAYER_GA_H
#include <QObject>
#include <iostream>
#include "positions_and_dice.h"

#include <cstdlib>
#include <vector>
#include "random"
#include "string"

#include "game.h"


#define ChromosomePool 50
#define chromosimeLifeTime 20
#define Training true
#define TOTALGAMES 5000

using namespace std;

struct Chromosomes{
    // Adds a many characteristics as thinkeble for the ludo player.
        float enterBoard = 0.8;
        float moveToSafeZone = 0.1;
        float sendEnemyHome = 0.1;
        float block = 0.1;
        float moveNormal = 0.1;
        float move2Star = 0.1;
        float move2Globe = 0.1;
        float moveInSafeZone = 0.1;
        float finishPiece = 0.1;

        // Container for my chromosomes
        vector<float> Weights {enterBoard,moveToSafeZone,sendEnemyHome,block,
                    moveNormal,move2Star,move2Globe,moveInSafeZone,finishPiece};
        // Container for my fitness score
        float fitnessScore;
};
struct Perfect_GA_Player{
    // Adds a many characteristics as thinkeble for the ludo player.
        float enterBoard = 0.8;
        float moveToSafeZone = 0.1;
        float sendEnemyHome = 0.1;
        float block = 0.1;
        float moveNormal = 0.1;
        float move2Star = 0.1;
        float move2Globe = 0.1;
        float moveInSafeZone = 0.1;
        float finishPiece = 0.1;

        // Container for my chromosomes
        vector<float> Weights {enterBoard,moveToSafeZone,sendEnemyHome,block,
                    moveNormal,move2Star,move2Globe,moveInSafeZone,finishPiece};
        // Container for my fitness score
        float fitnessScore;
};

//class ludo_player_ga : public QObject {
class ludo_player_ga : public QObject {
    Q_OBJECT
private:
    std::vector<int> pos_start_of_turn;
    std::vector<int> pos_end_of_turn;



    int dice_roll;
    /*** GA functions **/
    int make_decision();
    void mutateGenes(double influence);
    void makeChildren(vector<float>child1,vector<float>child2);
    int calculateScores(vector<vector<bool> >availibleMoves, vector<float> Chromosome);
    vector<Chromosomes> initialize_random_genes(vector<Chromosomes>SomeGenesToRandomize);
    void initialize_population();
    void printGeneration();
    void test();
    void printMoves();
    void statistics();



    void sortFitness();
    /*** GA variables **/
    bool init_flag = true;     // Global flag for only initialize my chromosomes once.
    vector<Chromosomes> generation;  //Container for each generation - size = population
    vector<bool> possibleMoves;
    vector<float>myWinners = {0,0,0,0};
    vector<int> SortedfitnessScore;

    long long int roundNr;
    int chromosomeCounterGlobal = 0;

    /* Statistic variables */
    int totalGenWins;
    int generationCounter;
    vector<float>biggest_win;
    vector<int> GenerationWins;
    vector<vector<float> > biggest_win_Weights;


    int myTotalGames;

    /* constants */
    const int goalStretch = 51;
    const int goal = 99;
    const int GOAL = 56;
    const int home = -1;

    // For making statistics about the moves chosen
    long int enterBoardCounter = 0;
    long int moveToSafeZoneCounter = 0;
    long int sendEnemyHomeCounter = 0;
    long int blockCounter = 0;
    long int moveNormalCounter = 0;
    long int move2StarCounter = 0;
    long int move2GlobeCounter = 0;
    long int moveInSafeZoneCounter = 0;
    long int finishPieceCounter = 0;

public:
    ludo_player_ga();
    /*** GA functions & variables **/
    vector<vector<bool>> exploreBoard(); // Explore the board
    Chromosomes myChromosomes;

signals:
    void select_piece(int);
    void turn_complete(bool);


public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
    //void end_game(int winner);
    void updateFitnessScore(int winner);

};



template<class bidiiter>
bidiiter random_unique(bidiiter begin, bidiiter end, size_t num_random) {
    size_t left = std::distance(begin, end);
    while (num_random--) {
        bidiiter r = begin;
        std::advance(r, rand()%left);
        std::swap(*begin, *r);
        ++begin;
        --left;
    }
    return begin;
}
    // Det er lånt fra https://ideone.com/fork/3A3cv

struct less_than_key
{
    inline bool operator() (const Chromosomes& struct1, const Chromosomes& struct2)
    {
        return (struct1.fitnessScore < struct2.fitnessScore);
    }
};
    // https://stackoverflow.com/questions/1380463/sorting-a-vector-of-custom-objects#1380496
#endif // LUDO_PLAYER_GA_H
