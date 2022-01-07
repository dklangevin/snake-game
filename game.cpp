#include <thread>
#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include "game.h"

using namespace std;

#define BODY_SEGMENT 'o'
#define CLEAR ' '
#define TARGET '*'

Game::Game(){
    view = new View;
    view->InitWindow();
    view->InitBoard();
    view->PrintTitle();
    int choice;
    std::cin >> choice;
    direction = snake::right;
    prev_direction = snake::up;
    delay = 150;
    level = 0;
    int y, x;
    getmaxyx(stdscr, y, x);
    snake = new Snake(MapCoord(BOARD_H/2, BOARD_W/2));
    Play();
};

Game::~Game(){
    endwin();
    delete snake;
    delete view;
};

coord Game::GetNewTargetPos(){
    int y = rand()%(BOARD_H-2) + 1;
    int x = rand()%(BOARD_W-2) + 1;
    return MapCoord(y, x);
}

void Game::Play(){
    srand(time(NULL));
    int y, x;
    coord head, tail;
    view->PrintScore(level);
    view->PrintInitSnake(snake->GetHeadPos(), snake->GetBody());
    target = GetNewTargetPos();
    view->PrintTarget(target);
    bool alive = true;
    while(alive){
        this_thread::sleep_for(chrono::milliseconds(delay));
        ProcessInput(view->GetInput());
        if(CheckForDeath(snake->GetHeadPos(), snake->GetBody())){
            alive = false;
        }
        else if(CheckForHit(target, snake->GetHeadPos())){
            target = GetNewTargetPos();
            view->PrintTarget(target);
            delay = max(1, delay-2);
            level++;
            view->PrintScore(level);
        }
        else{
            tail = snake->GetTailPos();
            snake->RemoveSegment();
        }
        snake->AddSegment(GetNewHeadPos());
        head = snake->GetHeadPos();
        view->Update(head, tail);
    }
    this_thread::sleep_for(chrono::milliseconds(750));
    view->PrintGameOver();
    while(view->GetInput() != -1){
        this_thread::sleep_for(chrono::milliseconds(250));
    }
}

coord Game::GetNewHeadPos(){
    coord new_head = snake->GetHeadPos();
    int y = new_head.y;
    int x = new_head.x;
    switch(direction){
        case snake::up:
            --new_head.y;
            if(new_head.y <= view->board_y0){
                new_head.y = MapCoord(BOARD_H-2, new_head.x).y;
            }
            break;
        case snake::right:
            ++new_head.x;
            if(new_head.x > MapCoord(new_head.y, BOARD_W-2).x){
                new_head.x = MapCoord(new_head.y, 1).x;
            }
            break;
        case snake::down:
            ++new_head.y;
            if(new_head.y > MapCoord(BOARD_H-2, new_head.x).y){
                new_head.y = MapCoord(1, new_head.x).y;
            }
            break;
        case snake::left:
            --new_head.x;
            if(new_head.x <= view->board_x0){
                new_head.x = MapCoord(new_head.y, BOARD_W-2).x;
            }
            break;
    };
    return new_head;
}

void Game::ChangeDirection(snake::Direction d){
    prev_direction = direction;
    direction = d;
};

coord Game::MapCoord(int y, int x){
    coord c = {view->board_y0+y, view->board_x0+x};
    return c;
};

void Game::ProcessInput(chtype c){
    switch(c){
        case KEY_UP:
            if(direction != snake::down){
                ChangeDirection(snake::up);
            }
            break;
        case KEY_RIGHT:
            if(direction != snake::left){
                ChangeDirection(snake::right);
            }
            break;
        case KEY_DOWN:
            if(direction != snake::up){
                ChangeDirection(snake::down);
            }
            break;
        case KEY_LEFT:
            if(direction != snake::right){
                ChangeDirection(snake::left);
            }
            break;
    }
}

bool Game::CheckForHit(coord target, coord head){
    return (target.y == head.y && target.x == head.x);
}

bool Game::CheckForDeath(coord head, list<coord> body){
    list<coord>::iterator segment;
    for(segment = body.begin(); segment!=body.end(); ++segment){
        if(head.y == segment->y && head.x == segment->x){
            return true;
        }
    }
    return false;
}

/*
void Game::MoveSnake(){
    switch(snake->direction){
        case 'n':
            snake->pos.y = snake->pos.y - snake->velocity;
            if(snake->pos.y == 0){
                pos.y = BOARD_H - 2;
            }
            break;
        case 'e':
            pos.x = pos.x + velocity;
            if(pos.x == BOARD_W - 1){
                pos.x = 1;
            }
            break;
        case 's':
            pos.y = pos.y + velocity;
            if(pos.y == BOARD_H - 1){
                pos.y = 1;
            }
            break;
        case 'w':
            pos.x = pos.x - velocity;
            if(pos.x == 0){
                pos.x = BOARD_W - 2;
            }
            break;
    }
};
*/

void View::InitWindow(){
    initscr();
    refresh();

    getmaxyx(stdscr, max_y, max_x);
    board_y0 = (max_y-BOARD_H)/2;
    board_x0 = (max_x-BOARD_W)/2;
    window = newwin(BOARD_H, BOARD_W, board_y0, board_x0);

    cbreak(); 
    nodelay(window, TRUE);
    noecho();
    keypad(window, TRUE);
    curs_set(0);
};

void View::InitBoard(){
    box(window, 0 , 0); 
    wrefresh(window);
};

void View::Update(coord head, coord tail){
    PrintHead(head);
    PrintTail(tail);
    refresh();
};

void View::PrintInitSnake(coord head, list<coord> body){
    for(int y=1; y<BOARD_H-1; ++y){
        for(int x=1; x<BOARD_W-1; ++x){
            mvaddch(board_y0+y, board_x0+x, CLEAR);
        } 
    }
    mvaddch(head.y, head.x, BODY_SEGMENT);
    coord segment = head;
    for(int i=0; i<body.size()-1; i++){
        --segment.x;
        mvaddch(segment.y, segment.x, BODY_SEGMENT);
    };
    refresh();
}

void View::PrintHead(coord head){
    mvaddch(head.y, head.x, BODY_SEGMENT);
};

void View::PrintTail(coord tail){
    mvaddch(tail.y, tail.x, CLEAR);
};

void View::PrintTarget(coord target){
    mvaddch(target.y, target.x, TARGET);
};

void View::PrintScore(int score){
    int y = board_y0 + BOARD_H;
    char str[4];
    sprintf(str, "%d", min(999, score+1));
    mvaddstr(y, board_x0+1, "Score");
    mvaddstr(y, board_x0+BOARD_W-4, str);

}

chtype View::GetInput(){
    return wgetch(window);
}

void View::PrintGameOver(){
    coord pos;
    for(int y=1; y<BOARD_H-1; ++y){
        for(int x=1; x<BOARD_W-1; ++x){
            mvaddch(board_y0+y, board_x0+x, CLEAR);
        } 
    }
    mvaddstr(board_y0+BOARD_H/2, board_x0+(BOARD_W-8)/2, "Game Over");
}

void View::PrintTitle(){
    int y = board_y0+BOARD_H/2-1;
    int x = board_x0+(BOARD_W-4)/2;
    mvaddstr(y, x, "SNAKE");
    y = board_y0+BOARD_H/2;
    x = board_x0+(BOARD_W-24)/2;
    mvaddstr(y, x, "(Press any key to start)");
    refresh();
}



