#ifndef TETRIS_H
#define TETRIS_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QVector>
#include "gameoverwindow.h"
#include "games.h"

namespace Tetris{//Tetris 이름공간내에 정의

const int Board_col = 10; // 테트리스의 열
const int Board_row = 20; // 테트리스의 행
const int Block_wid = 4;  // 블럭의 너비
const int Block_hei = 4;  // 블럭의 높이
const int Block_size = 30; //블럭 크기


class Block {//block 클래스
public:
    Block();
    ~Block();
    void initialize(int type);//현재 블럭의 type으로 초기화하는 함수
    int getCell(int x, int y) const;//x, y에 해당하는 숫자 반환
    const int BLOCKS[7][Block_wid * Block_hei] = {//7개의 블럭을 가지는 2차원 배열, 각 행을 4x4의 2차원 배열로 보면 블럭
        { 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0 }, // I
        { 0,0,0,0,0,2,0,0,0,2,2,2,0,0,0,0 }, // J
        { 0,0,0,0,0,0,0,2,0,2,2,2,0,0,0,0 }, // L
        { 0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,0 }, // O
        { 0,0,0,0,0,0,2,2,0,2,2,0,0,0,0,0 }, // S
        { 0,0,0,0,0,2,0,0,2,2,2,0,0,0,0,0 }, // T
        { 0,0,0,0,0,2,2,0,0,0,2,2,0,0,0,0 }  // Z
    };
    void rotate_CW();//시계방향 회전
    void rotate_CCW();//반시계방향 회전

private:
    int block[Block_wid][Block_hei];//현재 블럭
    void copyTemp(int temp[]);//현재 블럭을 복사하는 함수
};

class Board : public QWidget {
    Q_OBJECT

public:
    explicit Board(QWidget* parent = nullptr);
    ~Board();
    void moveBlockDown();//블럭 하강
    void moveBlockLeft();//왼쪽으로 이동
    void moveBlockRight();//오른쪽으로 이동
    void updateGameArea();//상태 업데이트 함수
    void generateRandomBlock();//랜덤 블럭 생성함수
    void rotate_CW();//시계 회전, 회전 가능 판단
    void rotate_CCW();//반시계 회전, 회전 가능 판단
    bool canMoveDown() const;//블럭이 아래로 움직일 수 있는지 판단하는 함수
    bool canMoveLeft() const;//블럭이 왼쪽으로 움직일 수 있는지 판단하는 함수
    bool canMoveRight() const;//블럭이 오른쪽으로 움직일 수 있는지 판단하는 함수
    void fixBlock();//블럭을 고정하는 함수
    void keyPressEvent(QKeyEvent* event) override;//키 입력을 감지하는 함수
    void gameOver();//게임 종료를 판단하는 함수
    void removeBlock();//블럭을 제거하는 함수
signals:
    void moveLeft();//왼쪽 한칸 이동
    void moveRight();//오른쪽 한칸 이동
    void moveDown();//아래 한칸 이동
    void moveBottom();//아래 끝까지 이동
    void rotate_CounterClockwise();//반시계 회전
    void rotate_Clockwise();//시계 회전

protected:
    void paintEvent(QPaintEvent* event) override;//테트리스 판과 블럭을 그리는 함수

private:
    int board[Board_row + 2][Board_col + 2];//게임판
    Block* block;
    GameoverWindow* gameover;
    Games* games;
    int currentBlockX, currentBlockY;// 블럭의 x, y 좌표 (좌측 상단 모서리)
    int currentBlockType;//블럭의 타입
    QVector<int> nextBlockType;//다음 생성될 블럭들의 타입
};
}

#endif // TETRIS_H
