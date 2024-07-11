#include "puyotet.h"
using namespace Puyotetris;

Puyotet::Puyotet(){
    unit=false;
    //블럭 초기화
    for(int i = 0; i < Block_wid; i++) {
        for(int j = 0; j < Block_hei; j++) {
            block[i][j] = 0;
        }
    }
    //뿌요 초기화
    for(int i = 0; i < Puyo_wid; i++) {
        for(int j = 0; j < Puyo_hei; j++) {
            puyo[i][j] = 0;
        }
    }
}

Puyotet::~Puyotet(){ }

void Puyotet::initialize_block(int type){
    for(int i = 0; i < Block_wid; i++) {
        for(int j = 0; j < Block_hei; j++) {
            block[i][j] = BLOCKS[type][i * Block_wid + j];
        }
    }
}

void Puyotet::initialize_puyo(int color1, int color2){
    for(int i = 0; i < Puyo_wid; i++) {
        for(int j = 0; j < Puyo_hei; j++) {
            puyo[i][j] = 0;
        }
    }
    puyo[0][1]=color1;
    puyo[1][1]=color2;
}

int Puyotet::getCell_block(int x, int y) const{
    return block[x][y];
}

int Puyotet::getCell_puyo(int x, int y) const{
    return puyo[x][y];
}

void Puyotet::setUnit(bool flag){
    unit=flag;
}

bool Puyotet::getUnit(){
    return unit;
}

void Puyotet::CopyTemp(int temp[]){
    //뿌요일 때
    if(unit){
        for (int i = 0; i < Puyo_hei; ++i) {
            for (int j = 0; j < Puyo_wid; ++j) {
                temp[i * Puyo_wid + j] = puyo[i][j];
            }
        }
    }
    //테트로미노일 때
    else{
        for (int i = 0; i < Block_hei; ++i) {
            for (int j = 0; j < Block_wid; ++j) {
                temp[i * Block_wid + j] = block[i][j];
            }
        }
    }
}

void Puyotet::rotate_CCW(){//반시계 회전
    //뿌요일 때
    if(unit){
        int temp[Puyo_hei*Puyo_wid];
        CopyTemp(temp);
        for(int i = 0; i < Puyo_hei; i++) {
            for(int j = 0; j < Puyo_wid; j++) {
                puyo[Puyo_wid-j-1][i] = temp[i * Puyo_wid + j];
            }
        }
    }
    //테트로미노일 때
    else{
        int temp[Block_hei*Block_wid];
        CopyTemp(temp);
        for(int i = 0; i < Block_hei; i++) {
            for(int j = 0; j < Block_wid; j++) {
                block[Block_wid-j-1][i] = temp[i * Block_wid + j];
            }
        }
    }
}

void Puyotet::rotate_CW(){//시계 회전
    //뿌요일 때
    if(unit){
        int temp[Puyo_hei*Puyo_wid];
        CopyTemp(temp);
        for(int i = 0; i < Puyo_hei; i++) {
            for(int j = 0; j < Puyo_wid; j++) {
                puyo[j][Puyo_hei-i-1] = temp[i * Puyo_wid + j];
            }
        }
    }
    //테트로미노일 때
    else{
        int temp[Block_hei*Block_wid];
        CopyTemp(temp);
        for(int i = 0; i < Block_hei; i++) {
            for(int j = 0; j < Block_wid; j++) {
                block[j][Block_hei-i-1] = temp[i * Block_wid + j];
            }
        }
    }
}

Board::Board(QWidget* parent) : QWidget(parent), block(new Puyotet), puyo(new Puyotet), gameover(new GameoverWindow), games(new Games)
{
    currentBlockX=3;
    currentPuyoX=3;
    currentBlockY=0;
    currentBlockY=0;
    //다음에 생성될 유닛이 뿌요인지 블럭인지 설정
    for(int i=0;i<5;i++){
        puyoORtet.append(QRandomGenerator::global()->bounded(2));//0 or 1
    }
    //다음 5개의 랜덤 블럭 모양
    for(int i=0;i<5;i++){
        nextBlockType.append(QRandomGenerator::global()->bounded(7));//0~6
    }
    //다음 5개의 랜덤 뿌요 색깔
    for(int i=0;i<10;i++){
        nextPuyoColor.append(QRandomGenerator::global()->bounded(5)+4);//4~8
    }
    games->SetState(false);
    games->SetScore(0);
    setFocusPolicy(Qt::StrongFocus);
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
    curunit=puyoORtet.front();//랜덤 유닛 저장
    puyoORtet.pop_front();//벡터의 맨 앞 값 삭제
    puyoORtet.append(QRandomGenerator::global()->bounded(2));//0 or 1추가
    if(curunit)//뿌요
        generateRandomPuyo();
    else//테트로미노
        generateRandomBlock();
}

Board::~Board(){
    delete block;
    delete puyo;
    delete games;
    delete gameover;
}

void Board::paintEvent(QPaintEvent* event){
    Q_UNUSED(event);
    QPainter painter(this);

    int cell_wid = 30;//셀 너비
    int cell_hei = 30;//셀 높이


    int x_offset = (width() - (Board_col * cell_wid)) / 2;
    int y_offset = (height() - (Board_row * cell_hei)) / 2;

    //보드 전체의 상태를 그림
    for (int i = 1; i < Board_row + 1; i++) {
        for (int j = 1; j < Board_col + 1; j++) {
            if(board[i][j]==2){
                painter.fillRect(x_offset + (j - 1) * cell_wid, y_offset + (i - 1) * cell_hei, cell_wid, cell_hei, Qt::gray);
                painter.setPen(Qt::black);
                painter.drawRect(x_offset + (j - 1) * cell_wid, y_offset + (i - 1) * cell_hei, cell_wid, cell_hei);
                continue;
            }
            QColor color;//뿌요 색
            bool drawEllipse = false;//뿌요를 그릴지 판단하는 변수
            switch (board[i][j]) {
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
            default://나머지는 빈칸처리
                color = Qt::white;
                drawEllipse = false;
                break;
            }
            painter.setPen(Qt::black);//테두리는 검은색
            if (drawEllipse) {//뿌요를 그릴 때
                painter.fillRect(x_offset + (j - 1) * cell_wid, y_offset + (i - 1) * cell_hei, cell_wid, cell_hei, Qt::white);
                painter.drawRect(x_offset + (j - 1) * cell_wid, y_offset + (i - 1) * cell_hei, cell_wid, cell_hei);
                //뿌요의 색을 따로 설정
                QPainterPath path;
                path.addEllipse(x_offset + (j - 1) * cell_wid, y_offset + (i - 1) * cell_hei, cell_wid, cell_hei);
                painter.fillPath(path, color);
                painter.drawPath(path);
            }
            else {
                painter.fillRect(x_offset + (j - 1) * cell_wid, y_offset + (i - 1) * cell_hei, cell_wid, cell_hei, Qt::white);
                painter.drawRect(x_offset + (j - 1) * cell_wid, y_offset + (i - 1) * cell_hei, cell_wid, cell_hei);
            }
        }
    }

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 14));
    painter.drawText(QRect(250 * 3 + 10 * Puyo_size, 5 * 2 + 4 * Puyo_size, Puyo_size * 6, Puyo_size * 6 + 800), Qt::AlignCenter, "current Score : " + QString::number(games->GetScore()));

    if(curunit){
        //현재 움직이는 뿌요를 그림
        for (int i = 0; i < Puyo_hei; i++) {
            for (int j = 0; j < Puyo_wid; j++) {
                int cellValue = puyo->getCell_puyo(i, j);
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
    }
    else{
        //현재 움직이는 블럭을 그림
        painter.setBrush(Qt::gray);
        for (int i = 0; i < Block_hei; ++i) {
            for (int j = 0; j < Block_wid; ++j) {
                if (block->getCell_block(i, j) == 2) {//블럭은 회색 사각형
                    painter.fillRect(x_offset + (currentBlockX + j - 1) * cell_wid, y_offset + (currentBlockY + i - 1) * cell_hei, cell_wid, cell_hei, Qt::gray);
                    painter.setPen(Qt::black);
                    painter.drawRect(x_offset + (currentBlockX + j - 1) * cell_wid, y_offset + (currentBlockY + i - 1) * cell_hei, cell_wid, cell_hei);
                }
            }
        }
    }

    //다음 나올 유닛 5개를 그림
    int rightMarginStart = Board_col * 130 + 20;//오른쪽 여백 시작 위치
    int topMargin = 30;//텍스트의 y축 시작 위치
    painter.setPen(Qt::white);
    painter.drawText(QRect(rightMarginStart, topMargin, Puyo_size * 30, Puyo_size * 5), Qt::AlignLeft, "Next Blocks or Puyos:");
    int unitSpace = 100;//유닛 간 간격

    for (int i = 0; i < 5; i++) {
        if (puyoORtet[i]) { // 다음 유닛이 뿌요라면
            int PuyoColor1 = nextPuyoColor[i * 2];
            int PuyoColor2 = nextPuyoColor[i * 2 + 1];
            int nextPuyo[Puyo_hei][Puyo_wid] = {0,};
            nextPuyo[0][1] = PuyoColor1;
            nextPuyo[1][1] = PuyoColor2;
            for (int row = 0; row < Puyo_hei; ++row) {
                for (int col = 0; col < Puyo_wid; ++col) {
                    if (nextPuyo[row][col] != 0) {
                        switch (nextPuyo[row][col]) {
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
                        QRect nextRect(rightMarginStart + col * 20, topMargin + 80 + i * unitSpace + row * 20, 20, 20);
                        painter.drawEllipse(nextRect); // 셀 경계 그리기
                    }
                }
            }
        } else { // 다음 유닛이 블럭이라면
            painter.setPen(Qt::black);
            painter. setBrush(Qt::gray);
            int blockType = nextBlockType[i];
            const int* nextBlock = block->BLOCKS[blockType];

            for (int row = 0; row < Block_hei; row++) {
                for (int col = 0; col < Block_wid; col++) {
                    if (nextBlock[row * Block_wid + col] != 0) {
                        QRect nextRect(rightMarginStart + col * 20, topMargin + 80 + i * unitSpace + row * 20, 20, 20);
                        painter.drawRect(nextRect); // 셀 경계 그리기
                    }
                }
            }
        }
    }
}

void Board::generateRandomBlock(){
    if(!games->GetState()){
        curunit=false;
        block->setUnit(false);
        currentBlockX=3;
        currentBlockY=0;
        currentBlockType = nextBlockType.front();//다음 블럭 타입의 맨 앞 블럭 생성
        nextBlockType.pop_front(); // 벡터에서 첫 번째 블록 제거
        nextBlockType.append(QRandomGenerator::global()->bounded(7)); // 벡터의 끝에 새로운 랜덤 블록 추가
        block->initialize_block(currentBlockType);
    }
}

void Board::generateRandomPuyo(){
    if(!games->GetState()){
        curunit=true;
        puyo->setUnit(true);
        currentPuyoX=3;
        currentPuyoY=0;
        currentPuyoColor1=nextPuyoColor.front();//다음 색상에서 하나 뽑음
        nextPuyoColor.pop_front();//해당 색상 삭제
        currentPuyoColor2=nextPuyoColor.front();//하나 더 뽑음
        nextPuyoColor.pop_front();//해당 색상 삭제
        nextPuyoColor.append(QRandomGenerator::global()->bounded(5)+4);//랜덤 색상 추가
        nextPuyoColor.append(QRandomGenerator::global()->bounded(5)+4);//랜덤 색상 추가
        puyo->initialize_puyo(currentPuyoColor1,currentPuyoColor2);//랜덤 색상 2개로 뿌요 초기화
    }
}

void Board::updateGameArea(){
    update();
}

void Board::moveDown(){
    if(curunit){//뿌요
        if(canMoveDown()){
            currentPuyoY+=1;
        }
        else{
            fixUnit();//뿌요 고정
            density();//밀도 적용
            gameOver();//게임 종료 판단
            removePuyo();//뿌요가 제거 가능한지 판단
            bool nextunit=puyoORtet.front();
            puyoORtet.pop_front();
            puyoORtet.append(QRandomGenerator::global()->bounded(2));
            if(nextunit)
                generateRandomPuyo();
            else
                generateRandomBlock();
        }
    }
    else{//테트로미노
        if(canMoveDown()){
            currentBlockY+=1;
        }
        else{
            fixUnit();//블럭 고정
            gameOver();//게임 종료 판단
            removeBlock();//블럭이 제거 가능한지 판단
            bool nextunit=puyoORtet.front();
            puyoORtet.pop_front();
            puyoORtet.append(QRandomGenerator::global()->bounded(2));
            if(nextunit)
                generateRandomPuyo();
            else
                generateRandomBlock();
        }
    }
}

void Board::moveLeft(){
    if(curunit){
        if(canMoveLeft()){
            currentPuyoX-=1;
        }
    }
    else{
        if(canMoveLeft()){
            currentBlockX-=1;
        }
    }
    updateGameArea();
}

void Board::moveRight(){
    if(curunit){
        if(canMoveRight()){
            currentPuyoX+=1;
        }
    }
    else{
        if(canMoveRight()){
            currentBlockX+=1;
        }
    }
    updateGameArea();
}

void Board::fixUnit(){
    if(curunit){
        for (int i = 0; i < Puyo_hei; i++) {
            for (int j = 0; j < Puyo_wid; j++) {
                if (puyo->getCell_puyo(i, j) != 0) {
                    board[currentPuyoY + i][currentPuyoX + j] = puyo->getCell_puyo(i, j);//보드에 뿌요의 값을 복사
                }
            }
        }
        gravity();//중력 적용
    }
    else{
        for (int i = 0; i < Block_hei; ++i) {
            for (int j = 0; j < Block_wid; ++j) {
                if (block->getCell_block(i, j) == 2) {
                    board[currentBlockY + i][currentBlockX + j] = 2;
                }
            }
        }
    }
}

bool Board::canMoveDown() const{
    if(!curunit){//블럭
        for (int i = 0; i < Block_hei; ++i) {
            for (int j = 0; j < Block_wid; ++j) {//블럭의 칸을 탐색
                if (block->getCell_block(i, j) == 2) {//2는 블럭이 존재하는 칸
                    int newX = currentBlockX + j;
                    int newY = currentBlockY + i + 1;//아래로 한칸 내린 위치
                    if (newY >= Board_row + 1 || board[newY][newX] != 0) {
                        return false;//뿌요테트의 판을 넘어가거나 다른 블럭 or 뿌요가 있다면 못움직임
                    }
                }
            }
        }
        return true;//아니라면 움직임
    }
    else{//뿌요
        for (int i = 0; i < Puyo_hei; ++i) {
            for (int j = 0; j < Puyo_wid; ++j) {//뿌요의 칸을 탐색
                if (puyo->getCell_puyo(i, j) != 0) {//뿌요가 존재하는 칸
                    int newX = currentPuyoX + j;
                    int newY = currentPuyoY + i + 1;//아래로 한칸 내린 위치
                    if (newY >= Board_row + 1 || board[newY][newX] != 0) {
                        return false;//뿌요테트의 판을 넘어가거나 다른 블럭 or 뿌요가 있다면 못움직임
                    }
                }
            }
        }
        return true;//아니라면 움직임
    }
}

bool Board::canMoveLeft() const{
    if(curunit){
        for (int i = 0; i < Puyo_hei; ++i) {
            for (int j = 0; j < Puyo_wid; ++j) {
                if (puyo->getCell_puyo(i, j) != 0) {//뿌요가 존재하는 칸만 탐색
                    int newX = currentPuyoX + j - 1;//왼쪽으로 한칸 이동한 위치
                    int newY = currentPuyoY + i;
                    if (newX <= 0 || board[newY][newX] != 0) {//뿌요테트판을 넘어가거나 다른 유닛이 있다면 못움직임
                        return false;
                    }
                }
            }
        }
        return true;//아니라면 움직임
    }
    else{
        for (int i = 0; i < Block_hei; ++i) {
            for (int j = 0; j < Block_wid; ++j) {
                if (block->getCell_block(i, j) == 2) {//블럭이 존재하는 칸만 탐색
                    int newX = currentBlockX + j - 1;//왼쪽으로 한칸 이동한 위치
                    int newY = currentBlockY + i;
                    if (newX <= 0 || board[newY][newX] != 0) {//뿌요테트판을 넘어가거나 다른 유닛이 있다면 못움직임
                        return false;
                    }
                }
            }
        }
        return true;//아니라면 움직임
    }
}

bool Board::canMoveRight() const{
    if(curunit){
        for (int i = 0; i < Puyo_hei; ++i) {
            for (int j = 0; j < Puyo_wid; ++j) {
                if (puyo->getCell_puyo(i, j) != 0) {//뿌요가 존재하는 칸만 탐색
                    int newX = currentPuyoX + j + 1;//오른쪽으로 한칸 이동한 위치
                    int newY = currentPuyoY + i;
                    if (newX >= Board_col + 1 || board[newY][newX] != 0) {//뿌요테트판을 넘어가거나 다른 유닛이 있다면 못움직임
                        return false;
                    }
                }
            }
        }
        return true;//아니라면 움직임
    }
    else{
        for (int i = 0; i < Block_hei; ++i) {
            for (int j = 0; j < Block_wid; ++j) {
                if (block->getCell_block(i, j) == 2) {//블럭이 존재하는 칸만 탐색
                    int newX = currentBlockX + j + 1;//오른쪽으로 한칸 이동한 위치
                    int newY = currentBlockY + i;
                    if (newX >= Board_col + 1 || board[newY][newX] != 0) {//뿌요테트판을 넘어가거나 다른 유닛이 있다면 못움직임
                        return false;
                    }
                }
            }
        }
        return true;//아니라면 움직임
    }
}

void Board::gravity(){
    for(int j=1; j<Board_col+1; j++){//열
        for (int i=Board_row-1; i>0; i--){//맨 밑에서 하나 위 행부터 맨 위 행까지
            if(board[i][j]==2)//블럭은 중력적용 X
                continue;
            int underrow=i+1;//뿌요의 바로 아래 행
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

void Board::removeBlock(){
    bool isRemove=false;//블럭이 삭제되었는지 판단하는 변수
    for(int i = Board_row; i > 0; i--){//Board_row부터 1까지 반복하므로 테트리스 판 상에서는 맨 밑줄부터 맨 윗줄까지 반복
        bool flag = true; // 한 줄이 모두 채워졌는지 확인하는 플래그
        for (int j = 1; j < Board_col + 1; j++) {
            if (board[i][j] == 0|| 4<=board[i][j] && board[i][j]<=8) {//빈칸이나 뿌요가 하나라도 있다면
                flag = false;
                break;
            }
        }
        if (flag) {//해당 줄이 모두 채워져 있다면 i번째 줄을 삭제하고 위의 모든 줄을 한 칸씩 내림
            isRemove=true;
            for (int k = i; k > 1; k--) {//해당 줄부터 가장 윗 줄까지 반복
                for (int j = 1; j < Board_col + 1; j++) {
                    board[k][j] = board[k - 1][j];//k-1번째줄을 k번째 줄에 저장하여 한 칸씩 내림
                }
            }
            for (int j = 1; j < Board_col + 1; j++) {
                board[1][j] = 0;//맨 윗줄은 0으로 초기화(빈칸)
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

void Board::removePuyo(){
    int dx[4] = {1, 0, -1, 0};//x좌표의 상하좌우
    int dy[4] = {0, 1, 0, -1}; //y좌표의 상하좌우

    while (1) {
        bool flag = false; // 종료 조건
        bool visit[Board_row + 2][Board_col + 2] = {0,}; // 해당 칸을 방문했는지 저장

        for (int i = 1; i < Board_row + 1; i++) {
            for (int j = 1; j < Board_col + 1; j++) {
                if (board[i][j] != 0 && board[i][j] != 2 && visit[i][j] == 0) {//빈칸, 블럭이 아니고 이미 방문한 좌표가 아니라면
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

void Board::rotate_CCW(){//반시계 회전
    if(curunit){
        int temp_block[Puyo_hei][Puyo_wid];
        puyo->rotate_CCW();//뿌요를 시계 방향으로 회전
        for (int i = 0; i < Puyo_hei; ++i) {
            for (int j = 0; j < Puyo_wid; ++j) {
                temp_block[i][j] = puyo->getCell_puyo(i, j);
            }
        }//회전시킨 뿌요를 temp_block에 저장
        bool canRotate = true;
        for (int i = 0; i < Puyo_hei; ++i) {
            for (int j = 0; j < Puyo_wid; ++j) {
                if (temp_block[i][j] != 0) {//뿌요가 존재하는 곳만 확인
                    if (board[currentPuyoY + i][currentPuyoX + j] != 0) {//회전한 곳에 유닛이 존재하거나, 판을 넘어가면
                        canRotate = false;
                        break;
                    }
                }
            }
        }
        // 유효하지 않다면 원래 모양으로 되돌림
        if (!canRotate) {
            puyo->rotate_CW();
        }
    }
    else{
        int temp_block[Block_hei][Block_wid];
        block->rotate_CCW();
        for (int i = 0; i < Block_hei; ++i) {
            for (int j = 0; j < Block_wid; ++j) {
                temp_block[i][j] = block->getCell_block(i, j);
            }
        }//회전시킨 블럭을 temp_block에 저장
        bool canRotate = true;
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
        // 유효하지 않다면 원래 블럭으로 되돌림
        if (!canRotate) {
            block->rotate_CW();
        }
    }
}

void Board::rotate_CW(){//시계 회전
    if(curunit){
        int temp_block[Puyo_hei][Puyo_wid];
        puyo->rotate_CW();//뿌요를 반시계 방향으로 회전
        for (int i = 0; i < Puyo_hei; ++i) {
            for (int j = 0; j < Puyo_wid; ++j) {
                temp_block[i][j] = puyo->getCell_puyo(i, j);
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
    else{
        int temp_block[Block_hei][Block_wid];
        block->rotate_CW();//블럭을 시계 방향으로 회전
        for (int i = 0; i < Block_hei; ++i) {
            for (int j = 0; j < Block_wid; ++j) {
                temp_block[i][j] = block->getCell_block(i, j);
            }
        }//회전시킨 블럭을 temp_block에 저장
        bool canRotate = true;
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
        // 유효하지 않다면 원래 블럭으로 되돌림
        if (!canRotate) {
            block->rotate_CCW();
        }
    }
}

void Board::gameOver(){
    bool flag=true;
    for(int i=2;i<=5;i++){
        if(board[1][i]!=0)//보드의 맨 윗줄 4칸(게임 종료 위치)에 유닛이 들어가면 게임 종료
            flag=false;
    }
    if(!flag){
        gameover->show();
        games->SetState(true);
    }
}

void Board::keyPressEvent(QKeyEvent* event){
    switch(event->key()){
    case Qt::Key_Left:
        emit _moveLeft();
        break;
    case Qt::Key_Right:
        emit _moveRight();
        break;
    case Qt::Key_Down:
        emit _moveDown();
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

void Board::density(){
    /*int between_puyo[Board_col][Board_row]={0,};//각 행은 게임판의 열, 각 열은 블록 사이에 끼인 뿌요의 정보
    int puyoline[Board_col]={0,};//중간에 낀 뿌요의 밑 행
    int puyocnt[Board_col]={0,};//뿌요 위에 존재하는 테트로미노의 개수

    for(int j=1;j<Board_col+1;j++){
        for(int i=Board_row;i>0;i--){
            if(board[i][j]==2){//블록이면
                puyoline[j-1]++;
            }
            else{//아니면
                break;
            }
        }
    }//한 열에서 뿌요 밑에 존재하는 블록의 갯수를 puyoline에 저장, 0인덱스는 1열, 1인덱스는 2열...

    for(int j=1;j<Board_col+1;j++){
        int k=0;
        for(int i=Board_row-puyoline[j-1];i>0;i--){//뿌요가 존재하는 바로 밑 행부터 시작
            if(board[i][j]==2)//블록을 만나면
                puyocnt[j-1]++;//뿌요 위에 존재하는 테트로미노의 개수 증가
            else if(board[i][j]==0)
                break;
            else{//뿌요를 만나면
                between_puyo[j-1][k]=board[i][j];//사이에 낀 뿌요 정보를 저장, 해당 뿌요를 0으로 만듦
                board[i][j]=0;
            }
        }
    }

    for(int j=1;j<Board_col+1;j++){
        for(int i=Board_row;i>0;i--){
            for(int k=0;k<Board_row;k++){
                if(between_puyo[j-1][k]!=0)
                    board[i][j]=between_puyo[j-1][k];//게임판에 가장 아래줄부터 사이에 끼어있던 뿌요로 초기화
            }
        }
    }

    int bet_puyo[Board_col]={0,};//보드의 각 열에 끼어있는 뿌요의 갯수
    for(int i=0;i<Board_col;i++){
        for(int j=0;j<Board_row;j++){
            if(between_puyo[i][j]!=0)
                bet_puyo[i]++;
        }
    }

    for(int j=1;j<Board_col+1;j++){
        for(int i=Board_row-bet_puyo[j-1];i>0;i--){
            board[i][j]=2;
        }
    }
    updateGameArea();*/
}





