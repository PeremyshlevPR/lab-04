#include "Header.h"

void find_minmax(const vector<double>& numbers, double& min, double& max)
{
    if (numbers.size() != 0)
    {
        min = numbers[0];
        max = numbers[0];
        for (double number : numbers) {
            if (number < min) {
                min = number;
            }
            if (number > max) {
                max = number;
            }
        }
    }
}

vector <size_t> make_histogram(Input data)
{
    double min, max;
    find_minmax(data.numbers, min, max);
    vector<size_t> result(data.bin_count);
    for (double number : data.numbers) {
        size_t bin = (size_t)((number - min) / (max - min) * data.bin_count);
        if (bin == data.bin_count) {
            bin--;
        }
        result[bin]++;
    }
    return result;
}
