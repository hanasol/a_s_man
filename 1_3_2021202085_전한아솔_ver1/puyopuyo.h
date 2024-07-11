#ifndef TEST_PUYO_H
#define TEST_PUYO_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QVector>
#include <queue>
#include "gameoverwindow.h"
#include "games.h"
using namespace std;

namespace Puyopuyo{

const int Board_col=6;//뿌요뿌요 판의 열
const int Board_row=12;//뿌요뿌요 판의 행
const int Puyo_wid=3;//뿌요의 너비
const int Puyo_hei=3;//뿌요의 높이
const int Puyo_size=30;

struct Coordinate{
    int x;
    int y;

    Coordinate(int x, int y): x(x), y(y) {}
};

class Puyo {
public:
    Puyo();
    ~Puyo();
    void initialize(int color1, int color2);//해당 색깔로 뿌요를 초기화
    int getCell(int x, int y);//해당 좌표 값 반환
    void rotate_CW();//시계방향 회전
    void rotate_CCW();//반시계방향 회전
private:
    int puyo[Puyo_wid][Puyo_hei];//현재 뿌요
    void CopyTemp(int temp[]);//현재 뿌요를 복사
};

class Board: public QWidget{
    Q_OBJECT
public:
    explicit Board(QWidget* parent = nullptr);
    ~Board();
    void movePuyoDown();//뿌요 하강
    void movePuyoLeft();//왼쪽으로 이동
    void movePuyoRight();//오른쪽으로 이동
    void updateGameArea();//상태 업데이트 함수
    void generateRandomPuyo();//랜덤 뿌요 생성함수
    void rotate_CW();//시계 회전, 회전 가능 판단
    void rotate_CCW();//반시계 회전, 회전 가능 판단
    bool canMoveDown() const;//뿌요가 아래로 움직일 수 있는지 판단하는 함수
    bool canMoveLeft() const;//뿌요가 왼쪽으로 움직일 수 있는지 판단하는 함수
    bool canMoveRight() const;//뿌요가 오른쪽으로 움직일 수 있는지 판단하는 함수
    void fixPuyo();//뿌요를 고정하는 함수
    void keyPressEvent(QKeyEvent* event) override;
    void gameOver();//게임 종료를 판단하는 함수
    void removePuyo();//뿌요를 제거하는 함수
    void gravity();//뿌요뿌요 판에 중력을 적용하는 함수
signals:
    void moveLeft();//왼쪽 한칸 이동
    void moveRight();//오른쪽 한칸 이동
    void moveDown();//아래 한칸 이동
    void moveBottom();//아래 끝까지 이동
    void rotate_CounterClockwise();//반시계 회전
    void rotate_Clockwise();//시계 회전
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    Puyo* puyo;
    int board[Board_row + 2][Board_col + 2];//게임판
    GameoverWindow* gameover;
    Games* games;
    int currentPuyoX, currentPuyoY;// 뿌요의 x, y 좌표 (좌측 상단 모서리)
    int currentPuyoColor1, currentPuyoColor2;//뿌요의 색깔들
    QVector<int> nextPuyoColor;//다음 생성될 뿌요들의 색깔
};
}

#endif // TEST_PUYO_H
