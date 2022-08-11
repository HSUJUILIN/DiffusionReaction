// Edit by Xurl
// Fisrt release: adding "class", "function: random number, ini_p, print array" 20210714 14:30
// Second release: adding "export text file" 20210714 16:30
// Third release: adding "calculate distance" and "export number of particles in regulated distance" 20210717 16:30
// Forth release: adding "sort by concentration" 20210718 17:00
// Fifth release: adding "fix probability of cell"
// Process:
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
#include <algorithm>
using namespace std;

// class "Particle"
class Particle
{
public:
    int id; // particle id
    int x_po;
    int y_po;
    int n_close; // how many points in range
    double fp;   // fission probability
};

// declare parameters
const int boxsize = 5;
const int NP = 5;
const int zone = 4; // remember this number must be "SQUARE" of distance
const int t_end = 10;
const string dir = "D:/google drive/cpp/BalloonSimulationData/";

// declare functions
int Random_num();
void ini_p(vector<Particle> &arr);
void print_array(const vector<Particle> &arr); // checking the random array exist or not
void write_in_txt(string filename, const int time, const vector<Particle> &arr);
int get_distance(int dx, int dy);
void judge_concentration(vector<Particle> &arr);
bool mycompare(Particle p1, Particle p2);
int sort_concentration(vector<Particle> &arr);

// main programme
int main()
{
    srand(time(NULL));
    vector<Particle> p_arr;
    ini_p(p_arr);
    printf("Initial Array is: \n");
    printf("id x_po y_po \n");
    print_array(p_arr); // prervent wasting too much time to create initial particles
    judge_concentration(p_arr);
    write_in_txt(dir, 0, p_arr);
    printf("Sorting result is: \n");
    printf("id n_close \n");
    sort_concentration(p_arr);
}

// function of getting random number(int) of time series i.e. range: (0,boxsixe) integer
int Random_num()
{
    int R;
    R = rand() % boxsize;
    return R + 1;
}

// give first particle of initial position
void ini_p(vector<Particle> &arr)
{
    Particle temp;
    for (auto i = 0; i < NP; i++)
    {
        temp.id = 1;
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

            temp.id = i + 1;
            arr.push_back(temp);
        }
    }
}

// function of print array
void print_array(const vector<Particle> &arr)
{
    for (auto k = 0; k < arr.size(); k++)
    {
        cout << arr[k].id << " " << arr[k].x_po << " " << arr[k].y_po << endl;
    }
}

// function of writting in text file
void write_in_txt(string filename, const int time, const vector<Particle> &arr)
{
    ofstream myfile;
    filename = filename + to_string(time) + ".txt";
    myfile.open(filename);

    myfile << "id x y number_of_too_close_particle" << endl;
    for (auto k = 0; k < arr.size(); k++)
    {
        myfile << arr[k].id << " " << arr[k].x_po << " " << arr[k].y_po << " " << arr[k].n_close << endl;
    }
    myfile.close();
}

// function of getting distance between "the one particle" and "the other particle"
int get_distance(int dx, int dy)
{
    int dr_2 = dx * dx + dy * dy;
    return dr_2;
}

// function of getting distance between "the one particle" and "the other particles"
void judge_concentration(vector<Particle> &arr)
{
    int dx, dy;
    for (auto i = 0; i < arr.size(); i++)
    {
        int count = 0;
        for (auto j = 0; j < arr.size(); j++)
        {
            if (i != j)
            {
                dx = arr[i].x_po - arr[j].x_po;
                dy = arr[i].y_po - arr[j].y_po;

                if (get_distance(dx, dy) <= zone)
                {
                    count += 1;
                }
            }
        }
        arr[i].n_close = count;
    }
}

// function of getting the concentration information and sort it
bool mycompare(Particle p1, Particle p2)
{
    return p1.n_close > p2.n_close;
}

int sort_concentration(vector<Particle> &arr)
{
    sort(arr.begin(), arr.end(), mycompare);
    for (auto i = 0; i < arr.size(); i++)
    {
        cout << arr[i].id << " " << arr[i].n_close << endl;
    }
}