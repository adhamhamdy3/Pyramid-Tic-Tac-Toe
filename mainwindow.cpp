#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QPushButton>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Pyramid_Tic_Tac_Toe)
{
    ui->setupUi(this);
    initGrid();

    player1 = true;
    player2 = false;

    nonHumanPlayerMode = false;
    gameOver = false;

    Board = new Pyramid_TicTacToe_Board<char>();

    getPlayerInfo();

    P_TTT_GAME = new GameManager<char>(Board, players);

    updateNoOfMovesLabel();

    ui->state1label->setText("State: YOUR TURN!");
    ui->state2Label->setText("State: Waiting...");

}

MainWindow::~MainWindow()
{
    delete ui;
    if (players[0]) delete players[0];
    if (players[1]) delete players[1];
    if (Board) delete Board;

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
                item->setBackground(Qt::transparent);
            } else {
                item->setFlags(Qt::ItemIsEnabled);
                item->setBackground(Qt::gray);
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
        updateCell(item, 0, row, column);

    } else if (player2) {
        P_TTT_GAME->boardPtr->update_board(row, column, players[1]->getsymbol());
        updateCell(item, 1, row, column);
    }

    isGameIsOver();

    if(gameOver) return void (ui->P_TTT_Grid->setEnabled(false));

    player1 ^= 1;

    updateState(); // toggle player1 ^= 1


    if(!nonHumanPlayerMode) player2 ^= 1;

    if(nonHumanPlayerMode)
        nonHumanPlayerTurn(2000);

    updateNoOfMovesLabel();
}

void MainWindow::updateNoOfMovesLabel() const{
    ui->noOfMovesLabel->setText("NUMBER OF MOVES = " + QString::fromStdString(std::to_string(P_TTT_GAME->boardPtr->n_moves)));

}


void MainWindow::getPlayerInfo(){
    QString player1Name = QInputDialog::getText(this, "Player 1 Name", "Enter Player 1 name:", QLineEdit::Normal, "Player 1");
    if(player1Name.isEmpty()) player1Name = "Player1";

    QChar player1Symbol = getSymbol("X");

    players[0] = new P_TTT_Player<char>(player1Name.toStdString(), player1Symbol.toLatin1());

    QMessageBox msgBox(this);

    msgBox.setText("Choose your opponent type:");
    QPushButton* aiButton = msgBox.addButton("AI Player", QMessageBox::ActionRole);
    QPushButton* randomButton = msgBox.addButton("Random Computer Player", QMessageBox::ActionRole);
    QPushButton* realButton = msgBox.addButton("Real Player", QMessageBox::ActionRole);

    msgBox.exec();

    QChar player2Symbol;

    if (msgBox.clickedButton() == aiButton) {
        player2Symbol = getSymbol("O");
        players[1] = new P_TTT_AI_Player<char>(player2Symbol.toLatin1());
        players[1]->setBoard(Board);
        nonHumanPlayerMode = true;
    } else if (msgBox.clickedButton() == randomButton) {
        player2Symbol = getSymbol("O");
        players[1] = new P_TTT_Random_Player<char>(player2Symbol.toLatin1());
        nonHumanPlayerMode = true;
    } else if (msgBox.clickedButton() == realButton) {
        QString player2Name = QInputDialog::getText(this, "Player 2 Name", "Enter Player 2 name:", QLineEdit::Normal, "Player 2");
        if (player2Name.isEmpty()) player2Name = "Player 2";

        player2Symbol = getSymbol("O");

        players[1] = new P_TTT_Player<char>(player2Name.toStdString(), player2Symbol.toLatin1());
    }

    ui->name1Label->setText("Name: " + QString::fromStdString(players[0]->getname()));
    ui->mark1Label->setText("Mark: " + QString::fromStdString(string(1, players[0]->getsymbol())));

    ui->name2label->setText("Name: " + QString::fromStdString(players[1]->getname()));
    ui->mark2Label->setText("Mark: " + QString::fromStdString(string(1, players[1]->getsymbol())));
}


void MainWindow::executeNonHumanPlayerTurn(){
    player1 = false;

    int x, y;
    players[1]->getmove(x, y);

    while(!P_TTT_GAME->boardPtr->update_board(x, y, players[1]->getsymbol())){
        players[1]->getmove(x, y);
    }

    QTableWidgetItem *item = ui->P_TTT_Grid->item(x, y);

    updateCell(item, 1, x, y);

    isGameIsOver();

    player1 = true;

    updateState();

    ui->P_TTT_Grid->setEnabled(!gameOver);

}

void MainWindow::nonHumanPlayerTurn(const int& delay = 2000) {
    ui->P_TTT_Grid->setEnabled(false);
    QTimer::singleShot(delay, this, &MainWindow::executeNonHumanPlayerTurn);
}


void MainWindow::updateCell(QTableWidgetItem *item, const int& playerIndex, const int& row, const int& column){
    item->setText(QString::fromStdString(std::string(1, players[playerIndex]->getsymbol())));
    item->setTextAlignment(Qt::AlignCenter);
    item->setFont(QFont("Outrun future", 30, QFont::Bold));
    if(!playerIndex) item->setBackground(Qt::blue);
    else item->setBackground(Qt::red);
    item->setForeground(Qt::white);

    item->setFlags(Qt::NoItemFlags);

}


void MainWindow::updateState(){
    if(player1){
        ui->state2Label->setText("State: Waiting...");
        ui->state1label->setText("State: YOUR TURN!");
    }
    else{
        ui->state1label->setText("State: Waiting...");
        ui->state2Label->setText("State: YOUR TURN!");
    }
}


void MainWindow::isGameIsOver(){
    if (P_TTT_GAME->boardPtr->game_is_over()) {
        if (P_TTT_GAME->boardPtr->is_win()) {
            if (player1)
                QMessageBox::information(this, "Win!", QString::fromStdString(players[0]->getname()) + " has won.");
            else
                QMessageBox::information(this, "Win!", QString::fromStdString(players[1]->getname()) + " has won.");
        } else if (P_TTT_GAME->boardPtr->is_draw()) {
            QMessageBox::information(this, "Draw!", "The match ended with a draw.");
        }
        gameOver = true;
    }
}

QChar MainWindow::getSymbol(const QString& defaultSymbol){
    QChar playerSymbol;
    while (true) {
        QString symbolInput = QInputDialog::getText(this, "Player 1 Symbol", "Enter Player 1 symbol (one character):", QLineEdit::Normal, defaultSymbol);
        if (symbolInput.size() == 1 && !symbolInput.at(0).isSpace()) {
            playerSymbol = symbolInput.at(0);
            break;
        } else {
            QMessageBox::warning(this, "Invalid Input", "Player 1 symbol must be a single non-whitespace character.");
        }
    }
    return playerSymbol;
}
