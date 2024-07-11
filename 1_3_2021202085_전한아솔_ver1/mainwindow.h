#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "tetris.h"
#include "puyopuyo.h"
#include "puyotet.h"

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
    //MainWindow
    void on_puyopuyotetris_button_clicked();
    void on_puyo_button_clicked();
    void on_tetris_button_clicked();

    void Generategame_puyotet(QString Gamename);
    void Generategame_puyo(QString Gamename);
    void Generategame_tet(QString Gamename);

    //테트리스
    void Tet_onTimeout();
    void Tet_onMoveLeft();
    void Tet_onMoveRight();
    void Tet_onMoveDown();
    void Tet_onMoveBottom();
    void Tet_onCounter_Clockwise();
    void Tet_onClockwise();

    //뿌요뿌요
    void Puyo_onTimeout();
    void Puyo_onMoveLeft();
    void Puyo_onMoveRight();
    void Puyo_onMoveDown();
    void Puyo_onMoveBottom();
    void Puyo_onCounter_Clockwise();
    void Puyo_onClockwise();

    //뿌요테트
    void Puyotet_onTimeout();
    void Puyotet_onMoveLeft();
    void Puyotet_onMoveRight();
    void Puyotet_onMoveDown();
    void Puyotet_onMoveBottom();
    void Puyotet_onCounter_Clockwise();
    void Puyotet_onClockwise();

signals:
    //MainWindow
    void buttonClicked_puyotet(QString Gamename);
    void buttonClicked_puyo(QString Gamename);
    void buttonClicked_tet(QString Gamename);

private:
    Ui::MainWindow *ui;
    QTimer* timer;
    Tetris::Board *board_tet;
    Puyopuyo::Board *board_puyo;
    Puyotetris::Board* board_puyotet;
};
#endif // MAINWINDOW_H
