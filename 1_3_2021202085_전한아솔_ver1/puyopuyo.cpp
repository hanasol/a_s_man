#include "puyopuyo.h"
using namespace Puyopuyo;
Puyo::Puyo(){
    for(int i = 0; i < Puyo_wid; i++) {
        for(int j = 0; j < Puyo_hei; j++) {
            puyo[i][j] = 0;
        }
    }
}

Puyo::~Puyo(){

}

void Puyo::initialize(int color1, int color2){//
    for(int i = 0; i < Puyo_wid; i++) {
        for(int j = 0; j < Puyo_hei; j++) {
            puyo[i][j] = 0;
        }
    }
    puyo[0][1]=color1;
    puyo[1][1]=color2;
}

int Puyo::getCell(int x, int y){
    return puyo[x][y];
}

void Puyo::rotate_CCW(){
    int temp[Puyo_hei*Puyo_wid];
    CopyTemp(temp);
    for(int i = 0; i < Puyo_hei; i++) {
        for(int j = 0; j < Puyo_wid; j++) {
            puyo[Puyo_wid-j-1][i] = temp[i * Puyo_wid + j];
        }
    }
}

void Puyo::rotate_CW(){
    int temp[Puyo_hei*Puyo_wid];
    CopyTemp(temp);
    for(int i = 0; i < Puyo_hei; i++) {
        for(int j = 0; j < Puyo_wid; j++) {
            puyo[j][Puyo_hei-i-1] = temp[i * Puyo_wid + j];
        }
    }
}

void Puyo::CopyTemp(int temp[]){
    for (int i = 0; i < Puyo_hei; ++i) {
        for (int j = 0; j < Puyo_wid; ++j) {
            temp[i * Puyo_wid + j] = puyo[i][j];
        }
    }
}


Board::Board(QWidget* parent) : QWidget(parent), puyo(new Puyo), currentPuyoX(2), currentPuyoY(0), gameover(new GameoverWindow), games(new Games){
    games->SetState(false);
    games->SetScore(0);
    for(int i=0;i<10;i++){
        nextPuyoColor.append(QRandomGenerator::global()->bounded(5)+4);
    }
    games->SetState(false);
    games->SetScore(0);
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
    generateRandomPuyo();
}

Board::~Board(){
    delete puyo;
    delete gameover;
    delete games;
}

void Board::generateRandomPuyo(){
    if(!games->GetState()){
        currentPuyoX=2;
        currentPuyoY=0;
        currentPuyoColor1=nextPuyoColor.front();//다음 색상에서 하나 뽑음
        nextPuyoColor.pop_front();//해당 색상 삭제
        currentPuyoColor2=nextPuyoColor.front();//하나 더 뽑음
        nextPuyoColor.pop_front();//해당 색상 삭제
        nextPuyoColor.append(QRandomGenerator::global()->bounded(5)+4);//랜덤 색상 추가
        nextPuyoColor.append(QRandomGenerator::global()->bounded(5)+4);//랜덤 색상 추가
        puyo->initialize(currentPuyoColor1,currentPuyoColor2);//랜덤 색상 2개로 뿌요 초기화
    }
}

void Board::paintEvent(QPaintEvent* event){
    Q_UNUSED(event);
    QPainter painter(this);

    int cell_wid = 30;//셀 너비
    int cell_hei = 30;//셀 높이

    int x_offset = (width() - (Board_col * cell_wid)) / 2;
    int y_offset = (height() - (Board_row * cell_hei)) / 2;//여백 설정
    //현재 보드를 그림
    for (int i = 1; i < Board_row + 1; i++) {
        for (int j = 1; j < Board_col + 1; j++) {
            QColor color;//뿌요 색
            bool drawEllipse = false;//뿌요를 그릴지 판단하는 변수
            switch (board[i][j]) {//보드의 숫자 판단
            case 0://0은 빈칸
                color = Qt::white;
                drawEllipse = false;
                break;
            case 4://4는 빨간 뿌요
                color = Qt::red;
                drawEllipse = true;
                break;
            case 5://5는 노란 뿌요
                color = Qt::yellow;
                drawEllipse = true;
                break;
            case 6://6는 초록 뿌요
                color = Qt::green;
                drawEllipse = true;
                break;
            case 7://7은 파란 뿌요
                color = Qt::blue;
                drawEllipse = true;
                break;
            case 8://8은 보라 뿌요
                color = QColorConstants::Svg::purple;
                drawEllipse = true;
                break;
            default://나머지
                color = Qt::white;
                drawEllipse = false;
                break;
            }
            painter.setPen(Qt::black);//테두리는 검은색
            painter.setBrush(Qt::white);//배경은 흰색
            if (drawEllipse) {//뿌요를 그릴 때
                painter.fillRect(x_offset + (j - 1) * cell_wid, y_offset + (i - 1) * cell_hei, cell_wid, cell_hei, Qt::white);
                painter.setPen(Qt::black);
                painter.drawRect(x_offset + (j - 1) * cell_wid, y_offset + (i - 1) * cell_hei, cell_wid, cell_hei);//하얀 사각형에 검은 테두리 생성
                //뿌요의 색을 따로 설정
                QBrush brush(color);//숫자에 따라서 뿌요 색깔 지정
                painter.setBrush(brush);
                painter.drawEllipse(x_offset + (j - 1) * cell_wid, y_offset + (i - 1) * cell_hei, cell_wid, cell_hei);
            }
            else {//뿌요를 그리지 않을 때
                painter.fillRect(x_offset + (j - 1) * cell_wid, y_offset + (i - 1) * cell_hei, cell_wid, cell_hei, Qt::white);
                painter.setPen(Qt::black);
                painter.drawRect(x_offset + (j - 1) * cell_wid, y_offset + (i - 1) * cell_hei, cell_wid, cell_hei);
            }
        }
    }

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 14));
    painter.drawText(QRect(100 * 3 + 10 * Puyo_size, 5 * 2 + 4 * Puyo_size, Puyo_size * 25, Puyo_size * 6 + 800), Qt::AlignCenter, "Score : " + QString::number(games->GetScore()));

    int rightMarginStart = Board_col * 150 + 20;//오른쪽 여백 시작 위치
    int topMargin = 20;//텍스트의 y 시작 위치
    painter.setPen(Qt::white);
    painter.drawText(QRect(rightMarginStart, topMargin + 40, Puyo_size * 10, Puyo_size * 4), Qt::AlignLeft, "Next Blocks:");

    //현재 움직이는 뿌요를 그림
    for (int i = 0; i < Puyo_hei; i++) {
        for (int j = 0; j < Puyo_wid; j++) {
            int cellValue = puyo->getCell(i, j);
            if (cellValue == 0)
                continue;
            QColor color;
            switch (cellValue) {
            case 4:
                color = Qt::red;
                break;
            case 5:
                color = Qt::yellow;
                break;
            case 6:
                color = Qt::green;
                break;
            case 7:
                color = Qt::blue;
                break;
            case 8:
                color = QColorConstants::Svg::purple;
                break;
            default:
                color = Qt::white;
                break;
            }
            painter.setPen(Qt::black);
            painter.setBrush(color);
            painter.drawEllipse(x_offset + (currentPuyoX + j - 1) * cell_wid, y_offset + (currentPuyoY + i - 1) * cell_hei, cell_wid, cell_hei);
        }
    }

    //다음 나올 뿌요 5개 그리기
    for (int k = 0; k < nextPuyoColor.size(); k+=2) {//뿌요 색을 하나씩 골라야하므로 k는 2씩 증가
        int PuyoColor1 = nextPuyoColor[k];
        int PuyoColor2 = nextPuyoColor[k+1];
        int nextPuyo[Puyo_hei][Puyo_wid]={0,};
        nextPuyo[0][1]=PuyoColor1;
        nextPuyo[1][1]=PuyoColor2;
        for (int i = 0; i < Puyo_hei; ++i) {
            for (int j = 0; j < Puyo_wid; ++j) {
                if (nextPuyo[i][j] != 0) {
                    switch(nextPuyo[i][j]){
                    case 4:
                        painter.setBrush(Qt::red);
                        break;
                    case 5:
                        painter.setBrush(Qt::yellow);
                        break;
                    case 6:
                        painter.setBrush(Qt::green);
                        break;
                    case 7:
                        painter.setBrush(Qt::blue);
                        break;
                    case 8:
                        painter.setBrush(QColorConstants::Svg::purple);
                        break;
                    default:
                        painter.setBrush(Qt::black);
                        break;
                    }
                    painter.setPen(Qt::black);
                    QRect nextRect(rightMarginStart + j * 20, topMargin + 80 + k * 50 + i * 20, 20, 20);
                    painter.drawEllipse(nextRect);
                }
            }
        }
    }
}


void Board::updateGameArea(){
    update();
}

void Board::movePuyoDown() {
    if (canMoveDown()) {//아래로 움직일 수 있다면
        currentPuyoY += 1;//y좌표를 증가시켜 아래로 이동
    } else {
        fixPuyo();//뿌요 고정
        gameOver();//게임 종료 판단
        updateGameArea();
        //QThread::msleep(500);
        removePuyo();//제거가 가능한지 판단
        generateRandomPuyo(); // 블럭이 멈추면 새로운 블럭 생성
    }
    updateGameArea();//판 업데이트
}

void Board::movePuyoLeft() {
    if (canMoveLeft()) {//왼쪽으로 움직일 수 있다면
        currentPuyoX -= 1;//x좌표를 감소시켜 왼쪽으로 이동
    }
    updateGameArea();//판 업데이트
}

void Board::movePuyoRight() {
    if (canMoveRight()) {//왼쪽으로 움직일 수 있다면
        currentPuyoX += 1;//x좌표를 감소시켜 왼쪽으로 이동
    }
    update();
    updateGameArea();//판 업데이트
}

bool Board::canMoveDown() const {
    for (int i = 0; i < Puyo_hei; ++i) {
        for (int j = 0; j < Puyo_wid; ++j) {//뿌요의 칸을 탐색
            if (puyo->getCell(i, j) != 0) {//뿌요가 존재하는 칸
                int newX = currentPuyoX + j;
                int newY = currentPuyoY + i + 1;//아래로 한칸 내린 위치
                if (newY >= Board_row + 1 || board[newY][newX] != 0) {
                    return false;//테트리스 판을 넘어가거나 테트리스 판의 다른 블럭이 있다면 못움직임
                }
            }
        }
    }
    return true;//아니라면 움직임
}

bool Board::canMoveLeft() const {
    for (int i = 0; i < Puyo_hei; ++i) {
        for (int j = 0; j < Puyo_wid; ++j) {
            if (puyo->getCell(i, j) != 0) {//블럭이 존재하는 칸만 탐색
                int newX = currentPuyoX + j - 1;//왼쪽으로 한칸 이동한 위치
                int newY = currentPuyoY + i;
                if (newX <= 0 || board[newY][newX] != 0) {//테트리스 판을 넘어가거나 다른 블럭이 있다면 못움직임
                    return false;
                }
            }
        }
    }
    return true;//아니라면 움직임
}

bool Board::canMoveRight() const {
    for (int i = 0; i < Puyo_hei; ++i) {
        for (int j = 0; j < Puyo_wid; ++j) {
            if (puyo->getCell(i, j) != 0) {//블럭이 존재하는 칸만 탐색
                int newX = currentPuyoX + j + 1;//오른쪽으로 한칸 이동한 위치
                int newY = currentPuyoY + i;
                if (newX >= Board_col + 1 || board[newY][newX] != 0) {//테트리스 판을 넘어가거나 다른 블럭이 있다면 못움직임
                    return false;
                }
            }
        }
    }
    return true;//아니라면 움직임
}

void Board::fixPuyo() {
    for (int i = 0; i < Puyo_hei; i++) {
        for (int j = 0; j < Puyo_wid; j++) {
            if (puyo->getCell(i, j) != 0) {
                board[currentPuyoY + i][currentPuyoX + j] = puyo->getCell(i, j);//보드에 뿌요의 값을 복사
            }
        }
    }
    gravity();//중력 적용
}

void Board::gravity(){//중력을 적용하는 함수
    for(int j=1; j<Board_col+1; j++){//열
        for (int i=Board_row-1; i>0; i--){//맨 밑에서 하나 위 행부터 맨 위 행까지
            int underrow=i+1;//뿌요의 아래 행
            int emptycnt=0;//빈칸의 갯수
            while(board[underrow][j]==0&&underrow!=Board_row+1){//아래 행이 비어있고 판의 경계를 넘지 않을 때까지 반복
                underrow++;
                emptycnt++;
            }
            if(emptycnt>0){//빈칸이 존재한다면
                board[underrow-1][j]=board[i][j];//뿌요를 최대한 아래로 내림
                board[i][j]=0;//원래 자리는 공백으로 만듦
            }
        }
    }
}

void Board::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
    case Qt::Key_Left:
        emit moveLeft();
        break;
    case Qt::Key_Right:
        emit moveRight();
        break;
    case Qt::Key_Down:
        emit moveDown();
        break;
    case Qt::Key_Space:
        emit moveBottom();
        break;
    case Qt::Key_Z:
        emit rotate_CounterClockwise();
        break;
    case Qt::Key_X:
        emit rotate_Clockwise();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void Board::rotate_CCW(){
    int temp_block[Puyo_hei][Puyo_wid];
    puyo->rotate_CCW();//블럭을 시계 방향으로 회전
    for (int i = 0; i < Puyo_hei; ++i) {
        for (int j = 0; j < Puyo_wid; ++j) {
            temp_block[i][j] = puyo->getCell(i, j);
        }
    }//회전시킨 블럭을 temp_block에 저장
    bool canRotate = true;
    for (int i = 0; i < Puyo_hei; ++i) {
        for (int j = 0; j < Puyo_wid; ++j) {
            if (temp_block[i][j] != 0) {//블럭이 존재하는 곳만 확인
                if (board[currentPuyoY + i][currentPuyoX + j] != 0) {//회전한 곳에 블럭이 존재하거나, 판을 넘어가면
                    canRotate = false;
                    break;
                }
            }
        }
    }
    // 유효하지 않다면 원래 블럭으로 되돌림
    if (!canRotate) {
        puyo->rotate_CW();
    }
}

void Board::rotate_CW(){
    int temp_block[Puyo_hei][Puyo_wid];
    puyo->rotate_CW();//블럭을 시계 방향으로 회전
    for (int i = 0; i < Puyo_hei; ++i) {
        for (int j = 0; j < Puyo_wid; ++j) {
            temp_block[i][j] = puyo->getCell(i, j);
        }
    }//회전시킨 블럭을 temp_block에 저장
    bool canRotate = true;
    for (int i = 0; i < Puyo_hei; ++i) {
        for (int j = 0; j < Puyo_wid; ++j) {
            if (temp_block[i][j] != 0) {//블럭이 존재하는 곳만 확인
                if (board[currentPuyoY + i][currentPuyoX + j] != 0) {//회전한 곳에 블럭이 존재하거나, 판을 넘어가면
                    canRotate = false;
                    break;
                }
            }
        }
    }
    // 유효하지 않다면 원래 블럭으로 되돌림
    if (!canRotate) {
        puyo->rotate_CCW();
    }
}

void Board::removePuyo() {
    int dx[4] = {1, 0, -1, 0};//x좌표의 상하좌우
    int dy[4] = {0, 1, 0, -1}; //y좌표의 상하좌우

    while (1) {
        bool flag = false;//뿌요가 터짐을 판단하는 조건
        bool visit[Board_row + 2][Board_col + 2] = {0,}; // 해당 칸을 방문했는지 저장

        for (int i = 1; i < Board_row + 1; i++) {
            for (int j = 1; j < Board_col + 1; j++) {
                if (board[i][j] != 0 && visit[i][j] == 0) {
                    QVector<Coordinate> coor_puyo; // 뿌요의 좌표가 저장될 벡터
                    queue<Coordinate> que; // 뿌요를 탐색하면서 좌표가 저장될 큐
                    que.push(Coordinate(i, j)); // 뿌요의 좌표를 큐에 삽입
                    visit[i][j] = true; // 해당 위치 체크
                    coor_puyo.push_back(Coordinate(i, j)); // 뿌요의 좌표를 벡터에 삽입

                    while (!que.empty()) {
                        int curX = que.front().x; // 현재 큐에 담긴 x좌표 저장 (행)
                        int curY = que.front().y; // 현재 큐에 담긴 y좌표 저장 (열)
                        que.pop(); // 큐의 좌표 제거
                        for (int k = 0; k < 4; k++) { // 상하좌우 탐색
                            int newX = curX + dx[k]; // 새로운 칸의 x좌표 (행)
                            int newY = curY + dy[k]; // 새로운 칸의 y좌표 (열)
                            if (newX <= 0 || newX > Board_row || newY <= 0 || newY > Board_col)
                                continue;
                            if (board[newX][newY] == board[curX][curY] && visit[newX][newY] == false) { // 인접한 칸의 뿌요의 색이 같고 방문하지 않은 칸이라면
                                que.push(Coordinate(newX, newY)); // 해당 좌표를 큐에 삽입
                                coor_puyo.push_back(Coordinate(newX, newY)); // 벡터에도 삽입
                                visit[newX][newY] = true; // 해당 위치 체크
                            }
                        }
                    }
                    if (coor_puyo.size() >= 4) {//4개 이상의 인접한 뿌요가 있다면
                        flag = true;
                        for (int i = 0; i < coor_puyo.size(); i++)
                            board[coor_puyo[i].x][coor_puyo[i].y] = 0;//해당 뿌요의 좌표를 0으로 초기화
                    }
                }
            }
        }
        if (!flag)//터질 뿌요가 없다면
            break;
        else {//뿌요가 터지면
            int x=games->GetScore();
            x++;
            games->SetScore(x);
            gravity();//중력 작용
            updateGameArea();//게임판 상태 업데이트
        }
    }
}


void Board::gameOver(){
    bool flag=true;
    if(board[1][3]!=0){//맨 윗줄 2번째 칸이 종료 조건이므로, 배열에서는 1,3의 위치임
        flag=false;
    }
    if(!flag){
        gameover->show();
        games->SetState(true);
    }
}


