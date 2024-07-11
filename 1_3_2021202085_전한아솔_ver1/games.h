#ifndef GAMES_H
#define GAMES_H

class Games
{
public:
    Games();
    ~Games();
    bool GetState();
    void SetState(bool state);
    int GetScore();
    void SetScore(int score);
private:
    bool State;//fasle면 게임 진행중, true면 게임 종료
    int Score;
};

#endif // GAMES_H
