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
using namespace std;

void restart(Bird &bird, Pipes &pipes, Pipes &pipes2, Results &results, bool &colision)
{
    bird.points = 0;
    bird.speed = 3.5;
    bird.y = 300;
    pipes.x = 800;
    pipes.speed = 4.5;
    pipes.rand_chimneys();
    pipes2.x = 1200;
    pipes2.speed = 4.5;
    pipes2.rand_chimneys();
    colision = false;
    results.import_file();

}

string setForm(int points)
{
    string form = "";

    if (points == 0 || points >= 5)
        form = " punktow";
    else if (points == 1)
        form = " punkt";
    else if (points >= 2 && points <= 4)
        form = " punkty";

    return form;
}
int main()
{
    srand(time(NULL));
    bool colision = false;
    Results results;
    sf::Font font;
    sf::Text text;
    sf::Text error;
    if (!font.loadFromFile("fonts/angrybirds-regular.ttf"))
        return -1;
    sf::Texture background;
    sf::Texture floor;
    if (!background.loadFromFile("img/background.png"))
        return -1;
    if (!floor.loadFromFile("img/floor.png"))
        return -1;
    sf::Sprite background_sprite; // sprite tla
    sf::Sprite floor_sprite; // sprite podlogi
    background_sprite.setTexture(background);
    background_sprite.setPosition(0,0); // ustawianie spritow
    floor_sprite.setTexture(floor);
    floor_sprite.setPosition(0, 550);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Flappy Me!"); // tworzenie glownego okna
    window.setFramerateLimit(60); // ustalenie limitu na 60 fps
    Bird bird; // tworzenie obiektu
    bird.load_file(); // ladowanie obrazka
    Pipes pipes(800); // ustalenie pozycji pierwszego komina
    Pipes pipes2(1200); // ustalenie pozycji drugiego i wyznaczenie przerw miedzy nimi (400 pikseli)
    pipes.load_file();
    pipes.rand_chimneys(); // losowanie wielkoscio kominow
    pipes2.load_file();
    pipes2.rand_chimneys(); // losowanie wielkosci kominow
    text.setFont(font);
    error.setFont(font);
    text.setPosition(400, 50);
    error.setPosition(5, 200);
    error.setCharacterSize(50);
    error.setColor(sf::Color::Red);
    results.import_file();
    while (window.isOpen()) // glowna petla
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                if (bird.speed != 0)
                {
                    bird.y-=40; // zmniejszanie wartosci y ptaka
                    bird.go_up.play();
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && bird.speed == 0)
            {
                restart(bird, pipes, pipes2, results, colision);
                cout <<"hmm?"<<endl;
            }

        }

        bird.over_chimney(pipes); // sprawdzanie czy
        bird.over_chimney(pipes2);
        if (bird.check_collision(pipes) == true ||
            bird.check_collision(pipes2) == true)
        {
            bird.speed = 0;
            pipes.speed = 0;
            pipes2.speed = 0;
            if (!colision)
            {
                colision = true;
                bird.die.play();
            }
            if (results.check(bird.points))
            {
                results.save();
            }
            error.setString("Przegrales! Zdobyles " + bird.return_points() + setForm(bird.points) + "\n" +
                            "Twoj najlepszy wynik to " + results.return_points() + setForm(results.best_result) + "\n" +
                            "Aby zagrac jeszcze raz wcisnij ENTER");
        }
        else
        {
             error.setString("");
        }
        text.setString(bird.return_points());
        bird.move_bird(); // ruch ptaka
        pipes.move_chimneys(); // ruch komina pierwszego
        pipes2.move_chimneys(); // ruch komina drugiego
        window.clear(); // czyszczenie okna
        window.draw(background_sprite); // rysowanie tla
        window.draw(pipes.pipe_up); // kominow
        window.draw(pipes.pipe_down);
        window.draw(pipes2.pipe_up);
        window.draw(pipes2.pipe_down);
        window.draw(floor_sprite); // podlogi
        window.draw(text);
        window.draw(bird.bird); // ptaka
        window.draw(error);
        window.display();
    }
    return 0;
}
