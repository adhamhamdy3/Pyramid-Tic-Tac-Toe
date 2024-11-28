#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>

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

    updateNoOfMovesLabel();

    getPlayerInfo();

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
    updateNoOfMovesLabel();
}

void MainWindow::updateNoOfMovesLabel() const{
    ui->noOfMovesLabel->setText("NUMBER OF MOVES = " + QString::fromStdString(std::to_string(P_TTT_GAME->boardPtr->n_moves)));

}


void MainWindow::getPlayerInfo(){
    QString player1Name, player2Name;
    QChar player1Symbol, player2Symbol;

    player1Name = QInputDialog::getText(this, "Player 1 Name", "Enter Player 1 name:", QLineEdit::Normal, "Player 1");
    if(player1Name.isEmpty())
        player1Name = "Player1";

    bool validInput = false;

    while (!validInput) {
        QString symbolInput = QInputDialog::getText(this, "Player 1 Symbol", "Enter Player 1 symbol (one character):", QLineEdit::Normal, "X");
        if (symbolInput.size() == 1) {
            player1Symbol = symbolInput.at(0);
            validInput = true;
        } else {
            QMessageBox::warning(this, "Invalid Input", "Player 1 symbol must be a single character.");
        }
    }

    player2Name = QInputDialog::getText(this, "Player 2 Name", "Enter Player 2 name:", QLineEdit::Normal, "Player 2");
    if (player2Name.isEmpty()) {
        player2Name = "Player 2"; // Default name if no input
    }

    validInput = false;
    while (!validInput) {
        QString symbolInput = QInputDialog::getText(this, "Player 2 Symbol", "Enter Player 2 symbol (one character):", QLineEdit::Normal, "O");
        if (symbolInput.size() == 1 && symbolInput.at(0) != player1Symbol) {
            player2Symbol = symbolInput.at(0);
            validInput = true;
        } else {
            QMessageBox::warning(this, "Invalid Input", "Player 2 symbol must be a single character and different from Player 1's symbol.");
        }
    }

    players[0]->symbol = player1Symbol.toLatin1();
    players[1]->symbol = player2Symbol.toLatin1();

    ui->name1Label->setText("Name: " + player1Name);
    ui->mark1Label->setText("Mark: " + QString::fromStdString(string(1, players[0]->getsymbol())));

    ui->name2label->setText("Name: " + player2Name);
    ui->mark2Label->setText("Mark: " + QString::fromStdString(string(1, players[1]->getsymbol())));

}

