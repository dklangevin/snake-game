#include "snake.h"

#define INIT_SNAKE_LEN 10

Snake::Snake(coord pos){
    // this->head = pos;
    // body.push_front((coord){pos.y, pos.x-3});
    // body.push_front((coord){pos.y, pos.x-2});
    // body.push_front((coord){pos.y, pos.x-1});
    InitSnake(pos, INIT_SNAKE_LEN);
};

void Snake::InitSnake(coord pos, int len){
    this->head = pos;
    coord segment = pos;
    // Create snake body (to the left of the head)
    for(int i=0; i<len-1; i++){
        --segment.x;
        body.push_back(segment);
    }
};

void Snake::AddSegment(coord pos){
    body.push_front(head);
    head = pos;
};

void Snake::RemoveSegment(){
    body.pop_back();
};

coord Snake::GetHeadPos(){
    return head;
};

list<coord> Snake::GetBody(){
    return body;
};

coord Snake::GetTailPos(){
    return body.back();
};