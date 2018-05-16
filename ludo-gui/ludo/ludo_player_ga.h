#ifndef LUDO_PLAYER_GA_H
#define LUDO_PLAYER_GA_H
#include <QObject>
#include <iostream>
#include "positions_and_dice.h"
#include <vector>


using namespace std;
class ludo_player_ga : public QObject {
    Q_OBJECT
private:
    std::vector<int> pos_start_of_turn;
    std::vector<int> pos_end_of_turn;
    vector<bool> possibleMoves;
    int dice_roll;
    int make_decision();
public:
    ludo_player_ga();
    vector<bool> exploreBoard();

signals:
    void select_piece(int);
    void turn_complete(bool);
public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
};

struct Genome{
    // Adds a many characteristics as thinkeble for the ludo player.
        float enterBoard =0.1;
        float moveHome =0.1;
        float sendEnemyHome = 0.1;
        float block = 0.1;
        float moveNormal = 0.1;
        float move2Star = 0.1;
        float move2Globe = 0.1;

        // Container for my chromosomes
        vector<float> Chromosomes {enterBoard,moveHome,sendEnemyHome,block,moveNormal,move2Star,move2Globe};
};

#endif // LUDO_PLAYER_GA_H
