#ifndef SNAKE_H
#define SNAKE_H

#include <queue>
#include <list>

using namespace std;

namespace snake {
    enum Direction{
        up,
        right,
        down,
        left
    };
}

struct coord {
    int y, x;
};

class Snake{
    private:
        coord head;
        list<coord> body;
        int velocity;
    public:
        Snake(coord pos);
        void InitSnake(coord pos, int len);
        void AddSegment(coord pos);
        void RemoveSegment();
        coord GetHeadPos();
        list<coord> GetBody();
        coord GetTailPos();
};

#endif