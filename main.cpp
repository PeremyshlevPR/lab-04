#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <curl/curl.h>
#include "Header.h"

using namespace std;

vector<double> input_numbers(istream& in, size_t count)
{
    vector<double> result(count);
    for (size_t i = 0; i < count; i++) {
        in >> result[i];
    }
    return result;
}

Input read_input(istream& in, bool prompt)
{
    Input data;

    if(prompt)
        cerr << "Enter number count: ";

    size_t number_count;
    in >> number_count;

    if(prompt)
        cerr << "Enter numbers: ";

    data.numbers = input_numbers(in, number_count);

    if(prompt)
        cerr << "Enter column count: ";

    cin >> data.bin_count;

    return data;
}

Input download(const string& adress)
{
    stringstream buffer;
        CURL *curl = curl_easy_init();
        if(curl)
        {
            CURLcode res;
            string str = adress.c_str();
            curl_easy_setopt(curl, CURLOPT_URL, str);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
            res = curl_easy_perform(curl);
            cout << "ERROR CODE = " << res << endl;
            if(res == 0)
            {
                double time = 0;
                double* timep = &time;
                curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, timep);
                cout << "Время, затраченное на установку соединения с сервером: " << timep;
            }
            else
            {
                cout << curl_easy_strerror(res);
                exit(1);
            }
            curl_easy_cleanup(curl);
        }
    return read_input(buffer, false);
}

size_t
write_data(void* items, size_t item_size, size_t item_count, void* ctx)
{
    stringstream* buffer = reinterpret_cast<stringstream*>(ctx);
    auto data_size = item_size * item_count;
    const char* items_char = reinterpret_cast<const char*>(items);
    buffer->write(items_char, data_size);
    return 0;
}

int main(int argc, char* argv[])
{
   Input input;
    if (argc > 1)
        {
            input = download(argv[1]);
        }
    else
        {
            input = read_input(cin, true);
        }

        const auto bins = make_histogram(input);

        // Вывод данных
        show_histogram_svg(bins);
      return 0;
}
