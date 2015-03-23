#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <time.h>
#include <fstream>
#include "../headers/results.h"
using namespace std;


int Results::import_file()
{
    results.open("score.txt", ios::in);
    if (results.good() == false)
    {
        cout << "Error! score.txt";
        return -1;
    }

    while(getline(results, line))
    {
        if (atoi(line.c_str()) % 128 == 0)
        best_result = atoi(line.c_str()) / 128;
        else
        {
            MessageBox(NULL,"Cheating! Go to score.txt and set it to 0!","ERROR",NULL);
            return -1;
        }
    }
    results.close();
        return 1;
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
