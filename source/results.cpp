#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <sstream>
#include <time.h>
#include <fstream>
#include "../headers/results.h"
using namespace std;


void Results::import_file()
{
    results.open("score.txt", ios::in);
    if (results.good() == false)
    {
        cout << "Error! score.txt";
        exit(0);
    }

    while(getline(results, line))
    {
        best_result = atoi(line.c_str()) / 128;
    }
    results.close();
}

bool Results::check(int actuall_result)
{
    if (best_result < actuall_result)
    {
        best_result = actuall_result;
        return true;
    }
    return false;
}

void Results::save()
{
    results.open("score.txt", ios::out);
    results<<(best_result * 128);
    results.close();

}

string Results::return_points()
{
    stringstream points_ss;
    points_ss << best_result;
    string points_to_view = points_ss.str();
    return points_to_view;
}
