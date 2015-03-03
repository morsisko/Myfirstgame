#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <sstream>
#include <time.h>
#include <fstream>
using namespace std;

class Results
{
    public:
    fstream results;
    string line;
    int best_result;

    void import_file();

    bool check(int actuall_result);

    void save();

    string return_points();
};
