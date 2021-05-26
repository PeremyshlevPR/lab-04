#include <iostream>
#include <vector>
#include <string>
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


void svg_begin(double width, double height)
{
    cout << "<?xml version='1.0' encoding='UTF-8'?>\n";
    cout << "<svg ";
    cout << "width='" << width << "' ";
    cout << "height='" << height << "' ";
    cout << "viewBox='0 0 " << width << " " << height << "' ";
    cout << "xmlns='http://www.w3.org/2000/svg'>\n";
}

void svg_end()
{
    cout << "</svg>\n";
}

void svg_text(double left, double baseline, string text)
{
    cout << "<text x='" << left << "' y='" << baseline << "'>" << text << "</text>\n";
}

void svg_rect(double left, double baseline, size_t width, double height, string stroke, string fill = "black")
{
    cout << "<rect x ='" << left << "' y ='" << baseline << "' width ='" << width << "' height ='" << height << "' stroke ='" << stroke << "' fill ='" << fill << "' />\n";
}

string enter_color()
{
    string color;
    bool correct = false;

    while (!correct)
    {
        cin >> color;
        bool probels = false;

        for (size_t i = 0; i < color.length(); i++)
        {
            if (color[i] == ' ')
                probels = true;
        }

        if ((color[0] == '#') || (!probels))
        {
            correct = true;
        }
        else
            cout << "Your enter is incorrect, try again";
    }
    return color;
}

void show_histogram_svg(const vector<size_t>& bins)
{
    const auto IMAGE_WIDTH = 400;
    const auto MAX_WIDTH = 350;
    const auto IMAGE_HEIGHT = 300;
    const auto TEXT_LEFT = 20;
    const auto TEXT_BASELINE = 20;
    const auto TEXT_WIDTH = 50;
    const auto BIN_HEIGHT = 30;
    const double BLOCK_WIDTH = 10;
    svg_begin(IMAGE_WIDTH, IMAGE_HEIGHT);
    double top = 0;
    string red = "red";
    string green = "green";

    size_t max_count = 0;
    for (size_t count : bins) {
        if (count > max_count) {
            max_count = count;
        }
    }
    const bool scaling_needed = (max_count * BLOCK_WIDTH) > MAX_WIDTH;

    string stroke;
    for (size_t bin : bins)
    {
        const double bin_width = BLOCK_WIDTH * bin;

        size_t width = bin_width;
        if (scaling_needed) {
            const double scaling_factor = (double)MAX_WIDTH / (max_count * BLOCK_WIDTH);
            width = (bin_width * scaling_factor);
        }



        stroke = enter_color();
        svg_text(TEXT_LEFT, top + TEXT_BASELINE, to_string(bin));
        svg_rect(TEXT_WIDTH, top, width, BIN_HEIGHT, stroke, green);
        top += BIN_HEIGHT;
    }
    svg_end();
}




void show_histogram_text(vector<double> bins)
{
    const size_t SCREEN_WIDTH = 80;
    const size_t MAX_ASTERISK = SCREEN_WIDTH - 4 - 1;

    size_t max_count = 0;
    for (size_t count : bins) {
        if (count > max_count) {
            max_count = count;
        }
    }
    const bool scaling_needed = max_count > MAX_ASTERISK;

    for (size_t bin : bins) {
        if (bin < 100) {
            cout << ' ';
        }
        if (bin < 10) {
            cout << ' ';
        }
        cout << bin << "|";

        size_t height = bin;
        if (scaling_needed) {
            const double scaling_factor = (double)MAX_ASTERISK / max_count;
            height = (size_t)(bin * scaling_factor);
        }

        for (size_t i = 0; i < height; i++) {
            cout << '*';
        }
        cout << '\n';
    }
}

int main()
{
    curl_global_init(CURL_GLOBAL_ALL);

    // Ввод данных
     const auto input = read_input(cin, true);

    // Построение гистограммы

    const auto bins = make_histogram(input);

    // Вывод данных
    show_histogram_svg(bins);

    return 0;
}
