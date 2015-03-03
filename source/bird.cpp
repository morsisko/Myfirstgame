#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <sstream>
#include <time.h>
#include <fstream>
#include "../headers/bird.h"
#include "../headers/pipes.h"
using namespace std;

int Bird::load_file() // metoda do ladowania grafik
{
    if (!texture.loadFromFile("img/flappy.png"))
        return -1;

    bird.setTexture(texture);

    if (!get_point_buffer.loadFromFile("sounds/get_point.ogg"))
        return -1;

    get_point.setBuffer(get_point_buffer);

    if (!go_up_buffer.loadFromFile("sounds/go_up.ogg"))
        return -1;

    go_up.setBuffer(go_up_buffer);

    if (!die_buffer.loadFromFile("sounds/die.ogg"))
        return -1;

    die.setBuffer(die_buffer);
}

void Bird::move_bird() // metoda poruszajaca spritem
{
    y+=speed;
    bird.setPosition(x, y);
}

string Bird::return_points()
{
    std::stringstream points_ss;
    points_ss << points;
    std::string points_to_view = points_ss.str();
    return points_to_view;
}

bool Bird::check_collision(Pipes pipes)
{

    if ((y < pipes.y_up+pipes.pipe_len && y > pipes.y_up) ||
            (y < pipes.y_down+pipes.pipe_len && y > pipes.y_down) ||
            (y+length < pipes.y_up+pipes.pipe_len && y + length > pipes.y_up) ||
            (y+length < pipes.y_down+pipes.pipe_len && y + length > pipes.y_down))
    {
        if (x + width > pipes.x && x + width < pipes.x + pipes.pipe_width)
            return true;

        if (y <= -40 || y >= 500)
            return true;
    }
    return false;
}

void Bird::over_chimney(Pipes &pipe)
{
    if (pipe.x <= -200) // warunek - jesli komin znajdzie sie na pozycji -200 to
    {
        pipe.x = 800; // nalezy przeniesc go na pozycje x = 800
        get_point.play();
        points += 1; // dodac punkt graczowi
        pipe.rand_chimneys(); // wylosowac nowe dlugosci kominow
        cout<<points<<std::endl;
    }
}
