#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include "game.h"

using namespace std;

int main() {
	Game *game = new Game();
	getch();
	delete game;
    return 0;
}