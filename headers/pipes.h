#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <sstream>
#include <time.h>
#include <fstream>
using namespace std;
class Bird;
class Pipes: public sf::Transformable // klasa kominow
{
public:
    sf::Texture texture_down_pipe;
    sf::Texture texture_up_pipe;
    sf::Sprite pipe_down;
    sf::Sprite pipe_up;
    int free_space = 150; // przerwa pomiedzy kominami
    float x = 800;
    float y_up = 100;
    float y_down = 100;
    float pipe_len = 375;
    float pipe_width = 150;
    float speed = 4.5; // predkosc przesuwania kominow

    Pipes(float xx); // konstruktor ktory ustala poczatkowa wartosc x pary kominow

    int load_file(); // ladowanie spritow kominow

    void rand_chimneys(); // losowanie pozycji kominow

    void move_chimneys(); // poruszenie para kominow


};
