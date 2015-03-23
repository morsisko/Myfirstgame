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

Game::Game()
{
    state = END;
    if (bird.load_file()!= 1)
        return ;
    if (!font.loadFromFile("fonts/angrybirds-regular.ttf"))
        return ;
    if (!background.loadFromFile("img/background.png"))
        return ;
    if (!floor.loadFromFile("img/floor.png"))
        return ;
    if (results.import_file() != 1)
        return ;
    window.create(sf::VideoMode(800, 600), "Flappy Me!");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
    sounds = true;
    state = MENU;
}

void Game::runGame()
{
    while (state != END)
    {
        switch (state)
        {
        case MENU:
            menu();
            break;

        case GAME:
            single();
            break;

        case GAME_OVER:
            state = GAME;
            single();
            break;

        case SCORE:
            score();
            break;

        case OPTIONS:
            options();
            break;

        case END:
            break;
        }
    }
}

void Game::menu()
{
    sf::Text title("Flappy Bird", font, 80);
    title.setStyle(sf::Text::Bold);
    title.setColor(sf::Color::Red);

    title.setPosition(800/2-title.getGlobalBounds().width/2, 20);

    const int buttons = 4;

    sf::Text buttons_text[buttons];

    string str[] = {"Play", "Score", "Options", "Exit"};
    for (int i = 0; i < buttons; i++)
    {
        buttons_text[i].setFont(font);
        buttons_text[i].setCharacterSize(65);

        buttons_text[i].setString(str[i]);
        buttons_text[i].setPosition(800/2-buttons_text[i].getGlobalBounds().width/2, 150+i*120);
    }

    while (state == MENU)
    {
        sf::Vector2f mouse(sf::Mouse::getPosition(window));
        sf::Event event;

        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed&&
                    event.key.code == sf::Keyboard::Escape)
                state = END;


            else if (buttons_text[0].getGlobalBounds().contains(mouse)&&
                     sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                cout << "true";
                state = GAME;
            }
            else if (buttons_text[1].getGlobalBounds().contains(mouse)&&
                     (event.type == sf::Event::MouseButtonReleased) &&
                     (event.key.code == sf::Mouse::Left))
            {
                state = SCORE;
            }

            else if (buttons_text[2].getGlobalBounds().contains(mouse)&&
                     sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                state = OPTIONS;
            }

            else if (buttons_text[3].getGlobalBounds().contains(mouse)&&
                     sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                state = END;
            }
        }
        window.clear();

        for (int i = 0; i<buttons; i++)
        {
            if (buttons_text[i].getGlobalBounds().contains(mouse))
                buttons_text[i].setColor(sf::Color::Yellow);
            else
                buttons_text[i].setColor(sf::Color::White);
        }

        for (int i = 0; i<buttons; i++)
        {
            window.draw(buttons_text[i]);
        }
        window.draw(title);
        window.display();
    }
}

void Game::single()
{
    bird.reset_bird();
    bool colision = false;
    Pipes pipes(800); // ustalenie pozycji pierwszego komina
    Pipes pipes2(1200); // ustalenie pozycji drugiego i wyznaczenie przerw miedzy nimi (400 pikseli)
    background_sprite.setTexture(background);
    background_sprite.setPosition(0,0); // ustawianie spritow
    floor_sprite.setTexture(floor);
    floor_sprite.setPosition(0, 550);
    pipes.rand_chimneys(); // losowanie wielkoscio kominow
    pipes2.rand_chimneys(); // losowanie wielkosci kominow
    text.setFont(font);
    error.setFont(font);
    text.setPosition(400, 50);
    error.setPosition(5, 200);
    error.setCharacterSize(50);
    error.setColor(sf::Color::Red);
    if (pipes.load_file()!= 1)
        state = END;
    if (pipes2.load_file()!= 1)
        state = END;
    if (results.import_file() != 1)
        state = END;

    while (state == GAME) // glowna petla
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
                    if (sounds) bird.go_up.play();
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && bird.speed == 0)
            {
                state = GAME_OVER;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && bird.speed == 0)
            {
                state = MENU;
            }

        }

        if (bird.over_chimney(pipes))
        {
            if (sounds) bird.get_point.play();
        }
        if (bird.over_chimney(pipes2))
        {
            if (sounds) bird.get_point.play();
        }
        if (bird.check_collision(pipes) == true ||
                bird.check_collision(pipes2) == true)
        {
            bird.speed = 0;
            pipes.speed = 0;
            pipes2.speed = 0;
            if (!colision)
            {
                colision = true;
                if (sounds) bird.die.play();
            }
            if (results.check(bird.points))
            {
                results.save();
            }
            error.setString("Przegrales! Zdobyles " + bird.return_points() + setForm(bird.points) + "\n" +
                            "Twoj najlepszy wynik to " + results.return_points() + setForm(results.best_result) + "\n" +
                            "Aby zagrac jeszcze raz wcisnij ENTER \n" +
                            "Aby wróciæ do menu wciœnij ESC.");
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
}

void Game::score()
{
    sf::Text title("Score info", font, 80);
    title.setStyle(sf::Text::Bold);

    title.setPosition(800/2-title.getGlobalBounds().width/2, 20);

    const int buttons = 2;

    sf::Text buttons_text[buttons];

    string str[] = {"Your best score = " + results.return_points(), "Return to menu"};
    for (int i = 0; i < buttons; i++)
    {
        buttons_text[i].setFont(font);
        buttons_text[i].setCharacterSize(65);

        buttons_text[i].setString(str[i]);
        buttons_text[i].setPosition(800/2-buttons_text[i].getGlobalBounds().width/2, 250+i*120);
    }

    while (state == SCORE)
    {
        sf::Vector2f mouse(sf::Mouse::getPosition(window));
        sf::Event event;

        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed&&
                    event.key.code == sf::Keyboard::Escape)
                state = MENU;

            else if (buttons_text[1].getGlobalBounds().contains(mouse)&&
                     (event.type == sf::Event::MouseButtonReleased) &&
                     (event.key.code == sf::Mouse::Left))
            {
                state = MENU;
            }
        }
        window.clear();

        for (int i = 0; i<buttons; i++)
        {
            if (buttons_text[i].getGlobalBounds().contains(mouse))
                buttons_text[i].setColor(sf::Color::Yellow);
            else
                buttons_text[i].setColor(sf::Color::White);
        }

        for (int i = 0; i<buttons; i++)
        {
            window.draw(buttons_text[i]);
        }
        window.draw(title);
        window.display();
    }
}

void Game::options()
{
    sf::Text title("Options", font, 80);
    title.setStyle(sf::Text::Bold);

    sf::Texture yellow_bird_texture;
    sf::Sprite yellow_bird;
    if (!yellow_bird_texture.loadFromFile("img/flappy.png"))
    {
        state = END;
        return ;
    }
    yellow_bird.setTexture(yellow_bird_texture);

    sf::Texture blue_bird_texture;
    sf::Sprite blue_bird;
    if (!blue_bird_texture.loadFromFile("img/flappy2.png"))
    {
        state = END;
        return ;
    }
    blue_bird.setTexture(blue_bird_texture);

    yellow_bird.setPosition(170, 250);
    blue_bird.setPosition(620, 250);

    title.setPosition(800/2-title.getGlobalBounds().width/2, 20);

    const int buttons = 4; // how_much buttons

    sf::Text buttons_text[buttons];

    string str[] = {"Yellow bird" , "Blue bird", "Return to menu", "Sounds: ON"};
    for (int i = 0; i < buttons; i++)
    {
        buttons_text[i].setFont(font);
        buttons_text[i].setCharacterSize(65);

        buttons_text[i].setString(str[i]);

    }
    buttons_text[0].setPosition(50, 150);
    buttons_text[1].setPosition(500, 150);
    buttons_text[2].setPosition(800/2-buttons_text[2].getGlobalBounds().width/2, 490);
    buttons_text[3].setPosition(250, 350);

    if (sounds)
    {
        buttons_text[3].setColor(sf::Color::Green);
        buttons_text[3].setString("Sounds: ON");
    }
    else
    {
        buttons_text[3].setColor(sf::Color::Red);
        buttons_text[3].setString("Sounds: OFF");
    }



    while (state == OPTIONS)
    {
        sf::Vector2f mouse(sf::Mouse::getPosition(window));
        sf::Event event;

        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed&&
            event.key.code == sf::Keyboard::Escape)
                state = MENU;

            else if ((buttons_text[0].getGlobalBounds().contains(mouse) || yellow_bird.getGlobalBounds().contains(mouse)) &&
            (event.type == sf::Event::MouseButtonReleased) &&
            (event.key.code == sf::Mouse::Left))
            {
                bird.bird.setTexture(bird.yellow_bird_texture);

            }

            else if ((buttons_text[1].getGlobalBounds().contains(mouse) || blue_bird.getGlobalBounds().contains(mouse)) &&
                     (event.type == sf::Event::MouseButtonReleased) &&
                     (event.key.code == sf::Mouse::Left))

            {
                bird.bird.setTexture(bird.blue_bird_texture);
            }
            else if ((buttons_text[2].getGlobalBounds().contains(mouse)) &&
                     (event.type == sf::Event::MouseButtonReleased) &&
                     (event.key.code == sf::Mouse::Left))
            {
                state = MENU;
            }

            else if ((buttons_text[3].getGlobalBounds().contains(mouse)) &&
                     event.type == sf::Event::MouseButtonReleased &&
                     event.key.code == sf::Mouse::Left)
            {
                if (sounds)
                {
                    buttons_text[3].setString("Sounds: OFF");
                    buttons_text[3].setColor(sf::Color::Red);
                    sounds = false;
                }

                else
                {
                    buttons_text[3].setString("Sounds: ON");
                    buttons_text[3].setColor(sf::Color::Green);
                    sounds = true;
                }

            }

        }
        window.clear();

        for (int i = 0; i<buttons - 1; i++) // because i dont want sounds button with yellow color
        {
            if (buttons_text[i].getGlobalBounds().contains(mouse))
                buttons_text[i].setColor(sf::Color::Yellow);
            else
                buttons_text[i].setColor(sf::Color::White);
        }

        for (int i = 0; i<buttons; i++)
        {
            window.draw(buttons_text[i]);
        }
        window.draw(title);
        window.draw(yellow_bird);
        window.draw(blue_bird);
        window.display();
    }
}

