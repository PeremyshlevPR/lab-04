#include <iostream>
#include <vector>
#include "Header.h"
#include <curl/curl.h>
#include <sstream>
#include <string>
#include <windows.h>

DWORD WINAPI GetVersion(void);
using namespace std;

vector<double>
input_numbers(istream& in, size_t count) {
    vector<double> result(count);
    for (size_t i = 0; i < count; i++)
    {
        in >> result[i];
    }
    return result;
}
Input
read_input(istream& in,bool prompt)
{
    Input data;
    if (prompt)
    {
        cerr << "Enter number count: ";
    }
    size_t number_count;
    in >> number_count;
    if(prompt)
    {
        cerr << "Enter numbers: ";
    }
    data.numbers = input_numbers(in, number_count);
    if(prompt)
    {
        cerr <<"Enter bin count:";
    }
    in>>data.bin_count;
    return data;
}

size_t
write_data(void* items, size_t item_size, size_t item_count, void* ctx) {
    size_t data_size=item_size*item_count;
    stringstream* buffer = reinterpret_cast<stringstream*>(ctx);
   buffer->write(reinterpret_cast<const char*>(items), data_size);
    return data_size;
}

Input
download(const string& address, string& required_time) {
    stringstream buffer;
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
        if(curl)
        {
            CURLcode res;
            curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
            res = curl_easy_perform(curl);
            if (res)
            {
                cerr << curl_easy_strerror(res) << endl;
                exit(1);
            }
                double time = 0;
                double* timep = &time;
                curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, timep);
                string ttime = to_string(*timep);
                string p = "Time required to establish a connection to the server: ";
                string s = "sec";
                required_time = p + ttime + s;

        }
    curl_easy_cleanup(curl);
    return read_input(buffer, false);
}

int
main(int argc, char* argv[])
{


    Input input;
    string required_time = "0";
    if (argc > 1)
        {
            input = download(argv[1], required_time);
        }
    else
        {
            input = read_input(cin, true);
        }
    const auto bins=make_histogram(input);

    svg_begin(400, 300);
    svg_text(20, 20, winversion());
    svg_text(20, 40, compname());
    double top = 50;
    show_histogram_svg(bins, top);
    svg_text(20, top + 20, required_time);
    svg_end();
    return 0;
}
