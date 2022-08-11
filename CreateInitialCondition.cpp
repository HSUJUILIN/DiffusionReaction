// Edit by Xurl
// Fisrt release: adding "class", "function: random number, ini_p, print array" 20210714 14:30
// Second release: adding "export text file" 20210714 16:30
// Process : give particles random positions and do not overlap and output
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>
#include <limits>
#include <unistd.h>
#include <windows.h>
#include <cmath>
#include <string>
#include <fstream>
using namespace std;

// class "Particle"
class Particle
{
public:
    int x_po;
    int y_po;
};

// // class "Circle"
// class Circle
// {
// public:
//     int cx;
//     int cy;
//     int r;
// };

// declare parameters
const int boxsize = 5;
const double step = 0.1;
const int t_end = 10;
const int NP = 5;
const string dir = "D:/google drive/cpp/BalloonSimulationData/";

// declare functions
int Random_num();
void ini_p(vector<Particle> &arr);
void print_array(const vector<Particle> &arr);
void write_in_txt(string filename, const int time, const vector<Particle> &arr);

// main programme
int main()
{
    srand(time(NULL));
    vector<Particle> p_arr;
    ini_p(p_arr);
    print_array(p_arr);
    write_in_txt(dir, 0, p_arr);
}

// function of getting random number(int) of time series
int Random_num()
{
    int R;
    R = rand() % boxsize + 1.0; // create [0 ~ boxsize] random number
    return R;
}

// give first particle of initial position
void ini_p(vector<Particle> &arr)
{
    Particle temp;
    for (auto i = 0; i < NP; i++)
    {
        temp.x_po = Random_num();
        temp.y_po = Random_num();
        int isexisted = 0;

        if (arr.size() == 0)
        {
            arr.push_back(temp);
        }
        else
        {
            do // prevent there are multiple particles overlapping
            {
                temp.x_po = Random_num();
                temp.y_po = Random_num();
                for (auto j = 0; j < arr.size(); j++)
                {
                    if (temp.x_po == arr[j].x_po && temp.y_po == arr[j].y_po)
                    {
                        isexisted = 1;
                        break;
                    }
                }
            } while (isexisted == 1);
            arr.push_back(temp);
        }
    }
}

// function of print array
void print_array(const vector<Particle> &arr)
{
    for (auto k = 0; k < arr.size(); k++)
    {
        cout << arr[k].x_po << " " << arr[k].y_po << endl;
    }
}

// function of writting in text file
void write_in_txt(string filename, const int time, const vector<Particle> &arr)
{
    ofstream myfile;
    filename = filename + to_string(time) + ".txt";
    myfile.open(filename);

    myfile << "x y" << endl;
    for (auto k = 0; k < arr.size(); k++)
    {
        myfile << arr[k].x_po << " " << arr[k].y_po << endl;
    }
    myfile.close();
}