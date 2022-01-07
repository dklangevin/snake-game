#ifndef GAME_H
#define GAME_H

#include <curses.h>
#include "snake.h"

#define HW_RATIO 2
#define BOARD_H 25
#define BOARD_W BOARD_H*HW_RATIO

class View{
    private:
        
    public:
        WINDOW *window;
        int max_y, max_x;
        int board_y0, board_x0;
        void InitWindow();
        void InitBoard();
        void Update(coord head, coord tail);
        void PrintInitSnake(coord head, list<coord> body);
        void PrintHead(coord head);
        void PrintTail(coord tail);
        void PrintTarget(coord target);
        void PrintGameOver();
        void PrintScore(int score);
        void PrintTitle();
        chtype GetInput();
};

class Game{
    private:
        Snake *snake;
        View *view;
        coord target;
        snake::Direction direction;
        snake::Direction prev_direction;
        int delay;
        int level;
    public:
        Game();
        ~Game();
        void Play();
        void ChangeDirection(snake::Direction d);
        coord GetNewHeadPos();
        coord GetNewTargetPos();
        void MoveSnake();
        coord MapCoord(int y, int x);
        void ProcessInput(chtype c);
        bool CheckForHit(coord target, coord head);
        bool CheckForDeath(coord head, list<coord> body);
};

#endif