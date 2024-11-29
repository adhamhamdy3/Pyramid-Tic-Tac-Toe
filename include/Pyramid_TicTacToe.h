#ifndef PYRAMID_TIC_TAC_TOE_PYRAMID_TICTACTOE_H
#define PYRAMID_TIC_TAC_TOE_PYRAMID_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <iomanip>

class MainWindow;

template<typename type>
class Pyramid_TicTacToe_Board : public Board<type>{
public:
    Pyramid_TicTacToe_Board();
    ~Pyramid_TicTacToe_Board();
    bool update_board(const int& x, const int& y, const type& symbol) override;
    void display_board() const override;
    bool is_win() const override;
    bool is_draw() const override;
    bool game_is_over() const override;
    void cleanUp();
};

/// Random Player class

template<typename type>
class P_TTT_Random_Player : public RandomPlayer<type>{
public:
    P_TTT_Random_Player(const type& symbol);
    void getmove(int& x, int& y) override;
};

template<typename type>
void P_TTT_Random_Player<type>::getmove(int &x, int &y) {
    x = rand() % 3;
    y = rand() % 5;
}

template<typename type>
P_TTT_Random_Player<type>::P_TTT_Random_Player(const type &symbol) : RandomPlayer<type>(symbol) {
    this->dimension = 3;
    this->name = "Random Computer Player";
    srand(static_cast<unsigned int>(time(0)));
}

template<typename type>
Pyramid_TicTacToe_Board<type>::~Pyramid_TicTacToe_Board() {
    this->cleanUp();
}

template<typename type>
void Pyramid_TicTacToe_Board<type>::cleanUp() {
    if (this->board) {
        for (int i = 0; i < this->rows; ++i) {
            delete[] this->board[i];
        }
        delete[] this->board;
        this->board = nullptr;
    }

    this->n_moves = 0;
}


/// Player class
template<typename type>
class P_TTT_Player : public Player<type>{
public:
    P_TTT_Player(string n, type symbol);
    void getmove(int& x, int& y) override;

    friend class MainWindow;
};

template<typename type>
void P_TTT_Player<type>::getmove(int &x, int &y) {
    cout << "\nPlease enter your move x and y (0 to 2) separated by spaces: ";
    cin >> x >> y;
}

template<typename type>
P_TTT_Player<type>::P_TTT_Player(string n, type symbol) : Player<type>(n, symbol) {}


template<typename type>
bool Pyramid_TicTacToe_Board<type>::game_is_over() const {
    return (this->is_win() || this->is_draw());
}

template<typename type>
bool Pyramid_TicTacToe_Board<type>::is_draw() const {
    return (this->n_moves == 9 && !this->is_win());
}

template<typename type>
bool Pyramid_TicTacToe_Board<type>::is_win() const {
    if(this->board[0][2] == this->board[1][2] && this->board[1][2] == this->board[2][2] && this->board[0][2] != ' ')
        return true;

    if (this->board[1][1] == this->board[1][2] && this->board[1][2] == this->board[1][3] && this->board[1][1] != ' ')
        return true;

    if (this->board[0][2] == this->board[1][3] && this->board[1][3] == this->board[2][4] && this->board[0][2] != ' ')
        return true;

    if (this->board[0][2] == this->board[1][1] && this->board[1][1] == this->board[2][0] && this->board[0][2] != ' ')
        return true;

    for (int i = 0; i < this->columns - 2; ++i) {
        if (this->board[2][i] == this->board[2][i + 1] && this->board[2][i + 1] == this->board[2][i + 2]
            && this->board[2][i] != ' ') {
            return true;
        }
    }

    return false;
}

template<typename type>
void Pyramid_TicTacToe_Board<type>::display_board() const {
    for (int i = 0; i < this->rows; i++) {
        cout << "\n| ";
        for (int j = 0; j < this->columns; j++) {
            cout << "(" << i << "," << j << ")";
            cout << setw(2) << this->board[i][j] << " |";
        }
        cout << "\n-----------------------------------------------";
    }
    cout << endl;
}

template<typename type>
bool Pyramid_TicTacToe_Board<type>::update_board(const int& x, const int& y, const type& symbol) {
    if(!(x < 0 || x >= this->rows || y < 0 || y >= this->columns) && this->board[x][y] == ' '){
        this->n_moves++;
        this->board[x][y] = toupper(symbol); // loop better
        return true;
    }
    return false;
}

template<typename type>
Pyramid_TicTacToe_Board<type>::Pyramid_TicTacToe_Board() {
    this->rows = 3;
    this->columns = 5;

    this->board = new char*[this->rows];

    for (int i = 0; i < this->rows; ++i) {

        this->board[i] = new char[this->columns];

        for (int j = 0; j < this->columns; ++j) {
            if((i == 0 && (j == 0 || j == 1 || j == 3 || j == 4)) || (i == 1 && (j == 0 || j == 4))){
                this->board[i][j] = '+';
            }
            else this->board[i][j] = ' ';
        }
    }

    this->n_moves = 0;
}

#endif //PYRAMID_TIC_TAC_TOE_PYRAMID_TICTACTOE_H
