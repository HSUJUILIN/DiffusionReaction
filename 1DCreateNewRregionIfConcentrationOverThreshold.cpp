// Edit by Xurl
// This programme is copy 1 dimension version (SimulationWithChang_1d.cpp).                      20220308 10:36
// If concentration over the expasional threshold, adding region                       
// method 1: create more region at begin                                                         20220309 11:50
// 

// TO BE DONE :

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

// class "Region"
class Region
{
public:
    int region;     // region order
    double tho_old; // old concentration of the region
    double tho_new; // new concentration of the region
    double tho_max; // limitation concentration of the region
};

// declare parameters
const int TNR = 21;     // total number of region
const int NR = 11;     // number of region
const int t_end = 1000; // simulation time (unit: times)
const double D = 1.0;
const double dx = 1.0;      // inverse number
const double deltat = 0.0008; // time interval (unit: s)
const double initial_tho = 1.0; // initial concentration of the center region
const double thre = 0.8;    // threshold of concentration
const string dir = "D:/google drive/cpp/1DMacroBalloonModelData/";

// declare functions
double Random_num();
void cre_r(vector<Region> &arr);
void ini_r(vector<Region> &arr);
void print_array(const vector<Region> &arr);
void balloon_equation(vector<Region> &arr);
double diffusion_equation(double target, double d1x_right, double d1x_left);
double reaction_rate_equation(double tho_tar);
void chemical_potential_equation(vector<Region> &arr); // rule : limited concentration
void update_data(vector<Region> &arr);
void write_in_txt(string filename, const int time, const vector<Region> &arr);

// main programme
int main()
{
    srand(time(NULL));
    vector<Region> r_arr;
    cre_r(r_arr);
    ini_r(r_arr);
    r_arr[int(0.5*(TNR-1))].tho_new = 2.0;
    cout << "Setting Initial Condition"
         << endl;
    print_array(r_arr);
    write_in_txt(dir + "DiffusionAndReaction ", 0, r_arr);
    for (auto count = 0.0; count < t_end; count++)
    {
        cout << "Calculating the " << count + 1 << " step"
             << endl;
        update_data(r_arr);
        r_arr[int(0.5*(TNR-1))].tho_old = 2.0;
        balloon_equation(r_arr);
        print_array(r_arr);
        write_in_txt(dir + "DiffusionAndReaction ", count + 1, r_arr);
    }
}

// function of create random float number [0.0,1.0]
double Random_num()
{
    double R;
    R = rand() / (RAND_MAX + 1.0);
    return R;
}

// function of creating zero vector
void cre_r(vector<Region> &arr)
{
    Region temp;
    for (auto i = 0; i < TNR; i++)
    {
        temp.region = i + 1;
        temp.tho_old = 0;
        temp.tho_new = 0;
        temp.tho_max = thre;

        arr.push_back(temp);
    }
}

// function of setting initial region property
void ini_r(vector<Region> &arr)
{
    for (auto i = int(0.5*(TNR-NR))-1; i < TNR - int(0.5*(TNR-NR)); i++)
    {
        arr[i].region = i + 1;
        arr[i].tho_old = 0;
        arr[i].tho_new = initial_tho * Random_num();
        arr[i].tho_max = thre;
    }
}

// fixing all equation
void balloon_equation(vector<Region> &arr)
{
    double d1x_right, d1x_left;

    for (auto i = 0; i < arr.size(); i++)
    {
        if (arr[i].tho_old != arr[i].tho_max)
        {
            if (i - 1 < 0)
            {
                d1x_right = (arr[i + 1].tho_old - arr[i].tho_old) * dx;
                d1x_left = (0 - arr[i].tho_old) * dx;
            }
            else if (i + 1 > (arr.size() - 1))
            {
                d1x_right = (0 - arr[i].tho_old) * dx;
                d1x_left = (arr[i - 1].tho_old - arr[i].tho_old) * dx;
            }
            else if (i - 1 >= 0 or i + 1 <= (arr.size() - 1))
            {
                d1x_right = (arr[i + 1].tho_old - arr[i].tho_old) * dx;
                d1x_left = (arr[i - 1].tho_old - arr[i].tho_old) * dx;
            }

            arr[i].tho_new = diffusion_equation(arr[i].tho_old, d1x_right, d1x_left) + reaction_rate_equation(arr[i].tho_old);
        }
        else if (arr[i].tho_old == arr[i].tho_max)
        {
            arr[i].tho_new = arr[i].tho_old;
        }
    }
}

// calculate diffusion
double diffusion_equation(double target, double d1x_right, double d1x_left)
{
    return (D * deltat * (d1x_right + d1x_left) * dx) + target;
}

// calculate reaction
double reaction_rate_equation(double tho_tar)
{
    const double r = 0.05;
    return r * tho_tar * (1 * thre - tho_tar);
}

// update "Region"
void update_data(vector<Region> &arr)
{
    for (auto i = 0; i < arr.size(); i++)
    {
        arr[i].tho_old = arr[i].tho_new;
    }
}

// print array
void print_array(const vector<Region> &arr)
{
    for (auto i = 0; i < arr.size(); i++)
    {
        cout << arr[i].region << " " << arr[i].tho_new << endl;
    }
}

// function of writting in text file
void write_in_txt(string filename, const int time, const vector<Region> &arr)
{
    ofstream myfile;
    filename = filename + to_string(time) + ".txt";
    myfile.open(filename);

    myfile << "tho_new" << endl;
    for (auto i = 0; i < arr.size(); i++)
    {
        myfile << arr[i].region << "," << arr[i].tho_new << endl;
    }
    myfile.close();
}
