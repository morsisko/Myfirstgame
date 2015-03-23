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
using namespace std;

Pipes::Pipes(float xx) // konstruktor ktory ustala poczatkowa wartosc x pary kominow
{
    x = xx;
}

int Pipes::load_file() // ladowanie spritow kominow
{
    if (!texture_up_pipe.loadFromFile("img/pipe_up.png"))
        return -1;
    pipe_up.setTexture(texture_up_pipe);

    if (!texture_down_pipe.loadFromFile("img/pipe_down.png"))
        return -1;
    pipe_down.setTexture(texture_down_pipe);
    return 1;
}

void Pipes::rand_chimneys() // losowanie pozycji kominow
{
    y_up = (std::rand() % (150+free_space)) - (150+free_space);
    y_down = y_up + pipe_len + free_space;

}
void Pipes::move_chimneys() // poruszenie para kominow
{
    x-=speed;
    pipe_up.setPosition(x, y_up);
    pipe_down.setPosition(x, y_down);
}

