#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <sstream>
#include <time.h>
#include <fstream>
using namespace std;
class Pipes;
class Bird :public sf::Transformable
{
public:
    sf::SoundBuffer get_point_buffer;
    sf::SoundBuffer go_up_buffer;
    sf::SoundBuffer die_buffer;
    sf::Texture yellow_bird_texture;
    sf::Texture blue_bird_texture;
    sf::Sprite bird;
    sf::Sound get_point;
    sf::Sound go_up;
    sf::Sound die;
    float y = 300;
    float x = 30;
    float length = 50;
    float width = 50;
    int points = 0;
    float speed = 3.5; // predkosc opadania ptaka

    int load_file(); // metoda do ladowania grafik

    void move_bird();

    string return_points();

    bool check_collision(Pipes pipes);

    bool over_chimney(Pipes &pipe);

    void reset_bird();


};
