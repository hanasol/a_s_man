#include "mainwindow.h"
#include "tetris.h"
#include "puyopuyo.h"
#include "./ui_mainwindow.h"
#include "QLabel"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)//ui 할당
    , timer(new QTimer(this))//타이머 할당
{
    ui->setupUi(this);
    setFixedSize(520, 400);

    connect(this, &MainWindow::buttonClicked_tet, this, &MainWindow::Generategame_tet);
    connect(this, &MainWindow::buttonClicked_puyo, this, &MainWindow::Generategame_puyo);
    connect(this, &MainWindow::buttonClicked_puyotet, this, &MainWindow::Generategame_puyotet);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}

void MainWindow::on_puyopuyotetris_button_clicked()
{
    emit buttonClicked_puyotet(QString("PuyoTetris"));
}

void MainWindow::on_puyo_button_clicked()
{
    emit buttonClicked_puyo(QString("Puyopuyo"));
}

void MainWindow::on_tetris_button_clicked()
{
    emit buttonClicked_tet(QString("Tetris"));
}

void MainWindow::Generategame_puyotet(QString gamename)//뿌요테트
{
    QMainWindow* Gamewindow=new QMainWindow;
    Gamewindow->resize(1920,1080);
    Gamewindow->setWindowTitle(gamename);

    board_puyotet = new Puyotetris::Board(this);
    Gamewindow->setCentralWidget(board_puyotet);
    board_puyotet->setFocus();
    //뿌요테트 시그널&슬롯 연결
    connect(timer, &QTimer::timeout, this, &MainWindow::Puyotet_onTimeout);//타이머가 끝나면 onTimeout 슬롯에 signal전달
    connect(board_puyotet, &Puyotetris::Board::_moveLeft, this, &MainWindow::Puyotet_onMoveLeft);
    connect(board_puyotet, &Puyotetris::Board::_moveRight, this, &MainWindow::Puyotet_onMoveRight);
    connect(board_puyotet, &Puyotetris::Board::_moveDown, this, &MainWindow::Puyotet_onMoveDown);
    connect(board_puyotet, &Puyotetris::Board::moveBottom, this, &MainWindow::Puyotet_onMoveBottom);
    connect(board_puyotet, &Puyotetris::Board::rotate_CounterClockwise, this, &MainWindow::Puyotet_onCounter_Clockwise);
    connect(board_puyotet, &Puyotetris::Board::rotate_Clockwise, this, &MainWindow::Puyotet_onClockwise);
    timer->start(1000);

    Gamewindow->show();
    this->close();
}

void MainWindow::Puyotet_onTimeout(){
    if (board_puyotet) {
        board_puyotet->moveDown();//뿌요가 아래로 이동
    }
    board_puyotet->updateGameArea();
}

void MainWindow::Puyotet_onMoveLeft(){
    if (board_puyotet->canMoveLeft()) {//왼쪽으로 갈 수 있는 상태라면 실행
        board_puyotet->moveLeft();
    }
    board_puyotet->updateGameArea();
}

void MainWindow::Puyotet_onMoveRight(){
    if (board_puyotet->canMoveRight()) {//오른쪽으로 갈 수 있는 상태라면 실행
        board_puyotet->moveRight();
    }
    board_puyotet->updateGameArea();
}

void MainWindow::Puyotet_onMoveDown(){
    if (board_puyotet->canMoveDown()) {//아래쪽으로 갈 수 있는 상태라면 실행
        board_puyotet->moveDown();
    }
    board_puyotet->updateGameArea();
}

void MainWindow::Puyotet_onMoveBottom(){
    while (board_puyotet->canMoveDown()) {//아래쪽으로 갈 수 있는 상태라면 실행
        board_puyotet->moveDown();
    }
    board_puyotet->updateGameArea();
}

void MainWindow::Puyotet_onCounter_Clockwise(){
    board_puyotet->rotate_CCW();
    board_puyotet->updateGameArea();
}

void MainWindow::Puyotet_onClockwise(){
    board_puyotet->rotate_CW();
    board_puyotet->updateGameArea();
}

void MainWindow::Generategame_puyo(QString gamename)//뿌요뿌요
{
    QMainWindow* Gamewindow=new QMainWindow;
    Gamewindow->resize(1920,1080);
    Gamewindow->setWindowTitle(gamename);

    board_puyo = new Puyopuyo::Board(this);
    Gamewindow->setCentralWidget(board_puyo);
    board_puyo->setFocus();

    //뿌요뿌요 시그널&슬롯 연결
    connect(timer, &QTimer::timeout, this, &MainWindow::Puyo_onTimeout);//타이머가 끝나면 onTimeout 슬롯에 signal전달
    connect(board_puyo, &Puyopuyo::Board::moveLeft, this, &MainWindow::Puyo_onMoveLeft);
    connect(board_puyo, &Puyopuyo::Board::moveRight, this, &MainWindow::Puyo_onMoveRight);
    connect(board_puyo, &Puyopuyo::Board::moveDown, this, &MainWindow::Puyo_onMoveDown);
    connect(board_puyo, &Puyopuyo::Board::moveBottom, this, &MainWindow::Puyo_onMoveBottom);
    connect(board_puyo, &Puyopuyo::Board::rotate_CounterClockwise, this, &MainWindow::Puyo_onCounter_Clockwise);
    connect(board_puyo, &Puyopuyo::Board::rotate_Clockwise, this, &MainWindow::Puyo_onClockwise);
    timer->start(1000);

    Gamewindow->show();
    this->close();
}

void MainWindow::Puyo_onTimeout(){
    if (board_puyo) {
        board_puyo->movePuyoDown();//뿌요가 아래로 이동
        board_puyo->updateGameArea();//상태 업데이트
    }
}

void MainWindow::Puyo_onMoveLeft(){
    if (board_puyo->canMoveLeft()) {//왼쪽으로 갈 수 있는 상태라면 실행
        board_puyo->movePuyoLeft();
        board_puyo->updateGameArea();
    }
}

void MainWindow::Puyo_onMoveRight(){
    if (board_puyo->canMoveRight()) {//오른쪽으로 갈 수 있는 상태라면 실행
        board_puyo->movePuyoRight();
        board_puyo->updateGameArea();
    }
}

void MainWindow::Puyo_onMoveDown(){
    if (board_puyo->canMoveDown()) {//오른쪽으로 갈 수 있는 상태라면 실행
        board_puyo->movePuyoDown();
        board_puyo->updateGameArea();
    }
    else{
        board_puyo->fixPuyo();
        board_puyo->generateRandomPuyo();
        board_puyo->updateGameArea();
    }
}

void MainWindow::Puyo_onMoveBottom(){
    while(board_puyo->canMoveDown()){
        board_puyo->movePuyoDown();
    }
    board_puyo->updateGameArea();
}

void MainWindow::Puyo_onCounter_Clockwise(){
    board_puyo->rotate_CCW();
    board_puyo->updateGameArea();
}

void MainWindow::Puyo_onClockwise(){
    board_puyo->rotate_CW();
    board_puyo->updateGameArea();
}

void MainWindow::Generategame_tet(QString gamename)//테트리스
{
    QMainWindow* Gamewindow = new QMainWindow;
    Gamewindow->resize(1920,1080);
    Gamewindow->setWindowTitle(gamename);//gamename으로 WindowTitle저장

    board_tet = new Tetris::Board(this);//this로 할당해주어야함
    Gamewindow->setCentralWidget(board_tet);
    board_tet->setFocus();

    //테트리스 시그널&슬롯 연결
    connect(timer, &QTimer::timeout, this, &MainWindow::Tet_onTimeout);//타이머가 끝나면 onTimeout 슬롯에 signal전달
    connect(board_tet, &Tetris::Board::moveLeft, this, &MainWindow::Tet_onMoveLeft);
    connect(board_tet, &Tetris::Board::moveRight, this, &MainWindow::Tet_onMoveRight);
    connect(board_tet, &Tetris::Board::moveDown, this, &MainWindow::Tet_onMoveDown);
    connect(board_tet, &Tetris::Board::moveBottom, this, &MainWindow::Tet_onMoveBottom);
    connect(board_tet, &Tetris::Board::rotate_CounterClockwise, this, &MainWindow::Tet_onCounter_Clockwise);
    connect(board_tet, &Tetris::Board::rotate_Clockwise, this, &MainWindow::Tet_onClockwise);

    timer->start(1000); // 1초 간격으로 타이머 시작

    Gamewindow->show();
    this->close();

}

void MainWindow::Tet_onTimeout()//타이머가 끝나면 실행
{
    if (board_tet) {
        board_tet->moveBlockDown();//블럭이 아래로 이동
        board_tet->updateGameArea();//상태 업데이트
    }
}

void MainWindow::Tet_onMoveLeft(){//왼쪽 방향키가 눌러지면 실행
    if (board_tet->canMoveLeft()) {//왼쪽으로 갈 수 있는 상태라면 실행
        board_tet->moveBlockLeft();
        board_tet->updateGameArea();
    }
}

void MainWindow::Tet_onMoveRight(){//오른쪽 방향키가 눌러지면 실행
    if (board_tet->canMoveRight()) {//오른쪽으로 갈 수 있는 상태라면 실행
        board_tet->moveBlockRight();
        board_tet->updateGameArea();
    }
}

void MainWindow::Tet_onMoveDown(){//아래쪽 방향키가 눌러지면 실행
    if (board_tet->canMoveDown()) {//아래쪽으로 갈 수 있는 상태라면 실행
        board_tet->moveBlockDown();
        board_tet->updateGameArea();
    } else {//아니라면
        board_tet->fixBlock();//그자리에 블럭 고정
        board_tet->generateRandomBlock();//다음 블럭 생성
        board_tet->updateGameArea();
    }
}

void MainWindow::Tet_onMoveBottom(){//스페이스바가 눌러지면 실행
    while(board_tet->canMoveDown()){//아래쪽으로 갈 수 없을 때까지 실행
        board_tet->moveBlockDown();
    }
    board_tet->fixBlock();
    board_tet->updateGameArea();
}

void MainWindow::Tet_onCounter_Clockwise(){//z키가 눌러지면 실행
    board_tet->rotate_CCW();
    board_tet->updateGameArea();
}

void MainWindow::Tet_onClockwise(){//x키가 눌러지면 실행
    board_tet->rotate_CW();
    board_tet->updateGameArea();
}



