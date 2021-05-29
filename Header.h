#pragma once
#include <vector>
using namespace std;

struct Input
{
    vector<double> numbers;
    size_t bin_count;
};

vector <size_t> make_histogram(const Input data);
void find_minmax(const vector<double>& numbers, double& min, double& max);
