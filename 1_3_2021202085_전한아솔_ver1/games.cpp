#include "games.h"

Games::Games() { }

Games::~Games(){ }

bool Games::GetState(){
    return State;
}

int Games::GetScore(){
    return Score;
}

void Games::SetState(bool state){
    this->State=state;
}

void Games::SetScore(int score){
    this->Score=score;
}
