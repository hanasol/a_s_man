#ifndef PUYOTET_H
#define PUYOTET_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QVector>
#include <queue>
#include <QThread>
#include <QPainterPath>
#include "gameoverwindow.h"
#include "games.h"
using namespace std;

namespace Puyotetris{
const int Board_col = 8; // 뿌요테트의 열 크기
const int Board_row = 16; // 뿌요테트의 행 크기

const int Block_wid = 4;  // 블럭의 너비
const int Block_hei = 4;  // 블럭의 높이
const int Block_size = 30;

const int Puyo_wid=3;//뿌요의 너비
const int Puyo_hei=3;//뿌요의 높이
const int Puyo_size=30;

struct Coordinate{//좌표를 저장하는 구조체
    int x;
    int y;

    Coordinate(int x, int y): x(x), y(y) {}
};

class Puyotet{
public:
    Puyotet();
    ~Puyotet();
    void initialize_block(int type);//테트로미노 랜덤 생성
    void initialize_puyo(int color1, int color2);//뿌요 색상 초기화
    int getCell_block(int x, int y) const;//블럭의 좌표값 반환
    int getCell_puyo(int x, int y) const;//뿌요의 좌표값 반환
    const int BLOCKS[7][Block_wid * Block_hei] = {
        { 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0 }, // I
        { 0,0,0,0,0,2,0,0,0,2,2,2,0,0,0,0 }, // J
        { 0,0,0,0,0,0,0,2,0,2,2,2,0,0,0,0 }, // L
        { 0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,0 }, // O
        { 0,0,0,0,0,0,2,2,0,2,2,0,0,0,0,0 }, // S
        { 0,0,0,0,0,2,0,0,2,2,2,0,0,0,0,0 }, // T
        { 0,0,0,0,0,2,2,0,0,0,2,2,0,0,0,0 }  // Z
    };
    void setUnit(bool flag);//현재 유닛이 뿌요인지 블럭인지 세팅하는 함수
    bool getUnit();
    void rotate_CW();//시계방향 회전
    void rotate_CCW();//반시계방향 회전
private:
    bool unit;//현재 유닛이 뿌요인지 블럭인지를 나타내는 변수/ true:뿌요, false:테트로미노
    int puyo[Puyo_wid][Puyo_hei];//현재 뿌요
    int block[Block_wid][Block_hei];//현재 블럭
    void CopyTemp(int temp[]);//현재 뿌요를 복사
};

class Board : public QWidget{
    Q_OBJECT
public:
    explicit Board(QWidget* parent = nullptr);
    ~Board();
    void moveDown();//아래로 이동
    void moveLeft();//왼쪽으로 이동
    void moveRight();//오른쪽으로 이동
    void updateGameArea();//상태 업데이트 함수
    void generateRandomPuyo();//랜덤 뿌요 생성함수
    void generateRandomBlock();//랜덤 블럭 생성함수
    void rotate_CW();//시계 회전, 회전 가능 판단
    void rotate_CCW();//반시계 회전, 회전 가능 판단
    bool canMoveDown() const;//아래로 움직일 수 있는지 판단하는 함수
    bool canMoveLeft() const;//왼쪽으로 움직일 수 있는지 판단하는 함수
    bool canMoveRight() const;//오른쪽으로 움직일 수 있는지 판단하는 함수
    void fixUnit();//현재 유닛을 뿌요테트 판에 고정
    void keyPressEvent(QKeyEvent* event) override;
    void gameOver();//게임 종료를 판단하는 함수
    void removeBlock();//블럭을 제거하는 함수
    void removePuyo();//뿌요를 제거하는 함수
    void gravity();//뿌요테트 판에 중력을 적용하는 함수
    void density();//뿌요와 테트로미노의 밀도를 적용하는 함수
signals:
    void _moveLeft();//왼쪽 한칸 이동
    void _moveRight();//오른쪽 한칸 이동
    void _moveDown();//아래 한칸 이동
    void moveBottom();//아래 끝까지 이동
    void rotate_CounterClockwise();//반시계 회전
    void rotate_Clockwise();//시계 회전

protected:
    void paintEvent(QPaintEvent* event) override;// 0

private:
    int board[Board_row + 2][Board_col + 2];//게임판
    bool curunit;//현재 유닛이 뿌요인지 블럭인지 (true: 뿌요, false: 테트로미노)
    QVector<int> puyoORtet;//다음 생성될 유닛이 뿌요인지 블럭인지 정해줄 벡터
    Puyotet* block;//블럭
    Puyotet* puyo;//뿌요
    GameoverWindow* gameover;
    Games* games;

    int currentBlockX, currentBlockY;// 블럭의 x, y 좌표 (좌측 상단 모서리)
    int currentBlockType;//블럭의 타입
    QVector<int> nextBlockType;//다음 생성될 블럭들의 타입

    int currentPuyoX, currentPuyoY;// 뿌요의 x, y 좌표 (좌측 상단 모서리)
    int currentPuyoColor1, currentPuyoColor2;//뿌요의 색깔들
    QVector<int> nextPuyoColor;//다음 생성될 뿌요들의 색깔
};
}
#endif // PUYOTET_H
