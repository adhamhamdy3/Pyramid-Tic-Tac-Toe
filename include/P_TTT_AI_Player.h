#ifndef P_TTT_AI_PLAYER_H
#define P_TTT_AI_PLAYER_H

#include "BoardGame_Classes.h"

template <typename type>
class P_TTT_AI_Player : public Player<type>{
private:
    pair<int, int> bestMove();
    int miniMax(const type& symbol, bool isMaximizing);
public:
    P_TTT_AI_Player(const type&);
    void getmove(int& x, int& y) override;
};

template<typename type>
int P_TTT_AI_Player<type>::miniMax(const type& symbol, bool isMaximizing) {
    if (this->boardPtr->is_win()) return isMaximizing ? 1 : -1;
    if (this->boardPtr->is_draw()) return 0;

    int bestScore = isMaximizing ? INT_MIN : INT_MAX;
    type opponentSymbol = (this->symbol == 'X') ? 'O' : 'X';

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (this->boardPtr->update_board(i, j, symbol)){
                int score = miniMax(opponentSymbol, !isMaximizing);
                this->boardPtr->update_board(i, j, ' ');

                if (isMaximizing) bestScore = max(bestScore, score);
                else bestScore = min(bestScore, score);
            }
        }
    }
    return bestScore;
}

template<typename type>
void P_TTT_AI_Player<type>::getmove(int &x, int &y) {
    pair<int, int> move = bestMove();
    x = move.first;
    y = move.second;
}

template<typename type>
pair<int, int> P_TTT_AI_Player<type>::bestMove() {
    int bestScore = INT_MIN;
    pair<int, int> Move = {-1, -1};

    type opponentSymbol = (this->symbol == 'X') ? 'O' : 'X';

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (this->boardPtr->update_board(i, j, this->symbol)){ // check if the cell is empty
                int score = miniMax(this->symbol, true);
                this->boardPtr->update_board(i, j, ' '); // undo Move that minimax caused

                if (score > bestScore){
                    bestScore = score;
                    Move = {i, j};
                }
            }

        }
    }
    return Move;
}

template<typename type>
P_TTT_AI_Player<type>::P_TTT_AI_Player(const type &symbol) : Player<type>(symbol) {
    this->name = "AI Player";
}

#endif //P_TTT_AI_PLAYER_H
