// Edit by Xurl
// This programme is copy 1 dimension version (SimulationWithChang_1d.cpp)                       20210915 11:00
// Fisrt release: changing vector to 2 dimension (2d vector) in function "ini_r"                 20210915 20:00
// Second release: fixing all function except function "balloon equation"                        20210918 23:30
// Third release: fixing function "balloon equation"                                             20210922 18:00
// Fourth release: limit initial condition to make concentration of center region is the largest 20220308 18:22

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
    int region_row; // region row order
    int region_col; // region column order
    double tho_old; // old concentration of the region
    double tho_new; // new concentration of the region
    double tho_max; // limitation concentration of the region
};

// declare parameters
const int TNR = 121;        // total number of region
const int TROW = 11;      // number of total row
const int TCOL = 11;      // number of total col
const int NR = 25;      // number of region
const int ROW = 5;      // number of row
const int COL = 5;      // number of col
const int t_end = 1000; // simulation time (unit: times)
const double D = 1.0;
const double dx = 1.0;       // inverse number
const double dy = 1.0;       // inverse number
const double deltat = 0.0001; // time interval (unit: s)
const double initial_tho = 1.0; // initial concentration of center region
const double thre = 1.2;     // threshold of concentration
const string dir = "D:/google drive/cpp/2DMacroBalloonModelData/";

// declare functions
double Random_num();
void cre_r(vector<vector<Region>> &arr);
void ini_r(vector<vector<Region>> &arr);
void print_array(const vector<vector<Region>> &arr);
void balloon_equation(vector<vector<Region>> &arr);
double diffusion_equation(double target, double d1x_right, double d1x_left, double d1y_up, double d1y_bottom);
double reaction_rate_equation(double tho_tar);
// void chemical_potential_equation(vector<Region> &arr); // rule : limited concentration
void update_data(vector<vector<Region>> &arr);
void write_in_txt(string filename, const int time, const vector<vector<Region>> &arr);

// main programme
int main()
{
    srand(time(NULL));
    vector<vector<Region>> r_arr;
    cout << "Setting Initial Condition"
         << endl;
    cre_r(r_arr);
    ini_r(r_arr);
    r_arr[int(0.5*(TROW-1))][int(0.5*(TCOL-1))].tho_new = 2.0;
    print_array(r_arr);
    write_in_txt(dir + "DiffusionAndReaction ", 0, r_arr);
    for (auto count = 0.0; count < t_end; count++)
    {
        cout << "Calculating the " << count + 1 << " step"
             << endl;
        update_data(r_arr);
        r_arr[int(0.5*(TROW-1))][int(0.5*(TCOL-1))].tho_old = 2.0;
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

// function of creating region
void cre_r(vector<vector<Region>> &arr)
{
    Region temp;
    for (auto i = 0; i < TROW; i++)
    {
        vector<Region> arr_row;
        for (auto j = 0; j < TCOL; j++)
        {
            temp.region_row = i + 1;
            temp.region_col = j + 1;
            temp.tho_old = 0;
            temp.tho_new = 0;
            temp.tho_max = thre;
            arr_row.push_back(temp);
        }
        arr.push_back(arr_row);
    }
}

// function of setting initial region property
void ini_r(vector<vector<Region>> &arr)
{
    for (auto i = int(0.5*(TROW-ROW)); i < TROW - int(0.5*(TROW-ROW)); i++)
    {
        for (auto j = int(0.5*(TCOL-COL)); j < TCOL - int(0.5*(TCOL-COL)); j++)
        {
            arr[i][j].region_row = i + 1;
            arr[i][j].region_col = j + 1;
            arr[i][j].tho_old = 0;
            arr[i][j].tho_new = initial_tho * Random_num();
            arr[i][j].tho_max = thre;
        }
    }
}

// fixing all equation
void balloon_equation(vector<vector<Region>> &arr)
{
    double d1x_right, d1x_left, d1y_up, d1y_bottom;

    for (auto i = 0; i < arr.size(); i++)
    {
        if (i - 1 < 0) // there is not region at left side
        {
            for (auto j = 0; j < arr[i].size(); j++)
            {
                if (arr[i][j].tho_old != arr[i][j].tho_max)
                {
                    if (j - 1 < 0)
                    {
                        d1x_right = (arr[i + 1][j].tho_old - arr[i][j].tho_old) * dx;
                        d1x_left = (0 - arr[i][j].tho_old) * dx;
                        d1y_up = (0 - arr[i][j].tho_old) * dy;
                        d1y_bottom = (arr[i][j + 1].tho_old - arr[i][j].tho_old) * dy;
                    }
                    else if (j + 1 > (arr[i].size() - 1))
                    {
                        d1x_right = (arr[i + 1][j].tho_old - arr[i][j].tho_old) * dx;
                        d1x_left = (0 - arr[i][j].tho_old) * dx;
                        d1y_up = (arr[i][j - 1].tho_old - arr[i][j].tho_old) * dy;
                        d1y_bottom = (0 - arr[i][j].tho_old) * dy;
                    }
                    else if (j - 1 >= 0 or j + 1 <= (arr.size() - 1))
                    {
                        d1x_right = (arr[i + 1][j].tho_old - arr[i][j].tho_old) * dx;
                        d1x_left = (0 - arr[i][j].tho_old) * dx;
                        d1y_up = (arr[i][j - 1].tho_old - arr[i][j].tho_old) * dy;
                        d1y_bottom = (arr[i][j + 1].tho_old - arr[i][j].tho_old) * dy;
                    }
                    arr[i][j].tho_new = diffusion_equation(arr[i][j].tho_old, d1x_right, d1x_left, d1y_up, d1y_bottom) + reaction_rate_equation(arr[i][j].tho_old);
                }
                else if (arr[i][j].tho_old == arr[i][j].tho_max)
                {
                    arr[i][j].tho_new = arr[i][j].tho_old;
                }
            }
        }
        else if (i + 1 > (arr.size() - 1)) // there is not region at right side
        {
            for (auto j = 0; j < arr[i].size(); j++)
            {
                if (arr[i][j].tho_old != arr[i][j].tho_max)
                {
                    if (j - 1 < 0)
                    {
                        d1x_right = (0 - arr[i][j].tho_old) * dx;
                        d1x_left = (arr[i - 1][j].tho_old - arr[i][j].tho_old) * dx;
                        d1y_up = (0 - arr[i][j].tho_old) * dy;
                        d1y_bottom = (arr[i][j + 1].tho_old - arr[i][j].tho_old) * dy;
                    }
                    else if (j + 1 > (arr[i].size() - 1))
                    {
                        d1x_right = (0 - arr[i][j].tho_old) * dx;
                        d1x_left = (arr[i - 1][j].tho_old - arr[i][j].tho_old) * dx;
                        d1y_up = (arr[i][j - 1].tho_old - arr[i][j].tho_old) * dy;
                        d1y_bottom = (0 - arr[i][j].tho_old) * dy;
                    }
                    else if (j - 1 >= 0 or j + 1 <= (arr.size() - 1))
                    {
                        d1x_right = (0 - arr[i][j].tho_old) * dx;
                        d1x_left = (arr[i - 1][j].tho_old - arr[i][j].tho_old) * dx;
                        d1y_up = (arr[i][j - 1].tho_old - arr[i][j].tho_old) * dy;
                        d1y_bottom = (arr[i][j + 1].tho_old - arr[i][j].tho_old) * dy;
                    }
                    arr[i][j].tho_new = diffusion_equation(arr[i][j].tho_old, d1x_right, d1x_left, d1y_up, d1y_bottom) + reaction_rate_equation(arr[i][j].tho_old);
                }
                else if (arr[i][j].tho_old == arr[i][j].tho_max)
                {
                    arr[i][j].tho_new = arr[i][j].tho_old;
                }
            }
        }
        else if (i - 1 >= 0 or i + 1 <= (arr.size() - 1))
        {
            for (auto j = 0; j < arr[i].size(); j++)
            {
                if (arr[i][j].tho_old != arr[i][j].tho_max)
                {
                    if (j - 1 < 0)
                    {
                        d1x_right = (arr[i + 1][j].tho_old - arr[i][j].tho_old) * dx;
                        d1x_left = (arr[i - 1][j].tho_old - arr[i][j].tho_old) * dx;
                        d1y_up = (0 - arr[i][j].tho_old) * dy;
                        d1y_bottom = (arr[i][j + 1].tho_old - arr[i][j].tho_old) * dy;
                    }
                    else if (j + 1 > (arr[i].size() - 1))
                    {
                        d1x_right = (arr[i + 1][j].tho_old - arr[i][j].tho_old) * dx;
                        d1x_left = (arr[i - 1][j].tho_old - arr[i][j].tho_old) * dx;
                        d1y_up = (arr[i][j - 1].tho_old - arr[i][j].tho_old) * dy;
                        d1y_bottom = (0 - arr[i][j].tho_old) * dy;
                    }
                    else if (j - 1 >= 0 or j + 1 <= (arr.size() - 1))
                    {
                        d1x_right = (arr[i + 1][j].tho_old - arr[i][j].tho_old) * dx;
                        d1x_left = (arr[i - 1][j].tho_old - arr[i][j].tho_old) * dx;
                        d1y_up = (arr[i][j - 1].tho_old - arr[i][j].tho_old) * dy;
                        d1y_bottom = (arr[i][j + 1].tho_old - arr[i][j].tho_old) * dy;
                    }
                    arr[i][j].tho_new = diffusion_equation(arr[i][j].tho_old, d1x_right, d1x_left, d1y_up, d1y_bottom) + reaction_rate_equation(arr[i][j].tho_old);
                }
                else if (arr[i][j].tho_old == arr[i][j].tho_max)
                {
                    arr[i][j].tho_new = arr[i][j].tho_old;
                }
            }
        }
    }
}

// calculate diffusion
double diffusion_equation(double target, double d1x_right, double d1x_left, double d1y_up, double d1y_bottom)
{
    return (D * deltat * ((d1x_right + d1x_left) * dx + (d1y_up + d1y_bottom) * dy)) + target;
}

// calculate reaction
double reaction_rate_equation(double tho_tar)
{
    const double r = 0.05;
    return r * tho_tar * (1 * thre - tho_tar);
}

// update "Region"
void update_data(vector<vector<Region>> &arr)
{
    for (auto i = 0; i < arr.size(); i++)
    {
        for (auto j = 0; j < arr[i].size(); j++)
        {
            arr[i][j].tho_old = arr[i][j].tho_new;
        }
    }
}

// print array
void print_array(const vector<vector<Region>> &arr)
{
    for (auto i = 0; i < arr.size(); i++)
    {
        for (auto j = 0; j < arr[i].size(); j++)
        {
            cout << arr[i][j].region_row << " " << arr[i][j].region_col << " " << arr[i][j].tho_new << endl;
        }
    }
}

// function of writting in text file
void write_in_txt(string filename, const int time, const vector<vector<Region>> &arr)
{
    ofstream myfile;
    filename = filename + to_string(time) + ".txt";
    myfile.open(filename);

    myfile << "x_region,"
           << "y_region,"
           << "tho_new" << endl;
    for (auto i = 0; i < arr.size(); i++)
    {
        for (auto j = 0; j < arr[i].size(); j++)
        {
            myfile << arr[i][j].region_row << "," << arr[i][j].region_col << "," << arr[i][j].tho_new << endl;
        }
    }
    myfile.close();
}
