#include "tetris.h"
using namespace Tetris;

Block::Block() {//생성자
    for(int i = 0; i < Block_wid; i++) {
        for(int j = 0; j < Block_hei; j++) {
            block[i][j] = 0;//4x4의 블럭을 모두 0으로 초기화
        }
    }
}

Block::~Block() { }

void Block::initialize(int type) {//현재 블럭의 type으로 초기화
    for(int i = 0; i < Block_wid; i++) {
        for(int j = 0; j < Block_hei; j++) {
            block[i][j] = BLOCKS[type][i * Block_wid + j];//1차원으로 정의된 블럭을 2차원에 저장
        }
    }
}

int Block::getCell(int x, int y) const {//해당 좌표값 반환
    return block[x][y];
}

void Block::copyTemp(int temp[]){//현재 블럭을 복사하는 함수
    for (int i = 0; i < Block_hei; ++i) {
        for (int j = 0; j < Block_wid; ++j) {
            temp[i * Block_wid + j] = block[i][j];//인자로 받은 1차원 배열에 현재 블럭(2차원 배열)을 저장
        }
    }
}

void Block::rotate_CW(){//현재 블럭을 반시계 회전
    int temp[Block_hei*Block_wid];
    copyTemp(temp);//temp에 현재 블럭 저장
    for(int i = 0; i < Block_hei; i++) {
        for(int j = 0; j < Block_wid; j++) {
            block[j][Block_hei-i-1] = temp[i * Block_wid + j];//현재 블럭을 반시계 방향으로 회전하여 다시 저장
        }
    }
}

void Block::rotate_CCW(){//현재 블럭을 시계 회전
    int temp[Block_hei*Block_wid];
    copyTemp(temp);
    for(int i = 0; i < Block_hei; i++) {
        for(int j = 0; j < Block_wid; j++) {
            block[Block_wid-j-1][i] = temp[i * Block_wid + j];//현재 블럭을 시계 방향으로 회전하여 다시 저장
        }
    }
}


Board::Board(QWidget* parent) : QWidget(parent), block(new Block), currentBlockX(4), currentBlockY(0), gameover(new GameoverWindow), games(new Games)
{//생성자
    for(int i=0;i<5;i++){
        nextBlockType.append(QRandomGenerator::global()->bounded(7));//int형 vector에 0~6의 랜덤 값을 5개 저장
    }
    games->SetState(false);//처음 상태는 false(게임 진행중)
    games->SetScore(0);//처음 점수는 0
    for(int i = 0; i < Board_row + 2; i++) {
        for(int j = 0; j < Board_col + 2; j++) {
            board[i][j] = 0;
        }
    }//모든 배열을 0으로 초기화

    for(int i = 0; i < Board_row + 2; i++) {
        for(int j = 0; j < Board_col + 2; j++) {
            if(i == 0 || i == Board_row + 1) {
                board[i][j] = 1;
            } else {
                if(j == 0 || j == Board_col + 1)
                    board[i][j] = 1;
            }
        }
    }//테두리는 1로 초기화
    generateRandomBlock();
}

Board::~Board() {
    delete gameover;
    delete block;
    delete games;
}

void Board::generateRandomBlock() {
    if(!games->GetState()){
        currentBlockType = nextBlockType.front();//다음 블럭 타입의 맨 앞 블럭 생성
        nextBlockType.pop_front(); // 벡터에서 첫 번째 블록 제거
        nextBlockType.append(QRandomGenerator::global()->bounded(7)); // 벡터의 끝에 새로운 랜덤 블록 추가
        currentBlockX = 4;//처음 블럭의 x좌표를 4로 초기화
        currentBlockY = 0;//처음 블럭의 y좌표를 0으로 초기화
        block->initialize(currentBlockType);
    }
}

void Board::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);

    int cell_wid = 30;//셀 너비
    int cell_hei = 30;//셀 높이

    int x_offset = (width() - (Board_col * cell_wid)) / 2;//x축 여백
    int y_offset = (height() - (Board_row * cell_hei)) / 2;//y축 여백
    //현재 보드의 블럭&빈칸을 그림
    for(int i = 1; i < Board_row + 1; i++) {
        for(int j = 1; j < Board_col + 1; j++) {
            if(board[i][j] == 0) {//빈칸은 하얀 사각형
                painter.fillRect(x_offset + (j - 1) * cell_wid, y_offset + (i - 1) * cell_hei, cell_wid, cell_hei, Qt::white);
                painter.setPen(Qt::black);
                painter.drawRect(x_offset + (j - 1) * cell_wid, y_offset + (i - 1) * cell_hei, cell_wid, cell_hei);
            }
            else if(board[i][j]==2){//블럭은 회색
                painter.fillRect(x_offset + (j - 1) * cell_wid, y_offset + (i - 1) * cell_hei, cell_wid, cell_hei, Qt::gray);
                painter.setPen(Qt::black);
                painter.drawRect(x_offset + (j - 1) * cell_wid, y_offset + (i - 1) * cell_hei, cell_wid, cell_hei);
            }
        }
    }
    //현재 점수 출력
    painter.setPen(Qt::white);//펜을 하얀색으로 초기화
    painter.setFont(QFont("Arial",14));
    painter.drawText(QRect(240*3+10*Block_size,30*2+4*Block_size,Block_size*4,Block_size*4 + 1000),Qt::AlignCenter,"Score : " + QString::number(games->GetScore()));

    //현재 움직이는 블럭을 그림
    painter.setBrush(Qt::gray);//펜을 회색으로 초기화
    for (int i = 0; i < Block_hei; ++i) {
        for (int j = 0; j < Block_wid; ++j) {
            if (block->getCell(i, j) == 2) {//움직이는 블럭은 회색 사각형
                painter.fillRect(x_offset + (currentBlockX + j - 1) * cell_wid, y_offset + (currentBlockY + i - 1) * cell_hei, cell_wid, cell_hei, Qt::gray);
                painter.setPen(Qt::black);
                painter.drawRect(x_offset + (currentBlockX + j - 1) * cell_wid, y_offset + (currentBlockY + i - 1) * cell_hei, cell_wid, cell_hei);
            }
        }
    }
    int rightMarginStart = Board_col * 100 + 20; // 오른쪽 여백
    int topMargin = 20;
    painter.setPen(Qt::white);
    painter.drawText(QRect(rightMarginStart, topMargin + 40, Block_size * 4, Block_size * 4), Qt::AlignLeft, "Next Blocks:");

    //다음에 나올 블럭 5개출력
    painter.setPen(Qt::black);
    for (int k = 0; k < nextBlockType.size(); k++) {
        int blockType = nextBlockType[k];//다음 블럭의 타입 저장
        const int* nextBlock = block->BLOCKS[blockType];//다음 블럭 저장
        for (int i = 0; i < Block_hei; i++) {
            for (int j = 0; j < Block_wid; j++) {
                if (nextBlock[i * Block_wid + j] != 0) {//0이 아닌 부분만 그리기
                    QRect nextRect(rightMarginStart + j * 20, topMargin + 80 + k * 100 + i * 20, 20, 20);
                    painter.drawRect(nextRect); // 셀 경계 그리기
                }
            }
        }
    }

}

void Board::moveBlockDown() {
    if (canMoveDown()) {//아래로 움직일 수 있다면
        currentBlockY += 1;//y좌표를 증가시켜 아래로 이동
    } else {
        fixBlock();//블럭 고정
        gameOver();//게임 종료 판단
        removeBlock();//제거가 가능한지 판단
        generateRandomBlock(); // 블럭이 멈추면 새로운 블럭 생성
    }
    updateGameArea();//판 업데이트
}

void Board::moveBlockLeft() {
    if (canMoveLeft()) {//왼쪽으로 움직일 수 있다면
        currentBlockX -= 1;//x좌표를 감소시켜 왼쪽으로 이동
    }
    updateGameArea();//판 업데이트
}

void Board::moveBlockRight() {
    if (canMoveRight()) {//오른쪽으로 움직일 수 있다면
        currentBlockX += 1;//x좌표를 증가시켜 오른쪽으로 이동
    }
    updateGameArea();//판 업데이트
}

void Board::updateGameArea() {
    update();//화면을 업데이트해서 다시 그리기
}

bool Board::canMoveDown() const {
    for (int i = 0; i < Block_hei; ++i) {
        for (int j = 0; j < Block_wid; ++j) {//블럭의 칸을 탐색
            if (block->getCell(i, j) == 2) {//2는 블럭이 존재하는 칸
                int newX = currentBlockX + j;
                int newY = currentBlockY + i + 1;//아래로 한칸 내린 위치
                if (newY >= Board_row + 1 || board[newY][newX] != 0) {
                    return false;//테트리스 판을 넘어가거나 테트리스 판의 다른 블럭이 있다면 못움직임
                }
            }
        }
    }
    return true;//아니라면 움직임
}


void Board::fixBlock() {
    for (int i = 0; i < Block_hei; ++i) {
        for (int j = 0; j < Block_wid; ++j) {
            if (block->getCell(i, j) == 2) {//블럭이 존재하는 칸
                board[currentBlockY + i][currentBlockX + j] = 2;//테트리스 판에 고정된 블럭을 3으로 저장
            }
        }
    }
}//블럭이 있는 칸(2)을 테트리스 판의 해당 위치에 복사

bool Board::canMoveLeft() const {
    for (int i = 0; i < Block_hei; ++i) {
        for (int j = 0; j < Block_wid; ++j) {
            if (block->getCell(i, j) == 2) {//블럭이 존재하는 칸만 탐색
                int newX = currentBlockX + j - 1;//왼쪽으로 한칸 이동한 위치
                int newY = currentBlockY + i;
                if (newX <= 0 || board[newY][newX] != 0) {//테트리스 판을 넘어가거나 다른 블럭이 있다면 못움직임
                    return false;
                }
            }
        }
    }
    return true;//아니라면 움직임
}

bool Board::canMoveRight() const {
    for (int i = 0; i < Block_hei; ++i) {
        for (int j = 0; j < Block_wid; ++j) {
            if (block->getCell(i, j) == 2) {//블럭이 존재하는 칸만 탐색
                int newX = currentBlockX + j + 1;//오른쪽으로 한칸 이동한 위치
                int newY = currentBlockY + i;
                if (newX >= Board_col + 1 || board[newY][newX] != 0) {//테트리스 판을 넘어가거나 다른 블럭이 있다면 못움직임
                    return false;
                }
            }
        }
    }
    return true;//아니라면 움직임
}

void Board::rotate_CW() {//시계 회전
    int temp_block[Block_hei][Block_wid];//2차원 임시블럭 생성
    block->rotate_CW();//블럭을 시계 방향으로 회전
    for (int i = 0; i < Block_hei; ++i) {
        for (int j = 0; j < Block_wid; ++j) {
            temp_block[i][j] = block->getCell(i, j);//현재 블럭을 저장
        }
    }//회전시킨 블럭을 temp_block에 저장
    bool canRotate = true;//블럭이 회전 가능한지 판단하는 변수
    for (int i = 0; i < Block_hei; ++i) {
        for (int j = 0; j < Block_wid; ++j) {
            if (temp_block[i][j] != 0) {//블럭이 존재하는 곳만 확인
                if (board[currentBlockY + i][currentBlockX + j] != 0) {//회전한 곳에 블럭이 존재하거나, 판을 넘어가면
                    canRotate = false;//false로 저장
                    break;
                }
            }
        }
    }
    if (!canRotate) {//블럭이 회전할 수 없다면
        block->rotate_CCW();//다시 반시계로 회전시켜서 원상복구
    }
}

void Board::rotate_CCW() {//반시계 회전
    int temp_block[Block_hei][Block_wid];//2차원 임시 블럭
    block->rotate_CCW();//반시계 방향으로 블럭 회전
    for (int i = 0; i < Block_hei; ++i) {
        for (int j = 0; j < Block_wid; ++j) {
            temp_block[i][j] = block->getCell(i, j);//현재 블럭 저장
        }
    }//회전시킨 블럭을 temp_block에 저장
    bool canRotate = true;//블럭이 회전 가능한지 판단하는 변수
    for (int i = 0; i < Block_hei; ++i) {
        for (int j = 0; j < Block_wid; ++j) {
            if (temp_block[i][j] != 0) {//블럭이 존재하는 곳만 확인
                if (board[currentBlockY + i][currentBlockX + j] != 0) {//회전한 곳에 블럭이 존재하거나, 판을 넘어가면
                    canRotate = false;
                    break;
                }
            }
        }
    }
    if (!canRotate) {//블럭이 회전할 수 없다면
        block->rotate_CW();//다시 시계방향으로 회전시켜서 원상복구
    }
}

void Board::keyPressEvent(QKeyEvent* event) {//키 입력을 감지하는 함수
    switch(event->key()) {
    case Qt::Key_Left://방향키 왼쪽
        emit moveLeft();
        break;
    case Qt::Key_Right://방향키 오른쪽
        emit moveRight();
        break;
    case Qt::Key_Down://방향키 아래
        emit moveDown();
        break;
    case Qt::Key_Space://스페이스 바
        emit moveBottom();
        break;
    case Qt::Key_Z://z키
        emit rotate_CounterClockwise();
        break;
    case Qt::Key_X://x키
        emit rotate_Clockwise();
        break;
    default://나머지 키
        QWidget::keyPressEvent(event);
    }
}

void Board::removeBlock(){
    bool isRemove=false;//블럭이 삭제되었는지 판단하는 변수
    for(int i = Board_row; i > 0; i--){//Board_row부터 1까지 반복하므로 테트리스 판 상에서는 맨 밑줄부터 맨 윗줄까지 반복
        bool flag = true; // 한 줄이 모두 채워졌는지 확인하는 플래그
        for (int j = 1; j < Board_col + 1; j++) {
            if (board[i][j] == 0) { // 하나라도 빈칸이 있다면
                flag = false;
                break;
            }
        }
        if (flag) {//해당 줄이 모두 채워져 있다면 i번째 줄을 삭제하고 위의 모든 줄을 한 칸씩 내림
            isRemove=true;
            for (int k = i; k > 1; k--) {//해당 줄부터 가장 윗 줄까지 반복
                for (int l = 1; l < Board_col + 1; l++) {
                    board[k][l] = board[k - 1][l];//k-1번째줄을 k번째 줄에 저장하여 한 칸씩 내림
                }
            }
            for (int m = 1; m < Board_col + 1; m++) {
                board[1][m] = 0;//맨 윗줄은 0으로 초기화(빈칸)
            }
            i++;//한 줄이 삭제되고 윗 줄이 내려오면 한번 더 검사해야하므로 i증가
        }
    }
    if(isRemove){//줄이 하나라도 삭제되면 실행
        int temp_score=games->GetScore();
        temp_score+=1;
        games->SetScore(temp_score);//점수 증가
    }
}

void Board::gameOver(){//게임 종료를 판단하는 함수
    bool flag=true;
    for(int i=1;i<Board_col+1;i++){
        if(board[1][i]==2){//맨 윗줄에 블럭이 하나라도 있다면
            flag=false;
            break;
        }
    }
    if(!flag){//게임 종료 조건을 만족한다면
        gameover->show();//게임 종료창 출력
        games->SetState(true);//게임 상태를 true로 변경
    }
}

