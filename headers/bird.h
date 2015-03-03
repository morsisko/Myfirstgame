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
class Bird :public sf::Transformable // klasa glownego elementu w grze - ptaka
{
public:
    sf::SoundBuffer get_point_buffer;
    sf::SoundBuffer go_up_buffer;
    sf::SoundBuffer die_buffer;
    sf::Texture texture;
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

    void move_bird(); // metoda poruszajaca spritem

    string return_points();

    bool check_collision(Pipes pipes);

    void over_chimney(Pipes &pipe);


};
