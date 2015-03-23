#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <sstream>
#include <time.h>
#include <fstream>
#include "../headers/pipes.h"
#include "../headers/bird.h"
#include "../headers/results.h"
#include "../headers/game.h"
using namespace std;


int main()
{
    srand(time(NULL));
    Game game;
    game.runGame();
    return 0;
}
