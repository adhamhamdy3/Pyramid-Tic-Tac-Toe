#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "BoardGame_Classes.h"
#include "Pyramid_TicTacToe.h"
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_P_TTT_Grid_cellDoubleClicked(int row, int column);

private:
    Ui::MainWindow *ui;
    Player<char>* players[2];
    Pyramid_TicTacToe_Board<char>* Board;
    GameManager<char>* P_TTT_GAME;

private:
    void initGrid();
    bool isPreDisabled(const int&, const int&) const;
    void updateNoOfMovesLabel() const;

    void getPlayerInfo();

    bool player1, player2;

    bool randomPlayerMode;

    void randomPlayerTurn(const int&);
    void executeRandomPlayerTurn();

    void updateCell(QTableWidgetItem*, const int&, const int&, const int&);

};
#endif // MAINWINDOW_H
