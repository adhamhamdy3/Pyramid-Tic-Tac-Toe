#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initGrid();

    player1 = true;
    player2 = false;

    Board = new Pyramid_TicTacToe_Board<char>();

    players[0] = new P_TTT_Player<char>("Adham", 'X');
    players[1] = new P_TTT_Player<char>("Hazem", 'O');

    P_TTT_GAME = new GameManager<char>(Board, players);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete Board;
    delete players[0];
    delete players[1];

    delete P_TTT_GAME;

}

void MainWindow::initGrid(){
    ui->P_TTT_Grid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->P_TTT_Grid->setSelectionMode(QAbstractItemView::NoSelection);


    for (int row = 0; row < ui->P_TTT_Grid->rowCount(); ++row) {
        for (int col = 0; col < ui->P_TTT_Grid->columnCount(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem();

            if (isPreDisabled(row, col)) {
                item->setFlags(Qt::NoItemFlags);
                item->setBackground(Qt::gray);
            } else {
                item->setFlags(Qt::ItemIsEnabled);
                item->setBackground(Qt::white);
                item->setTextAlignment(Qt::AlignCenter);
            }

            ui->P_TTT_Grid->setItem(row, col, item);
        }
    }
}


bool MainWindow::isPreDisabled(const int& row, const int& col) const{
    return ((row == 0 && (col == 0 || col == 1 || col == 3 || col == 4)) ||
            (row == 1 && (col == 0 || col == 4)));
}

void MainWindow::on_P_TTT_Grid_cellDoubleClicked(int row, int column)
{
    QTableWidgetItem *item = ui->P_TTT_Grid->item(row, column);

    if (!item || item->flags() & Qt::ItemIsEnabled == 0 || !item->text().isEmpty()) {
        return;
    }


    if (player1) {
        P_TTT_GAME->boardPtr->update_board(row, column, players[0]->getsymbol());
        item->setText("X");
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont("Outrun future", 30, QFont::Bold));
        item->setBackground(Qt::blue);
        item->setForeground(Qt::white);
    } else if (player2) {
        P_TTT_GAME->boardPtr->update_board(row, column, players[1]->getsymbol());
        item->setText("O");
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont("Outrun future", 30, QFont::Bold));
        item->setBackground(Qt::red);
        item->setForeground(Qt::white);
    }

    item->setFlags(Qt::NoItemFlags);

    if (P_TTT_GAME->boardPtr->game_is_over()) {
        if (P_TTT_GAME->boardPtr->is_win()) {
            if (player1)
                QMessageBox::information(this, "Win!", "Player 1 has won.");
            else
                QMessageBox::information(this, "Win!", "Player 2 has won.");
        } else if (P_TTT_GAME->boardPtr->is_draw()) {
            QMessageBox::information(this, "Draw!", "The match ended with a draw.");
        }

        ui->P_TTT_Grid->setEnabled(false);
        return;
    }

    player1 ^= 1;
    player2 ^= 1;
}


